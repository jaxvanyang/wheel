// udpnet - telnet in UDP

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <wheel.h>

int main(int argc, const char **const argv) {
	if (argc != 3) {
		error("expected 3 and only 3 arguments");
	}

	u16 port;
	sscanf(argv[2], "%hu", &port);
	SockAddr server_sa = {.ip = net_addr(argv[1]), .port = port};

	char *addr = format_sa(server_sa);
	printf("Send message to %s:\n", addr);
	FREE(addr);

	char buffer[1024];

	int sock = new_udp_socket();
	if (sock == -1) {
		printf("Error Creating Socket");
		return EXIT_FAILURE;
	}

	while (true) {
		printf("> ");
		if (scanf("%s", buffer) == EOF || strcmp(buffer, "exit") == 0) {
			break;
		}

		isize bytes_sent = send_to(sock, server_sa, buffer, strlen(buffer), 0);
		if (bytes_sent < 0) {
			printf("Error sending packet: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		SockAddr client;
		isize len = recv_from(sock, &client, buffer, sizeof(buffer), 0);

		if (len < 0) {
			printf("Error receiving packet: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}

		printf("%.*s\n", (int)len, buffer);
	}

	close(sock);
}
