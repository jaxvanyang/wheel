#include <assert.h>
#include <basics/list.h>
#include <stdio.h>

int main() {
	{
		usize array[] = {1, 2, 3, 4, 5};
		usize size = sizeof(array) / sizeof(usize);
		Ulist *list = ulist_from(array, size);

		for (usize i = 0; i < size; ++i) {
			assert(array[i] == ulist_get(list, i));
		}
	}

	{
		isize array[] = {1, 2, 3, 4, 5};
		isize size = sizeof(array) / sizeof(isize);
		Ilist *list = ilist_from(array, size);

		for (usize i = 0; i < size; ++i) {
			assert(array[i] == ilist_get(list, i));
		}
	}
}
