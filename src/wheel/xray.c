#include "xray.h"
#include "path.h"
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

Texture2D load_texture(const char *path){
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
