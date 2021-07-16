CC?=gcc

CFLAGS-base= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs`
CFLAGS-base-dev= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs` -Wall -Wextra -g

SOURCES_BASE = $(wildcard SokS-base/*.c)
OBJECTS_BASE = $(patsubst %.c,%.o,$(SOURCES_BASE))

TESTS_SRC = $(wildcard tests/*.c)

all: build base 

dev: build base-dev

base: $(OBJECTS_BASE)
	$(CC) $^ $(CFLAGS-base) -o bin/soks

$(OBJECTS_BASE): $(SOURCES_BASE)
	$(CC) $(CFLAGS-base) -c -o $@ $*.c

base-dev: $(SOURCES_BASE)
	$(CC) $^ $(CFLAGS-base-dev) -o bin/soks

build:
	mkdir -p bin/

tests: $(OBJECTS_BASE) 
	$(CC) SokS-base/graph.o $(CFLAGS-base) $(TESTS_SRC) -o tests/test.run -g -fno-stack-protector
	sh ./tests/run_tests.sh

clean:
	rm -rf bin/
	rm -rf SokS-base/*.o
	rm -rf tests/*.jpg
	rm -rf tests/*.run
	rm -rf tests/*.log

.PHONY: tests clean
