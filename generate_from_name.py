#!/usr/bin/env python3
"""
generate_from_name.py

Gera stubs para uma biblioteca Arduino / C para uso com ESBMC.

O que faz:
  - cria `include/arduino_<name>.h`
  - cria `src/arduino_<name>.c`
  - cria `tests/test_<name>.c` (harness para ESBMC)
  - se você fornecer um header com `--header path/to/foo.h`, tenta extrair os protótipos
    e gerar implementações mínimas automaticamente.
  - caso contrário, gera um template com instruções para preencher.

Uso:
  python3 generate_from_name.py --name foo
  python3 generate_from_name.py --name foo --header path/to/original/foo.h
  python3 generate_from_name.py --name foo --outdir ./esbmc-arduino-plus --force --loop-iterations 1

Opções relevantes:
  --name NAME            (obrigatório) nome base da lib (ex: "servo" -> arduino_servo)
  --header PATH          header para extrair protótipos (opcional)
  --outdir DIR           raiz do projeto (default: .)
  --force                sobrescrever arquivos existentes
  --loop-iterations N    quantas iterações do loop chamar no harness (default 1)
  --verbose              mensagens extras
"""
import argparse
import os
import re
import sys
from textwrap import dedent

# ----------------- Helpers para parsing -----------------

PROTO_RE = re.compile(r'^\s*([^\(\);]+?)\s+([_A-Za-z]\w*)\s*\(([^;{}]*)\)\s*;') 
# captura: (tipo_retorno e possivelmente qualifiers) (nome) (param-list)

PARAM_SPLIT_RE = re.compile(r'\s*,\s*')

def extract_prototypes_from_header(path, verbose=False):
    """
    Lê um header e tenta extrair linhas de protótipos simples.
    Retorna lista de strings com protótipos (com ';' no final).
    """
    protos = []
    try:
        with open(path, 'r', encoding='utf-8', errors='ignore') as f:
            text = f.read()
    except Exception as e:
        raise RuntimeError(f"Falha ao ler header {path}: {e}")

    # remover comentários de bloco e linha para reduzir ruído
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    text = re.sub(r'//.*', '', text)

    # analisar linha a linha procurando protótipos simples
    for line in text.splitlines():
        line = line.strip()
        if not line:
            continue
        # ignorar macros que terminam com '\'
        if line.endswith('\\'):
            continue
        # tentar casar com expressão
        m = PROTO_RE.match(line)
        if m:
            rettype = m.group(1).strip()
            name = m.group(2).strip()
            params = m.group(3).strip()
            proto = f"{rettype} {name}({params});"
            protos.append(proto)
            if verbose:
                print(f"[proto] found: {proto}")
    return protos


# ----------------- Geração de stubs -----------------

def make_header(outname, prototypes, includes=None):
    guard = f"ARDUINO_{outname.upper()}_H"
    lines = []
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    lines.append("/* Auto-generated header by generate_from_name.py */")
    lines.append("#include \"esbmc.h\"  /* ensure project intrinsics available */")
    if includes:
        for inc in includes:
            lines.append(inc)
    lines.append("")
    lines.append("/* Prototypes */")
    if prototypes:
        for p in prototypes:
            lines.append(p)
    else:
        lines.append("/* TODO: Adicione protótipos da biblioteca aqui. Exemplo: */")
        lines.append("/* void begin(); */")
        lines.append("/* int readSensor(int channel); */")
    lines.append("")
    lines.append(f"#endif /* {guard} */")
    return "\n".join(lines) + "\n"


# map return type -> nondet helper name (from esbmc.h)
RETURN_NONDDET_MAP = {
    'int': '__VERIFIER_nondet_int',
    'unsigned int': '__VERIFIER_nondet_uint',
    'long': '__VERIFIER_nondet_long',
    'unsigned long': '__VERIFIER_nondet_ulong',
    'char': '__VERIFIER_nondet_char',
    'unsigned char': '__VERIFIER_nondet_uchar',
    'float': '__VERIFIER_nondet_float',
    'double': '__VERIFIER_nondet_double',
    # boolean-ish mapping
    '_Bool': '__VERIFIER_nondet_int',  # no header for _Bool nondet; fallback int
    'bool': '__VERIFIER_nondet_int',
}

