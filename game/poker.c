#include <raylib.h>
#include <wheel.h>
#include <wheel/game/poker.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

void main_loop(void *arg) {
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
	emscripten_set_main_loop_arg(main_loop, &poker, 0, true);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		main_loop(&poker);
	}
#endif

	CloseWindow();
}
