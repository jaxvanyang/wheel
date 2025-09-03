#include "_jump/draw.c"
#include "_jump/draw.h"
#include "_jump/player.c"
#include "_jump/player.h"
#include "_jump/types.h"
#include <raylib.h>
#include <raymath.h>
#include <wheel.h>
#include <wheel/xray.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

static const i32 WIDTH = 450;
static const i32 HEIGHT = 600;
static const i32 FPS = 60;
static const i32 SPRITE_FPS = 10;
static const i32 FPS_K = FPS / SPRITE_FPS;

ResourceManager *new_resource_manager() {
	ResourceManager *manager = malloc(sizeof(ResourceManager));

	manager->platform =
		load_texture("assets/brackeys_platformer_assets/sprites/platforms.png");
	manager->player =
		load_texture("assets/brackeys_platformer_assets/sprites/knight.png");
	manager->jump = load_sound("assets/brackeys_platformer_assets/sounds/jump.wav");

	return manager;
}

// TODO: fully memory free seems not needed
// void unload_resource_manager(ResourceManager manager) {
// }

#define new_platform(texture, x, y, ...)                                               \
	_new_platform(texture, x, y, (PlatformArg){__VA_ARGS__})
Entity _new_platform(Texture2D texture, f32 x, f32 y, PlatformArg arg) {
	Rectangle source = {0, 0, 16, 9};

	switch (arg.color) {
	case PLATFORM_BROWN:
		source.y = 16;
		break;
	case PLATFORM_GOLD:
		source.y = 32;
		break;
	case PLATFORM_BLUE:
		source.y = 48;
		break;
	default:
		break;
	}

	if (arg.size == PLATFORM_LARGE) {
		source.x = 16;
		source.width = 32;
	}

	Rectangle dest = {x, y, 2 * source.width, 2 * source.height};
	return (Entity){texture, source, dest, dest};
}

Game *new_game() {
	Game *game = malloc(sizeof(Game));
	game->manager = new_resource_manager();
	game->frame_counter = 0;
	game->player = new_player(game->manager->player, 0, 0);
	game->tiles = elist_new();
	game->camera = (Camera2D){
		.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0, .zoom = 1
	};

	for (usize i = 0; i < (WIDTH + 63) / 64; ++i) {
		elist_push(
			game->tiles,
			new_platform(game->manager->platform, i * 64, HEIGHT - 18, .size = PLATFORM_LARGE)
		);
	}

	elist_push(
		game->tiles,
		new_platform(game->manager->platform, 100, HEIGHT - 80, .color = PLATFORM_BROWN)
	);
	elist_push(
		game->tiles,
		new_platform(game->manager->platform, 200, HEIGHT - 80, .color = PLATFORM_GOLD)
	);
	elist_push(
		game->tiles,
		new_platform(
			game->manager->platform,
			300,
			HEIGHT - 80,
			.color = PLATFORM_BLUE,
			.size = PLATFORM_LARGE
		)
	);

	return game;
}

void hit_and_correct(Game *game) {
	for (EntityNode *p = game->tiles->head; p; p = p->next) {
		Rectangle hit = GetCollisionRec(game->player.entity.hitbox, p->value.hitbox);
		if (hit.width != 0) {
			game->player.v.y = 0;
			game->player.entity.dest.y = hit.y - game->player.entity.dest.height;
		}
	}
}

// Process input
void input(Game *game) {
	if (IsKeyPressed(KEY_K)) {
		game->player.v.y = -13;
		game->player.state = RUN;
		PlaySound(game->manager->jump);
	}

	if (IsKeyDown(KEY_A)) {
		game->player.v.x = -5;
		game->player.state = RUN;
		game->player.entity.source.width = -fabsf(game->player.entity.source.width);
	}
	if (IsKeyDown(KEY_D)) {
		game->player.v.x = 5;
		game->player.state = RUN;
		game->player.entity.source.width = fabsf(game->player.entity.source.width);
	}

	if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D)) {
		game->player.state = IDLE;
		game->player.v.x = 0;
	}
}

void update(Game *game) {
	game->frame_counter = (game->frame_counter + 1) % FPS_K;
	if (game->frame_counter == 0) {
		player_update_frame(&game->player);
	}

	player_update(&game->player);

	game->camera.offset.y = HEIGHT / 2 - game->player.entity.dest.y;

	hit_and_correct(game);
}

static Game *game = NULL;

void update_draw() {
	input(game);
	update(game);
	draw(game);
}

int main() {
	InitWindow(WIDTH, HEIGHT, "Jump");
	InitAudioDevice();

	game = new_game();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(update_draw, FPS, true);
#else
	SetTargetFPS(FPS);

	while (!WindowShouldClose()) {
		update_draw();
	};
#endif // __EMSCRIPTEN__

	CloseAudioDevice();
	CloseWindow();
}
