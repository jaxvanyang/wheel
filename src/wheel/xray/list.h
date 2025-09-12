#pragma once

#include "core.h"

DEQUEUE(Entity, EntityNode, EntityList)

EntityList *elist_new();
void elist_free(EntityList *list);
void elist_push_front(EntityList *list, Entity entity);
void elist_push_back(EntityList *list, Entity entity);
Entity elist_pop_front(EntityList *list);
Entity elist_pop_back(EntityList *list);
void elist_clear(EntityList *list);
