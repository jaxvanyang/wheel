#pragma once

// this module needs sqlite, we don't want to build sqlite for Web
#ifndef PLATFORM_WEB

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
// Initialize and start the service.
void start_casino(Casino *casino);
void close_casino(Casino *casino);

void init_db(const Casino *casino);

void *casino_service(void *arg);

// DB interfaces

// Create or update user. Return true on success, false otherwise.
bool create_user(sqlite3 *db, int id, SockAddr addr);
// Return an unused ID for new user. Return -1 on error.
int get_new_user_id(sqlite3 *db);
int get_room_usage(sqlite3 *db, int id);

#endif // ifndef PLATFORM_WEB
