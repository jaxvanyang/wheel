CC := clang
CFLAGS := -Isrc -O3 -Wall
LDFLAGS := -Lsrc -lbasics

.PHONY: all
all: bin/a

bin/%: bin/%.c src/libbasics.so
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

src/libbasics.so: src/core/utils.o src/core/error.o src/linear/list.o
	$(CC) $(CFLAGS) -shared -fpic -o $@ $^

src/core/%.o: src/core/%.c
src/linear/%.o: src/linear/%.c

.PHONY: clean
clean:
	find src -type f -name '*.o' -delete -print
	find src -type f -name '*.so' -delete -print
	find bin -type f -executable -delete -print
