#include "draw.h"
#include "consts.h"
#include "player.h"
#include <raylib.h>

void draw_hud(const Game *game) {
	DrawFPS(0, -game->camera.offset.y);

	const char *score = TextFormat("%010d", game->frame_counter / FPS);
	Vector2 size = MeasureTextEx(game->manager->pixel_operator8, score, 15, 1);
	DrawTextEx(
		game->manager->pixel_operator8,
		score,
		(Vector2){WIDTH - size.x - 5, 5 -game->camera.offset.y},
		15,
		1,
		RAYWHITE
	);
}

void draw_debug_info(const Game *game) {
	// below are debug infos
	DrawRectangleLinesEx(game->player.entity.dest, 1, GREEN);
	DrawRectangleLinesEx(game->player.entity.hitbox, 1, RED);

	Str *text = str_new();

	str_push_str(text, TextFormat("state: %s\n", state_string(game->player.state)));
	str_push_str(
		text,
		TextFormat(
			"pos: %.1f, %.1f\n", game->player.entity.dest.x, game->player.entity.dest.y
		)
	);
	str_push_str(text, TextFormat("v: %.1f, %.1f\n", game->player.v.x, game->player.v.y));
	str_push_str(text, TextFormat("camera Y offset: %.1f\n", game->camera.offset.y));
	str_push_str(text, TextFormat("frame counter: %u\n", game->frame_counter));

	DrawText(text->data, 0, 20 - game->camera.offset.y, 10, RAYWHITE);

	str_free(text);
	text = NULL;
}

void draw(const Game *game) {
	BeginDrawing();
	BeginMode2D(game->camera);
	ClearBackground(DARKBLUE);

	for (EntityNode *p = game->tiles->head; p; p = p->next) {
		draw_entity(p->value);
	}

	draw_entity(game->player.entity);

	draw_hud(game);
	draw_debug_info(game);

	EndMode2D();
	EndDrawing();
}
