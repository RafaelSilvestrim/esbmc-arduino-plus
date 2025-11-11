# Makefile (sugestão)
CC = esbmc
BASE_CFLAGS = --no-bounds-check -Iinclude
UNWIND = --unwind 5

LOGDIR = logs
prepare:
	mkdir -p $(LOGDIR)

# default
.PHONY: all ci prepare clean
all: prepare core analog time serial wire spi eeprom ethernet lcd servo

# core
core:
	$(CC) tests/test_core.c src/arduino_core.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/core.log 2>&1 || true

analog:
	$(CC) tests/test_analog.c src/arduino_analog.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/analog.log 2>&1 || true

time:
	$(CC) tests/test_time.c src/arduino_time.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/time.log 2>&1 || true

serial:
	$(CC) tests/test_serial.c src/arduino_serial.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/serial.log 2>&1 || true

wire:
	$(CC) tests/test_wire.c src/arduino_wire.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/wire.log 2>&1 || true

spi:
	$(CC) tests/test_spi.c src/arduino_spi.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/spi.log 2>&1 || true

eeprom:
	$(CC) tests/test_eeprom.c src/arduino_eeprom.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/eeprom.log 2>&1 || true

# override unwind for ethernet target (strings may need more unwinding)
ethernet:
	$(CC) tests/test_ethernet.c src/arduino_ethernet.c $(BASE_CFLAGS) --unwind 12 > $(LOGDIR)/ethernet.log 2>&1 || true

lcd:
	$(CC) tests/test_liquidcrystal.c src/arduino_liquidcrystal.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/lcd.log 2>&1 || true

servo:
	$(CC) tests/test_servo.c src/arduino_servo.c $(BASE_CFLAGS) $(UNWIND) > $(LOGDIR)/servo.log 2>&1 || true

# run CI (all) and concat log to single file
ci: prepare
	@echo "Running full suite; combined log: $(LOGDIR)/make_all.log"
	@rm -f $(LOGDIR)/make_all.log
	@for t in analog core time serial wire spi eeprom ethernet lcd servo; do \
	  echo "=== $$t ===" >> $(LOGDIR)/make_all.log; \
	  make $$t; \
	  cat $(LOGDIR)/$$t.log >> $(LOGDIR)/make_all.log; \
	done
	@echo "Done. Tail of combined log:"; tail -n 200 $(LOGDIR)/make_all.log

clean:
	rm -rf $(LOGDIR)/*.log

# convenience: run a single test (usage: make run TEST=core)
run:
	$(CC) tests/test_$(TEST).c src/arduino_$(TEST).c $(BASE_CFLAGS) $(UNWIND)
