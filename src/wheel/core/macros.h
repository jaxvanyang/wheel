#pragma once

#define VECTOR(T, Name) \
	typedef struct { \
		usize size; \
		usize length; \
		T *data; \
	} Name;

#define DEQUE(T, NodeName, Name) \
	typedef struct NodeName { \
		T value; \
		struct NodeName *prev; \
		struct NodeName *next; \
	} NodeName; \
\
	typedef struct { \
		usize size; \
		NodeName *head; \
		NodeName *tail; \
	} Name;
