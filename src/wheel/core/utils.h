#pragma once

#include <sys/time.h>

#include "./types.h"

typedef struct {
	unsigned int major;
	unsigned int minor;
	unsigned int patch;
} Version;

typedef struct timeval TimeVal;

// Return whether a is compatible with b (a can use b).
bool is_version_compatible(Version a, Version b);
// Only "0.0.0" is invalid.
bool is_version_valid(Version version);

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
