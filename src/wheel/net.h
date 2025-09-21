#pragma once

// To work with <windows.h>, we need a compatibility layer, see
// https://github.com/raysan5/raylib/issues/1217
// Here we choose the option 2 - only use incompatible functions in .c files.
// And wrap them with new names.

#include "core.h"

// For more information: https://en.wikipedia.org/wiki/Berkeley_sockets

#ifndef INADDR_ANY
#define INADDR_ANY 0
#endif

// how arguments for shutdown(2), specified by Posix.1g.
#define SHUT_RD 0 // shut down the reading side
#define SHUT_WR 1 // shut down the writing side
#define SHUT_RDWR 2 // shut down both sides

typedef struct {
	u32 addr;
	u16 port;
} SockAddr;

// Return a string in format of "address:port".
// NOTE: the returned pointer should be freed.
char *format_sa(SockAddr sa);

// Return -1 if an error occurs, file descriptor otherwise.
int new_udp_socket();
// Return -1 if an error occurs, file descriptor otherwise.
int new_tcp_socket();

// Convert string address to integer address.
// For more information, see inet_addr().
u32 net_addr(const char *addr);
// Bind an unnamed socket to an socket address.
// For more information, see bind().
int net_bind(int socket, SockAddr sa);
// Return -1 if an error occurs, 0 otherwise.
int listen(int socket, int backlog);
// For more information, see accept().
int net_accept(int socket, SockAddr *sa);

// For more information, see recv().
isize net_recv(int socket, void *buffer, usize buffer_size, int flags);
// For more information, see recvfrom()
isize recv_from(int socket, SockAddr *src, void *buffer, usize buffer_size, int flags);
isize send(int socket, const void *buffer, usize buffer_size, int flags);
// Return -1 if failed, sent message length otherwise.
// For more information, see sendto()
isize send_to(int socket, SockAddr dest, void *buffer, usize buffer_size, int flags);
int shutdown(int socket, int how);
