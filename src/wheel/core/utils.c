#include "utils.h"
#include <stdlib.h>
#include <string.h>

TimeVal time_now() {
	TimeVal t;
	gettimeofday(&t, NULL);

	return t;
}

f64 elapsed(TimeVal t0) {
	TimeVal t1 = time_now();
	f64 dt = (t1.tv_sec - t0.tv_sec) * 1e3;
	dt += (t1.tv_usec - t0.tv_usec) * 1e-3;

	return dt;
}

void memswap(void *a, void *b, usize size) {
	if (size == 0) {
		return;
	}

	void *tmp = malloc(size);

	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);

	free(tmp);
}
