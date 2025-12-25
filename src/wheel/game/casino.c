#ifndef PLATFORM_WEB

#include <string.h>

#include "../lol.h"
#include "../path.h"
#include "../str.h"
#include "../sys.h"
#include "casino.h"

#ifdef _WIN32
// for _mkdir()
#include <direct.h>
#else
// for mkdir()
#include <sys/stat.h>
#endif // _WIN32

Casino new_casino(u32 ip, u16 port) {
	Casino casino = {.sa = {.ip = ip, .port = port}};
	return casino;
}

void init_casino(Casino *casino) {
	casino->udp_sock = new_udp_socket();

	if (casino->udp_sock == -1) {
		lol_term_e("failed to create UDP socket");
	}

	if (net_bind(casino->udp_sock, casino->sa) == -1) {
		lol_term_e("failed to bind the UDP socket");
	}

	if (pthread_create(&casino->udp_thread, NULL, handle_udp, &casino->udp_sock) != 0) {
		lol_term_e("failed to create thread");
	}

	Str *db_path = get_data_dir();
	path_join(db_path, "jaxvanyang");
	if (!is_dir(db_path->data)) {
#ifdef _WIN32
		_mkdir(db_path->data);
#else
		mkdir(db_path->data, 0755);
#endif // _WIN32
	}
	path_join(db_path, "casino.db");
	int rc = sqlite3_open(db_path->data, &casino->db);
	if (rc) {
		lol_term(
			"failed to open the DB: %s: %s", db_path->data, sqlite3_errmsg(casino->db)
		);
	}
	str_free(db_path);

	init_db(casino);
}

void close_casino(Casino *casino) {
	pthread_join(casino->udp_thread, NULL);
	CLOSE(casino->udp_sock);
	sqlite3_close(casino->db);
}

void init_db(const Casino *casino) {
	char buffer[BUFFER_SIZE];
	sprintf(
		buffer,
		"create table if not exists version (version text unique);"
		"insert or ignore into version (version) values ('%u.%u.%u');",
		VERSION.major,
		VERSION.minor,
		VERSION.patch
	);
	char *err_msg = NULL;
	int rc = sqlite3_exec(casino->db, buffer, NULL, NULL, &err_msg);
	if (rc) {
		lol_term("failed to init DB: %s", err_msg);
	}
	sqlite3_free(err_msg);
}

void *handle_udp(void *arg) {
	int udp_sock = *(int *)arg;
	char buffer[BUFFER_SIZE];
	char resp[BUFFER_SIZE];

	while (true) {
		SockAddr client;
		isize len = recv_from(udp_sock, &client, (void *)buffer, sizeof(buffer) - 1, 0);

		if (len < 0) {
			lol_error_e("failed to receive from the UDP socket");
			continue;
		} else {
			buffer[len] = '\0';
		}

		char *client_addr = format_sa(client);
		lol_info("%s> %.*s", client_addr, (int)len, buffer);

		Command command = parse_command(buffer);

		switch (command.type) {
		case COMMAND_VERSION:
			snprintf(
				resp,
				sizeof(resp),
				"version %u.%u.%u",
				VERSION.major,
				VERSION.minor,
				VERSION.patch
			);
			send_to(udp_sock, client, resp, strlen(resp), 0);
			break;
		default:
			snprintf(resp, sizeof(resp), "unknown");
			send_to(udp_sock, client, resp, strlen(resp), 0);
			lol_warn("unknown command from %s: %s", client_addr, buffer);
		}

		FREE(client_addr);
	}

	return NULL;
}

#endif // ifndef PLATFORM_WEB
