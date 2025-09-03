#include "game.h"
#include "consts.h"
#include "entity.h"
#include "player.h"
#include "utils.h"
#include <raymath.h>
#include <wheel/xray.h>

Game *new_game() {
	Game *game = malloc(sizeof(Game));
	game->manager = new_resource_manager();
	game->frame_counter = 0;
	game->player = new_player(game->manager->player, WIDTH / 2, 0);
	game->tiles = elist_new();
	game->camera = (Camera2D
	){.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0, .zoom = 1};

	for (usize i = 0; i < (WIDTH + 63) / 64; ++i) {
		elist_push_back(
			game->tiles,
			new_platform(game->manager->platform, i * 64, HEIGHT - 18, .size = PLATFORM_LARGE)
		);
	}

	elist_push_back(
		game->tiles,
		new_platform(game->manager->platform, 100, HEIGHT - 80, .color = PLATFORM_BROWN)
	);
	elist_push_back(
		game->tiles,
		new_platform(game->manager->platform, 200, HEIGHT - 80, .color = PLATFORM_GOLD)
	);
	elist_push_back(
		game->tiles,
		new_platform(
			game->manager->platform,
			300,
			HEIGHT - 80,
			.color = PLATFORM_BLUE,
			.size = PLATFORM_LARGE
		)
	);

	return game;
}

void hit_and_correct(Game *game) {
	for (EntityNode *p = game->tiles->head; p; p = p->next) {
		Rectangle hit = GetCollisionRec(game->player.entity.hitbox, p->value.hitbox);
		if (FloatEquals(hit.width, 0))
			continue;

		f32 dt_x = fabsf(hit.width / game->player.v.x);
		f32 dt_y = fabsf(hit.height / game->player.v.y);
		f32 dt = min(dt_x, dt_y);

		// move back to determine which direction
		player_move(&game->player, Vector2Scale(game->player.v, -dt));

		// set velocity to 0 in the hit direction
		if (FloatEquals(
					game->player.entity.hitbox.y + game->player.entity.hitbox.height,
					p->value.hitbox.y
				) ||
				FloatEquals(
					game->player.entity.hitbox.y, p->value.hitbox.y + p->value.hitbox.height
				)) {
			game->player.v.y = 0;
		} else {
			game->player.v.x = 0;
		}

		// move with only unhit direction velocity
		player_move(&game->player, Vector2Scale(game->player.v, dt));
	}
}

void input(Game *game) {
	if (IsKeyPressed(KEY_K)) {
		game->player.v.y = -13;
		game->player.state = RUN;
		PlaySound(game->manager->jump);
	}

	if (IsKeyDown(KEY_A)) {
		game->player.v.x = -5;
		game->player.state = RUN;
		game->player.entity.source.width = -fabsf(game->player.entity.source.width);
	}
	if (IsKeyDown(KEY_D)) {
		game->player.v.x = 5;
		game->player.state = RUN;
		game->player.entity.source.width = fabsf(game->player.entity.source.width);
	}

	if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D)) {
		game->player.state = IDLE;
		game->player.v.x = 0;
	}
}

void update(Game *game) {
	game->frame_counter = (game->frame_counter + 1) % FPS_K;
	if (game->frame_counter == 0) {
		player_update_frame(&game->player);
	}

	// player updates
	player_update(&game->player);
	hit_and_correct(game);
	if (FloatEquals(game->player.v.x, 0)) {
		game->player.state = IDLE;
	}

	game->camera.offset.y = HEIGHT / 2 - game->player.entity.dest.y;
}
