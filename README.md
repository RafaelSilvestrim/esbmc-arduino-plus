# ESBMC-Arduino Plus

Projeto de integração do ESBMC com várias bibliotecas Arduino, estendendo o trabalho original.

## Estrutura
- `include/`: headers dos módulos operacionais
- `src/`: implementações dos modelos
- `tests/`: testes unitários e casos de bug
- `Makefile`: comandos para executar modelos e testes

## Cronograma
- **Semana 1**: `arduino_core` (pinMode, digitalRead, digitalWrite)
- **Semana 2**: `arduino_time` (delay, millis) & `arduino_serial`
- **Semana 3**: `arduino_analog`, `arduino_wire`
- **Semana 4**: `arduino_spi`, `arduino_eeprom` & suite de testes completa

## Como usar
```bash
cd esbmc-arduino-plus
make all
```
