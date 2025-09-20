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

	char *addr = format_sa(server.sa);
	printf("server: listening on %s...\n", addr);
	FREE(addr);

	while (true) {
		RecvInfo recv = udp_server_recv(server, (void *)buffer, sizeof(buffer));

		if (recv.len < 0) {
			perror("error: receive failed");
			return EXIT_FAILURE;
		}

		char *addr = format_sa(recv.sa);

		printf("received %ld bytes from %s:\n", recv.len, addr);
		printf("> %.*s\n", (int)recv.len, buffer);

		FREE(addr);

		snprintf(resp, sizeof(resp), "Echo: %.*s", (int)recv.len, buffer);
		send_to(server.sock, recv.sa, resp, strlen(resp));

		if (strncmp(buffer, "shutdown", 8) == 0) {
			break;
		}
	}

	udp_server_down(&server);
}
