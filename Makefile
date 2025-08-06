CC := clang
LD := $(shell [ $$(uname) = 'Darwin' ] && echo ld || echo mold)
CFLAGS += -O3 -g -Isrc -Wall
LDFLAGS += -fuse-ld=$(LD) -Lsrc -lbasics

.PHONY: all
all: bins tests

.PHONY: bins
bins: bin/uniq

.PHONY: tests
tests: tests/fib tests/kmp

bin/%: bin/%.c src/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%: tests/%.c src/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

src/libbasics.a: src/core/utils.o src/linear/list.o src/str.o
	ar rs $@ $^

src/%.o: src/%.c
src/core/%.o: src/core/%.c
src/linear/%.o: src/linear/%.c

.PHONY: clean
clean:
	find . -type f -name '*.a' -delete -print
	find . -type f -name '*.o' -delete -print
	find bin -type f -perm -111 -delete -print
	find tests -type f -perm -111 -delete -print
