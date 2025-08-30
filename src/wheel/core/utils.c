#include "utils.h"

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
