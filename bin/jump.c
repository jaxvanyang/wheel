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

Player new_player(Texture2D texture, f32 x, f32 y) {
	Rectangle source = {9, 9, 14, 19};
	Rectangle dest = {x, y, source.width * 2, source.height * 2};
	// Rectangle hitbox = {x + 1, y + 1, dest.width - 2, dest.height - 2};
	Rectangle hitbox = dest;

	return (Player){
		{
			texture,
			source,
			dest,
			hitbox,
		},
		Vector2Zero(),
		0,
		IDLE,
	};
}

void player_update_frame(Player *player) {
	player->frame_counter = (player->frame_counter + 1) % 16;

	switch (player->state) {
	case IDLE:
		player->entity.source.y = 9;
		player->entity.source.x = 8 + player->frame_counter % 4 * 32;
		break;
	case RUN:
		player->entity.source.y = 74 + player->frame_counter / 8 * 32;
		player->entity.source.x = 8 + player->frame_counter % 8 * 32;
		break;
	case ROLL:
		player->entity.source.y = 169;
		player->entity.source.x = 8 + player->frame_counter % 8 * 32;
		break;
	case HIT:
		player->entity.source.y = 201;
		player->entity.source.x = 8 + player->frame_counter % 4 * 32;
		break;
	case DEATH:
		player->entity.source.y = 233;
		player->entity.source.x = 8 + player->frame_counter % 4 * 32;
		break;
	}
}

void player_update(Player *player) {
	player->v.y += 1;
	player->entity.dest.x += player->v.x;
	player->entity.dest.y += player->v.y;
	player->entity.hitbox.x = player->entity.dest.x;
	player->entity.hitbox.y = player->entity.dest.y;
}

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

void draw_hud(const Game *game) {
	DrawFPS(0, 0);
	DrawText(TextFormat("state: %d", game->player.state), 0, 20, 10, WHITE);
}

void draw(const Game *game) {
	BeginDrawing();
	ClearBackground(DARKBLUE);

	for (EntityNode *p = game->tiles->head; p; p = p->next) {
		draw_entity(p->value);
	}

	draw_entity(game->player.entity);

	draw_hud(game);

	EndDrawing();
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
