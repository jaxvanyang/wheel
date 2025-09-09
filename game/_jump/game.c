#include "game.h"
#include "consts.h"
#include "entity.h"
#include "player.h"
#include "raylib.h"
#include "utils.h"
#include <raymath.h>
#include <wheel/xray.h>

Game *new_game() {
	Game *game = malloc(sizeof(Game));

	game->manager = new_resource_manager();
	game->tiles = elist_new();
	
	reset(game);

	return game;
}

void hit_and_correct(Game *game) {
	for (EntityNode *p = game->tiles->head; p; p = p->next) {
		Rectangle hit = GetCollisionRec(game->player.entity.hitbox, p->value.hitbox);
		if (FloatEquals(hit.width, 0) || FloatEquals(hit.width, 0))
			continue;

		// lengthes that player should move back
		f32 width = hit.width, height = hit.height;

		if (game->player.v.y > 0) {
			height = game->player.entity.hitbox.y + game->player.entity.hitbox.height -
							 p->value.hitbox.y;
		} else if (game->player.v.y < 0) {
			height =
				p->value.hitbox.y + p->value.hitbox.height - game->player.entity.hitbox.y;
		}

		if (game->player.v.x > 0) {
			width = game->player.entity.hitbox.x + game->player.entity.hitbox.width -
							p->value.hitbox.x;
		} else if (game->player.v.x < 0) {
			width = p->value.hitbox.x + p->value.hitbox.width - game->player.entity.hitbox.x;
		}

		f32 dt_x = fabsf(width / game->player.v.x);
		f32 dt_y = fabsf(height / game->player.v.y);
		f32 dt = min(dt_x, dt_y);

		// move back to determine which direction
		player_move(&game->player, Vector2Scale(game->player.v, -dt));

		// set velocity to 0 in the hit direction
		if (dt_x < dt_y) {
			game->player.v.x = 0;
		} else {
			// including dt_x == dt_y, otherwise may stuck in walking on flat platforms
			
			if (game->player.v.y > 0) {
				game->player.is_on_ground = true;
			}

			game->player.v.y = 0;
		}

		// move with only unhit direction velocity
		player_move(&game->player, Vector2Scale(game->player.v, dt));
	}
}

void handle_input(Game *game) {
	if (IsKeyPressed(KEY_K) && game->player.is_on_ground) {
		game->player.v.y = -15;
		game->player.is_on_ground = false;
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
	++game->frame_counter;

	if (game->frame_counter % FPS_K == 0) {
		player_update_frame(&game->player);
	}

	if (game->player.state == DEATH) {
		return;
	}

	game->deadline -= 0.5;
	if (game->frame_counter % FPS == 0) {
		++game->score;
	}

	player_update(&game->player);
	hit_and_correct(game);
	if (FloatEquals(game->player.v.x, 0)) {
		game->player.state = IDLE;
	}

	game->camera.target.y = get_screen_start_y(&game->player);

	bool should_destroy_tile = false;

	for (EntityNode *p = game->tiles->head; p;) {
		if (p->value.hitbox.y + p->value.hitbox.height > game->deadline) {
			should_destroy_tile = true;

			EntityNode *next = p->next;
			elist_pop_front(game->tiles);
			p = next;
		} else {
			break;
		}
	}

	if (should_destroy_tile) {
		PlaySound(game->manager->explosion);
	}

	if (game->player.entity.hitbox.y + game->player.entity.hitbox.height >
			game->deadline) {
		game->player.state = DEATH;
		PlaySound(game->manager->hurt);
	}
}

void reset(Game *game) {
	game->frame_counter = 0;
	game->player = new_player(game->manager->player, (f32)WIDTH / 2, 0);
	game->camera = (Camera2D){
		.target = {0, game->player.entity.hitbox.y}, .offset = Vector2Zero(), .rotation = 0, .zoom = 1
	};
	game->deadline = 100;
	game->score = 0;

	elist_clear(game->tiles);

	for (usize i = 0; i < (WIDTH + 63) / 64; ++i) {
		elist_push_back(
			game->tiles,
			new_platform(game->manager->platform, i * 64, 0, .size = PLATFORM_LARGE)
		);
	}

	for (isize i = 1; i <= (WIDTH + 63) / 64; ++i) {
		elist_push_back(
			game->tiles,
			new_platform(game->manager->platform, 100 * i, -80 * i, .color = PLATFORM_BLUE)
		);
	}
}

i32 get_screen_start_y(const Player *player) {
	return (i32)player->entity.hitbox.y - HEIGHT / 2;
}
