#include <stdio.h>
#include <basics/list.h>

void uniq(Ilist *list) {
	usize i = 0, j = 0;

	while (j < list->length) {
		while(j < list->length && ilist_get(list, i) == ilist_get(list, j)) {
			++j;
		}
		if (j < list->length) {
			ilist_set(list, ++i, ilist_get(list, j));
		}
	}

	list->length = i + 1;
}

int main() {
	isize n = 100;
	Ilist list = ilist_new();

	for (isize i = 0; i <= n; ++i) {
		for (isize j = 0; j < i; ++j) {
			ilist_insert(&list, list.length, i);
		}
	}

	uniq(&list);

	for (usize i = 0; i < list.length; ++i) {
		printf("%llu\n", ilist_get(&list, i));
	}
}
