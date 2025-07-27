CC := clang
CFLAGS := -g -Isrc -O3 -Wall
LDFLAGS := -fuse-ld=mold -Lsrc -lbasics

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
	find bin -type f -executable -delete -print
	find tests -type f -executable -delete -print
