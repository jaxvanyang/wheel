#include "linear.h"
#include "str.h"
#include <string.h>

IsizeList build_next(char *t) {
	usize len = strlen(t);
	IsizeList next = isize_list_new_with_size(len);

	isize_list_set(&next, 0, -1);

	usize i = 1;
	isize j = -1;
	
	while (i < len) {
		if (j == -1 || t[j] == t[i - 1]) {
			isize_list_set(&next, i++, ++j);
		} else {
			j = isize_list_get(&next, j);
		}
	}

	return next;
}

IsizeList build_nextval(char *t) {
	usize len = strlen(t);
	IsizeList nextval = isize_list_new_with_size(len);

	isize_list_set(&nextval, 0, -1);

	usize i = 1;
	isize j = -1;
	
	while (i < len) {
		if (j == -1 || t[j] == t[i - 1]) {
			++j;
			if (t[j] == t[i]) {
				isize_list_set(&nextval, i, isize_list_get(&nextval, j));
			} else {
				isize_list_set(&nextval, i, j);
			}
			++i;
		} else {
			j = isize_list_get(&nextval, j);
		}
	}

	return nextval;
}

isize kmp(char *s, char *t) {
	isize slen = strlen(s);
	isize tlen = strlen(t);

	IsizeList next = build_nextval(t);

	isize i = 0, j = 0;

	while (i < slen) {
		if (j == -1 || s[i] == t[j]) {
			++i, ++j;

			if (j == tlen) {
				return i - tlen;
			}
		} else {
			j = isize_list_get(&next, j);
		}
	}

	return -1;
}
