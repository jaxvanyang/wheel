#include "sort.h"

#include <string.h>

void _ulist_qsort(Ulist *list, usize begin, usize end) {
	if (end <= begin + 1)
		return;

	usize i = begin, j = end - 1;
	usize pivot = ulist_get(list, i);

	while (i < j) {
		while (i < j && ulist_get(list, j) >= pivot)
			--j;
		ulist_set(list, i, ulist_get(list, j));

		while (i < j && ulist_get(list, i) <= pivot)
			++i;
		ulist_set(list, j, ulist_get(list, i));
	}

	ulist_set(list, i, pivot);

	_ulist_qsort(list, begin, i);
	_ulist_qsort(list, i + 1, end);
}

void ulist_qsort(Ulist *list) {
	ulist_shuffle(list);
	_ulist_qsort(list, 0, list->length);
}

void _ilist_qsort(Ilist *list, usize begin, usize end) {
	if (end <= begin + 1)
		return;

	usize i = begin, j = end - 1;
	usize pivot = ilist_get(list, i);

	while (i < j) {
		while (i < j && ilist_get(list, j) >= pivot)
			--j;
		ilist_set(list, i, ilist_get(list, j));

		while (i < j && ilist_get(list, i) <= pivot)
			++i;
		ilist_set(list, j, ilist_get(list, i));
	}

	ilist_set(list, i, pivot);

	_ilist_qsort(list, begin, i);
	_ilist_qsort(list, i + 1, end);
}

void ilist_qsort(Ilist *list) {
	ilist_shuffle(list);
	_ilist_qsort(list, 0, list->length);
}

void _slist_qsort(Slist *list, usize begin, usize end) {
	if (end <= begin + 1)
		return;

	usize i = begin, j = end - 1;
	Str *pivot = slist_get(list, i);

	while (i < j) {
		while (i < j && strcmp(slist_get(list, j)->data, pivot->data) >= 0)
			--j;
		slist_set(list, i, slist_get(list, j));

		while (i < j && strcmp(slist_get(list, i)->data, pivot->data) <= 0)
			++i;
		slist_set(list, j, slist_get(list, i));
	}

	slist_set(list, i, pivot);

	_slist_qsort(list, begin, i);
	_slist_qsort(list, i + 1, end);
}

void slist_qsort(Slist *list) {
	slist_shuffle(list);
	_slist_qsort(list, 0, list->length);
}
