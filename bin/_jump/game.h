#pragma once

#include "types.h"

Game *new_game();
void hit_and_correct(Game *game);
// Process input
void input(Game *game);
void update(Game *game);
