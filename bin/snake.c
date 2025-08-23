#include "basics/core/error.h"
#include <basics.h>
#include <raylib.h>

const i32 WIDTH = 800, HEIGHT = 450;
const i32 UNIT = 10;
const i32 N_WIDTH = WIDTH / UNIT, N_HEIGHT = HEIGHT / UNIT;

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
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
	Snake *snake;
	Position fruit;
	bool is_over;
	bool automatic;
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

void draw_snake(Snake *snake) {
	DrawRectangle(
		dequeue_first(snake->xs), dequeue_first(snake->ys), UNIT, UNIT, RAYWHITE);

	DequeueNode *xp = snake->xs->head->next;
	DequeueNode *yp = snake->ys->head->next;

	while (xp && yp) {
		DrawRectangle(xp->value, yp->value, UNIT, UNIT, LIME);
		xp = xp->next;
		yp = yp->next;
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

void snake_eat(Snake *snake, Position fruit) {
	dequeue_push_front(snake->xs, fruit.x);
	dequeue_push_front(snake->ys, fruit.y);
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
			UNIT * random_range(0, N_WIDTH), UNIT * random_range(0, N_HEIGHT)};

		if (snake_contains(snake, fruit))
			continue;

		return fruit;
	}
}

void draw_fruit(Position fruit) { DrawRectangle(fruit.x, fruit.y, UNIT, UNIT, PINK); }

void draw_fps() {
	Color c = LIME;
	c.a = 150;
	DrawText(TextFormat("FPS: %2i", GetFPS()), 0, 0, 20, c);
}

void draw_score(usize score) {
	char *text;
	asprintf(&text, "Score: %lu", score);
	Color c = ORANGE;
	c.a = 150;
	DrawText(text, 0, 20, 20, c);
}

Game *new_game() {
	Game *game = malloc(sizeof(Game));
	game->snake = new_snake();
	game->fruit = random_fruit(game->snake);
	game->is_over = false;
	game->automatic = false;

	return game;
}

void free_game(Game *game) {
	free_snake(game->snake);
	game->snake = NULL;

	free(game);
}

void draw(Game *game) {
	BeginDrawing();
	ClearBackground(BLACK);

	draw_fruit(game->fruit);
	draw_snake(game->snake);
	draw_score(game->snake->xs->size - 2);
	draw_fps();
	if (game->automatic) {
		Color c = WHITE;
		c.a = 150;
		DrawText("Auto", 0, 40, 20, c);
	}
	if (game->is_over) {
		DrawText("Game Over", WIDTH / 2 - 80, HEIGHT / 2 - 20, 40, RED);
	}

	EndDrawing();
}

void reset(Game *game) {
	game->is_over = false;
	free_snake(game->snake);
	game->snake = new_snake();
	game->fruit = random_fruit(game->snake);
}

void auto_update(Game *game) {
	if (N_WIDTH & 1) {
		error("auto mode expects even WIDTH, but found: %i\n", WIDTH);
	}

	switch (GetKeyPressed()) {
	case KEY_M:
		game->automatic = false;
		break;
	case KEY_R:
		reset(game);
		break;
	}

	Snake *snake = game->snake;
	i32 x = dequeue_first(snake->xs);
	i32 y = dequeue_first(snake->ys);
	i32 nx = x / UNIT, ny = y / UNIT;

	if (ny == 0 && nx == 0) {
		turn_down(snake);
	} else if (ny == 0) {
		turn_left(snake);
	} else if ((nx & 1) == 0 && ny + 1 == N_HEIGHT) {
		turn_right(snake);
	} else if ((nx & 1) == 0) {
		turn_down(snake);
	} else if (nx + 1 == N_WIDTH) {
		turn_up(snake);
	} else if (nx & 1 && ny == 1) {
		turn_right(snake);
	} else if (nx & 1) {
		turn_up(snake);
	} else {
		error("unexpected situation");
	}
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
	}
}

void update(Game *game) {
	if (game->automatic) {
		auto_update(game);
	} else {
		manual_update(game);
	}

	if (game->is_over)
		return;

	Position next = snake_next(game->snake);

	if (snake_contains(game->snake, next) || position_out_of_screen(next)) {
		game->is_over = true;
		return;
	}

	if (next.x == game->fruit.x && next.y == game->fruit.y) {
		snake_eat(game->snake, next);
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
	SetTargetFPS(10);
	HideCursor();

	tsrandom();

	Game *game = new_game();

	while (!WindowShouldClose()) {
		draw(game);
		update(game);
	}

	CloseWindow();

	free_game(game);
	game = NULL;
}
