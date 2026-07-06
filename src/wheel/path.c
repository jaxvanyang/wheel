#include "path.h"
#include "lol.h"
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

void path_join(Str *path, const char *relative_path) {
	str_push(path, PATH_SEP);
	char *tmp = os_path(relative_path);
	str_push_str(path, tmp);
	free(tmp);
}

size_t os_path_strip(char *path, size_t n) {
	size_t len = strlen(path);
	while (len > 0 && path[len - 1] == PATH_SEP) {
		path[--len] = '\0';
	}

	while (len > 0 && n > 0) {
		if (path[len - 1] == PATH_SEP) {
			--n;
		}
		path[--len] = '\0';
	}

	return len;
}

Str *path_new(const char *path) {
	char *tmp = os_path(path);
	Str *ret = str_from(tmp);
	free(tmp);

	return ret;
}

void path_strip(Str *path, size_t n) { path->len = os_path_strip(path->data, n); }

void path_strip_and_join(Str *path, size_t n, const char *relative_path) {
	path_strip(path, n);
	path_join(path, relative_path);
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

Str *get_exe_dir() {
	Str *ret = get_exe_path();
	if (ret == NULL) {
		return NULL;
	}
	os_path_strip(ret->data, 1);
	ret->len = strlen(ret->data);

	return ret;
}

Str *get_assets_dir() {
	Str *ret = get_exe_dir();
	if (ret == NULL) {
		return NULL;
	}
	char *relative_path = os_path("../share/wheel/assets");
	path_join(ret, relative_path);
	free(relative_path);

	return ret;
}
