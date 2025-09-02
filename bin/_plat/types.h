#pragma once

#include <raylib.h>
#include <wheel/xray.h>

typedef struct {
	Texture2D platform;
	Texture2D player;
	Sound jump;
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
	// velocity
	Vector2 v;
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
} Game;
