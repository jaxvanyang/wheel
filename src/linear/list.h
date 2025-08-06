#pragma once

#include "../core.h"
#include <stdbool.h>

extern const usize LIST_DEFAULT_SIZE;
extern const usize LIST_MAX_INCREASE;

typedef struct {
	usize size;
	usize length;
	usize *start;
} UsizeList;

UsizeList usize_list_new();
UsizeList usize_list_new_with_size(usize size);
usize usize_list_get(UsizeList *list, usize i);
void usize_list_set(UsizeList *list, usize i, usize val);
void usize_list_insert(UsizeList *list, usize i, usize val);
usize usize_list_delete(UsizeList *list, usize i);
bool usize_list_is_empty(UsizeList *list);
void usize_list_free(UsizeList *list);

typedef struct {
	usize size;
	usize length;
	isize *start;
} IsizeList;

IsizeList isize_list_new();
IsizeList isize_list_new_with_size(usize size);
isize isize_list_get(IsizeList *list, usize i);
void isize_list_set(IsizeList *list, usize i, isize val);
void isize_list_insert(IsizeList *list, usize i, isize val);
isize isize_list_delete(IsizeList *list, usize i);
bool isize_list_is_empty(IsizeList *list);
void isize_list_free(IsizeList *list);
