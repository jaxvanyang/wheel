#include <assert.h>
#include <string.h>
#include <wheel/list.h>
#include <wheel/str.h>

int main() {
	{
		Str *s = str_from("hello");
		assert(s->len == 5);

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

	{
		const char *s = "    a ab \t abc \n abcd   ";
		Slist *list = str_split(s);

		assert(list->len == 4);
		assert(strcmp(slist_get(list, 0)->data, "a") == 0);
		assert(strcmp(slist_get(list, 1)->data, "ab") == 0);
		assert(strcmp(slist_get(list, 2)->data, "abc") == 0);
		assert(strcmp(slist_get(list, 3)->data, "abcd") == 0);

		SLIST_FREE(list);
	}

	{
		Str *s = str_from(" abc  ");
		str_lstrip(s);

		assert(strcmp(s->data, "abc  ") == 0);

		str_free(s);
	}

	{
		Str *s = str_from(" abc  ");
		str_rstrip(s);

		assert(strcmp(s->data, " abc") == 0);

		str_free(s);
	}

	{
		Str *s = str_from(" abc  ");
		str_strip(s);

		assert(strcmp(s->data, "abc") == 0);

		str_free(s);
	}

	{
		Str *s = str_from("      ");
		str_strip(s);

		assert(strcmp(s->data, "") == 0);

		str_free(s);
	}

	{
		Str *s = str_from("abc");
		str_strip(s);

		assert(strcmp(s->data, "abc") == 0);

		str_free(s);
	}
}
