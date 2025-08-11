#include <basics.h>

int main() {
	Slist *list = slist_new();

	while (true) {
		Str *s = str_new();
		str_readline(s, stdin);

		if (s->length == 0) {
			break;
		}

		str_delete(s, s->length - 1);

		slist_push(list, s);
	}

	qsort_s(list);

	for (usize i = 0; i < list->length; ++i) {
		Str *s = slist_get(list, i);
		printf("%s\n", s->data);
	}
}