def select_nondet_for_type(typ):
    t = ' '.join(typ.split())  # normalize spaces
    # try exact match
    if t in RETURN_NONDDET_MAP:
        return RETURN_NONDDET_MAP[t]
    # heuristics: contains keywords
    if 'char' in t and 'unsigned' not in t:
        return '__VERIFIER_nondet_char'
    if 'unsigned' in t and 'char' in t:
        return '__VERIFIER_nondet_uchar'
    if 'int' in t or 'short' in t or 'long' in t:
        return '__VERIFIER_nondet_int'
    if 'float' in t:
        return '__VERIFIER_nondet_float'
    if 'double' in t:
        return '__VERIFIER_nondet_double'
    # default
    return '__VERIFIER_nondet_int'

def default_arg_for_param(param):
    """
    Dada uma declaração de parâmetro como 'const char *s' ou 'int x',
    retorna um literal seguro para chamar a função no teste:
     - int/unsigned -> 0
     - pointers/strings -> NULL or ""
     - float/double -> 0.0
     - custom types -> 0
    """
    p = param.strip()
    if not p or p == 'void':
        return None
    # remove param name heuristically (keep type)
    # ex: "const char *s" -> "const char *"
    # simple approach: drop last token if it doesn't contain '*' and isn't a type keyword
    tokens = p.split()
    # if last token is a parameter name (identifier), drop it
    if len(tokens) > 1 and re.match(r'^[A-Za-z_]\w*$', tokens[-1]) and '*' not in tokens[-1]:
        tokens = tokens[:-1]
    typ = ' '.join(tokens)
    if '*' in typ or 'char' in typ:
        # prefer string literal for char*; NULL for other pointers
        if 'char' in typ:
            return '""'
        return 'NULL'
    if 'float' in typ or 'double' in typ:
        return '0.0'
    # integer-like
    return '0'

def make_c_source(outname, prototypes):
    """
    Gera implementações C mínimas para os protótipos.
    Para funções não-void, retorna nondet apropriado.
    """
    lines = []
    lines.append('/* Auto-generated source by generate_from_name.py */')
    lines.append('#include "esbmc.h"')
    lines.append(f'#include "{outname}.h"')
    lines.append("")
    for p in prototypes:
        m = PROTO_RE.match(p)
        if not m:
            continue
        rettype = m.group(1).strip()
        fname = m.group(2).strip()
        params = m.group(3).strip()
        # reconstruct signature (without trailing ';')
        signature = f"{rettype} {fname}({params})"
        lines.append(signature)
        lines.append("{")
        # body
        if rettype == 'void' or 'void' == rettype:
            lines.append("    /* stub: no-op */")
        else:
            nondet = select_nondet_for_type(rettype)
            # call nondet and return
            lines.append(f"    {nondet}(); /* nondet value for {rettype} */")
            # actually return the nondet value (use cast for safety)
            if nondet.endswith('_int'):
                lines.append(f"    return {nondet}();")
            else:
                lines.append(f"    return {nondet}();")
        lines.append("}")
        lines.append("")
    # if no prototypes, add a sample stub to guide user
    if not prototypes:
        lines.append("/* No prototypes detected; add stubs manually. Example: */")
        lines.append("void begin_stub(void) { }")
    return "\n".join(lines) + "\n"

