#include <pthread.h>
#include <string.h>
#include <wheel.h>
#include <wheel/game/casino.h>

const Version VERSION = {0, 1, 0};

void *handle_udp(void *arg) {
	int udp_sock = *(int *)arg;
	char buffer[BUFFER_SIZE];
	char resp[BUFFER_SIZE];

	while (true) {
		SockAddr client;
		isize len = recv_from(udp_sock, &client, (void *)buffer, sizeof(buffer) - 1, 0);

		if (len < 0) {
			lol_error_e("failed to receive from the UDP socket");
			continue;
		} else {
			buffer[len] = '\0';
		}

		char *client_addr = format_sa(client);
		lol_info("%s> %.*s", client_addr, (int)len, buffer);

		Command command = parse_command(buffer);

		switch (command.type) {
		case COMMAND_VERSION:
			snprintf(
				resp,
				sizeof(resp),
				"version %u.%u.%u",
				VERSION.major,
				VERSION.minor,
				VERSION.patch
			);
			send_to(udp_sock, client, resp, strlen(resp), 0);
			break;
		default:
			snprintf(resp, sizeof(resp), "unknown");
			send_to(udp_sock, client, resp, strlen(resp), 0);
			lol_warn("unknown command from %s: %s", client_addr, buffer);
		}

		FREE(client_addr);
	}

	return NULL;
}

Casino new_casino(u32 ip, u16 port) {
	Casino casino = {.sa = {.ip = ip, .port = port}};
	return casino;
}

void init_casino(Casino *casino) {
	casino->udp_sock = new_udp_socket();

	if (casino->udp_sock == -1) {
		lol_term_e("failed to create UDP socket");
	}

	if (net_bind(casino->udp_sock, casino->sa) == -1) {
		lol_term_e("failed to bind the UDP socket");
	}

	if (pthread_create(&casino->udp_thread, NULL, handle_udp, &casino->udp_sock) != 0) {
		lol_term_e("failed to create thread");
	}
}

void close_casino(Casino *casino) {
	pthread_join(casino->udp_thread, NULL);
	CLOSE(casino->udp_sock);
}

int main(int argc, const char **argv) {
	lol_init2();

	if (argc != 2) {
		eprintln("usage: casino <port>");
		return EXIT_FAILURE;
	}

	u16 port;
	sscanf(argv[1], "%hu", &port);

	Casino casino = new_casino(INADDR_ANY, port);
	init_casino(&casino);

	char *addr = format_sa(casino.sa);
	lol_info("created casino at %s", addr);
	FREE(addr);

	close_casino(&casino);
}
