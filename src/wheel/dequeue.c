#include "dequeue.h"

#include "wheel/core/error.h"

DequeueNode *dequeue_node_new(isize value) {
	DequeueNode *node = malloc(sizeof(DequeueNode));
	node->value = value;
	node->prev = node->next = NULL;

	return node;
}

void _dequeue_node_disconnect(DequeueNode *node) {
	if (node->prev) {
		node->prev->next = NULL;
		node->prev = NULL;
	}
	if (node->next) {
		node->next->prev = NULL;
		node->next = NULL;
	}
}

void dequeue_node_free(DequeueNode *node) {
	_dequeue_node_disconnect(node);

	node->value = 0;
	free(node);
}

void dequeue_node_insert(DequeueNode *prev, DequeueNode *node, DequeueNode *next) {
	_dequeue_node_disconnect(node);

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

Dequeue *dequeue_new() {
	Dequeue *queue = malloc(sizeof(Dequeue));
	queue->size = 0;
	queue->head = queue->tail = NULL;

	return queue;
}

void dequeue_free(Dequeue *queue) {
	DequeueNode *p = queue->head;

	while (p) {
		DequeueNode *next = p->next;
		dequeue_node_free(p);
		p = next;
	}

	queue->size = 0;
	queue->head = queue->tail = NULL;
	free(queue);
}

void dequeue_push_front(Dequeue *queue, isize value) {
	DequeueNode *node = dequeue_node_new(value);

	if (queue->size == 0) {
		queue->head = queue->tail = node;
	} else {
		dequeue_node_insert(NULL, node, queue->head);
		queue->head = node;
	}

	++queue->size;
}

void dequeue_push_back(Dequeue *queue, isize value) {
	DequeueNode *node = dequeue_node_new(value);

	if (queue->size == 0) {
		queue->head = queue->tail = node;
	} else {
		dequeue_node_insert(queue->tail, node, NULL);
		queue->tail = node;
	}

	++queue->size;
}

isize dequeue_pop_front(Dequeue *queue) {
	if (queue->size == 0) {
		error("expected a non-empty Dequeue");
	}

	isize value = queue->head->value;
	DequeueNode *next = queue->head->next;
	dequeue_node_free(queue->head);
	queue->head = next;

	if (--queue->size == 0) {
		queue->tail = NULL;
	}

	return value;
}

isize dequeue_pop_back(Dequeue *queue) {
	if (queue->size == 0) {
		error("expected a non-empty Dequeue");
	}

	isize value = queue->tail->value;
	DequeueNode *prev = queue->tail->prev;
	dequeue_node_free(queue->tail);
	queue->tail = prev;

	if (--queue->size == 0) {
		queue->head = NULL;
	}

	return value;
}

isize dequeue_first(Dequeue *queue) {
	if (queue->size == 0) {
		error("expected a non-empty Dequeue");
	}

	return queue->head->value;
}

isize dequeue_last(Dequeue *queue) {
	if (queue->size == 0) {
		error("expected a non-empty Dequeue");
	}

	return queue->tail->value;
}

bool dequeue_empty(Dequeue *queue) { return queue->size == 0; }
