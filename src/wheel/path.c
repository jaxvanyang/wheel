#include "lol.h"
#include "path.h"
#include "sys.h"
#include <limits.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#elifdef __APPLE__
#include <mach-o/dyld.h>
#endif

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

Str *get_exe_path() {
	Str *ret = str_new_with_size(PATH_MAX);

#ifdef _WIN32
	if (GetModuleFileNameA(NULL, ret->data, PATH_MAX) == 0) {
		str_free(ret);
		return NULL;
	}
#elifdef __linux__
	ssize_t len = readlink("/proc/self/exe", ret->data, ret->size - 1);
	if (len == -1) {
		str_free(ret);
		return NULL;
	}
	ret->data[len] = '\0';
	ret->len = len;
#elifdef __APPLE__
	char path[PATH_MAX];
	uint32_t size = PATH_MAX;
	if (_NSGetExecutablePath(path, &size) != 0) {
		str_free(ret);
		return NULL;
	}
	if (realpath(path, ret->data) == NULL) {
		str_free(ret);
		return NULL;
	}
#else
	lol_error("get_exe_path: unknown platform, not supported");
	str_free(ret);
	return NULL;
#endif

	return ret;
}
