#pragma once

#include "types.h"

const i32 WIDTH = 450;
const i32 HEIGHT = 600;
const i32 FPS = 60;
const i32 SPRITE_FPS = 10;
const i32 FPS_K = FPS / SPRITE_FPS;


Game *new_game();
void hit_and_correct(Game *game);
// Process input
void input(Game *game);
void update(Game *game);
