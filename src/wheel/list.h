#pragma once

#include "core.h"
#include "list/types.h"
#include "str.h"

extern const usize LIST_DEFAULT_SIZE;
extern const usize LIST_MAX_INCREASE;

Ulist *ulist_new();
Ulist *ulist_new_with_size(usize size);
Ulist *ulist_clone(const Ulist *list);
usize ulist_get(const Ulist *list, usize i);
void ulist_set(Ulist *list, usize i, usize val);
void ulist_insert(Ulist *list, usize i, usize val);
void ulist_push(Ulist *list, usize val);
void ulist_pop(Ulist *list);
usize ulist_delete(Ulist *list, usize i);
bool ulist_is_empty(Ulist *list);
void ulist_free(Ulist *list);
Ulist *ulist_from(usize *array, usize size);
bool ulist_equal(Ulist *a, Ulist *b);
void ulist_shuffle(Ulist *list);
void ulist_print(Ulist *list);

Ilist *ilist_new();
Ilist *ilist_new_with_size(usize size);
Ilist *ilist_clone(const Ilist *list);
isize ilist_get(const Ilist *list, usize i);
void ilist_set(Ilist *list, usize i, isize val);
void ilist_insert(Ilist *list, usize i, isize val);
void ilist_push(Ilist *list, isize val);
void ilist_pop(Ilist *list);
isize ilist_delete(Ilist *list, usize i);
bool ilist_is_empty(Ilist *list);
void ilist_free(Ilist *list);
Ilist *ilist_from(isize *array, usize size);
bool ilist_equal(Ilist *a, Ilist *b);
void ilist_shuffle(Ilist *list);
void ilist_print(Ilist *list);

Slist *slist_new();
Slist *slist_new_with_size(usize size);
Slist *slist_clone(const Slist *list);
Str *slist_get(const Slist *list, usize i);
void slist_set(Slist *list, usize i, Str *s);
void slist_insert(Slist *list, usize i, Str *s);
void slist_push(Slist *list, Str *s);
void slist_pop(Slist *list);
// NOTE: remember to free the return value
Str *slist_delete(Slist *list, usize i);
bool slist_is_empty(Slist *list);
Slist *slist_from(Str **array, usize size);
// NOTE: this function is implemented but should not be used
// bool slist_equal(Slist *a, Slist *b);

// NOTE: strings in the list should be freed first. See also SLIST_FREE().
void slist_free(Slist *list);
void slist_shuffle(Slist *list);
// Empty the list and free the strings.
void slist_clear(Slist *list);

#define SLIST_FREE(list) \
	slist_clear(list); \
	slist_free(list); \
	list = NULL;
