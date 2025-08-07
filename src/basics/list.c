#include "list.h"
#include <stdlib.h>
#include <string.h>

const usize LIST_DEFAULT_SIZE = 128;
const usize LIST_MAX_INCREASE = 1024;

Ulist *ulist_new() {
	Ulist *list = malloc(sizeof(Ulist));

	list->length = 0;
	list->size = LIST_DEFAULT_SIZE;
	list->start = malloc(list->size * sizeof(usize));
	memset(list->start, 0x00, list->size * sizeof(usize));

	return list;
}

Ulist *ulist_new_with_size(usize size) {
	Ulist *list = malloc(sizeof(Ulist));

	list->length = 0;
	list->size = size;
	list->start = malloc(list->size * sizeof(usize));
	memset(list->start, 0x00, list->size * sizeof(usize));

	return list;
}

usize ulist_get(Ulist *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	return list->start[i];
}

void ulist_set(Ulist *list, usize i, usize val) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	list->start[i] = val;
}

void ulist_insert(Ulist *list, usize i, usize val) {
	if (i > list->length) {
		error("expected i <= length, found: %llu > %llu\n", i, list->length);
	}

	if (list->size < list->length + 1) {
		usize new_size = list->size + (list->size < LIST_MAX_INCREASE ? list->size : LIST_MAX_INCREASE);
		usize *new_start = malloc(new_size * sizeof(usize));
		memcpy(new_start, list->start, list->length * sizeof(usize));
		free(list->start);
		list->start = new_start;
		list->size = new_size;
	}

	list->length += 1;

	for (usize j = list->length - 1; j > i; --j) {
		ulist_set(list, j, ulist_get(list, j - 1));
	}

	ulist_set(list, i, val);
}

void ulist_push(Ulist *list, usize val) {
	ulist_insert(list, list->length, val);
}

usize ulist_delete(Ulist *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	usize ret = ulist_get(list, i);

	for (usize j = i; j < list->length - 1; ++j) {
		ulist_set(list, j, ulist_get(list, j + 1));
	}

	list->length -= 1;

	if (list->size > list->length * 2) {
		usize new_size = list->size / 2;
		usize *new_start = malloc(new_size * sizeof(usize));
		memcpy(list->start, new_start, list->length * sizeof(usize));
		free(list->start);
		list->start = new_start;
		list->size = new_size;
	}

	return ret;
}

bool ulist_is_empty(Ulist *list) {
	return list->length == 0;
}

void ulist_free(Ulist *list) {
	list->size = 0;
	list->length = 0;
	free(list->start);
	list->start = NULL;
	free(list);
}

Ulist *ulist_from(usize *array, usize size) {
	Ulist *list = ulist_new_with_size(size);

	for (usize i = 0; i < size; ++i) {
		ulist_push(list, array[i]);
	}

	return list;
}

bool ulist_equal(Ulist *a, Ulist *b) {
	if (a->length != b->length) {
		return false;
	}

	for (usize i = 0; i < a->length; ++i) {
		if (ulist_get(a, i) != ulist_get(b, i)) {
			return false;
		}
	}

	return true;
}

Ilist *ilist_new() {
	Ilist *list = malloc(sizeof(Ilist));

	list->length = 0;
	list->size = LIST_DEFAULT_SIZE;
	list->start = malloc(list->size * sizeof(isize));
	memset(list->start, 0x00, list->size * sizeof(isize));

	return list;
}

Ilist *ilist_new_with_size(usize size) {
	Ilist *list = malloc(sizeof(Ilist));

	list->length = 0;
	list->size = size;
	list->start = malloc(list->size * sizeof(isize));
	memset(list->start, 0x00, list->size * sizeof(isize));

	return list;
}

isize ilist_get(Ilist *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}
	return list->start[i];
}

void ilist_set(Ilist *list, usize i, isize val) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}
	list->start[i] = val;
}

void ilist_insert(Ilist *list, usize i, isize val) {
	if (i > list->length) {
		error("expected i <= length, found: %llu > %llu\n", i, list->length);
	}

	if (list->size < list->length + 1) {
		usize new_size = list->size + (list->size < LIST_MAX_INCREASE ? list->size : LIST_MAX_INCREASE);
		usize *new_start = malloc(new_size * sizeof(usize));
		memcpy(new_start, list->start, list->length * sizeof(usize));
		free(list->start);
		list->start = (isize *)new_start;
		list->size = new_size;
	}

	list->length += 1;

	for (usize j = list->length - 1; j > i; --j) {
		ilist_set(list, j, ilist_get(list, j - 1));
	}

	ilist_set(list, i, val);
}

void ilist_push(Ilist *list, isize val) {
	ilist_insert(list, list->length, val);
}

isize ilist_delete(Ilist *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	isize ret = ilist_get(list, i);

	for (usize j = i; j < list->length - 1; ++j) {
		ilist_set(list, j, ilist_get(list, j + 1));
	}

	list->length -= 1;

	if (list->size > list->length * 2) {
		usize new_size = list->size / 2;
		usize *new_start = malloc(new_size * sizeof(usize));
		memcpy(list->start, new_start, list->length * sizeof(usize));
		free(list->start);
		list->start = (isize *)new_start;
		list->size = new_size;
	}

	return ret;
}

bool ilist_is_empty(Ilist *list) {
	return list->length == 0;
}

void ilist_free(Ilist *list) {
	list->size = 0;
	list->length = 0;
	free(list->start);
	list->start = NULL;
	free(list);
}

Ilist *ilist_from(isize *array, usize size) {
	Ilist *list = ilist_new_with_size(size);

	for (usize i = 0; i < size; ++i) {
		ilist_push(list, array[i]);
	}

	return list;
}

bool ilist_equal(Ilist *a, Ilist *b) {
	if (a->length != b->length) {
		return false;
	}

	for (usize i = 0; i < a->length; ++i) {
		if (ilist_get(a, i) != ilist_get(b, i)) {
			return false;
		}
	}

	return true;
}
