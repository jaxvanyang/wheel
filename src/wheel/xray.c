#include "xray.h"
#include "path.h"
#include "raylib.h"
#include <raymath.h>

void draw_entity(Entity entity) {
	DrawTexturePro(entity.texture, entity.source, entity.dest, Vector2Zero(), 0, WHITE);
}

EntityList *elist_new() {
	EntityList *list = malloc(sizeof(EntityList));
	list->size = 0;
	list->head = list->tail = NULL;

	return list;
}

void elist_free(EntityList *list) {
	EntityNode *node = list->head;

	while (node) {
		EntityNode *next = node->next;
		node->next = NULL;
		free(node);
		node = next;
	}

	list->head = list->tail = NULL;
	list->size = 0;
}

void elist_push_front(EntityList *list, Entity entity) {
	EntityNode *node = malloc(sizeof(EntityNode));
	node->value = entity;

	node->prev = NULL;
	node->next = list->head;

	if (list->size) {
		list->head->prev = node;
		list->head = node;
	} else {
		list->head = list->tail = node;
	}

	++list->size;
}

void elist_push_back(EntityList *list, Entity entity) {
	EntityNode *node = malloc(sizeof(EntityNode));
	node->value = entity;

	node->prev = list->tail;
	node->next = NULL;

	if (list->size) {
		list->tail->next = node;
		list->tail = node;
	} else {
		list->head = list->tail = node;
	}

	++list->size;
}

Entity elist_pop_front(EntityList *list) {
	if (list->size == 0) {
		error("expected non-empty list");
	}

	EntityNode *node = list->head;
	Entity entity = node->value;

	list->head = node->next;
	--list->size;
	node->prev = node->next = NULL;
	free(node);

	if (list->size == 0) {
		list->tail = NULL;
	}

	return entity;
}

Entity elist_pop_back(EntityList *list) {
	if (list->size == 0) {
		error("expected non-empty list");
	}

	EntityNode *node = list->tail;
	Entity entity = node->value;

	list->tail = node->prev;
	--list->size;
	node->prev = node->next = NULL;
	free(node);

	if (list->size == 0) {
		list->head = NULL;
	}

	return entity;
}

void elist_clear(EntityList *list) {
	while (list->size != 0) {
		elist_pop_front(list);
	}
}

Texture2D load_texture(const char *path) {
	char *p = os_path(path);
	Texture2D ret = LoadTexture(p);

	free(p);
	p = NULL;

	return ret;
}

Sound load_sound(const char *path) {
	char *p = os_path(path);
	Sound ret = LoadSound(p);

	free(p);
	p = NULL;

	return ret;
}

Music load_music_stream(const char *path) {
	char *p = os_path(path);
	Music ret = LoadMusicStream(p);

	free(p);
	p = NULL;

	return ret;
}

Font load_font(const char *path) {
	char *p = os_path(path);
	Font ret = LoadFont(p);

	free(p);
	p = NULL;

	return ret;
}

float get_default_font_spacing(int font_size) {
	int default_font_size = 10; // Default Font chars height in pixel
	if (font_size < default_font_size)
		font_size = default_font_size;
	int spacing = font_size / default_font_size;

	return (float)spacing;
}

void draw_text_tr(const char *text, int x, int y, int font_size, Color color) {
	int width = MeasureText(text, font_size);
	x -= width;

	DrawText(text, x, y, font_size, color);
}

void draw_text_ex_tr(
	Font font, const char *text, Vector2 position, float font_size, float spacing,
	Color tint
) {
	Vector2 size = MeasureTextEx(font, text, font_size, spacing);
	position.x -= size.x;

	DrawTextEx(font, text, position, font_size, spacing, tint);
}

void draw_text_center(const char *text, int x, int y, int font_size, Color color) {
	int spacing = get_default_font_spacing(font_size);
	Vector2 size = MeasureTextEx(GetFontDefault(), text, font_size, spacing);
	x -= size.x / 2;
	y -= size.y / 2;

	DrawText(text, x, y, font_size, color);
}

void draw_text_ex_center(
	Font font, const char *text, Vector2 position, float font_size, float spacing,
	Color tint
) {
	Vector2 size = MeasureTextEx(font, text, font_size, spacing);
	position.x -= size.x / 2;
	position.y -= size.y / 2;

	DrawTextEx(font, text, position, font_size, spacing, tint);
}

void draw_texture_rec_scale(
	Texture2D texture, Rectangle source, Vector2 position, Color tint, f32 scale
) {
	Rectangle dest = {
		position.x, position.y, source.width * scale, source.height * scale
	};
	DrawTexturePro(texture, source, dest, Vector2Zero(), 0, tint);
}
