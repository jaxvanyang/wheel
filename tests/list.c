#include <assert.h>
#include <stdio.h>
#include <wheel/list.h>

int main() {
	{
		usize array[] = {1, 2, 3, 4, 5};
		usize size = sizeof(array) / sizeof(usize);
		Ulist *a = ulist_from(array, size);
		Ulist *b = ulist_new();

		for (usize i = 0; i < size; ++i) {
			assert(array[i] == ulist_get(a, i));
			ulist_push(b, array[i]);
		}

		assert(ulist_equal(a, b));

		ulist_free(a);
		ulist_free(b);
	}

	{
		isize array[] = {1, 2, 3, 4, 5};
		usize size = sizeof(array) / sizeof(isize);
		Ilist *a = ilist_from(array, size);
		Ilist *b = ilist_new();

		for (usize i = 0; i < size; ++i) {
			assert(array[i] == ilist_get(a, i));
			ilist_push(b, array[i]);
		}

		assert(ilist_equal(a, b));

		ilist_free(a);
		ilist_free(b);
	}

	{
		usize array[] = {1, 2, 3, 4, 5};
		usize size = sizeof(array) / sizeof(usize);
		Ulist *list = ulist_from(array, size);

		ulist_insert(list, 0, 0);
		for (usize i = 0; i < list->len; ++i) {
			assert(ulist_get(list, i) == i);
		}

		for (usize len = list->len, i = 0; i < len; ++i) {
			assert(ulist_delete(list, 0) == i);
		}

		ulist_free(list);
	}
}
