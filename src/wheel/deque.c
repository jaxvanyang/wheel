#include "deque.h"

DequeNode *deque_node_new(isize value) {
	DequeNode *node = malloc(sizeof(DequeNode));
	node->value = value;
	node->prev = node->next = NULL;

	return node;
}

void _deque_node_disconnect(DequeNode *node) {
	if (node->prev) {
		node->prev->next = NULL;
		node->prev = NULL;
	}
	if (node->next) {
		node->next->prev = NULL;
		node->next = NULL;
	}
}

void deque_node_free(DequeNode *node) {
	_deque_node_disconnect(node);

	node->value = 0;
	free(node);
}

void deque_node_insert(DequeNode *prev, DequeNode *node, DequeNode *next) {
	_deque_node_disconnect(node);

	if (prev) {
		if (prev->next) {
			prev->next->prev = NULL;
		}
		prev->next = node;
	}

	if (next) {
		if (next->prev) {
			next->prev->next = NULL;
		}
		next->prev = node;
	}

	node->prev = prev;
	node->next = next;
}

Deque *deque_new() {
	Deque *queue = malloc(sizeof(Deque));
	queue->size = 0;
	queue->head = queue->tail = NULL;

	return queue;
}

void deque_free(Deque *queue) {
	DequeNode *p = queue->head;

	while (p) {
		DequeNode *next = p->next;
		deque_node_free(p);
		p = next;
	}

	queue->size = 0;
	queue->head = queue->tail = NULL;
	free(queue);
}

void deque_push_front(Deque *queue, isize value) {
	DequeNode *node = deque_node_new(value);

	if (queue->size == 0) {
		queue->head = queue->tail = node;
	} else {
		deque_node_insert(NULL, node, queue->head);
		queue->head = node;
	}

	++queue->size;
}

void deque_push_back(Deque *queue, isize value) {
	DequeNode *node = deque_node_new(value);

	if (queue->size == 0) {
		queue->head = queue->tail = node;
	} else {
		deque_node_insert(queue->tail, node, NULL);
		queue->tail = node;
	}

	++queue->size;
}

isize deque_pop_front(Deque *queue) {
	if (queue->size == 0) {
		lol_term("expected a non-empty Deque\n");
	}

	isize value = queue->head->value;

	DequeNode *next = queue->head->next;

	deque_node_free(queue->head);
	queue->head = next;

	if (--queue->size == 0) {
		queue->tail = NULL;
	}

	return value;
}

isize deque_pop_back(Deque *queue) {
	if (queue->size == 0) {
		lol_term("expected a non-empty Deque\n");
	}

	isize value = queue->tail->value;

	DequeNode *prev = queue->tail->prev;

	deque_node_free(queue->tail);
	queue->tail = prev;

	if (--queue->size == 0) {
		queue->head = NULL;
	}

	return value;
}

isize deque_first(Deque *queue) {
	if (queue->size == 0) {
		lol_term("expected a non-empty Deque\n");
	}

	return queue->head->value;
}

isize deque_last(Deque *queue) {
	if (queue->size == 0) {
		lol_term("expected a non-empty Deque\n");
	}

	return queue->tail->value;
}

bool deque_empty(Deque *queue) { return queue->size == 0; }
