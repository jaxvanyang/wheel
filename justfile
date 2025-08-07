build:
	make

run bin:
	make bin/{{bin}}
	bin/{{bin}}

test test:
	make tests/{{test}}
	tests/{{test}}

# run all tests
check:
	for test in $(ls tests/*.c); do \
		just test $(basename "$test" .c); \
	done
