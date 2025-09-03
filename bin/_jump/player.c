#include "player.h"
#include <raymath.h>

Player new_player(Texture2D texture, f32 x, f32 y) {
	Rectangle source = {9, 9, 14, 19};
	Rectangle dest = {x, y, source.width * 2, source.height * 2};
	// Rectangle hitbox = {x + 1, y + 1, dest.width - 2, dest.height - 2};
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

void player_update(Player *player) {
	player->v.y += 1;
	player->entity.dest.x += player->v.x;
	player->entity.dest.y += player->v.y;
	player->entity.hitbox.x = player->entity.dest.x;
	player->entity.hitbox.y = player->entity.dest.y;
}
