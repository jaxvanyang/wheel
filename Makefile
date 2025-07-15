CC := clang
CFLAGS := -Isrc -O3 -Wall
LDFLAGS := -Lsrc -lbasics

.PHONY: all
all: bin/uniq bin/fib

bin/%: bin/%.c src/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

src/libbasics.a: src/core/utils.o src/core/error.o src/linear/list.o
	ar rs $@ $^

src/core/%.o: src/core/%.c
src/linear/%.o: src/linear/%.c

.PHONY: clean
clean:
	find src -type f -name '*.o' -delete -print
	find src -type f -name '*.a' -delete -print
	find bin -type f -executable -delete -print
