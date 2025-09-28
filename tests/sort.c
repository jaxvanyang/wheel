#include <assert.h>
#include <wheel/sort.h>

int main() {
	{
		usize n = 100;
		Ulist *list = ulist_new();
		for (usize i = 0; i < n; ++i) {
			ulist_push(list, i);
		}

		ulist_shuffle(list);

		ulist_qsort(list);
		for (usize i = 0; i < list->len; ++i) {
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

		ilist_qsort(list);
		for (usize i = 0; i < list->len; ++i) {
			assert(ilist_get(list, i) == (isize)i);
		}
	}
}
