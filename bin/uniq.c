#include <stdio.h>
#include <linear.h>

void uniq(IsizeList *list) {
	usize i = 0, j = 0;

	while (j < list->length) {
		while(j < list->length && isize_list_get(list, i) == isize_list_get(list, j)) {
			++j;
		}
		if (j < list->length) {
			isize_list_set(list, ++i, isize_list_get(list, j));
		}
	}

	list->length = i + 1;
}

int main() {
	isize n = 100;
	IsizeList list = isize_list_new();

	for (isize i = 0; i <= n; ++i) {
		for (isize j = 0; j < i; ++j) {
			isize_list_insert(&list, list.length, i);
		}
	}

	uniq(&list);

	for (usize i = 0; i < list.length; ++i) {
		printf("%llu\n", isize_list_get(&list, i));
	}
}
