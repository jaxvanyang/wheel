#include <assert.h>
#include <string.h>
#include <wheel/str.h>

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

	{
		char s[] = "abcdefg";
		assert(str_start_with(s, "abc"));
		assert(str_end_with(s, "efg"));

		str_remove_prefix(s, "abc");
		assert(strcmp(s, "defg") == 0);
		str_remove_postfix(s, "efg");
		assert(strcmp(s, "d") == 0);

		str_remove_prefix(s, "x");
		assert(strcmp(s, "d") == 0);
		str_remove_postfix(s, "x");
		assert(strcmp(s, "d") == 0);
	}
}
