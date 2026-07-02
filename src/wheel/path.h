#pragma once

#include "core.h"
#include "str.h"

#ifdef _WIN32
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

// create a new system path string from Unix-style path
char *os_path(const char *path);
// Strip `n` components from `path`.
void path_strip(char *path, size_t n);
void path_join(Str *path, const char *component);

Str *get_home_dir();
Str *get_config_dir();
Str *get_cache_dir();
Str *get_data_dir();

// Get the path of current executable, return `NULL` if failed.
Str *get_exe_path();
