#pragma once

#include "core.h"

DEQUE(Entity, EntityNode, EntityList)
DEQUE(Vector2, Vector2Node, Vector2List)

EntityList *elist_new();
void elist_free(EntityList *list);
void elist_push_front(EntityList *list, Entity value);
void elist_push_back(EntityList *list, Entity value);
Entity elist_pop_front(EntityList *list);
Entity elist_pop_back(EntityList *list);
void elist_clear(EntityList *list);

Vector2List *v2list_new();
void v2list_free(Vector2List *list);
void v2list_push_front(Vector2List *list, Vector2 value);
void v2list_push_back(Vector2List *list, Vector2 value);
Vector2 v2list_pop_front(Vector2List *list);
Vector2 v2list_pop_back(Vector2List *list);
void v2list_clear(Vector2List *list);
