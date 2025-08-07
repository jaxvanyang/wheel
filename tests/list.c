#include <assert.h>
#include <basics/list.h>
#include <stdio.h>

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
	}

	{
		isize array[] = {1, 2, 3, 4, 5};
		isize size = sizeof(array) / sizeof(isize);
		Ilist *a = ilist_from(array, size);
		Ilist *b = ilist_new();

		for (usize i = 0; i < size; ++i) {
			assert(array[i] == ilist_get(a, i));
			ilist_push(b, array[i]);
		}

		assert(ilist_equal(a, b));
	}
}
