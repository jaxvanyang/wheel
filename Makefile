CC := clang
CFLAGS := -Isrc -O3 -Wall
LDFLAGS := -Lsrc -lbasics

.PHONY: all
all: bins tests

.PHONY: bins
bins: bin/uniq

.PHONY: tests
tests: tests/fib

bin/%: bin/%.c src/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%: tests/%.c src/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

src/libbasics.a: src/core/utils.o src/core/error.o src/linear/list.o
	ar rs $@ $^

src/core/%.o: src/core/%.c
src/linear/%.o: src/linear/%.c

.PHONY: clean
clean:
	-rm -f **/*.a **/*.o
	find bin -type f -executable -delete -print
	find tests -type f -executable -delete -print
