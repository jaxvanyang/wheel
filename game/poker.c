#include <raylib.h>
#include <wheel.h>
#include <wheel/game/poker.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

void main_loop(void *arg) {
	Game *game = arg;

	handle_input(game);

	update(game);

	draw(game);
}

int main() {
	lol_init("poker", LOL_DEBUG, NULL, LOL_NONE);

	InitWindow(1280, 720, "Poker");
	Game game = new_game();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(main_loop, &game, 0, true);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		main_loop(&game);
	}
#endif

	CloseWindow();
}
