#pragma once

#include "types.h"

#define new_platform(texture, x, y, ...) \
	_new_platform(texture, x, y, (PlatformArg){__VA_ARGS__})
Entity _new_platform(Texture2D texture, f32 x, f32 y, PlatformArg arg);
