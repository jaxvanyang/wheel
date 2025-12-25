#include "command.h"
#include "../str.h"

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
	}

	return command;
}
