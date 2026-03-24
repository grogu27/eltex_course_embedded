.PHONY: all clean lib app

CC ?= gcc
DEBUG ?= 0
LD_LIBRARY_PATH:=./lib
PREFIX ?=~/opt/calc

all: app

lib:
	$(MAKE) -C ./source/lib CC=$(CC) DEBUG=$(DEBUG)

app: lib
	$(MAKE) -C ./source/app CC=$(CC) DEBUG=$(DEBUG)

clean:
	@ echo "Cleaning..."
	@$(MAKE) -C ./source/lib clean
	@$(MAKE) -C ./source/app clean
	@rm -rf ./obj ./bin ./lib

run: app
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) ./bin/calc

install:
	@echo "Installing to $(PREFIX)"
	mkdir -p $(PREFIX)/bin $(PREFIX)/lib $(PREFIX)/include
	cp bin/calc $(PREFIX)/bin/
	cp lib/libcalc.so $(PREFIX)/lib/
	cp include/dynamic_calc.h $(PREFIX)/include/

uninstall:
	rm -f $(PREFIX)/bin/calc
	rm -f $(PREFIX)/lib/libcalc.so
	rm -f $(PREFIX)/include/dynamic_calc.h