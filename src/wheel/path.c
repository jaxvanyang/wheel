#include "path.h"
#include "sys.h"
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

Str *get_home_dir() {
	OSType os = get_os();

	if (os == OS_WINDOWS) {
		unimplemented();
	}

	return str_from(getenv("HOME"));
}

Str *get_config_dir() {
	OSType os = get_os();

	if (os == OS_WINDOWS) {
		unimplemented();
	}

	Str *ret = get_home_dir();

	if (os == OS_MACOS) {
		path_join(ret, "Library/Application Support");
	} else {
		path_join(ret, ".config");
	}

	return ret;
}

Str *get_cache_dir() {
	OSType os = get_os();

	if (os == OS_WINDOWS) {
		unimplemented();
	}

	Str *ret = get_home_dir();

	if (os == OS_MACOS) {
		path_join(ret, "Library/Caches");
	} else {
		path_join(ret, ".cache");
	}

	return ret;
}

Str *get_data_dir() {
	OSType os = get_os();

	if (os == OS_WINDOWS) {
		unimplemented();
	}

	Str *ret = get_home_dir();

	if (os == OS_MACOS) {
		path_join(ret, "Library/Application Support");
	} else {
		path_join(ret, ".local/share");
	}

	return ret;
}
