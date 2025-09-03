#include "game.h"
#include "utils.h"
#include "player.h"
#include <wheel/xray.h>
#include <raymath.h>
#include "entity.h"

Game *new_game() {
	Game *game = malloc(sizeof(Game));
	game->manager = new_resource_manager();
	game->frame_counter = 0;
	game->player = new_player(game->manager->player, 0, 0);
	game->tiles = elist_new();
	game->camera = (Camera2D
	){.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0, .zoom = 1};

	for (usize i = 0; i < (WIDTH + 63) / 64; ++i) {
		elist_push(
			game->tiles,
			new_platform(game->manager->platform, i * 64, HEIGHT - 18, .size = PLATFORM_LARGE)
		);
	}

	elist_push(
		game->tiles,
		new_platform(game->manager->platform, 100, HEIGHT - 80, .color = PLATFORM_BROWN)
	);
	elist_push(
		game->tiles,
		new_platform(game->manager->platform, 200, HEIGHT - 80, .color = PLATFORM_GOLD)
	);
	elist_push(
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
		if (hit.width != 0) {
			game->player.v.y = 0;
			game->player.entity.dest.y = hit.y - game->player.entity.dest.height;
		}
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

	player_update(&game->player);

	game->camera.offset.y = HEIGHT / 2 - game->player.entity.dest.y;

	hit_and_correct(game);
}
