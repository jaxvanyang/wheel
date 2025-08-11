CC := clang
LD := $(shell [ $$(uname) = 'Darwin' ] && echo ld || echo mold)
CFLAGS += -O2 -g -Isrc -Wall
LDFLAGS += -fuse-ld=$(LD) -Lsrc -lbasics

BASICS := src/basics
CORE := $(BASICS)/core

LIBA := src/libbasics.a
LIBSO := src/libbasics.so

CORES := $(patsubst %.c,%.o,$(wildcard $(CORE)/*.c))
OBJS := $(patsubst %.c,%.o,$(wildcard $(BASICS)/*.c))
OBJS += $(CORES)

BINS := $(patsubst %.c,%,$(wildcard bin/*.c))
TESTS := $(patsubst %.c,%,$(wildcard tests/*.c))

.PHONY: all
all: bins tests

.PHONY: bins
bins: $(BINS)

.PHONY: tests
tests: $(TESTS)

bin/%: bin/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%: tests/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(LIBA): $(OBJS)
	ar rs $@ $^

$(BASICS)/%.o: $(BASICS)/%.c
$(CORE)/%.o: $(CORE)/%.c

.PHONY: clean
clean:
	rm -f $(LIBA) $(LIBSO) $(BINS) $(TESTS) $(OBJS)
