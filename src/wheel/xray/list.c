#include "list.h"

#define DEQUEUE_IMPLEMENTATION(T, NodeName, Name, prefix) \
\
	Name *prefix##_new() { \
		Name *list = malloc(sizeof(Name)); \
		list->size = 0; \
		list->head = list->tail = NULL; \
\
		return list; \
	} \
\
	void prefix##_free(Name *list) { \
		NodeName *node = list->head; \
\
		while (node) { \
			NodeName *next = node->next; \
			node->next = NULL; \
			free(node); \
			node = next; \
		} \
\
		list->head = list->tail = NULL; \
		list->size = 0; \
	} \
\
	void prefix##_push_front(Name *list, T value) { \
		NodeName *node = malloc(sizeof(NodeName)); \
		node->value = value; \
\
		node->prev = NULL; \
		node->next = list->head; \
\
		if (list->size) { \
			list->head->prev = node; \
			list->head = node; \
		} else { \
			list->head = list->tail = node; \
		} \
\
		++list->size; \
	} \
\
	void prefix##_push_back(Name *list, T value) { \
		NodeName *node = malloc(sizeof(NodeName)); \
		node->value = value; \
\
		node->prev = list->tail; \
		node->next = NULL; \
\
		if (list->size) { \
			list->tail->next = node; \
			list->tail = node; \
		} else { \
			list->head = list->tail = node; \
		} \
\
		++list->size; \
	} \
\
	T prefix##_pop_front(Name *list) { \
		if (list->size == 0) { \
			error("expected non-empty list\n"); \
		} \
\
		NodeName *node = list->head; \
		T value = node->value; \
\
		list->head = node->next; \
		--list->size; \
		node->prev = node->next = NULL; \
		free(node); \
\
		if (list->size == 0) { \
			list->tail = NULL; \
		} \
\
		return value; \
	} \
\
	T prefix##_pop_back(Name *list) { \
		if (list->size == 0) { \
			error("expected non-empty list\n"); \
		} \
\
		NodeName *node = list->tail; \
		T value = node->value; \
\
		list->tail = node->prev; \
		--list->size; \
		node->prev = node->next = NULL; \
		free(node); \
\
		if (list->size == 0) { \
			list->head = NULL; \
		} \
\
		return value; \
	} \
\
	void prefix##_clear(Name *list) { \
		while (list->size != 0) { \
			prefix##_pop_front(list); \
		} \
	}

DEQUEUE_IMPLEMENTATION(Entity, EntityNode, EntityList, elist)
