#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "random.h"

const usize LIST_DEFAULT_SIZE = 128;
const usize LIST_MAX_INCREASE = 1024;

#define VECTOR_IMPLEMENTATION(T, Name, prefix) \
\
	Name *prefix##_new() { \
		Name *list = malloc(sizeof(Name)); \
\
		list->length = 0; \
		list->size = LIST_DEFAULT_SIZE; \
		list->data = malloc(list->size * sizeof(T)); \
		memset(list->data, 0x00, list->size * sizeof(T)); \
\
		return list; \
	} \
\
	Name *prefix##_new_with_size(usize size) { \
		Name *list = malloc(sizeof(Name)); \
\
		list->length = 0; \
		list->size = size; \
		list->data = malloc(list->size * sizeof(T)); \
		memset(list->data, 0x00, list->size * sizeof(T)); \
\
		return list; \
	} \
\
	T prefix##_get(Name *list, usize i) { \
		if (i >= list->length) { \
			error("expected i < length, found: %zu >= %zu\n", i, list->length); \
		} \
\
		return list->data[i]; \
	} \
\
	void prefix##_set(Name *list, usize i, T val) { \
		if (i >= list->length) { \
			error("expected i < length, found: %zu >= %zu\n", i, list->length); \
		} \
\
		list->data[i] = val; \
	} \
\
	void prefix##_insert(Name *list, usize i, T val) { \
		if (i > list->length) { \
			error("expected i <= length, found: %zu > %zu\n", i, list->length); \
		} \
\
		if (list->size <= list->length) { \
			usize new_size = \
				list->size + \
				(list->size < LIST_MAX_INCREASE ? list->size : LIST_MAX_INCREASE); \
			T *new_data = malloc(new_size * sizeof(T)); \
			memcpy(new_data, list->data, list->size * sizeof(T)); \
			free(list->data); \
			list->data = new_data; \
			list->size = new_size; \
		} \
\
		list->length += 1; \
\
		for (usize j = list->length - 1; j > i; --j) { \
			prefix##_set(list, j, prefix##_get(list, j - 1)); \
		} \
\
		prefix##_set(list, i, val); \
	} \
\
	void prefix##_push(Name *list, T val) { prefix##_insert(list, list->length, val); } \
\
	T prefix##_delete(Name *list, usize i) { \
		if (i >= list->length) { \
			error("expected i < length, found: %zu >= %zu\n", i, list->length); \
		} \
\
		T ret = prefix##_get(list, i); \
\
		for (usize j = i; j < list->length - 1; ++j) { \
			prefix##_set(list, j, prefix##_get(list, j + 1)); \
		} \
\
		list->length -= 1; \
\
		if (list->size > list->length * 2) { \
			usize new_size = list->size / 2; \
			T *new_data = malloc(new_size * sizeof(T)); \
			memcpy(new_data, list->data, list->length * sizeof(T)); \
			free(list->data); \
			list->data = new_data; \
			list->size = new_size; \
		} \
\
		return ret; \
	} \
\
	bool prefix##_is_empty(Name *list) { return list->length == 0; } \
\
	void prefix##_free(Name *list) { \
		list->size = 0; \
		list->length = 0; \
		free(list->data); \
		list->data = NULL; \
		free(list); \
	} \
\
	Name *prefix##_from(T *array, usize size) { \
		Name *list = prefix##_new_with_size(size); \
\
		for (usize i = 0; i < size; ++i) { \
			prefix##_push(list, array[i]); \
		} \
\
		return list; \
	} \
\
	bool prefix##_equal(Name *a, Name *b) { \
		if (a->length != b->length) { \
			return false; \
		} \
\
		for (usize i = 0; i < a->length; ++i) { \
			if (prefix##_get(a, i) != prefix##_get(b, i)) { \
				return false; \
			} \
		} \
\
		return true; \
	} \
\
	void prefix##_shuffle(Name *list) { \
		for (usize i = 1; i < list->length; ++i) { \
			usize j = random_range(i, list->length); \
			T tmp = prefix##_get(list, i - 1); \
			prefix##_set(list, i - 1, prefix##_get(list, j)); \
			prefix##_set(list, j, tmp); \
		} \
	}

VECTOR_IMPLEMENTATION(usize, Ulist, ulist)

void ulist_print(Ulist *list) {
	printf("{");
	if (list->length > 0) {
		printf("%zu", ulist_get(list, 0));
	}
	for (usize i = 1; i < list->length; ++i) {
		printf(", %zu", ulist_get(list, i));
	}
	printf("}\n");
}

VECTOR_IMPLEMENTATION(isize, Ilist, ilist)

void ilist_print(Ilist *list) {
	printf("{");
	if (list->length > 0) {
		printf("%ld", ilist_get(list, 0));
	}
	for (usize i = 1; i < list->length; ++i) {
		printf(", %ld", ilist_get(list, i));
	}
	printf("}");
}

VECTOR_IMPLEMENTATION(Str *, Slist, slist)
