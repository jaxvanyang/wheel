#pragma once

#include "core.h"

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

// Return whether path is a directory.
bool is_dir(char *path);
