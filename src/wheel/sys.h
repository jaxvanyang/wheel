#pragma once

#ifdef _WIN32
// for _mkdir()
#include <direct.h>
#else
// for mkdir()
#include <sys/stat.h>
#endif // _WIN32

#include "core.h"

#ifdef _WIN32
#define make_dir(path, mode) _mkdir(path)
#else
#define make_dir(path, mode) mkdir(path, mode)
#endif // _WIN32

typedef enum {
	OS_LINUX,
	OS_MACOS,
	OS_IOS,
	OS_WINDOWS,
	OS_UNKNOWN,
} OSType;

OSType get_os();

// Return logical processor count.
usize get_nproc();

// Return whether path exists as a directory.
bool is_dir(const char *path);
// Return whether path exists as a file.
bool is_file(const char *path);
