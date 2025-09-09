#pragma once

#include "types.h"

Game *new_game();
void hit_and_correct(Game *game);
void handle_input(Game *game);
void update(Game *game);
void reset(Game *game);
i32 get_screen_start_y(const Player *player);
