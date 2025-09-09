#pragma once

#include <raylib.h>
#include <wheel/xray.h>

typedef struct {
	Texture2D platform;
	Texture2D player;
	Sound jump;
	Sound explosion;
	Font pixel_operator8;
} ResourceManager;

typedef enum {
	IDLE,
	RUN,
	ROLL,
	HIT,
	DEATH,
} PlayerState;

typedef struct {
	Entity entity;
	Vector2 v; // velocity
	u8 frame_counter;
	PlayerState state;
} Player;

typedef enum {
	PLATFORM_GREEN,
	PLATFORM_BROWN,
	PLATFORM_GOLD,
	PLATFORM_BLUE,
} PlatformColor;

typedef enum {
	PLATFORM_SMALL,
	PLATFORM_LARGE,
} PlatformSize;

typedef struct {
	PlatformColor color;
	PlatformSize size;
} PlatformArg;

typedef struct {
	ResourceManager *manager;
	Player player;
	EntityList *tiles;
	u32 frame_counter;
	Camera2D camera;
	f32 deadline;
	u32 score;
} Game;
