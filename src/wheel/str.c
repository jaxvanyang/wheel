#include "str.h"

#include <errno.h>
#include <string.h>

#include "list.h"

Ilist *build_next(char *t) {
	usize len = strlen(t);
	Ilist *next = ilist_new();

	ilist_push(next, -1);

	usize i = 1;
	isize j = -1;

	while (i < len) {
		if (j == -1 || t[j] == t[i - 1]) {
			ilist_push(next, ++j);
			i += 1;
		} else {
			j = ilist_get(next, j);
		}
	}

	return next;
}

Ilist *build_nextval(char *t) {
	usize len = strlen(t);
	Ilist *nextval = ilist_new();

	ilist_push(nextval, -1);

	usize i = 1;
	isize j = -1;

	while (i < len) {
		if (j == -1 || t[j] == t[i - 1]) {
			++j;
			if (t[j] == t[i]) {
				ilist_push(nextval, ilist_get(nextval, j));
			} else {
				ilist_push(nextval, j);
			}
			++i;
		} else {
			j = ilist_get(nextval, j);
		}
	}

	return nextval;
}

isize kmp(char *s, char *t) {
	isize slen = strlen(s);
	isize tlen = strlen(t);

	Ilist *next = build_nextval(t);

	isize i = 0, j = 0;

	while (i < slen) {
		if (j == -1 || s[i] == t[j]) {
			++i, ++j;

			if (j == tlen) {
				return i - tlen;
			}
		} else {
			j = ilist_get(next, j);
		}
	}

	return -1;
}

bool str_start_with(const char *s, const char *pattern) {
	usize s_len = strlen(s), p_len = strlen(pattern);

	if (s_len < p_len) {
		return false;
	}

	for (usize i = 0; i < p_len; ++i) {
		if (s[i] != pattern[i]) {
			return false;
		}
	}

	return true;
}

bool str_end_with(const char *s, const char *pattern) {
	usize s_len = strlen(s), p_len = strlen(pattern);

	if (s_len < p_len) {
		return false;
	}

	usize diff = s_len - p_len;
	for (usize i = 0; i < p_len; ++i) {
		if (s[i + diff] != pattern[i]) {
			return false;
		}
	}

	return true;
}

void str_remove_prefix(char *s, const char *pattern) {
	if (!str_start_with(s, pattern)) {
		return;
	}

	usize s_len = strlen(s), p_len = strlen(pattern);
	usize new_len = s_len - p_len;
	for (usize i = 0; i < new_len + 1; ++i) {
		s[i] = s[i + p_len];
	}
}

void str_remove_postfix(char *s, const char *pattern) {
	if (!str_end_with(s, pattern)) {
		return;
	}

	usize s_len = strlen(s), p_len = strlen(pattern);
	usize new_len = s_len - p_len;
	s[new_len] = '\0';
}

Str *str_new_with_size(usize size) {
	Str *ret = malloc(sizeof(Str));

	ret->length = 0;
	ret->size = size;
	ret->data = malloc(ret->size * sizeof(char));
	memset(ret->data, 0x00, ret->size * sizeof(char));

	return ret;
}

Str *str_new() { return str_new_with_size(LIST_DEFAULT_SIZE); }

Str *str_from(const char *s) {
	Str *str = malloc(sizeof(Str));

	str->length = strlen(s);
	str->size = str->length + 1;

	str->data = malloc(sizeof(char) * str->size);
	strcpy(str->data, s);

	return str;
}

void str_free(Str *s) {
	free(s->data);
	s->data = NULL;
	s->size = s->length = 0;

	free(s);
}

void str_insert(Str *s, const usize i, const char c) {
	if (i > s->length) {
		lol_term("expected i <= length, found: %zu > %zu\n", i, s->length);
	}

	if (s->size <= s->length + 1) {
		usize new_size =
			s->size + (s->size < LIST_MAX_INCREASE ? s->size : LIST_MAX_INCREASE);
		char *new_data = malloc(new_size * sizeof(char));
		memcpy(new_data, s->data, s->size * sizeof(char));
		free(s->data);
		s->data = new_data;
		s->size = new_size;
	}

	s->length += 1;

	for (usize j = s->length - 1; j > i; --j) {
		s->data[j] = s->data[j - 1];
	}

	s->data[s->length] = '\0';
	s->data[i] = c;
}

void str_insert_str(Str *s, const usize i, const char *t) {
	usize len = strlen(t);
	for (usize j = 0; j < len; ++j) {
		str_insert(s, i + j, t[j]);
	}
}

void str_push(Str *s, char c) { str_insert(s, s->length, c); }

void str_push_str(Str *s, const char *t) {
	usize len = strlen(t);

	for (usize i = 0; i < len; ++i) {
		str_push(s, t[i]);
	}
}

char str_delete(Str *s, usize i) {
	if (i >= s->length) {
		lol_term("expected i < length, found: %zu >= %zu\n", i, s->length);
	}

	char ret = s->data[i];

	for (usize j = i; j <= s->length; ++j) {
		s->data[j] = s->data[j + 1];
	}

	s->length -= 1;

	if (s->size > (s->length + 1) * 2) {
		usize new_size = s->size / 2;
		char *new_data = malloc(new_size * sizeof(char));
		memcpy(new_data, s->data, (s->length + 1) * sizeof(char));
		free(s->data);
		s->data = new_data;
		s->size = new_size;
	}

	return ret;
}

void str_readline(Str *s, FILE *f) {
	s->length = 0;
	s->data[0] = '\0';

	char c;

	while ((c = fgetc(f)) != EOF) {
		str_push(s, c);

		if (c == '\n') {
			break;
		}
	}

	if (c == EOF && ferror(f)) {
		lol_error_e(NULL);
		exit(errno);
	}
}
