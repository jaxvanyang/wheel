#pragma once

#include "../core.h"
#include "../str/types.h"

typedef struct {
	usize size;
	usize length;
	usize *data;
} Ulist;

typedef struct {
	usize size;
	usize length;
	isize *data;
} Ilist;

typedef struct {
	usize size;
	usize length;
	Str **data;
} Slist;
