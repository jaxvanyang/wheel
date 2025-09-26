#include "player.h"
#include "consts.h"
#include <raymath.h>

const char *state_string(PlayerState state) {
	switch (state) {
	case IDLE:
		return "idle";
	case RUN:
		return "run";
	case ROLL:
		return "roll";
	case HIT:
		return "hit";
	case DEATH:
		return "death";
	default:
		error("unexpected player state: %d\n", state);
	}
}

Player new_player(Texture2D texture, f32 x, f32 y) {
	Rectangle source = {0, 0, 32, 32};
	Rectangle hitbox = {x, y, 20, 30};
	Rectangle dest = {x - 22, y - 24, source.width * 2, source.height * 2};

	return (Player){
		.entity =
			{
				texture,
				source,
				dest,
				hitbox,
			},
		.v = Vector2Zero(),
		.frame_counter = 0,
		.state = IDLE,
		.is_on_ground = true,
	};
}

bool player_update_frame(Player *player) {
	if (player->state == DEATH && player->frame_counter % 4 == 3) {
		return false;
	}

	player->frame_counter = (player->frame_counter + 1) % 16;

	switch (player->state) {
	case IDLE:
		player->entity.source.y = 0;
		player->entity.source.x = player->frame_counter % 4 * 32;
		break;
	case RUN:
		player->entity.source.y = 64 + player->frame_counter / 8 * 32;
		player->entity.source.x = player->frame_counter % 8 * 32;
		break;
	case ROLL:
		player->entity.source.y = 160;
		player->entity.source.x = player->frame_counter % 8 * 32;
		break;
	case HIT:
		player->entity.source.y = 192;
		player->entity.source.x = player->frame_counter % 4 * 32;
		break;
	case DEATH:
		player->entity.source.y = 224;
		player->entity.source.x = player->frame_counter % 4 * 32;
		break;
	}

	return true;
}

void player_move(Player *player, Vector2 v) {
	f32 x = player->entity.hitbox.x + v.x;
	x = clamp(x, 0, WIDTH - player->entity.hitbox.width);
	player->entity.dest.x += x - player->entity.hitbox.x;
	player->entity.hitbox.x = x;

	player->entity.hitbox.y += v.y;
	player->entity.dest.y += v.y;
}

void player_update(Player *player) {
	// hit_and_correct() will correct this
	player->is_on_ground = false;

	player->v.y = min(player->v.y + 1, MAX_SPEED_Y);

	player_move(player, player->v);
}
