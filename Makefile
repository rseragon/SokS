CC?=gcc

CFLAGS-base= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs`

SOURCES_BASE = $(wildcard SokS-base/*.c)

all: build base 


base: $(SOURCES_BASE)
	$(CC) $^ $(CFLAGS-base) -o bin/soks


build:
	mkdir -p bin/

tests:
	@echo "WIP"

clean:
	rm -rf bin/
