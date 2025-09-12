#include "math.h"

usize usize_log2(usize n) {
	if (n == 0) {
		error("expected n != 0\n");
	}

	usize c = 0;
	while (n != 1) {
		n >>= 1;
		++c;
	}

	return c;
}
