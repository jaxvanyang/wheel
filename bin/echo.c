#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <wheel.h>

typedef struct {
	SockAddr sa;
	int udp_sock;
	int tcp_sock;
} Server;

Server new_server(u32 ip, u16 port) {
	Server server = {.sa = {.ip = ip, .port = port}};
	server.udp_sock = new_udp_socket();
	server.tcp_sock = new_tcp_socket();

	if (server.udp_sock == -1) {
		lol_term("failed to create UDP socket");
	} else if (server.tcp_sock == -1) {
		lol_term("failed to create TCP socket");
	}

	if (net_bind(server.udp_sock, server.sa) == -1) {
		lol_term("failed to bind the UDP socket");
	} else if (net_bind(server.tcp_sock, server.sa) == -1) {
		lol_term("failed to bind the TCP socket");
	}

	if (listen(server.tcp_sock, 1) == -1) {
		lol_term("failed to listen for TCP connenction");
	}

	return server;
}

void *handle_udp(void *arg) {
	Server server = *(Server *)arg;
	char buffer[1024];
	char resp[1024];

	while (true) {
		SockAddr client;
		isize len = recv_from(server.udp_sock, &client, (void *)buffer, sizeof(buffer), 0);

		if (len < 0) {
			lol_term("failed to receive from the UDP socket\n");
		}

		char *addr = format_sa(client);
		lol_info("received %" ISIZE_FMT " bytes from UDP %s:\n", len, addr);
		lol_info("> %.*s\n", (int)len, buffer);
		FREE(addr);

		snprintf(resp, sizeof(resp), "Echo: %.*s", (int)len, buffer);
		send_to(server.udp_sock, client, resp, strlen(resp), 0);
	}

	return NULL;
}

void *handle_tcp(void *arg) {
	Server server = *(Server *)arg;
	char buffer[1024];
	char resp[1024];

	while (true) {
		SockAddr client;
		int conn_sock = net_accept(server.tcp_sock, &client);
		if (conn_sock == -1) {
			lol_term("failed to accept TCP connection");
		}
		char *addr = format_sa(client);

		while (true) {
			isize len = net_recv(conn_sock, (void *)buffer, sizeof(buffer), 0);

			if (len == -1) {
				lol_error_e("failed to receive from TCP connection");

				if (shutdown(conn_sock, SHUT_RDWR) == -1) {
					lol_error_e("failed to shutdown TCP connection");
				}

				exit(EXIT_FAILURE);
			} else if (len == 0) {
				assert(shutdown(conn_sock, SHUT_WR) == 0);
				break;
			}

			lol_info("received %" ISIZE_FMT " bytes from TCP %s:\n", len, addr);
			lol_info("> %.*s\n", (int)len, buffer);

			snprintf(resp, sizeof(resp), "Echo: %.*s", (int)len, buffer);
			if (send(conn_sock, resp, strlen(resp), 0) < 0) {
				lol_error_e("failed to send to TCP connection");
			}
		}

		FREE(addr);

		CLOSE(conn_sock);
	}

	return NULL;
}

int main(int argc, const char **argv) {
	if (argc != 2) {
		lol_term("expected one and only one argument");
	}

	u16 port = 0;
	sscanf(argv[1], "%hu", &port);

	Server server = new_server(INADDR_ANY, port);

	char *addr = format_sa(server.sa);
	lol_info("server: listening on %s...\n", addr);
	FREE(addr);

	pthread_t udp_thread;
	pthread_create(&udp_thread, NULL, handle_udp, &server);
	pthread_t tcp_thread;
	pthread_create(&tcp_thread, NULL, handle_tcp, &server);

	pthread_join(udp_thread, NULL);
	CLOSE(server.udp_sock);
	pthread_join(tcp_thread, NULL);
	CLOSE(server.tcp_sock);
}
