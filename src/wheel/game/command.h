#pragma once

#include "../core.h"
#include "consts.h"

typedef enum {
	COMMAND_OK, // command succeeded
	COMMAND_INVALID, // command or parameter is invalid
	COMMAND_UNKNOWN, // command is unknown
	COMMAND_ERROR, // command failed
	COMMAND_VERSION, // get server version
	COMMAND_ID, // login with ID
	COMMAND_ROOMS, // get room usage
	COMMAND_JOIN, // join the room
	COMMAND_SYNC, // synchronize user state
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
