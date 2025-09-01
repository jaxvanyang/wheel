#define _GNU_SOURCE
#include <raylib.h>
#include <string.h>
#include <wheel.h>
#include <wheel/xray.h>

const i32 WIDTH = 800, HEIGHT = 450;
const i32 UNIT = 10;
const i32 N_WIDTH = WIDTH / UNIT, N_HEIGHT = HEIGHT / UNIT;

typedef enum {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
} Direction;

typedef struct {
	i32 x;
	i32 y;
} Position;

typedef struct {
	Direction direction;
	Dequeue *xs;
	Dequeue *ys;
} Snake;

typedef struct {
	Sound die;
	Sound eat;
} Sounds;

typedef struct {
	Snake *snake;
	Position fruit;
	Sounds sounds;
	Music bgm;

	bool is_over;
	bool automatic;
	bool paused;
	bool fastforward;
} Game;

Snake *new_snake() {
	Snake *snake = malloc(sizeof(Snake));

	snake->direction = RIGHT;
	snake->xs = dequeue_new();
	snake->ys = dequeue_new();

	i32 cx = N_WIDTH / 2 * UNIT, cy = N_HEIGHT / 2 * UNIT;

	dequeue_push_back(snake->xs, cx + UNIT);
	dequeue_push_back(snake->ys, cy);
	dequeue_push_back(snake->xs, cx);
	dequeue_push_back(snake->ys, cy);

	return snake;
}

void free_snake(Snake *snake) {
	dequeue_free(snake->xs);
	snake->xs = NULL;
	dequeue_free(snake->ys);
	snake->ys = NULL;

	free(snake);
}

void draw_block(DequeueNode *xp, DequeueNode *yp) {
	Color color = xp->prev ? LIME : GREEN;
	DrawRectangle(xp->value, yp->value, UNIT, UNIT, color);

	bool left = true, right = true, up = true, down = true;

	if (xp->prev) {
		if (xp->prev->value < xp->value) {
			left = false;
		} else if (xp->prev->value > xp->value) {
			right = false;
		} else if (yp->prev->value < yp->value) {
			up = false;
		} else {
			down = false;
		}
	}
	if (xp->next) {
		if (xp->next->value < xp->value) {
			left = false;
		} else if (xp->next->value > xp->value) {
			right = false;
		} else if (yp->next->value < yp->value) {
			up = false;
		} else {
			down = false;
		}
	}

	if (left) {
		DrawLine(xp->value, yp->value, xp->value, yp->value + UNIT, BLACK);
	}
	if (right) {
		DrawLine(xp->value + UNIT, yp->value, xp->value + UNIT, yp->value + UNIT, BLACK);
	}
	if (up) {
		DrawLine(xp->value, yp->value, xp->value + UNIT, yp->value, BLACK);
	}
	if (down) {
		DrawLine(xp->value, yp->value + UNIT, xp->value + UNIT, yp->value + UNIT, BLACK);
	}
}

void draw_snake(Snake *snake) {
	for (DequeueNode *xp = snake->xs->head, *yp = snake->ys->head; xp && yp;
			 xp = xp->next, yp = yp->next) {
		draw_block(xp, yp);
	}
}

Position snake_next(Snake *snake) {
	i32 x = dequeue_first(snake->xs);
	i32 y = dequeue_first(snake->ys);

	switch (snake->direction) {
	case 0:
		return (Position){x, y - UNIT};
	case 1:
		return (Position){x, y + UNIT};
	case 2:
		return (Position){x - UNIT, y};
	default:
		return (Position){x + UNIT, y};
	}
}

void snake_move(Snake *snake) {
	Position pos = snake_next(snake);

	dequeue_pop_back(snake->xs);
	dequeue_pop_back(snake->ys);
	dequeue_push_front(snake->xs, pos.x);
	dequeue_push_front(snake->ys, pos.y);
}

void turn_up(Snake *snake) {
	if (snake->direction != DOWN) {
		snake->direction = UP;
	}
}

void turn_down(Snake *snake) {
	if (snake->direction != UP) {
		snake->direction = DOWN;
	}
}

void turn_left(Snake *snake) {
	if (snake->direction != RIGHT) {
		snake->direction = LEFT;
	}
}

void turn_right(Snake *snake) {
	if (snake->direction != LEFT) {
		snake->direction = RIGHT;
	}
}

void snake_eat(Game *game, Position fruit) {
	PlaySound(game->sounds.eat);
	dequeue_push_front(game->snake->xs, fruit.x);
	dequeue_push_front(game->snake->ys, fruit.y);
}

bool snake_contains(Snake *snake, Position position) {
	for (DequeueNode *xp = snake->xs->head, *yp = snake->ys->head; xp && yp;
			 xp = xp->next, yp = yp->next) {
		if (position.x == xp->value && position.y == yp->value)
			return true;
	}

	return false;
}

