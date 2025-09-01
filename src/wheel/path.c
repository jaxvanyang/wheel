#include "path.h"
#include "str.h"
#include <string.h>

char *os_path(const char *path) {
#ifdef _WIN32
	Str *str = str_new();

	for (usize i = 0, len = strlen(path); i < len; ++i) {
		str_push(str, path[i] == '/' ? '\\' : path[i]);
	}

	char *s = str->data;
	str->length = str->size = 0;
	free(str);
#else
	char *s = malloc(sizeof(char) * (strlen(path) + 1));
	strcpy(s, path);
#endif

	return s;
}
