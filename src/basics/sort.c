#include "sort.h"

void _qsort_u(Ulist *list, usize begin, usize end) {
	if (end <= begin + 1) return;

	usize i = begin, j = end - 1;
	usize pivot = ulist_get(list, i);
	
	while (i < j) {
		while (i < j && ulist_get(list, j) >= pivot) --j;
		ulist_set(list, i, ulist_get(list, j));

		while (i < j && ulist_get(list, i) <= pivot) ++i;
		ulist_set(list, j, ulist_get(list, i));
	}

	ulist_set(list, i, pivot);

	_qsort_u(list, begin, i);
	_qsort_u(list, i + 1, end);
}

void qsort_u(Ulist *list) {
	ulist_shuffle(list);
	_qsort_u(list, 0, list->length);
}

void _qsort_i(Ilist *list, usize begin, usize end) {
	if (end <= begin + 1) return;

	usize i = begin, j = end - 1;
	usize pivot = ilist_get(list, i);
	
	while (i < j) {
		while (i < j && ilist_get(list, j) >= pivot) --j;
		ilist_set(list, i, ilist_get(list, j));

		while (i < j && ilist_get(list, i) <= pivot) ++i;
		ilist_set(list, j, ilist_get(list, i));
	}

	ilist_set(list, i, pivot);

	_qsort_i(list, begin, i);
	_qsort_i(list, i + 1, end);
}

void qsort_i(Ilist *list) {
	ilist_shuffle(list);
	_qsort_i(list, 0, list->length);
}
