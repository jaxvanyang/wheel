#pragma once

#include "../core.h"

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
