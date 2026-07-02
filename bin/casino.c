#include <pthread.h>
#include <sqlite3.h>
#include <string.h>
#include <wheel.h>
#include <wheel/game/casino.h>

int main(int argc, const char **argv) {
	lol_init2();

	if (argc != 2) {
		eprintln("usage: casino <port>");
		return EXIT_FAILURE;
	}

	u16 port;
	sscanf(argv[1], "%hu", &port);

	Casino casino = new_casino(INADDR_ANY, port);
	init_casino(&casino);

	char *addr = format_sa(casino.sa);
	lol_info("created casino at %s", addr);
	FREE(addr);

	close_casino(&casino);
}
