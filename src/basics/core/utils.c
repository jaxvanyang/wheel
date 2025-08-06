#include "utils.h"

f64 elapsed(struct timeval t0, struct timeval t1) {
	f64 dt = (t1.tv_sec - t0.tv_sec) * 1e3;
	dt += (t1.tv_usec - t0.tv_usec) * 1e-3;

	return dt;
}
