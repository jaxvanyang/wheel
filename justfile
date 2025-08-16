build:
	make

run bin *args:
	make bin/{{bin}}
	bin/{{bin}} {{args}}

test test:
	make tests/{{test}}
	tests/{{test}}

# run all tests
check:
	#!/usr/bin/env bash
	set -e
	for test in $(ls tests/*.c); do \
		just test $(basename "$test" .c); \
	done
