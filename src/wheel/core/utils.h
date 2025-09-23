#pragma once

#include <sys/time.h>

#include "./types.h"

typedef struct timeval TimeVal;

TimeVal time_now();

// elapsed time in micro seconds
f64 elapsed(TimeVal t0);

#ifndef max
#define max(a, b) (a > b ? a : b)
#endif
#ifndef min
#define min(a, b) (a < b ? a : b)
#endif

#define clamp(value, min, max) (value < min ? min : (value > max ? max : value))

void memswap(void *a, void *b, usize size);
