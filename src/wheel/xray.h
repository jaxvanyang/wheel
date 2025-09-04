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

typedef struct EntityNode {
	Entity value;
	struct EntityNode *prev;
	struct EntityNode *next;
} EntityNode;

// this is actually dequeue
typedef struct {
	usize size;
	EntityNode *head;
	EntityNode *tail;
} EntityList;

void draw_entity(Entity entity);

EntityList *elist_new();
void elist_free(EntityList *list);
void elist_push_front(EntityList *list, Entity entity);
void elist_push_back(EntityList *list, Entity entity);
Entity elist_pop_front(EntityList *list);
Entity elist_pop_back(EntityList *list);

// These functions act as the same as their raylib brothers, but convert the
// UNIX-style path to system style first.
Texture2D load_texture(const char *path);
Sound load_sound(const char *path);
Music load_music_stream(const char *path);
Font load_font(const char *path);
