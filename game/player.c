#include <pthread.h>
#include <raylib.h>
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

	Version server_version;
	f64 latency;

	// communication thread
	pthread_t task;
	f32 task_timer;

	Music bgm;
} Poker;

Poker new_poker(u32 server_ip, u16 server_port) {
	Poker poker = {
		.server = {.ip = server_ip, .port = server_port},
	};

	return poker;
}

void init_poker(Poker *poker) {
	poker->bgm =
		load_music_stream("assets/brackeys_platformer_assets/music/time_for_adventure.mp3");

	poker->socket = new_udp_socket();
	if (poker->socket == -1) {
		lol_term_e("failed to create UDP socket");
	}
}
void *connect_server(void *arg) {
	Poker *poker = arg;

	char buffer[BUFFER_SIZE] = "version";
	isize bytes_sent = send_to(poker->socket, poker->server, buffer, strlen(buffer), 0);
	if (bytes_sent < 0) {
		lol_error_e("failed to connect server");
		return NULL;
	}
	lol_info("sent msg to server: %s", buffer);

	TimeVal t0 = time_now();

	SockAddr client;
	isize len = recv_from(poker->socket, &client, buffer, sizeof(buffer) - 1, 0);
	if (len < 0) {
		lol_error_e("failed to receive response from server");
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

	return NULL;
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

		if (!is_version_valid(poker->server_version)) {
			if (pthread_create(&poker->task, NULL, connect_server, poker) != 0) {
				lol_error_e("failed to create connect server task");
			}
			lol_info("started a new connect server task");
		}
	} else {
		poker->task_timer -= GetFrameTime();
	}

	// draw
	BeginDrawing();

	ClearBackground(CASINO_GREEN);

	// draw debug info
	DrawFPS(5, 5);
	// version invalid means server not connected
	if (is_version_valid(poker->server_version)) {
		draw_text_tr(
			TextFormat("Server version: %s", version_text(poker->server_version)),
			GetScreenWidth() - 5,
			5,
			20,
			LIME
		);
		draw_text_tr(
			TextFormat("Latency: %.0lfms", poker->latency), GetScreenWidth() - 5, 25, 20, LIME
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

int main() {
	lol_init2();
	InitWindow(1280, 720, "Poker");
	InitAudioDevice();

	Poker poker = new_poker(net_addr("127.0.0.1"), 1888);
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
