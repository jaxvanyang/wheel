build:
	make

run bin:
	make bin/{{bin}}
	bin/{{bin}}

test test:
	make tests/{{test}}
	tests/{{test}}

# run all tests
check: (test "fib") (test "kmp")
