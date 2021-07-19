CC?=gcc
DEST?=/usr/bin/

CFLAGS-base= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs`
CFLAGS-base-dev= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs` -Wall -Wextra -g

SOURCES_BASE = $(wildcard base/*.c)
OBJECTS_BASE = $(patsubst %.c,%.o,$(SOURCES_BASE))

TESTS_SRC = $(wildcard tests/*.c)

TARGET = bin/soks

all: build base 

dev: build base-dev

base: $(OBJECTS_BASE)
	$(CC) $^ $(CFLAGS-base) -o $(TARGET)

$(OBJECTS_BASE): $(SOURCES_BASE)
	$(CC) $(CFLAGS-base) -c -o $@ $*.c

base-dev: $(SOURCES_BASE)
	$(CC) $^ $(CFLAGS-base-dev) -o $(TARGET)

build:
	mkdir -p bin/

install: $(TARGET)
	install -c $(TARGET) $(DEST)

tests: $(OBJECTS_BASE) 
	$(CC) -Ibase/ base/graph.o base/host_utils.o $(CFLAGS-base) $(TESTS_SRC) -o tests/test.run -g -fno-stack-protector
	sh ./tests/run_tests.sh

clean:
	rm -rf ./bin/
	rm -rf base/*.o
	rm -rf tests/*.jpg
	rm -rf tests/*.run
	rm -rf tests/*.log

.PHONY: tests clean
