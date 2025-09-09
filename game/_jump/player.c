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
	Rectangle source = {9, 9, 14, 19};
	Rectangle dest = {x, y, source.width * 2, source.height * 2};
	Rectangle hitbox = dest;

	return (Player){
		{
			texture,
			source,
			dest,
			hitbox,
		},
		Vector2Zero(),
		0,
		IDLE,
	};
}

void player_update_frame(Player *player) {
	player->frame_counter = (player->frame_counter + 1) % 16;

	switch (player->state) {
	case IDLE:
		player->entity.source.y = 9;
		player->entity.source.x = 8 + player->frame_counter % 4 * 32;
		break;
	case RUN:
		player->entity.source.y = 74 + player->frame_counter / 8 * 32;
		player->entity.source.x = 8 + player->frame_counter % 8 * 32;
		break;
	case ROLL:
		player->entity.source.y = 169;
		player->entity.source.x = 8 + player->frame_counter % 8 * 32;
		break;
	case HIT:
		player->entity.source.y = 201;
		player->entity.source.x = 8 + player->frame_counter % 4 * 32;
		break;
	case DEATH:
		player->entity.source.y = 233;
		player->entity.source.x = 8 + player->frame_counter % 4 * 32;
		break;
	}
}

void player_move(Player *player, Vector2 v) {
	f32 x = clamp(player->entity.dest.x + v.x, 0, WIDTH - player->entity.hitbox.width);
	player->entity.hitbox.x += x - player->entity.dest.x;
	player->entity.dest.x = x;

	player->entity.dest.y += v.y;
	player->entity.hitbox.y += v.y;
}

void player_update(Player *player) {
	player->v.y = min(player->v.y + 1, MAX_SPEED_Y);

	player_move(player, player->v);
}
