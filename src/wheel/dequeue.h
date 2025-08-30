#pragma once

#include "core.h"

typedef struct DequeueNode {
	isize value;
	struct DequeueNode *prev;
	struct DequeueNode *next;
} DequeueNode;

typedef struct {
	usize size;
	DequeueNode *head;
	DequeueNode *tail;
} Dequeue;

DequeueNode *dequeue_node_new(isize value);
void dequeue_node_free(DequeueNode *node);
void dequeue_node_insert(DequeueNode *prev, DequeueNode *node, DequeueNode *next);

Dequeue *dequeue_new();
void dequeue_free(Dequeue *queue);
void dequeue_push_front(Dequeue *queue, isize value);
void dequeue_push_back(Dequeue *queue, isize value);
isize dequeue_pop_front(Dequeue *queue);
isize dequeue_pop_back(Dequeue *queue);
isize dequeue_first(Dequeue *queue);
isize dequeue_last(Dequeue *queue);
bool dequeue_empty(Dequeue *queue);
