#pragma once

#include "../core.h"

typedef enum {
	COMMAND_UNKNOWN,
	COMMAND_VERSION,
	COMMAND_OK,
	COMMAND_ID,
} CommandType;

typedef union {
	Version version;
	int id;
} CommandArg;

typedef struct {
	CommandType type;
	CommandArg arg;
} Command;

Command parse_command(const char *s);
