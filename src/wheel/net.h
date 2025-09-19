#pragma once

#include <netinet/in.h>

#include "core.h"

// For more information: https://en.wikipedia.org/wiki/Berkeley_sockets

typedef struct {
	struct sockaddr_in addr;
	int sock;
} UDPServer;

// Received information from a socket
typedef struct {
	struct sockaddr_in addr; // message source address, i.e., the client
	i32 msg_len; // received message length, negative if error occurs
	socklen_t addr_len;
} RecvInfo;

// Return a string in format of "address:port".
char *format_addr(struct sockaddr_in sa);

UDPServer udp_server(u32 addr, u16 port);
// Return false if failed, true otherwise.
bool udp_server_init(UDPServer *server);
// Return a negative number if failed, received data length otherwise.
RecvInfo udp_server_recv(UDPServer server, void *buffer, usize buffer_size);
// Shutdown the server.
void udp_server_down(UDPServer *server);
