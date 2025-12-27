#ifndef PLATFORM_WEB

#include <string.h>

#include "../lol.h"
#include "../path.h"
#include "../str.h"
#include "../sys.h"
#include "casino.h"

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

	Str *db_path = get_data_dir();
	path_join(db_path, "jaxvanyang");
	if (!is_dir(db_path->data)) {
		make_dir(db_path->data, 0755);
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

	// start casino service
	if (pthread_create(&casino->udp_thread, NULL, casino_service, casino) != 0) {
		lol_term_e("failed to create service thread");
	}
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
		"insert or ignore into version (version) values ('%u.%u.%u');"
		"create table if not exists user ("
		"	id integer primary key,"
		"	ip text not null,"
		"	port integer not null"
		");"
		// placeholder for get_largest_user_id()
		"insert or ignore into user (id, ip, port) values (0, 0, 0);",
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

void *casino_service(void *arg) {
	Casino *casino = arg;
	int udp_sock = casino->udp_sock;
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
		case COMMAND_ID:
			if (command.arg.id != 0) {
				if (!create_user(casino->db, command.arg.id, client)) {
					break;
				}
				sprintf(resp, "ok");
			} else {
				int id = get_new_user_id(casino->db);
				if (id == 0 || !create_user(casino->db, id, client)) {
					lol_error("failed to create user %d for %s", id, client_addr);
					break;
				}
				sprintf(resp, "id %d", id);
			}

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

bool create_user(sqlite3 *db, int id, SockAddr addr) {
	if (id <= 0) {
		lol_warn("cannot create user: id <= 0 is invalid");
		return false;
	}

	bool ret = true;

	const char sql[] = "insert or replace into user (id, ip, port) values (?, ?, ?);";
	const char *ip = format_ip(addr.ip);
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, id);
	sqlite3_bind_text(stmt, 2, ip, strlen(ip), SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 3, addr.port);

	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		lol_error("failed to create user: %s", sqlite3_errstr(rc));
		ret = false;
	}

	sqlite3_finalize(stmt);
	return ret;
}

int get_new_user_id(sqlite3 *db) {
	int ret = -1;

	const char sql[] = "select id from user order by id desc limit 1;";
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		lol_error("failed to get largest user id: %s", sqlite3_errstr(rc));
	} else {
		int id = sqlite3_column_int(stmt, 0);
		ret = id + 1;
	}

	sqlite3_finalize(stmt);

	return ret;
}

#endif // ifndef PLATFORM_WEB
