#include "draw.h"
#include "player.h"

void draw_hud(const Game *game) {
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

	DrawFPS(0, -game->camera.offset.y);
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

	EndMode2D();
	EndDrawing();
}
