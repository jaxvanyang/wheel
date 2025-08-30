CC ?= cc
RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib)
RAYLIB_LDFLAGS ?= $(shell pkg-config --libs raylib)
CFLAGS := -O2 -g -Wall -Wextra -Isrc $(RAYLIB_CFLAGS) $(CFLAGS)
# FIXME: macOS doesn't support simple static linking
# https://stackoverflow.com/questions/844819/how-to-static-link-on-os-x
LDFLAGS := $(shell [ $$(uname) = 'Darwin' ] && echo '' || echo '-static') \
	-Lsrc -lwheel $(RAYLIB_LDFLAGS) -lm $(LDLAGS)

PREFIX ?= install

WHEEL := src/wheel
CORE := $(WHEEL)/core

LIBA := src/libwheel.a
LIBSO := src/libwheel.so

CORES := $(patsubst %.c,%.o,$(wildcard $(CORE)/*.c))
OBJS := $(patsubst %.c,%.o,$(wildcard $(WHEEL)/*.c))
OBJS += $(CORES)

BINS := $(patsubst %.c,%,$(wildcard bin/*.c))
TESTS := $(patsubst %.c,%,$(wildcard tests/*.c))

.PHONY: all
all: bins tests

.PHONY: install
install: bins
	cp $(BINS) $(PREFIX)
	cp -r assets $(PREFIX)

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

$(WHEEL)/%.o: $(WHEEL)/%.c
$(CORE)/%.o: $(CORE)/%.c

.PHONY: format
format:
	clang-format -i **/*.c **/*.h

.PHONY: clean
clean:
	rm -f $(LIBA) $(LIBSO) $(BINS) $(TESTS) $(OBJS)
