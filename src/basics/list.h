#pragma once

#include "core.h"

extern const usize LIST_DEFAULT_SIZE;
extern const usize LIST_MAX_INCREASE;

typedef struct {
	usize size;
	usize length;
	usize *start;
} Ulist;

Ulist *ulist_new();
Ulist *ulist_new_with_size(usize size);
usize ulist_get(Ulist *list, usize i);
void ulist_set(Ulist *list, usize i, usize val);
void ulist_insert(Ulist *list, usize i, usize val);
void ulist_push(Ulist *list, usize val);
usize ulist_delete(Ulist *list, usize i);
bool ulist_is_empty(Ulist *list);
void ulist_free(Ulist *list);

typedef struct {
	usize size;
	usize length;
	isize *start;
} Ilist;

Ilist *ilist_new();
Ilist *ilist_new_with_size(usize size);
isize ilist_get(Ilist *list, usize i);
void ilist_set(Ilist *list, usize i, isize val);
void ilist_insert(Ilist *list, usize i, isize val);
void ilist_push(Ilist *list, isize val);
isize ilist_delete(Ilist *list, usize i);
bool ilist_is_empty(Ilist *list);
void ilist_free(Ilist *list);
