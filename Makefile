# Define target platform: PLATFORM_DESKTOP, PLATFORM_WEB
PLATFORM ?= PLATFORM_DESKTOP

# By default we suppose we are working on Windows
PLATFORM_OS ?= WINDOWS

# Target path for artifacts
PREFIX ?= install

# By default we use pkg-config to find raylib
# Use RAYLIB_SRC_PATH if you build raylib from source
# Use RAYLIB_PATH if you download prebuilt raylib from its releases
# Use RAYLIB_CFLAGS and RAYLIB_LDFLAGS if you want more control
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

UNAMEOS = $(shell uname)
ifeq ($(UNAMEOS),Linux)
	PLATFORM_OS = LINUX
endif
ifeq ($(UNAMEOS),Darwin)
	PLATFORM_OS = OSX
endif

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

SQLITE3_CFLAGS ?= $(shell pkg-config --cflags sqlite3)
SQLITE3_LDFLAGS ?= $(shell pkg-config --libs sqlite3)

	ifeq ($(PLATFORM_OS),WINDOWS)
		# see https://github.com/raysan5/raylib/wiki/Working-on-Windows
		LDFLAGS := -lgdi32 -lwinmm -lws2_32 $(LDFLAGS)
	endif
	ifneq ($(PLATFORM_OS),OSX)
		# only use daynamic linking on macOS, see https://stackoverflow.com/questions/844819/how-to-static-link-on-os-x
		LDFLAGS := -static $(LDFLAGS)
	endif
endif

CC ?= cc
AR ?= ar
CFLAGS := $(RAYLIB_CFLAGS) $(SQLITE3_CFLAGS) $(CFLAGS)
CFLAGS := -Os -Wall -Wextra -Wno-comment -Wno-initializer-overrides -Wno-override-init -Isrc $(CFLAGS)
LDFLAGS := -Lsrc -lwheel $(RAYLIB_LDFLAGS) $(SQLITE3_LDFLAGS) -lm $(LDFLAGS)

WHEEL := src/wheel
CORE := $(WHEEL)/core
XRAY := $(WHEEL)/xray
GAME_MODULE := $(WHEEL)/game

LIBA := src/libwheel.a
LIBSO := src/libwheel.so

CORES := $(patsubst %.c,%.o,$(wildcard $(CORE)/*.c))
XRAYS := $(patsubst %.c,%.o,$(wildcard $(XRAY)/*.c))
GAME_O := $(patsubst %.c,%.o,$(wildcard $(GAME_MODULE)/*.c))
OBJS := $(patsubst %.c,%.o,$(wildcard $(WHEEL)/*.c))
OBJS += $(CORES) $(XRAYS) $(GAME_O)

BINS ?= $(patsubst %.c,%,$(wildcard bin/*.c))
GAMES ?= $(patsubst %.c,%,$(wildcard game/*.c))
TESTS ?= $(patsubst %.c,%,$(wildcard tests/*.c))

ifeq ($(PLATFORM_OS),WINDOWS)
	BINS := $(patsubst %,%.exe,$(BINS))
	GAMES := $(patsubst %,%.exe,$(GAMES))
	TESTS := $(patsubst %,%.exe,$(TESTS))
endif

.PHONY: all
all: bins games tests

.PHONY: install
install: bins games
	cp $(BINS) $(PREFIX)
	cp -r assets $(PREFIX)

.PHONY: bins
bins: $(BINS)

.PHONY: games
games: $(GAMES)

.PHONY: tests
tests: $(TESTS)

bin/%: bin/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/%.exe: bin/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

game/%: game/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

game/%.exe: game/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

game/%.html: game/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%: tests/%.c $(LIBA)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tests/%.exe: tests/%.c $(LIBA)
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
	-rm -f **/*.o **/*.a **/*.so **/*.exe
	-rm -rf **/*.dSYM
	-rm -rf game/*.html **/*.wasm game/*.js **/*.data game/*.dSYM
