ESBMC-Arduino Plus

Automação e operational models para adaptar bibliotecas Arduino ao verificador ESBMC.
Fornece stubs, templates e uma suíte de testes para verificar propriedades (pré-condições, limites, invariantes) das APIs Arduino.

Status do Projeto (11 Nov 2025)

Estamos na transição entre "CLI stabilization" e o começo das fases "AI assist (opcional)" e "Pipeline end-to-end".
Resumo: protótipos, headers e testes existem; a maior parte dos módulos já roda; o foco atual é automatizar a geração de include/src/tests, estabilizar a interface de linha de comando (Makefile / scripts/) e construir um pipeline batch/CI.

Estrutura do repositório

include/ — headers (ex.: esbmc.h, arduino_core.h, arduino_*.h)

src/ — implementações dos stubs / modelos (ex.: arduino_core.c, arduino_time.c)

tests/ — casos de teste (ex.: test_core.c, test_time.c)

scripts/ — utilitários (ex.: generate_from_name.py)

logs/ — saída dos runs do ESBMC (gerados pelos alvos do Makefile)

Makefile — targets para rodar testes, CI, limpar logs

docs/ — (opcional) documentação estendida

Quickstart — comandos principais

Abra um terminal na raiz do repositório.

1. Preparar ambiente
# criar pasta de logs (se não existir)
mkdir -p logs

2. Rodar toda a suíte (Makefile)
make all
# Ou, para log combinado:
make all > logs/make_all.log 2>&1
tail -n 200 logs/make_all.log


O que faz: executa cada target do Makefile (core, analog, time, serial, ...). Cada target invoca esbmc com os arquivos de teste e src apropriados.

3. Rodar um teste específico (via Makefile)
make analog      # roda apenas o teste do módulo 'analog'

4. Rodar um teste específico (invocação direta ESBMC)
esbmc tests/test_time.c src/arduino_time.c -Iinclude --unwind 5 > logs/time.log 2>&1
tail -n 200 logs/time.log


Observação: -Iinclude adiciona o diretório include ao caminho de includes; --unwind 5 define profundidade de desfazer loops/recursões.

5. Rodar um caso novo (ex.: foobar)

Se já gerou os arquivos (veja seção de automação abaixo):

make foobar
# ou:
esbmc tests/test_foobar.c src/arduino_foobar.c -Iinclude --unwind 5 > logs/foobar.log 2>&1
tail -n 200 logs/foobar.log

6. Limpar artefatos e logs
make clean
rm -rf logs/*

Como interpretar resultado do ESBMC

VERIFICATION SUCCESSFUL → nenhuma asserção violada com o --unwind atual.

VERIFICATION FAILED → existe contraexemplo; o log mostra a função/linha e a propriedade violada (ex.: digitalRead: pino não configurado como INPUT).

ERROR: PARSING ERROR → includes faltando, conflito de tipos, header mal formatado.

unwinding assertion loop X → loop ultrapassou o unwind; aumente --unwind ou forneça um stub/limite.

Problemas comuns & soluções rápidas
1) file not found: include/esbmc.h

Garanta -Iinclude no comando esbmc (Makefile deve incluir).

Verifique se o arquivo existe e se não há erros de encoding ou linhas estranhas.

2) Conflitos NUM_PINS / re-declaração de pin_modes

Causa: NUM_PINS definido em mais de um lugar ou arrays declarados/definidos com tamanhos diferentes.
Solução recomendada (padrão):

Centralize NUM_PINS num header (por exemplo include/arduino_core.h) com #ifndef NUM_PINS guard;

Em include/esbmc.h NÃO redefina NUM_PINS — apenas use #ifndef NUM_PINS para um default opcional;

Declare arrays como extern em headers e defina (sem extern) em exatamente um src/*.c.

Exemplo seguro em include/esbmc.h:

#ifndef ESBMC_H
#define ESBMC_H
/* ... intrínsecos ... */

/* não define NUM_PINS aqui; só usa se já definido externamente */
#ifdef NUM_PINS
extern unsigned char pin_modes[NUM_PINS];
extern int pin_states[NUM_PINS];
#else
/* opcional: ter fallback sem redeclarar tamanho (melhor evitar) */
extern unsigned char pin_modes[]; /* tamanho determinado no src */
extern int pin_states[];
#endif

#endif


E em src/arduino_core.c:

#ifndef NUM_PINS
#define NUM_PINS 14    /* ou 256, conforme sua modelagem */
#endif

unsigned char pin_modes[NUM_PINS];
int pin_states[NUM_PINS];

3) conflicting types for '__ESBMC_assert'

Use protótipos consistentes:

extern void __ESBMC_assert(_Bool cond, const char *msg);
extern void __ESBMC_assume(_Bool cond);


Não #define a macro com assinatura diferente que conflite com esbmc intrínsecos.

4) unwinding assertion loop em libs padrão (ex.: strlen)

Aumente --unwind ou forneça stub limitado (ex.: strlen com limite) para evitar loops não-controlados.

5) Windows: chmod / ls -la / ls -la tests não funcionam

Use WSL, Git Bash ou adapte os scripts para PowerShell; chmod é nativo *nix.

Automação: gerar arquivos (template) — generate_from_name.py

Objetivo: dado foobar gerar:

include/arduino_foobar.h

src/arduino_foobar.c

tests/test_foobar.c

Uso:

python3 scripts/generate_from_name.py foobar
# depois edite os arquivos gerados e rode: make foobar  (ou esbmc ...)


Exemplo de um template mínimo (coloque em scripts/generate_from_name.py):

#!/usr/bin/env python3
# scripts/generate_from_name.py
import sys, os
if len(sys.argv) != 2:
    print("Uso: generate_from_name.py <nome>")
    sys.exit(1)
name = sys.argv[1]
h = f"include/arduino_{name}.h"
src = f"src/arduino_{name}.c"
test = f"tests/test_{name}.c"
os.makedirs("include", exist_ok=True)
os.makedirs("src", exist_ok=True)
os.makedirs("tests", exist_ok=True)
# header
with open(h, "w") as f:
    f.write(f"""#ifndef ARDUINO_{name.upper()}_H
#define ARDUINO_{name.upper()}_H
#include "esbmc.h"
/* protótipos para {name} */
void {name}_init(void);
int {name}_do_something(int arg);
#endif
""")
# src
with open(src, "w") as f:
    f.write(f"""#include "arduino_{name}.h"
void {name}_init(void) {{ /* stub */ }}
int {name}_do_something(int arg) {{
  int v = __VERIFIER_nondet_int();
  __ESBMC_assume(v >= 0 && v <= 10);
  return v;
}}
""")
# test
with open(test, "w") as f:
    f.write(f"""#include "arduino_{name}.h"
int main() {{
  /* caso ok */
  {name}_init();
  int r = {name}_do_something(0);
  __ESBMC_assert(r >= 0 && r <= 10, "ok-range");
  /* caso bad (descomente para testar falha) */
  // __ESBMC_assert(r >= 100, "force-fail");
  return 0;
}}
""")
print("Gerado:", h, src, test)