#include <assert.h>
#include <basics/sort.h>

int main() {
	{
		usize n = 100;
		Ulist *list = ulist_new();
		for (usize i = 0; i < n; ++i) {
			ulist_push(list, i);
		}

		ulist_shuffle(list);

		qsort_u(list);
		for (usize i = 0; i < list->length; ++i) {
			assert(ulist_get(list, i) == i);
		}
	}

	{
		usize n = 100;
		Ilist *list = ilist_new();
		for (usize i = 0; i < n; ++i) {
			ilist_push(list, i);
		}

		ilist_shuffle(list);

		qsort_i(list);
		for (usize i = 0; i < list->length; ++i) {
			assert(ilist_get(list, i) == i);
		}
	}
}
