#include <assert.h>
#include <basics/str.h>
#include <string.h>

int main() {
	{
		Str *s = str_from("hello");
		assert(s->length == 5);

		str_push_str(s, " world");
		assert(strcmp(s->data, "hello world") == 0);

		assert('h' == str_delete(s, 0));
		assert(strcmp(s->data, "ello world") == 0);

		str_insert_str(s, 0, "c");
		assert(strcmp(s->data, "cello world") == 0);

		str_free(s);
	}
}
