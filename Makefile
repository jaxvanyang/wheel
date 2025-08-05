CC := clang
LD := $(shell [ $$(uname) = 'Darwin' ] && echo ld || echo mold)
CFLAGS += -O3 -g -Isrc -Wall
LDFLAGS += -fuse-ld=$(LD) -Lsrc -lbasics

SRC := src
CORE := $(SRC)/core
DATA := $(SRC)/data

.PHONY: all
all: bins tests

.PHONY: bins
bins: bin/uniq

.PHONY: tests
tests: tests/fib tests/kmp

bin/%: bin/%.c $(SRC)/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%: tests/%.c $(SRC)/libbasics.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(SRC)/libbasics.a: $(CORE)/utils.o $(DATA)/linear/list.o $(DATA)/str.o
	ar rs $@ $^

$(SRC)/%.o: $(SRC)/%.c
$(CORE)/%.o: $(CORE)/%.c
$(DATA)/%.o: $(DATA)/%.c
$(DATA)/linear/%.o: $(DATA)/linear/%.c

.PHONY: clean
clean:
	find . -type f -name '*.a' -delete -print
	find . -type f -name '*.o' -delete -print
	find bin -type f -executable -delete -print
	find tests -type f -executable -delete -print
