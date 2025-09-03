#pragma once

#include "types.h"

Player new_player(Texture2D texture, f32 x, f32 y);
void player_update_frame(Player *player);
void player_update(Player *player);
