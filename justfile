build:
	make

run bin *args:
	make bin/{{bin}}
	bin/{{bin}} {{args}}

# clean run (force rebuild)
crun bin *args:
	rm -f bin/{{bin}}
	make bin/{{bin}}
	bin/{{bin}} {{args}}

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
