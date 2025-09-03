#pragma once

#include <sys/time.h>

#include "./types.h"

typedef struct timeval TimeVal;

TimeVal time_now();

// elapsed time in micro seconds
f64 elapsed(TimeVal t0);

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define clamp(value, min, max) (value < min ? min : (value > max ? max : value))
