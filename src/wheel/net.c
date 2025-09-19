#include <arpa/inet.h>
#include <unistd.h>
#include <wheel/str.h>

#include "net.h"

char *format_addr(struct sockaddr_in sa) {
	// max IPv4 socket address is 4 * 4 + 5 + 1 = 22
	char *ret = malloc(24);
	char *addr = inet_ntoa(sa.sin_addr);

	snprintf(ret, 24, "%s:%d", addr, ntohs(sa.sin_port));

	return ret;
}

UDPServer udp_server(u32 addr, u16 port) {
	UDPServer ret = {};

	ret.addr.sin_family = AF_INET;
	ret.addr.sin_addr.s_addr = htonl(addr);
	ret.addr.sin_port = htons(port);

	return ret;
}

bool udp_server_init(UDPServer *server) {
	server->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (server->sock == -1) {
		perror("error: failed to create socket");
		return false;
	}

	if (bind(server->sock, (struct sockaddr *)&server->addr, sizeof(server->addr)) ==
			-1) {
		perror("error: bind failed");
		close(server->sock);
		server->sock = -1;

		return false;
	}

	return true;
}

RecvInfo udp_server_recv(UDPServer server, void *buffer, usize buffer_size) {
	RecvInfo ret = {.addr_len = sizeof(struct sockaddr)};
	ret.msg_len = recvfrom(
		server.sock, buffer, buffer_size, 0, (struct sockaddr *)&ret.addr, &ret.addr_len
	);

	return ret;
}

void udp_server_down(UDPServer *server) {
	close(server->sock);
	server->sock = -1;
}
