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
	struct EntityNode *next;
} EntityNode;

typedef struct {
	usize size;
	EntityNode *head;
} EntityList;

void draw_entity(Entity entity);

EntityList *elist_new();
void elist_free(EntityList *list);
void elist_insert(EntityList *list, Entity entity);
Entity elist_pop(EntityList *list);
