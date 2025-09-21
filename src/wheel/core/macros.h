#pragma once

// Free a pointer, and set it to NULL.
#define FREE(p) \
	free(p); \
	p = NULL;

// Close a file descriptor, and set it to -1.
#define CLOSE(fd) \
	close(fd); \
	fd = -1;

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