bool position_out_of_screen(Position position) {
	return position.x < 0 || position.x + UNIT > WIDTH || position.y < 0 ||
				 position.y + UNIT > HEIGHT;
}

Position random_fruit(Snake *snake) {
	while (true) {
		Position fruit = {
			UNIT * random_range(0, N_WIDTH), UNIT * random_range(0, N_HEIGHT)
		};

		if (snake_contains(snake, fruit))
			continue;

		return fruit;
	}
}

void draw_fruit(Position fruit) {
	DrawRectangle(fruit.x, fruit.y, UNIT, UNIT, RAYWHITE);
}

void draw_fps() {
	Color c = DARKGREEN;
	c.a = 150;
	DrawText(TextFormat("FPS: %2i", GetFPS()), 4, 4, 20, c);
}

void draw_score(usize score) {
	char *text;
	if (asprintf(&text, "Score: %lu", score) == -1) {
        return;
    }
	Color c = BLUE;
	c.a = 150;
	DrawText(text, 4, 24, 20, c);
}

Game *new_game() {
	Game *game = malloc(sizeof(Game));
	game->snake = new_snake();
	game->fruit = random_fruit(game->snake);
	game->is_over = false;
	game->automatic = false;
	game->paused = false;
	game->fastforward = false;

	game->sounds.die =
		load_sound("assets/brackeys_platformer_assets/sounds/explosion.wav");
	game->sounds.eat = load_sound("assets/brackeys_platformer_assets/sounds/coin.wav");
	game->bgm =
		load_music_stream("assets/brackeys_platformer_assets/music/time_for_adventure.mp3");

	return game;
}

void free_game(Game *game) {
	free_snake(game->snake);
	game->snake = NULL;

	UnloadSound(game->sounds.die);
	UnloadSound(game->sounds.eat);
	UnloadMusicStream(game->bgm);

	free(game);
}

void draw_hud(Game *game) {
	draw_score(game->snake->xs->size - 2);
	draw_fps();

	if (game->automatic) {
		Color c = WHITE;
		c.a = 150;
		DrawText("Auto", 4, 44, 20, c);
	}

	if (game->fastforward) {
		Color c = WHITE;
		c.a = 150;
		DrawText("Fastforward", 4, game->automatic ? 64 : 44, 20, c);
	}

	if (game->is_over) {
		const char *text = "GAME OVER";
		Vector2 size = MeasureTextEx(GetFontDefault(), text, 40, 2);
		DrawText(text, (WIDTH - size.x) / 2, (HEIGHT - size.y) / 2, 40, RED);
		return;
	}

	if (game->paused) {
		const char *text = "Paused";
		Vector2 size = MeasureTextEx(GetFontDefault(), text, 40, 2);
		DrawText(text, (WIDTH - size.x) / 2, (HEIGHT - size.y) / 2, 40, WHITE);
	}
}

void draw(Game *game) {
	BeginDrawing();
	ClearBackground(BLACK);

	draw_fruit(game->fruit);
	draw_snake(game->snake);
	draw_hud(game);

	EndDrawing();
}

void reset(Game *game) {
	game->is_over = false;
	free_snake(game->snake);
	game->snake = new_snake();
	game->fruit = random_fruit(game->snake);
	game->paused = false;
}

