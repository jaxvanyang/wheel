#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <wheel.h>

int main(int argc, const char **argv) {
	if (argc != 2) {
		error("expected one and only one argument\n");
	}

	u16 port = 0;
	sscanf(argv[1], "%hu", &port);

	char buffer[1024];
	char resp[1024];

	UDPServer server = udp_server(INADDR_ANY, port);
	udp_server_init(&server);

	char *addr = format_addr(server.addr);
	printf("server: running on %s...\n", addr);
	FREE(addr);

	while (true) {
		RecvInfo info = udp_server_recv(server, (void *)buffer, sizeof(buffer));

		if (info.msg_len < 0) {
			perror("error: receive failed");
			return EXIT_FAILURE;
		}

		char *addr = format_addr(info.addr);

		printf("received %d bytes from %s:\n", info.msg_len, addr);
		printf("> %.*s\n", info.msg_len, buffer);

		FREE(addr);

		snprintf(resp, sizeof(resp), "Echo: %.*s", info.msg_len, buffer);
		sendto(
			server.sock, resp, strlen(resp), 0, (struct sockaddr *)&info.addr, info.addr_len
		);

		if (strncmp(buffer, "shutdown", 8) == 0) {
			break;
		}
	}

	udp_server_down(&server);
}
