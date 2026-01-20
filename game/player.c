#include <assert.h>
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

	Version server_version;
	TimeVal t0; // used to calculate latency
	double latency;
	// loss rate = 1 - received / sent
	float packets_received;
	float packets_sent;

	// communication thread
	pthread_t task;
	float task_timer;
	TimeVal last_refresh;

	Music bgm;

	int id; // user ID, which is only valid when > 0
	bool is_logged;
	bool is_seated;
	int room_id; // default is -1, which indicates no room selected
	int rooms[ROOM_CNT]; // player counts in rooms
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
		.room_id = -1,
		.last_refresh = time_now(),
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

bool send_request(Poker *poker, const char *request) {
	isize bytes_sent =
		send_to(poker->socket, poker->server, (void *)request, strlen(request), 0);
	++poker->packets_sent;

	// NOTE: this is almost impossible to happen because UDP is connectionless
	if (bytes_sent <= 0) {
		lol_error_e("failed to send: %s", request);
		return false;
	}

	poker->t0 = time_now();
	lol_info("sent request to server: %s", request);

	return true;
}

// Return NULL if failed.
Str *recv_response(Poker *poker) {
	Str *response = str_new_with_size(BUFFER_SIZE);
	isize len = recv_from(poker->socket, NULL, response->data, response->size - 1, 0);
	poker->latency = elapsed(poker->t0);

	if (len < 0) {
		str_free(response);
		return NULL;
	}
	++poker->packets_received;

	response->len = len;
	response->data[len] = '\0';
	return response;
}

void *login(void *arg) {
	Poker *poker = arg;

	// get version
	if (!send_request(poker, "version")) {
		return NULL;
	}

	Str *response = recv_response(poker);
	if (response == NULL) {
		lol_error_e("LOGIN: failed to receive response for version");
		return NULL;
	}

	lol_info("server latency: %lfms", poker->latency);

	Command command = parse_command(response->data);
	if (command.type != COMMAND_VERSION) {
		lol_warn("unexpected command: %s", response->data);
		str_free(response);
		return NULL;
	}
	str_free(response);

	poker->server_version = command.arg.version;

	// login
	if (poker->id > 0) {
		if (!send_request(poker, TextFormat("id %d", poker->id))) {
			return NULL;
		}

		Str *response = recv_response(poker);
		if (response == NULL) {
			lol_error_e("LOGIN: failed to receive response for: id ?");
			return NULL;
		}

		Command command = parse_command(response->data);
		if (command.type != COMMAND_OK) {
			lol_warn("login failed: %s", response->data);
			str_free(response);
			return NULL;
		}
		str_free(response);
	} else {
		if (!send_request(poker, "id")) {
			return NULL;
		}

		Str *response = recv_response(poker);
		if (response == NULL) {
			lol_error_e("LOGIN: failed to receive response for: id");
			return NULL;
		}

		Command command = parse_command(response->data);
		if (command.type != COMMAND_ID) {
			lol_warn("login failed: %s", response->data);
			str_free(response);
			return NULL;
		}
		str_free(response);

		FILE *config_file = open_config("w");
		if (config_file == NULL) {
			lol_term_e("failed to open config");
		}
		fprintf(config_file, "id=%d\n", command.arg.id);
		fclose(config_file);

		poker->id = command.arg.id;
	}

	poker->is_logged = true;
	lol_info("logged as %d", poker->id);

	return NULL;
}

void *join(void *arg) {
	Poker *poker = arg;

	todo();

	return NULL;
}

void *refresh_rooms(void *arg) {
	Poker *poker = arg;

	if (!send_request(poker, "rooms")) {
		return NULL;
	}

	Str *response = recv_response(poker);
	if (response == NULL) {
		lol_error_e("REFRESH_ROOMS: failed to receive response for rooms");
		return NULL;
	}

	Command command = parse_command(response->data);
	if (command.type != COMMAND_ROOMS) {
		lol_warn("unexpected command: %s", response->data);
		str_free(response);
		return NULL;
	}
	str_free(response);

	for (int i = 0; i < ROOM_CNT; ++i) {
		poker->rooms[i] = command.arg.rooms[i];
	}

	poker->last_refresh = time_now();

	return NULL;
}

Rectangle get_room_rect(int id) {
	assert(1 <= id && id <= ROOM_CNT);

	int row = (id - 1) / 4;
	int col = (id - 1) % 4;

	return (Rectangle){
		.x = 130 + col * 280,
		.y = 80 + row * 320,
		.width = 180,
		.height = 240,
	};
}

