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
// Strip `n` components from `path` (C string), return the new string's length.
size_t os_path_strip(char *path, size_t n);

Str *path_new(const char *path);
// Strip `n` components from `path` (`Str`).
void path_strip(Str *path, size_t n);
void path_join(Str *path, const char *relative_path);
void path_strip_and_join(Str *path, size_t n, const char *relative_path);

Str *get_home_dir();
Str *get_config_dir();
Str *get_cache_dir();
Str *get_data_dir();

// Get the path of current executable, return `NULL` if failed.
Str *get_exe_path();
// Get the directory of current executable, return `NULL` if failed.
Str *get_exe_dir();
// Get the install directory of assets, return `NULL` if failed.
Str *get_assets_dir();
