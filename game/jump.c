#include "_jump/draw.c"
#include "_jump/draw.h"
#include "_jump/entity.c"
#include "_jump/entity.h"
#include "_jump/game.c"
#include "_jump/game.h"
#include "_jump/player.c"
#include "_jump/player.h"
#include "_jump/types.h"
#include "_jump/utils.c"
#include "_jump/utils.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

static Game *game = NULL;

void main_loop() {
	if (game->player.state != DEATH) {
		handle_input(game);
	} else {
		if (IsKeyPressed(KEY_R)) {
			reset(game);
		}
	}

	update(game);
	draw(game);
}

int main() {
	InitWindow(WIDTH, HEIGHT, "Jump");
	InitAudioDevice();

	game = new_game();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(update_draw, FPS, true);
#else
	SetTargetFPS(FPS);

	while (!WindowShouldClose()) {
		main_loop();
	};
#endif // __EMSCRIPTEN__

	CloseAudioDevice();
	CloseWindow();
}
