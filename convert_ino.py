#!/usr/bin/env python3
"""
convert_ino.py

Converte (heuristicamente) um sketch Arduino .ino em:
  - include/arduino_<name>.h
  - src/arduino_<name>.c
  - tests/test_<name>_from_ino.c

Uso:
  python3 convert_ino.py path/to/sketch.ino \
      --outdir ./esbmc-arduino-plus \
      --name foobar \
      --loop-iterations 1

Opções:
  --outdir DIR            Diretório raiz do projeto (cria subpastas include/, src/, tests/ se necessário)
  --name BASE             Nome base para os arquivos gerados (padrão: nome do sketch)
  --loop-iterations N     Quantas vezes chamar loop() no teste (padrão 1)
  --force                 Sobrescrever arquivos existentes sem pedir
  --verbose               Mensagens extras
"""
import argparse
import os
import re
import sys
from textwrap import dedent

# ---------- Heurísticas ----------
FUNC_START_RE = re.compile(r'^[\s\w\*\&\<\>\:\,]+?\([^\)]*\)\s*\{')  # linha que parece assinatura com '{' na mesma linha
FUNC_SIGNATURE_LINE_RE = re.compile(r'^[\s\w\*\&\<\>\:\,]+\([^\)]*\)\s*$')  # assinatura sem '{', continua na próxima linha
CONTROL_KEYWORDS = {'if', 'for', 'while', 'switch', 'else', 'do', 'case', 'return'}


def is_include_line(line):
    return line.strip().startswith('#include') or line.strip().startswith('#define') or line.strip().startswith('#ifdef') or line.strip().startswith('#ifndef') or line.strip().startswith('#endif') or line.strip().startswith('#pragma')


def looks_like_function_start(line):
    s = line.strip()
    # quick filter: ignore lines that start with control keywords
    first_tok = s.split(None, 1)[0] if s else ''
    if first_tok in CONTROL_KEYWORDS:
        return False
    # must have '(' and not end with ';' (so not a prototype)
    if '(' not in s or s.rstrip().endswith(';'):
        return False
    # try regex
    if FUNC_START_RE.match(s):
        return True
    if FUNC_SIGNATURE_LINE_RE.match(s):
        return True
    return False


def extract_blocks(lines, verbose=False):
    """
    Retorna (includes_and_pragmas, globals, functions)
    functions: list of dict {'signature': str, 'body': [lines], 'raw_header_lines': [lines]}
    globals: list of lines outside functions and not includes
    """
    includes = []
    globals_ = []
    functions = []

    i = 0
    n = len(lines)

    in_function = False
    while i < n:
        line = lines[i]
        if is_include_line(line):
            includes.append(line.rstrip())
            i += 1
            continue

        if looks_like_function_start(line):
            # attempt to capture full signature + body using brace counting
            sig_lines = []
            # gather signature lines until we hit a line containing '{'
            while i < n:
                sig_line = lines[i].rstrip('\n')
                sig_lines.append(sig_line)
                if '{' in sig_line:
                    break
                i += 1
            # if we never found '{', treat sig_lines as global fallback
            if not any('{' in l for l in sig_lines):
                # fallback: treat as global
                globals_.extend(sig_lines)
                i += 1
                continue

            # Now capture body until matching braces
            body_lines = []
            # count braces; initialize with count of '{' minus '}' in sig lines
            brace_count = sum(l.count('{') - l.count('}') for l in sig_lines)
            i += 1
            while i < n and brace_count > 0:
                l = lines[i]
                body_lines.append(l.rstrip('\n'))
                brace_count += l.count('{') - l.count('}')
                i += 1
            signature = ' '.join(s.strip() for s in sig_lines)
            functions.append({
                'signature': signature,
                'raw_header_lines': sig_lines,
                'body': body_lines
            })
            if verbose:
                print(f"[parse] found function: {signature.split('(')[0].strip()}")
            continue

        # otherwise, treat as top-level global line (variable decls, comments)
        globals_.append(line.rstrip('\n'))
        i += 1

    return includes, globals_, functions


def make_header(name, includes, globals_, functions):
    guard = f"ARDUINO_{name.upper()}_H"
    lines = []
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    # keep includes that look like libraries
    for inc in includes:
        lines.append(inc)
    lines.append("")
    lines.append("/* Prototypes extracted from sketch */")
    # prototypes: make from function signatures: remove function body if any
    for fn in functions:
        sig = fn['signature']
        # remove trailing '{' if any and keep signature up to ')'
        sig_clean = sig
        if '{' in sig_clean:
            sig_clean = sig_clean.split('{', 1)[0].strip()
        # ensure ends with ';'
        if not sig_clean.endswith(';'):
            sig_clean = sig_clean.rstrip() + ';'
        lines.append(sig_clean)
    lines.append("")
    lines.append("/* Globals from sketch (please review/adjust types as needed) */")
    for g in globals_:
        # skip empty lines
        if g.strip():
            lines.append(g)
    lines.append("")
    lines.append(f"#endif /* {guard} */")
    return '\n'.join(lines) + '\n'


