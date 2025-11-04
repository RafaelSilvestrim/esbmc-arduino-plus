# ESBMC-Arduino Plus Makefile

CC      = esbmc
CFLAGS  = --unwind 5 --no-bounds-check -Iinclude

# Alvos padrão
all: core analog time serial wire spi overflow eeprom ethernet lcd servo

# Teste do módulo core (digital)
core:
	$(CC) tests/test_core.c src/arduino_core.c $(CFLAGS)

# Teste do módulo analógico
analog:
	$(CC) tests/test_analog.c src/arduino_analog.c $(CFLAGS)

# Teste do módulo de tempo
time:
	$(CC) tests/test_time.c src/arduino_time.c $(CFLAGS)

# Teste do módulo Serial
serial:
	$(CC) tests/test_serial.c src/arduino_serial.c $(CFLAGS)

# Teste do módulo I2C (Wire)
wire:
	$(CC) tests/test_wire.c src/arduino_wire.c $(CFLAGS)

# Teste do módulo SPI
spi:
	$(CC) tests/test_spi.c src/arduino_spi.c $(CFLAGS)


# Teste do módulo EEPROM
eeprom:
	$(CC) tests/test_eeprom.c src/arduino_eeprom.c $(CFLAGS)

# Teste do módulo Ethernet
ethernet:
	$(CC) tests/test_ethernet.c src/arduino_ethernet.c $(CFLAGS)

# Teste do módulo LiquidCrystal
lcd:
	$(CC) tests/test_liquidcrystal.c src/arduino_liquidcrystal.c $(CFLAGS)

# Teste do módulo Servo
servo:
	$(CC) tests/test_servo.c src/arduino_servo.c $(CFLAGS)
	
# Teste de overflow aritmético
#overflow:
	#$(CC) tests/test_overflow.c src/test_overflow.c $(CFLAGS)
	
		
foobar:
	$(CC) tests/test_foobar_ok.c src/arduino_foobar.c $(CFLAGS)	
	

# Limpeza de artefatos do ESBMC
clean:
	rm -f *.assert *.counterexample *.out

.PHONY: all core analog time serial wire spi overflow eeprom ethernet lcd servo clean 


# make          # rodará todos os testes, um a um
# make analog   # só o teste analógico, por exemplo
# make clean    # limpa as saídas do ESBMC
