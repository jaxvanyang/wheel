# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB
PLATFORM ?= PLATFORM_DESKTOP

# raylib library variables
ifdef RAYLIB_SRC_PATH
	RAYLIB_CFLAGS ?= -I$(RAYLIB_SRC_PATH)
	RAYLIB_LDFLAGS ?= -L$(RAYLIB_SRC_PATH) -lraylib
else
	ifdef RAYLIB_PATH
		RAYLIB_CFLAGS ?= -I$(RAYLIB_PATH)/include
		RAYLIB_LDFLAGS ?= -L$(RAYLIB_PATH)/lib -lraylib
	else
		RAYLIB_CFLAGS ?= $(shell pkg-config --cflags raylib)
		RAYLIB_LDFLAGS ?= $(shell pkg-config --libs raylib)
	endif
endif

# Target path for artifacts
PREFIX ?= install

ifeq ($(PLATFORM),PLATFORM_WEB)
	CC = emcc
	AR = emar
	LDFLAGS := -static \
		-s USE_GLFW=3 \
		--shell-file src/minshell.html \
		--preload-file assets \
		$(LDFLAGS)
	GAMES ?= $(patsubst %.c,%.html,$(wildcard game/*.c))

	ifdef RAYLIB_SRC_PATH
		RAYLIB_LDFLAGS := $(RAYLIB_SRC_PATH)/libraylib.web.a
	endif
else
	UNAMEOS = $(shell uname)
	ifneq ($(UNAMEOS),Darwin)
		# only use daynamic linking on macOS, see https://stackoverflow.com/questions/844819/how-to-static-link-on-os-x
		LDFLAGS := -static $(LDFLAGS)
	endif
endif

CC ?= cc
AR ?= ar
CFLAGS := $(RAYLIB_CFLAGS) $(CFLAGS)
CFLAGS := -Os -Wall -Wextra -Wno-comment -Wno-initializer-overrides -Isrc $(CFLAGS)
LDFLAGS := -Lsrc -lwheel $(RAYLIB_LDFLAGS) -lm $(LDFLAGS)

WHEEL := src/wheel
CORE := $(WHEEL)/core
XRAY := $(WHEEL)/xray

LIBA := src/libwheel.a
LIBSO := src/libwheel.so

CORES := $(patsubst %.c,%.o,$(wildcard $(CORE)/*.c))
XRAYS := $(patsubst %.c,%.o,$(wildcard $(XRAY)/*.c))
OBJS := $(patsubst %.c,%.o,$(wildcard $(WHEEL)/*.c))
OBJS += $(CORES) $(XRAYS)

GAMES ?= $(patsubst %.c,%,$(wildcard game/*.c))
BINS ?= $(patsubst %.c,%,$(wildcard bin/*.c)) $(GAMES)
TESTS ?= $(patsubst %.c,%,$(wildcard tests/*.c))

.PHONY: all
all: bins tests

.PHONY: install
install: bins
	cp $(BINS) $(PREFIX)
	cp -r assets $(PREFIX)

.PHONY: games
games: $(GAMES)

.PHONY: bins
bins: $(BINS)

.PHONY: tests
tests: $(TESTS)

bin/%: bin/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

game/%: game/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

game/%.html: game/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%: tests/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(LIBA): $(OBJS)
	$(AR) rs $@ $^

$(WHEEL)/%.o: $(WHEEL)/%.c
$(CORE)/%.o: $(CORE)/%.c
$(XRAY)/%.o: $(XRAY)/%.c

.PHONY: format
format:
	find src bin game tests -type f -name "*.[ch]" | xargs clang-format -style=file:.clang-format -i

build/compile_commands.json:
	@mkdir -p build
	bear --output build/compile_commands.json -- make

.PHONY: clean
clean:
	rm -f $(LIBA) $(LIBSO) $(BINS) $(TESTS) $(OBJS)
	-rm -f **/*.o **/*.a **/*.so
	-rm -rf bin/*.dSYM
	-rm -rf game/*.html game/*.wasm game/*.js game/*.data game/*.dSYM
