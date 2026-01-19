#pragma once

#include "../core.h"
#include "consts.h"

typedef enum {
	COMMAND_UNKNOWN,
	COMMAND_VERSION,
	COMMAND_OK,
	COMMAND_ID,
	COMMAND_ROOMS,
} CommandType;

typedef union {
	Version version;
	int id;
	int rooms[ROOM_CNT];
} CommandArg;

typedef struct {
	CommandType type;
	CommandArg arg;
} Command;

Command parse_command(const char *s);
