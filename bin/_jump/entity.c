#include "entity.h"

Entity _new_platform(Texture2D texture, f32 x, f32 y, PlatformArg arg) {
	Rectangle source = {0, 0, 16, 9};

	switch (arg.color) {
	case PLATFORM_BROWN:
		source.y = 16;
		break;
	case PLATFORM_GOLD:
		source.y = 32;
		break;
	case PLATFORM_BLUE:
		source.y = 48;
		break;
	default:
		break;
	}

	if (arg.size == PLATFORM_LARGE) {
		source.x = 16;
		source.width = 32;
	}

	Rectangle dest = {x, y, 2 * source.width, 2 * source.height};
	return (Entity){texture, source, dest, dest};
}

