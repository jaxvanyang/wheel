build:
	meson compile -C build || echo 'run "just setup" first'

setup:
	meson setup build

run bin *args: build
	build/bin/{{bin}} {{args}}

play game *args: build
	build/game/{{game}} {{args}}

test *tests:
	meson test -C build {{tests}}

check:
	typos --exclude 'raylib*' --exclude subprojects --exclude 'build*'

format:
	find src bin game tests -type f -name "*.[ch]" | xargs clang-format -style=file:.clang-format -i

fix:
	typos -w --exclude 'raylib*' --exclude subprojects --exclude 'build*'
	just format
