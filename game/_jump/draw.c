#include "draw.h"
#include "consts.h"
#include "game.h"
#include "player.h"
#include "wheel/xray.h"
#include <raylib.h>

void draw_hud(const Game *game) {
	i32 start_y = get_screen_start_y(&game->player);

	DrawFPS(0, start_y);

	const char *score = TextFormat("%010u", game->score);
	draw_text_ex_tr(
		game->manager->pixel_operator8,
		score,
		(Vector2){WIDTH - 5, start_y + 5},
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

	DrawText(text->data, 0, get_screen_start_y(&game->player) + 20, 10, RAYWHITE);

	str_free(text);
	text = NULL;
}

static void _draw_background(Texture2D texture, Rectangle source, f32 y) {
	i32 width = (i32)source.width * 2;

	for (i32 i = 0; i <= WIDTH / width; ++i) {
		Vector2 position = {i * width, y};
		draw_texture_rec_scale(texture, source, position, WHITE, 2);
	}
}

void draw_background(const Game *game) {
	Rectangle source = {0, 175, 16, 80};
	i32 start_y = get_screen_start_y(&game->player);
	i32 end_y = start_y + HEIGHT;
	i32 height = (i32)source.height * 2 + 34;

	for (i32 i = start_y / height - 1; i <= end_y / height; ++i) {
		_draw_background(game->manager->tileset, source, i * height);
	}
}

void draw(const Game *game) {
	BeginDrawing();
	BeginMode2D(game->camera);

	Color bg_color = {3, 38, 89, 255};

	ClearBackground(bg_color);

	draw_background(game);

	for (EntityNode *p = game->tiles->head; p; p = p->next) {
		draw_entity(p->value);
	}

	draw_entity(game->player.entity);

	DrawLineEx((Vector2){0, game->deadline}, (Vector2){WIDTH, game->deadline}, 2, RED);

	draw_hud(game);
	draw_debug_info(game);

	EndMode2D();
	EndDrawing();
}
