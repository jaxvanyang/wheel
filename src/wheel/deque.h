#pragma once

#include "core.h"

DEQUE(isize, DequeNode, Deque)

DequeNode *deque_node_new(isize value);
void deque_node_free(DequeNode *node);
void deque_node_insert(DequeNode *prev, DequeNode *node, DequeNode *next);

Deque *deque_new();
void deque_free(Deque *queue);
void deque_push_front(Deque *queue, isize value);
void deque_push_back(Deque *queue, isize value);
isize deque_pop_front(Deque *queue);
isize deque_pop_back(Deque *queue);
isize deque_first(Deque *queue);
isize deque_last(Deque *queue);
bool deque_empty(Deque *queue);
