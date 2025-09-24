#include <raylib.h>
#include <wheel.h>
#include <wheel/game/poker.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

void main_loop(void *arg) {
	Game *game = arg;

	BeginDrawing();
	ClearBackground(DARKGREEN);

	handle_input(game);
	draw(game);

	EndDrawing();
}

int main() {
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
