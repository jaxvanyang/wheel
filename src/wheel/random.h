#pragma once

#include "core.h"

// Use current time as random seed
void tsrandom();
usize random_usize();
isize random_isize();
f64 random_f64();
usize random_range(usize begin, usize end);
