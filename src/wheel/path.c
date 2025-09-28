#include "path.h"
#include <string.h>

char *os_path(const char *path) {
#ifdef _WIN32
	Str *str = str_new();

	for (usize i = 0, len = strlen(path); i < len; ++i) {
		str_push(str, path[i] == '/' ? '\\' : path[i]);
	}

	char *s = str->data;
	str->len = str->size = 0;
	free(str);
#else
	char *s = malloc(sizeof(char) * (strlen(path) + 1));
	strcpy(s, path);
#endif

	return s;
}

void path_join(Str *path, const char *component) {
#ifdef _WIN32
	str_push(path, '\\');
#else
	str_push(path, '/');
#endif
	str_push_str(path, component);
}
