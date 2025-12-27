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
	}

	return command;
}
