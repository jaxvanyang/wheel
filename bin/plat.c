#include <raylib.h>
#include <raymath.h>
#include <wheel.h>
#include <wheel/xray.h>

const i32 WIDTH = 800;
const i32 HEIGHT = 450;
const i32 FPS = 60;
const i32 SPRITE_FPS = 10;
const i32 FPS_K = FPS / SPRITE_FPS;

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

typedef struct {
	ResourceManager *manager;
	Player player;
	Entity *tiles;
	u32 frame_counter;
} Game;

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

void unload_resource_manager(ResourceManager manager) {
	// TODO
}

Entity new_platform(Texture2D texture, f32 x, f32 y) {
	Rectangle source = {0, 0, 16, 9};
	Rectangle dest = {x, y, 2 * source.width, 2 * source.height};
	return (Entity){texture, source, dest, dest};
}

Game *new_game() {
	Game *game = malloc(sizeof(Game));
	game->manager = new_resource_manager();
	game->frame_counter = 0;
	game->player = new_player(game->manager->player, 0, 0);

	usize len = WIDTH / 32;
	game->tiles = malloc(sizeof(Entity) * len);
	for (usize i = 0; i < len; ++i) {
		game->tiles[i] = new_platform(game->manager->platform, i * 32, HEIGHT - 18);
	}

	return game;
}

void draw_hud(const Game *game) {
	DrawFPS(0, 0);
	DrawText(TextFormat("state: %d", game->player.state), 0, 20, 10, WHITE);
}

void draw(const Game *game) {
	BeginDrawing();
	ClearBackground(DARKBLUE);

	usize len = WIDTH / 32;
	for (usize i = 0; i < len; ++i) {
		draw_entity(game->tiles[i]);
	}

	draw_entity(game->player.entity);

	draw_hud(game);

	EndDrawing();
}

void hit_and_correct(Game *game) {
	usize len = WIDTH / 32;

	for (usize i = 0; i < len; ++i) {
		Rectangle hit = GetCollisionRec(game->player.entity.hitbox, game->tiles[i].hitbox);
		if (hit.width != 0) {
			game->player.v.y = 0;
			game->player.entity.dest.y = hit.y - game->player.entity.dest.height;
		}
	}
}

// Process input
void input(Game *game) {
	if (IsKeyPressed(KEY_K)) {
		game->player.v.y = -10;
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

int main() {
	InitWindow(WIDTH, HEIGHT, "Play");
	InitAudioDevice();
	SetTargetFPS(FPS);

	Game *game = new_game();

	while (!WindowShouldClose()) {
		input(game);
		update(game);
		draw(game);
	}

	CloseAudioDevice();
	CloseWindow();
}
