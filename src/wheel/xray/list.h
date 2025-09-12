#pragma once

#include "core.h"

DEQUEUE(Entity, EntityNode, EntityList)

EntityList *elist_new();
void elist_free(EntityList *list);
void elist_push_front(EntityList *list, Entity value);
void elist_push_back(EntityList *list, Entity value);
Entity elist_pop_front(EntityList *list);
Entity elist_pop_back(EntityList *list);
void elist_clear(EntityList *list);
