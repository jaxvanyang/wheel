#include "list.h"
#include "str.h"
#include <string.h>
#include <sys/errno.h>

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

Str *str_new() {
	Str *s = malloc(sizeof(Str));

	s->length = 0;
	s->size = LIST_DEFAULT_SIZE;
	s->data = malloc(s->size * sizeof(char));
	memset(s->data, 0x00, s->size * sizeof(char));

	return s;
}

Str *str_from(char *s) {
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

void str_insert(Str *s, usize i, char c) {
	if (i > s->length) {
		error("expected i <= length, found: %zu > %zu\n", i, s->length);
	}

	if (s->size <= s->length + 1) {
		usize new_size = s->size + (s->size < LIST_MAX_INCREASE ? s->size : LIST_MAX_INCREASE);
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

void str_insert_str(Str *s, usize i, char *t) {
	usize len = strlen(t);
	for (usize j = 0; j < len; ++j) {
		str_insert(s, i + j, t[j]);
	}
}

void str_push(Str *s, char c) {
	str_insert(s, s->length, c);
}

void str_push_str(Str *s, char *t) {
	usize len = strlen(t);

	for (usize i = 0; i < len; ++i) {
		str_push(s, t[i]);
	}
}

usize str_delete(Str *s, usize i) {
	if (i >= s->length) {
		error("expected i < length, found: %zu >= %zu\n", i, s->length);
	}

	char ret = s->data[i];

	for (usize j = i; j < s->length; ++j) {
		s->data[j] = s->data[j + 1];
	}

	s->length -= 1;

	if (s->size > (s->length) * 2) {
		usize new_size = s->size / 2;
		char *new_data = malloc(new_size * sizeof(char));
		memcpy(s->data, new_data, s->length * sizeof(char));
		s->data[s->length] = '\0';
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
		perror(NULL);
		exit(errno);
	}
}
