#include <unistd.h>
#include <string.h>

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
	struct sockaddr_in ret = {};
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

const char *format_ip(u32 ip) {
	SockAddr sa = {.ip = ip};
	struct sockaddr os_sa = to_os_sa(sa);

	return inet_ntoa(((struct sockaddr_in *)&os_sa)->sin_addr);
}

char *format_sa(SockAddr sa) {
	const char *addr = format_ip(sa.ip);

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

Str *recv_str(Socket socket, usize buffer_size, int flags) {
	Str *buffer = str_new_with_size(buffer_size + 1);
	isize len = net_recv(socket, buffer->data, buffer_size, flags);
	if (len < 0) {
		str_free(buffer);
		return NULL;
	}
	buffer->len = len;
	buffer->data[len] = '\0';

	return buffer;
}

Str *recv_str_from(Socket socket, SockAddr *src, usize buffer_size, int flags) {
	Str *buffer = str_new_with_size(buffer_size + 1);
	isize len = recv_from(socket, src, buffer->data, buffer_size, flags);
	if (len < 0) {
		str_free(buffer);
		return NULL;
	}
	buffer->len = len;
	buffer->data[len] = '\0';

	return buffer;
}

isize send_str(Socket socket, const char *str, int flags) {
	usize len = strlen(str);
	return send(socket, str, len, flags);
}

isize send_str_to(Socket socket, SockAddr dest, const char *str, int flags) {
	usize len = strlen(str);
	return send_to(socket, dest, (void *)str, len, flags);
}
