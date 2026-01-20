#include <string.h>

#include "../str.h"
#include "command.h"

Command parse_command(const char *s) {
	Command command = {.type = COMMAND_UNKNOWN};

	if (str_start_with(s, "version")) {
		command.type = COMMAND_VERSION;
		sscanf(
			s,
			"version %u.%u.%u",
			&command.arg.version.major,
			&command.arg.version.minor,
			&command.arg.version.patch
		);
	} else if (strcmp(s, "ok") == 0) {
		command.type = COMMAND_OK;
	} else if (str_start_with(s, "id")) {
		command.type = COMMAND_ID;
		sscanf(s, "id %d", &command.arg.id);
	} else if (str_start_with(s, "rooms")) {
		command.type = COMMAND_ROOMS;
		sscanf(
			s,
			"rooms %d %d %d %d %d %d %d %d",
			&command.arg.rooms[0],
			&command.arg.rooms[1],
			&command.arg.rooms[2],
			&command.arg.rooms[3],
			&command.arg.rooms[4],
			&command.arg.rooms[5],
			&command.arg.rooms[6],
			&command.arg.rooms[7]
		);
	} else if (str_start_with(s, "join")) {
		sscanf(s, "join %d", &command.arg.id);
		if (command.arg.id < 1 || command.arg.id > ROOM_CNT) {
			command.type = COMMAND_INVALID;
		} else {
			command.type = COMMAND_JOIN;
		}
	}

	return command;
}
