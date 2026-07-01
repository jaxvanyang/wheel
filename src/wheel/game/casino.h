#pragma once

// this module needs SQLite, we don't want to build SQLite for Web
#ifndef __EMSCRIPTEN__

#include <pthread.h>
#include <sqlite3.h>

#include "../core.h"
#include "../net.h"
#include "command.h"
#include "consts.h"

typedef struct {
	SockAddr sa;
	int udp_sock;

	pthread_t udp_thread;

	sqlite3 *db;
} Casino;

Casino new_casino(u32 ip, u16 port);
void init_casino(Casino *casino);
void close_casino(Casino *casino);

void init_db(const Casino *casino);

void *casino_service(void *arg);

// DB interfaces

// Create or update user. Return true on success, false otherwise.
bool create_user(sqlite3 *db, int id, SockAddr addr);
// Return an unused ID for new user. Return -1 on error.
int get_new_user_id(sqlite3 *db);

#endif // ifndef __EMSCRIPTEN__
