build:
	make

run bin:
	make bin/{{bin}}
	bin/{{bin}}

test test:
	make tests/{{test}}
	tests/{{test}}

check: (test "fib")