Direction search_path(Game *game) {
	Snake *snake = game->snake;
	i8 *map = malloc(sizeof(i8) * N_WIDTH * N_HEIGHT);
	memset(map, 0, sizeof(i8) * N_WIDTH * N_HEIGHT);

	map[game->fruit.x / UNIT * N_HEIGHT + game->fruit.y / UNIT] = -1;

	DequeueNode *xp = snake->xs->head, *yp = snake->ys->head;
	map[xp->value / UNIT * N_HEIGHT + yp->value / UNIT] = 1;
	xp = xp->next, yp = yp->next;
	for (; xp && yp; xp = xp->next, yp = yp->next) {
		map[xp->value / UNIT * N_HEIGHT + yp->value / UNIT] = -1;
	}

	Dequeue *xq = dequeue_new(), *yq = dequeue_new();
	dequeue_push_back(xq, game->fruit.x / UNIT);
	dequeue_push_back(yq, game->fruit.y / UNIT);

	i8 ret = -1;

	while (!dequeue_empty(xq) && !dequeue_empty(yq)) {
		i32 x = dequeue_pop_front(xq), y = dequeue_pop_front(yq);

		if (x > 0) {
			if (map[(x - 1) * N_HEIGHT + y] == 1) {
				ret = RIGHT;
				break;
			} else if (map[(x - 1) * N_HEIGHT + y] == 0) {
				map[(x - 1) * N_HEIGHT + y] = 2;
				dequeue_push_back(xq, x - 1);
				dequeue_push_back(yq, y);
			}
		}
		if (x + 1 < N_WIDTH) {
			if (map[(x + 1) * N_HEIGHT + y] == 1) {
				ret = LEFT;
				break;
			} else if (map[(x + 1) * N_HEIGHT + y] == 0) {
				map[(x + 1) * N_HEIGHT + y] = 2;
				dequeue_push_back(xq, x + 1);
				dequeue_push_back(yq, y);
			}
		}
		if (y > 0) {
			if (map[x * N_HEIGHT + y - 1] == 1) {
				ret = DOWN;
				break;
			} else if (map[x * N_HEIGHT + y - 1] == 0) {
				map[x * N_HEIGHT + y - 1] = 2;
				dequeue_push_back(xq, x);
				dequeue_push_back(yq, y - 1);
			}
		}
		if (y + 1 < N_HEIGHT) {
			if (map[x * N_HEIGHT + y + 1] == 1) {
				ret = UP;
				break;
			} else if (map[x * N_HEIGHT + y + 1] == 0) {
				map[x * N_HEIGHT + y + 1] = 2;
				dequeue_push_back(xq, x);
				dequeue_push_back(yq, y + 1);
			}
		}
	}

	dequeue_free(xq);
	dequeue_free(yq);

	if (ret != -1) {
		free(map);
		return ret;
	}

	i32 x = dequeue_first(snake->xs) / UNIT;
	i32 y = dequeue_first(snake->ys) / UNIT;

	if (x > 0 && map[(x - 1) * N_HEIGHT + y] == 0) {
		ret = LEFT;
	} else if (x + 1 < N_WIDTH && map[(x + 1) * N_HEIGHT + y] == 0) {
		ret = RIGHT;
	} else if (y > 0 && map[x * N_HEIGHT + y - 1] == 0) {
		ret = UP;
	} else if (y + 1 < N_HEIGHT && map[x * N_HEIGHT + y + 1] == 0) {
		ret = DOWN;
	} else {
		ret = snake->direction;
	}

	free(map);
	return ret;
}

void toggle_music(Music music) {
	if (IsMusicStreamPlaying(music))
		PauseMusicStream(music);
	else
		PlayMusicStream(music);
}

void auto_update(Game *game) {
	switch (GetKeyPressed()) {
	case KEY_M:
		game->automatic = false;
		break;
	case KEY_R:
		reset(game);
		break;
	case KEY_P:
		game->paused = !game->paused;
		break;
	case KEY_F:
		game->fastforward = !game->fastforward;
		SetTargetFPS(game->fastforward ? 0 : 10);
		break;
	case KEY_B:
		toggle_music(game->bgm);
		break;
	}

	if (game->is_over || game->paused)
		return;

	game->snake->direction = search_path(game);
}

void manual_update(Game *game) {
	switch (GetKeyPressed()) {
	case KEY_UP:
	case KEY_W:
	case KEY_K:
		turn_up(game->snake);
		break;
	case KEY_DOWN:
	case KEY_S:
	case KEY_J:
		turn_down(game->snake);
		break;
	case KEY_LEFT:
	case KEY_A:
	case KEY_H:
		turn_left(game->snake);
		break;
	case KEY_RIGHT:
	case KEY_D:
	case KEY_L:
		turn_right(game->snake);
		break;
	case KEY_R:
		reset(game);
		break;
	case KEY_M:
		game->automatic = true;
		break;
	case KEY_P:
		game->paused = !game->paused;
		break;
	case KEY_F:
		game->fastforward = !game->fastforward;
		SetTargetFPS(game->fastforward ? 0 : 10);
		break;
	case KEY_B:
		toggle_music(game->bgm);
		break;
	}
}

void update(Game *game) {
	if (game->automatic) {
		auto_update(game);
	} else {
		manual_update(game);
	}

	if (game->is_over || game->paused)
		return;

	Position next = snake_next(game->snake);

	if (snake_contains(game->snake, next) || position_out_of_screen(next)) {
		PlaySound(game->sounds.die);
		game->is_over = true;
		return;
	}

	if (next.x == game->fruit.x && next.y == game->fruit.y) {
		snake_eat(game, next);
		if (game->snake->xs->size == N_WIDTH * N_HEIGHT) {
			game->is_over = true;
		} else {
			game->fruit = random_fruit(game->snake);
		}
	} else {
		snake_move(game->snake);
	}
}

int main() {
	InitWindow(WIDTH, HEIGHT, "Snake");
	InitAudioDevice();
	SetTargetFPS(10);
	HideCursor();

	tsrandom();

	Game *game = new_game();
	PlayMusicStream(game->bgm);

	while (!WindowShouldClose()) {
		UpdateMusicStream(game->bgm);

		draw(game);
		update(game);
	}

	free_game(game);
	game = NULL;

	CloseAudioDevice();
	CloseWindow();
}
