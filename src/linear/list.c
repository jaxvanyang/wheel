#include "list.h"
#include <stdlib.h>
#include <string.h>

const usize LIST_DEFAULT_SIZE = 128;
const usize LIST_MAX_INCREASE = 1024;

UsizeList usize_list_new() {
	UsizeList list;

	list.length = 0;
	list.size = LIST_DEFAULT_SIZE;
	list.start = malloc(list.size * sizeof(usize));
	memset(list.start, 0x00, list.size * sizeof(usize));

	return list;
}

UsizeList usize_list_new_with_size(usize size) {
	UsizeList list;

	list.length = size;
	list.size = size;
	list.start = malloc(list.size * sizeof(usize));
	memset(list.start, 0x00, list.size * sizeof(usize));

	return list;
}

usize usize_list_get(UsizeList *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	return list->start[i];
}

void usize_list_set(UsizeList *list, usize i, usize val) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	list->start[i] = val;
}

void usize_list_insert(UsizeList *list, usize i, usize val) {
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
		usize_list_set(list, j, usize_list_get(list, j - 1));
	}

	usize_list_set(list, i, val);
}

usize usize_list_delete(UsizeList *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	usize ret = usize_list_get(list, i);

	for (usize j = i; j < list->length - 1; ++j) {
		usize_list_set(list, j, usize_list_get(list, j + 1));
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

bool usize_list_is_empty(UsizeList *list) {
	return list->length == 0;
}

void usize_list_free(UsizeList *list) {
	list->size = 0;
	list->length = 0;
	free(list->start);
	list->start = NULL;
}

IsizeList isize_list_new() {
	IsizeList list;

	list.length = 0;
	list.size = LIST_DEFAULT_SIZE;
	list.start = malloc(list.size * sizeof(isize));
	memset(list.start, 0x00, list.size * sizeof(isize));

	return list;
}

IsizeList isize_list_new_with_size(usize size) {
	IsizeList list;

	list.length = size;
	list.size = size;
	list.start = malloc(list.size * sizeof(isize));
	memset(list.start, 0x00, list.size * sizeof(isize));

	return list;
}

isize isize_list_get(IsizeList *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}
	return list->start[i];
}

void isize_list_set(IsizeList *list, usize i, isize val) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}
	list->start[i] = val;
}

void isize_list_insert(IsizeList *list, usize i, isize val) {
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
		isize_list_set(list, j, isize_list_get(list, j - 1));
	}

	isize_list_set(list, i, val);
}

isize isize_list_delete(IsizeList *list, usize i) {
	if (i >= list->length) {
		error("expected i < length, found: %llu >= %llu\n", i, list->length);
	}

	isize ret = isize_list_get(list, i);

	for (usize j = i; j < list->length - 1; ++j) {
		isize_list_set(list, j, isize_list_get(list, j + 1));
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

bool isize_list_is_empty(IsizeList *list) {
	return list->length == 0;
}

void isize_list_free(IsizeList *list) {
	list->size = 0;
	list->length = 0;
	free(list->start);
	list->start = NULL;
}
