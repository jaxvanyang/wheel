# Wheel

## Build

Wheel uses [Meson] and [Ninja]. You also need a C compiler, [Raylib] 5.5 or
newer. The repository includes a Meson wrap for SQLite; during configuration,
Meson can download and build that copy when a suitable system SQLite 3
dependency is not available. The first setup may therefore need network
access. You can also install SQLite 3 development files through your system's
package manager.

If Raylib is not installed in a standard system location, pass its
installation directory with
`-D raylib_path=/path/to/raylib`; alternatively, pass the path to a Raylib
source `src` directory with `-D raylib_src_path=/path/to/raylib/src`.

> **Note:** These steps are examples and may need to be adjusted for your
> operating system, package manager, compiler, dependency locations, and local
> environment.

### Linux

Install the prerequisites using your distribution's package manager, then run:

```sh
meson setup build -D buildtype=release
meson compile -C build
meson test -C build
```

For a local installation, use `meson install -C build`. On Linux, building
against a pre-built Raylib also requires the X11 development files.

### macOS

With [Homebrew], install Meson, Ninja, SQLite, and Raylib:

```sh
brew install meson ninja pkg-config sqlite raylib
```

Then configure and build:

```sh
meson setup build -D buildtype=release
meson compile -C build
meson test -C build
```

### Windows

Use a MinGW-w64 C compiler and install Meson and Ninja (for example, with
Python's `pip`). From a shell with those tools on `PATH`, configure and build:

```sh
python -m pip install meson
meson setup build -D buildtype=release -D raylib_path=C:/path/to/raylib
meson compile -C build
meson test -C build
```

`raylib_path` must point to a Raylib installation containing `include` and
`lib` directories. The generated executables are in `build/bin` and
`build/game`.

### WebAssembly

Install and activate the [Emscripten SDK], build Raylib for WebAssembly, and
run Meson with the included cross file:

```sh
cd raylib/src
make RAYLIB_BUILD_MODE=RELEASE RAYLIB_LIBTYPE=STATIC RAYLIB_RELEASE_PATH=.
cd ../..

meson setup build-wasm --cross-file wasm.ini \
  -D raylib_src_path=raylib/src
meson compile -C build-wasm
```

The generated `.html`, `.js`, `.wasm`, and `.data` files are in
`build-wasm/game`. Copy `src/index.html` alongside them before serving the
game.

### Cross-compiling Windows from Linux

Install the MinGW-w64 toolchain, Wine (for running the tests), and a Raylib
installation built for `x86_64-w64-mingw32`. Then use the included cross file:

```sh
meson setup build-win --cross-file x86_64-w64-mingw32.ini \
  -D raylib_path=/path/to/mingw-raylib
meson compile -C build-win
meson test -C build-win
```

The cross file configures `x86_64-w64-mingw32-gcc` and uses `wine` as the
executable wrapper for tests.

## Assets

Thanks for all the creators! <3

| Path | Source | License |
|:-|:-|:-|
| `assets/brackeys_platformer_assets` | [Brackeys' Platformer Bundle]| CC0 |
| `assets/poker` | [(Pixel) Poker Cards by IvoryRed] | [CC-BY-4.0] |

Note of `assets/poker`: `button.png` is built upon `fiches_addon.png`.

[Brackeys' Platformer Bundle]: https://brackeysgames.itch.io/brackeys-platformer-bundle
[(Pixel) Poker Cards by IvoryRed]: https://ivoryred.itch.io/pixel-poker-cards
[CC-BY-4.0]: https://creativecommons.org/licenses/by/4.0
[Meson]: https://mesonbuild.com/
[Ninja]: https://ninja-build.org/
[Raylib]: https://www.raylib.com/
[Homebrew]: https://brew.sh/
[Emscripten SDK]: https://emscripten.org/docs/getting_started/downloads.html
