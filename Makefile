CC?=gcc

CFLAGS-base= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs`
CFLAGS-base-dev= -O2 `pkg-config libgvc --cflags` `pkg-config libgvc --libs` -Wall -Wextra -g

SOURCES_BASE = $(wildcard SokS-base/*.c)
OBJECTS_BASE = $(patsubst %.c,%.o,$(SOURCES_BASE))

all: build base 

dev: build base-dev

# Switch to dynamic patter rule instead of building all sources again
base: $(SOURCES_BASE)
	$(CC) $^ $(CFLAGS-base) -o bin/soks

base-dev: $(SOURCES_BASE)
	$(CC) $^ $(CFLAGS-base-dev) -o bin/soks

build:
	mkdir -p bin/

tests:
	@echo "WIP"

clean:
	rm -rf bin/

.PHONY: tests clean
