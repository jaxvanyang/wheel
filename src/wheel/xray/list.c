#include "list.h"

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
