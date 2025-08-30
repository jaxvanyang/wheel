#include <wheel.h>
#include <string.h>

int main() {
	Str *last = str_new();

	while (true) {
		Str *s = str_new();
		str_readline(s, stdin);

		if (s->length == 0) {
			break;
		}

		if (strcmp(last->data, s->data) != 0) {
			printf("%s", s->data);
			str_free(last);
			last = s;
		}
	}

	str_free(last);
}
