#include <wheel.h>

int main() {
	Slist *list = slist_new();

	while (true) {
		Str *s = str_new();
		str_readline(s, stdin);

		if (s->len == 0) {
			break;
		}

		str_delete(s, s->len - 1);

		slist_push(list, s);
	}

	slist_qsort(list);

	for (usize i = 0; i < list->len; ++i) {
		Str *s = slist_get(list, i);
		printf("%s\n", s->data);
	}
}
