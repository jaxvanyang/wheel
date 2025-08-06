#pragma once

#include "./types.h"
#include <sys/time.h>

// elapsed time in micro seconds
f64 elapsed(struct timeval t0, struct timeval t1);
