build:
	make

run bin *args:
	make bin/{{bin}}
	bin/{{bin}} {{args}}

# run game (force rebuild)
play game *args:
	rm -f game/{{game}}
	make game/{{game}}
	game/{{game}} {{args}}

run-test test:
	make tests/{{test}}
	tests/{{test}}

test *tests:
	#!/usr/bin/env bash
	set -euo pipefail
	if [ -z "{{tests}}" ]; then
		for test in $(ls tests/*.c); do \
			just run-test $(basename "$test" .c); \
		done
	else
		for test in {{tests}}; do \
			just run-test "$test"; \
		done
	fi

check:
	typos --exclude 'raylib*'

fix:
	make format
	typos -w --exclude 'raylib*'
