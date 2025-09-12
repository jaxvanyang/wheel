// Extension to raylib
#pragma once

#include "core.h"
#include <raylib.h>

typedef struct {
	Texture2D texture;
	Rectangle source;
	Rectangle dest;
	Rectangle hitbox;
} Entity;

DEQUEUE(Entity, EntityNode, EntityList)

void draw_entity(Entity entity);

EntityList *elist_new();
void elist_free(EntityList *list);
void elist_push_front(EntityList *list, Entity entity);
void elist_push_back(EntityList *list, Entity entity);
Entity elist_pop_front(EntityList *list);
Entity elist_pop_back(EntityList *list);
void elist_clear(EntityList *list);

// These functions act as the same as their raylib brothers, but convert the
// UNIX-style path to system style first.
Texture2D load_texture(const char *path);
Sound load_sound(const char *path);
Music load_music_stream(const char *path);
Font load_font(const char *path);

float get_default_font_spacing(int font_size);
// Draw text (using default font, using top right position)
void draw_text_tr(const char *text, int x, int y, int font_size, Color color);
// Draw text using font and additional parameters (using top right position)
void draw_text_ex_tr(
	Font font, const char *text, Vector2 position, float font_size, float spacing,
	Color tint
);
// Draw text (using default font, using center position)
void draw_text_center(const char *text, int x, int y, int font_size, Color color);
// Draw text using font and additional parameters (using center position)
void draw_text_ex_center(
	Font font, const char *text, Vector2 position, float font_size, float spacing,
	Color tint
);

void draw_texture_rec_scale(
	Texture2D texture, Rectangle source, Vector2 position, Color tint, f32 scale
);
