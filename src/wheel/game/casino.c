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

void start_casino(Casino *casino) {
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

void *casino_service(void *arg) {
	Casino *casino = arg;
	int udp_sock = casino->udp_sock;
	char buffer[PACKET_BUFSIZ];
	char resp[PACKET_BUFSIZ];

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
				// TODO: use autoincrement
				int id = get_new_user_id(casino->db);
				if (id == 0 || !create_user(casino->db, id, client)) {
					lol_error("failed to create user %d for %s", id, client_addr);
					break;
				}
				sprintf(resp, "id %d", id);
			}

			send_to(udp_sock, client, resp, strlen(resp), 0);
			break;
		case COMMAND_ROOMS:
			int rooms[ROOM_CNT] = {};
			for (int i = 0; i < ROOM_CNT; ++i) {
				rooms[i] = get_room_usage(casino->db, i + 1);
			}
			sprintf(
				resp,
				"rooms %d %d %d %d %d %d %d %d",
				rooms[0],
				rooms[1],
				rooms[2],
				rooms[3],
				rooms[4],
				rooms[5],
				rooms[6],
				rooms[7]
			);
			send_to(udp_sock, client, resp, strlen(resp), 0);
			break;
		case COMMAND_JOIN:
			int user_id = get_user_id_by_addr(casino->db, client);
			int seat = get_empty_seat(casino->db, command.arg.id);
			if (seat < 0) {
				send_str_to(udp_sock, client, "invalid", 0);
				break;
			}
			if (!seat_user(casino->db, user_id, command.arg.id, seat)) {
				send_str_to(udp_sock, client, "error", 0);
				break;
			}
			send_str_to(udp_sock, client, "ok", 0);
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

void init_db(const Casino *casino) {
	char buffer[PACKET_BUFSIZ];
	char *err_msg = NULL;
	int rc;

	// TODO: wrap exec
	sprintf(
		buffer,
		"pragma foreign_keys = on;"
		// "drop table room;"
		// "drop table user;"
		// "drop table version;"
		"create table if not exists version (version text unique);"
		"insert or ignore into version (version) values ('%u.%u.%u');"
		"create table if not exists user ("
		"	id integer primary key autoincrement,"
		"	ip text not null,"
		"	port integer not null,"
		"	unique (ip, port) on conflict replace"
		");"
		// placeholder to retrieve the largest user ID easily
		"insert or ignore into user (id, ip, port) values (0, 0, 0);",
		VERSION.major,
		VERSION.minor,
		VERSION.patch
	);
	rc = sqlite3_exec(casino->db, buffer, NULL, NULL, &err_msg);
	if (rc) {
		lol_term("DB: failed to initialize table version or user: %s", err_msg);
	}
	sqlite3_free(err_msg);

	rc = sqlite3_exec(
		casino->db,
		"create table if not exists room ("
		"	id integer not null check (id >= 1),"
		"	seat_number integer not null check (seat_number between 1 and 5),"
		"	user_id integer references user (id) unique check (user_id != 0),"
		"	unique (id, seat_number),"
		"	unique (id, user_id)"
		");",
		NULL,
		NULL,
		&err_msg
	);
	if (rc) {
		lol_term("DB: failed to create table room: %s", err_msg);
	}
	sqlite3_free(err_msg);

	lol_info("DB: initialized");
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
		lol_error("failed to create user: %s", sqlite3_errmsg(db));
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

int get_room_usage(sqlite3 *db, int id) {
	if (id < 1 || id > ROOM_CNT) {
		lol_warn("invalid room ID: %d", id);
	}

	const char sql[] = "select count(*) from room where id = ?;";
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, id);

	int ret = 0;
	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		lol_error("DB: failed to get room usage for %d: %s", id, sqlite3_errstr(rc));
	} else {
		ret = sqlite3_column_int(stmt, 0);
	}

	return ret;
}

int get_user_id_by_addr(sqlite3 *db, SockAddr addr) {
	const char *sql = "select id from user where ip = ? and port = ?;";
	const char *ip = format_ip(addr.ip);
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	sqlite3_bind_text(stmt, 1, ip, strlen(ip), SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, addr.port);

	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		lol_error("failed to get user ID by socket address: %s", format_sa(addr));
		sqlite3_finalize(stmt);
		return -1;
	}

	int ret = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	return ret;
}

int get_empty_seat(sqlite3 *db, int room_id) {
	if (room_id < 1 || room_id > ROOM_CNT) {
		lol_warn("invalid room_id: %d", room_id);
		return -1;
	}

	bool seats[SEAT_CNT + 1] = {};
	const char *sql = "select seat_number from room where id = ?;";
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, room_id);

	int rc;
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int seat = sqlite3_column_int(stmt, 0);
		seats[seat] = true;
	}
	if (rc != SQLITE_DONE) {
		lol_error("DB: failed to execute: %s", sql);
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	for (int i = 1; i <= SEAT_CNT; ++i) {
		if (!seats[i]) {
			return i;
		}
	}

	return -1;
}

bool seat_user(sqlite3 *db, int user_id, int room_id, int seat) {
	const char *sql = "insert into room (id, seat_number, user_id) values (?, ?, ?);";
	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, room_id);
	sqlite3_bind_int(stmt, 2, seat);
	sqlite3_bind_int(stmt, 3, user_id);

	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		lol_error("DB: failed to seat user: %s", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

#endif // ifndef PLATFORM_WEB
