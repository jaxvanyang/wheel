#pragma once

#include "../core.h"
#include <raylib.h>

typedef struct {
	Texture2D texture;
	Rectangle source;
	Rectangle dest;
	Rectangle hitbox;
} Entity;

void draw_entity(Entity entity);