def make_test_harness(outname, prototypes, loop_iterations=1):
    """
    Gera um harness de teste que chama funções encontradas para que ESBMC as analise.
    O objetivo é ter chamadas simples com argumentos default.
    """
    lines = []
    lines.append('/* Auto-generated test harness by generate_from_name.py */')
    lines.append('#include "esbmc.h"')
    lines.append(f'#include "{outname}.h"')
    lines.append("")
    lines.append("int main() {")
    # call each prototype with defaults (skip void with params void)
    for p in prototypes:
        m = PROTO_RE.match(p)
        if not m:
            continue
        rettype = m.group(1).strip()
        fname = m.group(2).strip()
        params = m.group(3).strip()
        if not params or params == 'void':
            # simple call
            lines.append(f"    {fname}();")
        else:
            # build arg list
            param_list = PARAM_SPLIT_RE.split(params)
            args = []
            for param in param_list:
                arg = default_arg_for_param(param)
                if arg is None:
                    # skip this call if we cannot generate args
                    args = None
                    break
                args.append(arg)
            if args is None:
                continue
            args_text = ', '.join(args)
            lines.append(f"    {fname}({args_text});")
    # also call setup/loop convention if present in prototypes
    proto_names = [ (PROTO_RE.match(p).group(2) if PROTO_RE.match(p) else None) for p in prototypes ]
    if 'setup' in proto_names:
        lines.append("    /* Call setup if present */")
        lines.append("    setup();")
    if 'loop' in proto_names:
        lines.append(f"    /* Call loop() {loop_iterations} time(s) */")
        for i in range(loop_iterations):
            lines.append("    loop();")
    lines.append("    return 0;")
    lines.append("}")
    return "\n".join(lines) + "\n"


# ----------------- I/O helpers -----------------

def safe_write(path, content, force=False):
    if os.path.exists(path) and not force:
        raise FileExistsError(f"File exists: {path} (use --force to overwrite)")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)


# ----------------- Main -----------------

def main():
    ap = argparse.ArgumentParser(description="Generate Arduino library stubs for ESBMC")
    ap.add_argument('--name', required=True, help='Base name (ex: servo -> generates arduino_servo.h/.c and test_servo.c)')
    ap.add_argument('--header', help='Optional existing header to parse for prototypes')
    ap.add_argument('--outdir', default='.', help='Project root (default: current dir)')
    ap.add_argument('--force', action='store_true', help='Overwrite existing files')
    ap.add_argument('--loop-iterations', type=int, default=1, help='How many times to call loop() in generated test harness')
    ap.add_argument('--verbose', action='store_true')
    args = ap.parse_args()

    base = args.name.strip()
    # sanitize and prefix to match project style "arduino_<name>"
    outbase = f"arduino_{base}".replace('-', '_')

    include_dir = os.path.join(args.outdir, 'include')
    src_dir = os.path.join(args.outdir, 'src')
    tests_dir = os.path.join(args.outdir, 'tests')

    header_path = os.path.join(include_dir, f"{outbase}.h")
    src_path = os.path.join(src_dir, f"{outbase}.c")
    test_path = os.path.join(tests_dir, f"test_{base}.c")

    prototypes = []
    includes = []

    if args.header:
        if args.verbose:
            print(f"[info] extracting prototypes from {args.header}")
        prototypes = extract_prototypes_from_header(args.header, verbose=args.verbose)
        # keep any external includes from the header? simple approach: none
    else:
        if args.verbose:
            print("[info] no header provided; generating template prototypes")

    header_content = make_header(outbase, prototypes, includes=includes)
    c_content = make_c_source(outbase, prototypes)
    test_content = make_test_harness(outbase, prototypes, loop_iterations=args.loop_iterations)

    try:
        safe_write(header_path, header_content, force=args.force)
        safe_write(src_path, c_content, force=args.force)
        safe_write(test_path, test_content, force=args.force)
    except FileExistsError as e:
        print("ERROR:", e, file=sys.stderr)
        sys.exit(2)

    print("Generated files:")
    print("  ", header_path)
    print("  ", src_path)
    print("  ", test_path)
    print("")
    print("Next steps:")
    print(f"  1) Review {header_path} and adjust typedefs / macros / includes as needed.")
    print(f"  2) Edit {src_path} to implement behaviors or refine returned nondet ranges.")
    print(f"  3) Run ESBMC on the generated test harness, e.g.:")
    print(f"       esbmc {test_path} {src_path} -I{include_dir} --unwind 5 --no-bounds-check")
    print("  4) If the library depends on other Arduino APIs (Wire/SPI/EEPROM), ensure you have those stubs in include/ and src/.")
    print("")
    print("Tip: use --header to parse an existing .h and get prototypes automatically (works best for simple C headers).")

if __name__ == '__main__':
    main()
