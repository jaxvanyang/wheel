#include <pthread.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <wheel.h>
#include <wheel/game/casino.h>
#include <wheel/game/command.h>
#include <wheel/game/consts.h>
#include <wheel/xray.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

typedef struct {
	SockAddr server;
	Socket socket;
	int id; // only valid when > 0
	bool is_logged;

	Version server_version;
	f64 latency;

	// communication thread
	pthread_t task;
	f32 task_timer;

	Music bgm;
} Poker;

FILE *open_config(const char *mode) {
	Str *path = get_config_dir();
	path_join(path, "jaxvanyang");
	if (!is_dir(path->data)) {
		make_dir(path->data, 0755);
	}
	path_join(path, "poker.conf");

	if (!is_file(path->data)) {
		FILE *f = fopen(path->data, "w");
		if (f != NULL) {
			fclose(f);
		}
	}

	FILE *f = fopen(path->data, mode);
	str_free(path);

	return f;
}

Poker new_poker(u32 server_ip, u16 server_port, int id) {
	Poker poker = {
		.server = {.ip = server_ip, .port = server_port},
		.id = id,
	};

	return poker;
}

void init_poker(Poker *poker) {
	// load resources
	poker->bgm =
		load_music_stream("assets/brackeys_platformer_assets/music/time_for_adventure.mp3");

	poker->socket = new_udp_socket();
	if (poker->socket == -1) {
		lol_term_e("failed to create UDP socket");
	}

	// load configuration
	// TODO: parse config
	if (poker->id > 0) {
		return;
	}

	FILE *config_file = open_config("r");
	if (config_file == NULL) {
		lol_term_e("failed to open config");
	}
	fscanf(config_file, "id=%d", &poker->id);
	if (poker->id == 0) {
		lol_info("no id in config");
	}

	fclose(config_file);
}

void *login(void *arg) {
	Poker *poker = arg;

	// get server version
	char buffer[BUFFER_SIZE] = "version";
	isize bytes_sent = send_to(poker->socket, poker->server, buffer, strlen(buffer), 0);
	if (bytes_sent < 0) {
		lol_error_e("failed to send: %s", buffer);
		return NULL;
	}
	lol_info("sent msg to server: %s", buffer);

	TimeVal t0 = time_now();

	SockAddr client;
	isize len = recv_from(poker->socket, &client, buffer, sizeof(buffer) - 1, 0);
	if (len < 0) {
		lol_error_e("failed to receive response for: %s", buffer);
		return NULL;
	} else {
		buffer[len] = '\0';
	}

	poker->latency = elapsed(t0);
	lol_info("server latency: %lfms", poker->latency);

	Command command = parse_command(buffer);
	if (command.type != COMMAND_VERSION) {
		lol_warn("unexpected response: %s", buffer);
		return NULL;
	}

	poker->server_version = command.arg.version;

	// login
	if (poker->id > 0) {
		sprintf(buffer, "id %d", poker->id);
		isize bytes_sent = send_to(poker->socket, poker->server, buffer, strlen(buffer), 0);
		if (bytes_sent < 0) {
			lol_error_e("failed to send: %s", buffer);
			return NULL;
		}
		lol_info("sent msg to server: %s", buffer);

		TimeVal t0 = time_now();

		SockAddr client;
		isize len = recv_from(poker->socket, &client, buffer, sizeof(buffer) - 1, 0);
		if (len < 0) {
			lol_error_e("failed to receive response for: %s", buffer);
			return NULL;
		} else {
			buffer[len] = '\0';
		}

		poker->latency = elapsed(t0);

		Command command = parse_command(buffer);
		if (command.type != COMMAND_OK) {
			lol_warn("login failed: %s", buffer);
			return NULL;
		}
	} else {
		sprintf(buffer, "id");
		isize bytes_sent = send_to(poker->socket, poker->server, buffer, strlen(buffer), 0);
		if (bytes_sent < 0) {
			lol_error_e("failed to send: %s", buffer);
			return NULL;
		}
		lol_info("sent msg to server: %s", buffer);

		TimeVal t0 = time_now();

		SockAddr client;
		isize len = recv_from(poker->socket, &client, buffer, sizeof(buffer) - 1, 0);
		if (len < 0) {
			lol_error_e("failed to receive response for: %s", buffer);
			return NULL;
		} else {
			buffer[len] = '\0';
		}

		poker->latency = elapsed(t0);

		Command command = parse_command(buffer);
		if (command.type != COMMAND_ID) {
			lol_warn("login failed: %s", buffer);
			return NULL;
		} else {
			FILE *config_file = open_config("w");
			if (config_file == NULL) {
				lol_term_e("failed to open config");
			}
			fprintf(config_file, "id=%d\n", command.arg.id);
			fclose(config_file);

			poker->id = command.arg.id;
		}
	}

	poker->is_logged = true;
	lol_info("logged as %d", poker->id);

	return NULL;
}

void draw(const Poker *poker) {
	BeginDrawing();

	ClearBackground(CASINO_GREEN);

	// draw debug info
	DrawFPS(5, 5);
	if (poker->is_logged) {
		draw_text_tr(
			TextFormat("Server version: %s", version_text(poker->server_version)),
			GetScreenWidth() - 5,
			5,
			20,
			LIME
		);
		draw_text_tr(
			TextFormat("Latency: %.0lfms", poker->latency), GetScreenWidth() - 5, 30, 20, LIME
		);
		draw_text_tr(
			TextFormat("ID: %03d", poker->id), GetScreenWidth() - 5, 55, 20, RAYWHITE
		);
	} else {
		draw_text_center(
			"Connecting to server...",
			GetScreenWidth() / 2,
			GetScreenHeight() / 2,
			50,
			LIGHTGRAY
		);
	}

	EndDrawing();
}

void main_loop(void *arg) {
	Poker *poker = arg;

	UpdateMusicStream(poker->bgm);

	// input

	// update
	if (poker->task_timer <= 0) {
		poker->task_timer = 2;
		if (poker->task != 0) {
			pthread_cancel(poker->task);
			poker->task = 0;
		}

		if (!poker->is_logged) {
			// login task
			if (pthread_create(&poker->task, NULL, login, poker) != 0) {
				lol_error_e("failed to create login task");
			}
			lol_info("started a new login task");
		}
	} else {
		poker->task_timer -= GetFrameTime();
	}

	draw(poker);
}

int main(int argc, const char **argv) {
	int id = 0;
	if (argc == 2) {
		sscanf(argv[1], "%d", &id);
	}

	lol_init2();
	InitWindow(1280, 720, "Poker");
	InitAudioDevice();

	Poker poker = new_poker(net_addr("127.0.0.1"), 1888, id);
	init_poker(&poker);

	PlayMusicStream(poker.bgm);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(main_loop, &poker, 0, true);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		main_loop(&poker);
	}
#endif

	CloseWindow();
}
