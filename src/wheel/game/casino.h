#pragma once

#include <pthread.h>

#include "../core.h"
#include "../net.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

typedef struct {
	SockAddr sa;
	int udp_sock;
	pthread_t udp_thread;
} Casino;

typedef enum {
	COMMAND_UNKNOWN,
	COMMAND_VERSION,
} CommandType;

typedef union {
	Version version;
} CommandArg;

typedef struct {
	CommandType type;
	CommandArg arg;
} Command;

Command parse_command(const char *s);
