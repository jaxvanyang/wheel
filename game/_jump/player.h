#pragma once

#include "types.h"

const char *state_string(PlayerState state);
Player new_player(Texture2D texture, f32 x, f32 y);
// Return true if updated, false otherwise.
bool player_update_frame(Player *player);
void player_move(Player *player, Vector2 v);
void player_update(Player *player);
