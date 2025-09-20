#pragma once

// To work with <windows.h>, we need a compatibility layer, see
// https://github.com/raysan5/raylib/issues/1217
// Here we choose the option 2 - only use it in .c files.

#include "core.h"

// For more information: https://en.wikipedia.org/wiki/Berkeley_sockets

#ifndef INADDR_ANY
#define INADDR_ANY 0
#endif

typedef struct {
	u32 addr;
	u16 port;
} SockAddr;

typedef struct {
	SockAddr sa;
	int sock;
} UDPServer;

// Received information from a socket
typedef struct {
	SockAddr sa; // message source address, i.e., the client
	isize len; // received message length, negative if error occurs
} RecvInfo;

typedef RecvInfo SendInfo;

// Return a string in format of "address:port".
// NOTE: the returned pointer should be freed.
char *format_sa(SockAddr sa);

UDPServer udp_server(u32 addr, u16 port);
// Return false if failed, true otherwise.
bool udp_server_init(UDPServer *server);
// Shutdown the server.
void udp_server_down(UDPServer *server);

RecvInfo udp_server_recv(UDPServer server, void *buffer, usize buffer_size);
// Return -1 if failed, sent message length otherwise.
isize send_to(int sock, SockAddr target, void *buffer, usize buffer_size);
