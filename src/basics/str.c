#include "str.h"
#include <string.h>

Ilist build_next(char *t) {
	usize len = strlen(t);
	Ilist next = ilist_new_with_size(len);

	ilist_set(&next, 0, -1);

	usize i = 1;
	isize j = -1;
	
	while (i < len) {
		if (j == -1 || t[j] == t[i - 1]) {
			ilist_set(&next, i++, ++j);
		} else {
			j = ilist_get(&next, j);
		}
	}

	return next;
}

Ilist build_nextval(char *t) {
	usize len = strlen(t);
	Ilist nextval = ilist_new_with_size(len);

	ilist_set(&nextval, 0, -1);

	usize i = 1;
	isize j = -1;
	
	while (i < len) {
		if (j == -1 || t[j] == t[i - 1]) {
			++j;
			if (t[j] == t[i]) {
				ilist_set(&nextval, i, ilist_get(&nextval, j));
			} else {
				ilist_set(&nextval, i, j);
			}
			++i;
		} else {
			j = ilist_get(&nextval, j);
		}
	}

	return nextval;
}

isize kmp(char *s, char *t) {
	isize slen = strlen(s);
	isize tlen = strlen(t);

	Ilist next = build_nextval(t);

	isize i = 0, j = 0;

	while (i < slen) {
		if (j == -1 || s[i] == t[j]) {
			++i, ++j;

			if (j == tlen) {
				return i - tlen;
			}
		} else {
			j = ilist_get(&next, j);
		}
	}

	return -1;
}
