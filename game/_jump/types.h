#pragma once

#include <raylib.h>
#include <wheel/xray.h>

typedef struct {
	Texture2D platform;
	Texture2D player;
	Texture2D tileset;
	Sound jump;
	Sound explosion;
	Sound hurt;
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
	bool is_on_ground;
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
	bool is_over;
} Game;