def make_src(name, includes, globals_, functions):
    lines = []
    # include project esbmc header (if exists) and generated header
    lines.append('#include "esbmc.h"')
    lines.append(f'#include "{name}.h"')
    lines.append("")
    # place globals (these are copied verbatim; user may need to adjust types)
    if globals_:
        lines.append("/* Globals (from sketch) */")
        for g in globals_:
            lines.append(g)
        lines.append("")
    # add functions bodies
    for fn in functions:
        # signature might include '{' if signature and body were on same line; remove trailing '{'
        header = fn['signature']
        header = header.split('{')[0].strip()
        # ensure header ends with newline
        lines.append(header)
        lines.append("{")
        for b in fn['body']:
            lines.append(b)
        lines.append("}")
        lines.append("")
    return '\n'.join(lines) + '\n'


def make_test(name, loop_iterations=1):
    """Gera um harness de teste que chama setup() e loop()"""
    test_name = f"test_{name}_from_ino.c"
    lines = []
    lines.append('/* Auto-generated test harness from .ino by convert_ino.py */')
    lines.append('#include "esbmc.h"')
    lines.append(f'#include "{name}.h"')
    lines.append("")
    lines.append("int main() {")
    lines.append("    /* Call Arduino setup once */")
    lines.append("    #ifdef setup")
    lines.append("    setup();")
    lines.append("    #endif")
    lines.append("")
    if loop_iterations <= 0:
        loop_iterations = 1
    lines.append(f"    /* Call loop() {loop_iterations} time(s). For ESBMC, keep small. */")
    for i in range(loop_iterations):
        lines.append("    #ifdef loop")
        lines.append("    loop();")
        lines.append("    #endif")
    lines.append("")
    lines.append("    return 0;")
    lines.append("}")
    return '\n'.join(lines) + '\n'


def safe_write(path, content, force=False):
    if os.path.exists(path) and not force:
        raise FileExistsError(f"File exists: {path} (use --force to overwrite)")
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)


def main():
    ap = argparse.ArgumentParser(description="Convert .ino to simple include/src/test for ESBMC (heuristic)")
    ap.add_argument('ino', help='Path to sketch.ino')
    ap.add_argument('--outdir', default='.', help='Project root (will create include/, src/, tests/ under it)')
    ap.add_argument('--name', default=None, help='Base name for generated files (default: sketch basename prefixed with arduino_)')
    ap.add_argument('--loop-iterations', type=int, default=1, help='How many times to call loop() in the generated test harness (default 1)')
    ap.add_argument('--force', action='store_true', help='Overwrite output files if exist')
    ap.add_argument('--verbose', action='store_true', help='Verbose parsing info')
    args = ap.parse_args()

    ino_path = args.ino
    if not os.path.isfile(ino_path):
        print(f"ERROR: .ino file not found: {ino_path}", file=sys.stderr)
        sys.exit(2)

    with open(ino_path, 'r', encoding='utf-8') as f:
        raw = f.read()

    # Normalize line endings and split
    lines = raw.replace('\r\n', '\n').split('\n')

    includes, globals_, functions = extract_blocks(lines, verbose=args.verbose)

    # name derivation
    base = args.name
    if not base:
        bn = os.path.basename(ino_path)
        base = os.path.splitext(bn)[0]
    # sanitize base: prefix arduino_ to match project style and remove extension
    base_safe = f"arduino_{base}".replace('-', '_')

    out_include_dir = os.path.join(args.outdir, 'include')
    out_src_dir = os.path.join(args.outdir, 'src')
    out_tests_dir = os.path.join(args.outdir, 'tests')
    os.makedirs(out_include_dir, exist_ok=True)
    os.makedirs(out_src_dir, exist_ok=True)
    os.makedirs(out_tests_dir, exist_ok=True)

    header_name = f"{base_safe}.h"
    src_name = f"{base_safe}.c"
    test_name = f"test_{base}_from_ino.c"

    header_path = os.path.join(out_include_dir, header_name)
    src_path = os.path.join(out_src_dir, src_name)
    test_path = os.path.join(out_tests_dir, test_name)

    header_content = make_header(base_safe, includes, globals_, functions)
    src_content = make_src(base_safe, includes, globals_, functions)
    test_content = make_test(base_safe, loop_iterations=args.loop_iterations)

    # write files
    try:
        safe_write(header_path, header_content, force=args.force)
        safe_write(src_path, src_content, force=args.force)
        safe_write(test_path, test_content, force=args.force)
    except FileExistsError as e:
        print("ERROR:", e, file=sys.stderr)
        sys.exit(3)

    print("Wrote:")
    print("  header:", header_path)
    print("  src:   ", src_path)
    print("  test:  ", test_path)
    print("")
    print("Next steps (suggestions):")
    print(f"  1) Review {header_path} and {src_path} for Arduino-only types and adjust includes.")
    print("  2) Add '#include \"esbmc.h\"' path if your project keeps it in include/.")
    print(f"  3) Run ESBMC on the test harness (adjust unwind flags):")
    print(f"     esbmc {test_path} {src_path} -I{out_include_dir} --unwind 5")
    print("")
    print("Note: this tool is heuristic. Complex sketches need manual fixups (classes, Wire/SPI usage, Arduino API specifics).")

if __name__ == '__main__':
    main()
