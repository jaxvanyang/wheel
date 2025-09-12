#include "core.h"
#include <raymath.h>

void draw_entity(Entity entity) {
	DrawTexturePro(entity.texture, entity.source, entity.dest, Vector2Zero(), 0, WHITE);
}
