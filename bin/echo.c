#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <wheel.h>

typedef struct {
	SockAddr sa;
	int udp_sock;
	int tcp_sock;
} Server;

int main(int argc, const char **argv) {
	if (argc != 2) {
		error("expected one and only one argument");
	}

	u16 port = 0;
	sscanf(argv[1], "%hu", &port);

	char buffer[1024];
	char resp[1024];

	Server server = {.sa = {.addr = INADDR_ANY, .port = port}};
	server.udp_sock = new_udp_socket();
	server.tcp_sock = new_tcp_socket();

	if (server.udp_sock == -1) {
		error("failed to create UDP socket");
	} else if (server.tcp_sock == -1) {
		error("failed to create TCP socket");
	}

	if (net_bind(server.udp_sock, server.sa) == -1) {
		error("failed to bind the UDP socket");
	} else if (net_bind(server.tcp_sock, server.sa) == -1) {
		error("failed to bind the TCP socket");
	}

	if (listen(server.tcp_sock, 1) == -1) {
		error("failed to listen for TCP connenction");
	}

	char *addr = format_sa(server.sa);
	printf("server: listening on %s...\n", addr);
	FREE(addr);

	while (true) {
		SockAddr client;
		isize len = recv_from(server.udp_sock, &client, (void *)buffer, sizeof(buffer), 0);

		if (len < 0) {
			error("failed to receive from the UDP socket\n");
		}

		char *addr = format_sa(client);
		printf("received %ld bytes from UDP %s:\n", len, addr);
		printf("> %.*s\n", (int)len, buffer);
		FREE(addr);

		snprintf(resp, sizeof(resp), "Echo: %.*s", (int)len, buffer);
		send_to(server.udp_sock, client, resp, strlen(resp), 0);

	}

	CLOSE(server.udp_sock);
	CLOSE(server.tcp_sock);
}