void poker_draw(const Poker *poker) {
	ClearBackground(CASINO_GREEN);

	DrawFPS(5, 5);

	draw_text_tr(
		TextFormat("Server version: %s", version_text(poker->server_version)),
		GetScreenWidth() - 5,
		5,
		20,
		LIME
	);
	draw_text_tr(
		TextFormat(
			"Packet loss: %.1f%%", 100 - poker->packets_received / poker->packets_sent * 100.0
		),
		GetScreenWidth() - 5,
		30,
		20,
		LIME
	);

	if (!poker->is_logged) {
		static int n = 1;
		static float duration = 0.0;

		Str *text = str_from("Connecting to server");
		for (int i = 0; i < n; ++i) {
			str_push(text, '.');
		}

		draw_text_center(
			text->data, GetScreenWidth() / 2, GetScreenHeight() / 2, 50, LIGHTGRAY
		);

		duration += GetFrameTime();
		if (duration >= 0.4) {
			duration = 0.0;
			n = 1 + n % 6;
		}
		str_free(text);

		return;
	}

	draw_text_tr(
		TextFormat("Latency: %.0lfms", poker->latency), GetScreenWidth() - 5, 55, 20, LIME
	);
	draw_text_tr(
		TextFormat("ID: %03d", poker->id), GetScreenWidth() - 5, 80, 20, RAYWHITE
	);

	// draw rooms
	draw_text_center("Select room to play", 640, 40, 50, BLACK);
	for (int i = 1; i <= ROOM_CNT; ++i) {
		Rectangle rect = get_room_rect(i);
		Vector2 mouse = GetMousePosition();
		float line_thick = CheckCollisionPointRec(mouse, rect) ? 4.0 : 1.0;
		float center_x = rect.x + rect.width / 2;
		float center_y = rect.y + rect.height / 2;
		DrawRectangleLinesEx(rect, line_thick, WHITE);
		draw_text_center(
			TextFormat("%d/5", poker->rooms[i]), center_x, center_y, 40, RAYWHITE
		);
		draw_text_center(TextFormat("%03d", i), center_x, rect.y + 20, 20, RAYWHITE);
	}
}

bool is_thread_running(pthread_t thread) { return pthread_kill(thread, 0) == 0; }

void poker_update(Poker *poker) {
	// wait old task
	if (poker->task != 0) {
		if (is_thread_running(poker->task)) {
			poker->task_timer -= GetFrameTime();
			if (poker->task_timer > 0.0) {
				return;
			}

			assert(pthread_cancel(poker->task) == 0);
		} else {
			assert(pthread_join(poker->task, NULL) == 0);
		}
		poker->task = 0;
	}

	// create a new task

	poker->task_timer = TASK_TIMEOUT;

	if (!poker->is_logged) {
		if (pthread_create(&poker->task, NULL, login, poker) != 0) {
			lol_error_e("TASK: failed to create login task");
		}
		lol_info("TASK: login started");
		return;
	}

	if (poker->room_id == -1) {
		if (elapsed_sec(poker->last_refresh) < TASK_TIMEOUT) {
			return;
		}

		if (pthread_create(&poker->task, NULL, refresh_rooms, poker) != 0) {
			lol_error_e("TASK: failed to create refresh_rooms task");
		}
		lol_info("TASK: refresh_rooms started");
		return;
	}

	if (poker->room_id != -1 && !poker->is_seated) {
		if (pthread_create(&poker->task, NULL, join, poker) != 0) {
			lol_error_e("TASK: failed to create join task");
		}
		lol_info("TASK: join started");
		return;
	}
}

void poker_input(Poker *poker) {
	// only handle mouse click for now
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		return;
	}

	if (poker->is_logged && poker->room_id == -1) {
		Vector2 mouse = GetMousePosition();
		for (int i = 1; i <= ROOM_CNT; ++i) {
			Rectangle rect = get_room_rect(i);
			if (CheckCollisionPointRec(mouse, rect)) {
				poker->room_id = i;
				return;
			}
		}
	}
}

void poker_loop(void *arg) {
	Poker *poker = arg;

	UpdateMusicStream(poker->bgm);

	poker_input(poker);

	poker_update(poker);

	BeginDrawing();
	poker_draw(poker);
	EndDrawing();
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
	emscripten_set_main_loop_arg(poker_loop, &poker, 0, true);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		poker_loop(&poker);
	}
#endif

	CloseWindow();
}
