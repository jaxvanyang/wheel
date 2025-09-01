#include "random.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

void tsrandom() { srand(time(NULL)); }

usize random_usize() {
	usize a = rand();
	usize b = rand();

	return a << 32 | b;
}

isize random_isize() { return random_usize(); }

f64 random_f64() { return (f64)random_usize() / (f64)USIZE_MAX; }

usize random_range(usize start, usize end) {
	assert(start <= end);

	usize w = end - start;

	return random_usize() % w + start;
}
