#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

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
	ret.sin_addr.s_addr = htonl(sa.addr);
	ret.sin_port = htons(sa.port);

	return *(struct sockaddr *)&ret;
}

static SockAddr from_os_sa(struct sockaddr sa) {
	struct sockaddr_in *p = (struct sockaddr_in *)&sa;
	SockAddr ret = {
		.addr = ntohl(p->sin_addr.s_addr),
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

UDPServer udp_server(u32 addr, u16 port) {
	UDPServer ret = {.sa = {.addr = addr, .port = port}, .sock = -1};

	return ret;
}

bool udp_server_init(UDPServer *server) {
	server->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (server->sock == -1) {
		perror("error: failed to create socket");
		return false;
	}

	struct sockaddr sa = to_os_sa(server->sa);

	if (bind(server->sock, &sa, sizeof(sa)) == -1) {
		perror("error: bind failed");
		close(server->sock);
		server->sock = -1;

		return false;
	}

	return true;
}

void udp_server_down(UDPServer *server) {
	close(server->sock);
	server->sock = -1;
}

RecvInfo udp_server_recv(UDPServer server, void *buffer, usize buffer_size) {
	struct sockaddr sa;
	socklen_t len = sizeof(sa);
	isize msg_len = recvfrom(server.sock, buffer, buffer_size, 0, &sa, &len);

	RecvInfo ret = {.sa = from_os_sa(sa), .len = msg_len};

	return ret;
}

isize send_to(int sock, SockAddr target, void *buffer, usize buffer_size) {
	struct sockaddr sa = to_os_sa(target);
	isize ret = sendto(sock, buffer, buffer_size, 0, &sa, sizeof(sa));

	return ret;
}
