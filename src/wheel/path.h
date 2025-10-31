#pragma once

#include "core.h"
#include "str.h"

// create a new system path string from Unix-style path
char *os_path(const char *path);
void path_join(Str *path, const char *component);

Str *get_home_dir();
Str *get_config_dir();
Str *get_cache_dir();
Str *get_data_dir();
