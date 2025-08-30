#pragma once

#include <stdio.h>
#include <stdlib.h>

#define error(...)                                                                     \
	fprintf(stderr, "%s:%d: ", __FILE__, __LINE__);                                      \
	fprintf(stderr, __VA_ARGS__);                                                        \
	exit(1)
