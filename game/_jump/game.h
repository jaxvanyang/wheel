#pragma once

#include "types.h"

Game *new_game();
void hit_and_correct(Game *game);
void handle_input(Game *game);
void update(Game *game);
void reset(Game *game);
