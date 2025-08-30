#include "xray.h"
#include "path.h"
#include <raymath.h>

void draw_entity(Entity entity) {
	DrawTexturePro(entity.texture, entity.source, entity.dest, Vector2Zero(), 0, WHITE);
}

EntityList *elist_new() {
	EntityList *list = malloc(sizeof(EntityList));
	list->size = 0;
	list->head = NULL;

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

	list->head = NULL;
	list->size = 0;
}

void elist_insert(EntityList *list, Entity entity) {
	EntityNode *node = malloc(sizeof(EntityNode));
	node->value = entity;

	node->next = list->head;
	list->head = node;
	++list->size;
}

Entity elist_pop(EntityList *list) {
	if (list->size == 0) {
		error("expected non-zero size list");
	}

	EntityNode *node = list->head;
	Entity entity = node->value;

	list->head = node->next;
	--list->size;
	node->next = NULL;
	free(node);

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
