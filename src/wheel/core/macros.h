#pragma once

#include <stdio.h>
#include <stdlib.h>

#define println(...) \
	printf(__VA_ARGS__); \
	putchar('\n')

#define eprint(...) fprintf(stderr, __VA_ARGS__)

#define eprintln(...) \
	eprint(__VA_ARGS__); \
	eprint("\n")

#define error(...) \
	eprint("%s:%d: ", __FILE__, __LINE__); \
	eprintln(__VA_ARGS__); \
	exit(EXIT_FAILURE)

#define todo(...) error("TODO: " __VA_ARGS__)

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
		usize len; \
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
