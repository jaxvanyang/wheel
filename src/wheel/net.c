#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

// These are also defined in <windows.h>
#undef min
#undef max

// <windows.h> should be included after <winsock2.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include "net.h"

static struct sockaddr to_os_sa(SockAddr sa) {
	struct sockaddr_in ret;
	ret.sin_family = AF_INET;
	ret.sin_addr.s_addr = htonl(sa.ip);
	ret.sin_port = htons(sa.port);

	return *(struct sockaddr *)&ret;
}

static SockAddr from_os_sa(struct sockaddr sa) {
	struct sockaddr_in *p = (struct sockaddr_in *)&sa;
	SockAddr ret = {
		.ip = ntohl(p->sin_addr.s_addr),
		.port = ntohs(p->sin_port),
	};

	return ret;
}

char *format_sa(SockAddr sa) {
	struct sockaddr os_sa = to_os_sa(sa);
	char *addr = inet_ntoa(((struct sockaddr_in *)&os_sa)->sin_addr);

	// max IPv4 socket address is 4 * 4 + 5 + 1 = 22
	char *ret = malloc(24);
	snprintf(ret, 24, "%s:%d", addr, sa.port);

	return ret;
}

int new_udp_socket() { return socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); }

int new_tcp_socket() { return socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); }

u32 net_addr(const char *addr) { return ntohl(inet_addr(addr)); }

int net_bind(Socket socket, SockAddr sa) {
	struct sockaddr os_sa = to_os_sa(sa);
	return bind(socket, &os_sa, sizeof(os_sa));
}

int net_accept(Socket socket, SockAddr *sa) {
	struct sockaddr os_sa;
	socklen_t len = sizeof(os_sa);
	int ret = accept(socket, &os_sa, &len);

	if (sa) {
		*sa = from_os_sa(os_sa);
	}

	return ret;
}

isize net_recv(Socket socket, void *buffer, usize buffer_size, int flags) {
	return recv_from(socket, NULL, buffer, buffer_size, flags);
}

isize recv_from(
	Socket socket, SockAddr *src, void *buffer, usize buffer_size, int flags
) {
	struct sockaddr sa;
	socklen_t len = sizeof(sa);
	isize ret = recvfrom(socket, buffer, buffer_size, flags, &sa, &len);

	if (src) {
		*src = from_os_sa(sa);
	}

	return ret;
}

isize send_to(
	Socket socket, SockAddr dest, void *buffer, usize buffer_size, int flags
) {
	struct sockaddr sa = to_os_sa(dest);
	isize ret = sendto(socket, buffer, buffer_size, flags, &sa, sizeof(sa));

	return ret;
}
