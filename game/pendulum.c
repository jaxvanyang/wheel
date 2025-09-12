#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <wheel.h>
#include <wheel/xray.h>
#include <assert.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

static const i32 WIDTH = 800;
static const i32 HEIGHT = 450;
static const i32 FPS = 60;
static const f32 TICK = 1.0 / (f32)FPS;
static const f32 GRAVITY = 10;

typedef struct {
	f32 theta;
	f32 v; // angular velocity
} PendulumArg;

typedef struct {
	Vector2 position;
	f32 length;
	f32 theta;
	f32 v; // angular velocity
} Pendulum;

typedef struct {
	Pendulum pendulum;
	Vector2List *points;
} Game;

Vector2 pendulum_end(Pendulum pendulum) {
	Vector2 diff = {
		pendulum.length * sinf(pendulum.theta), pendulum.length * cosf(pendulum.theta)
	};
	Vector2 end = Vector2Add(pendulum.position, diff);

	return end;
}

void draw_pendulum(Pendulum pendulum, Color color) {
	Vector2 end = pendulum_end(pendulum);
	f32 thick = pendulum.length / 40;

	DrawLineEx(pendulum.position, end, thick, RAYWHITE);
	DrawCircle(end.x, end.y, thick * 2, color);
}

#define new_pendulum(x, y, length, ...) \
	_new_pendulum(x, y, length, (PendulumArg){__VA_ARGS__})
Pendulum _new_pendulum(f32 x, f32 y, f32 length, PendulumArg arg) {
	return (Pendulum
	){.position = (Vector2){x, y}, .length = length, .theta = arg.theta, .v = arg.v};
}

void update_pendulum(Pendulum *pendulum) {
	f32 dv = -GRAVITY / pendulum->length * sinf(pendulum->theta);
	pendulum->v += dv * TICK;
	pendulum->theta += pendulum->v;
}

void handle_input(Pendulum *pendulum) {
	Vector2 mouse = GetMousePosition();
	Vector2 direction = Vector2Subtract(mouse, pendulum->position);

	pendulum->theta = atan2f(direction.x, direction.y);
	pendulum->v = 0;
}

Game new_game() {
	f32 center_x = (f32)WIDTH / 2;
	f32 center_y = (f32)HEIGHT / 2;
	Pendulum pendulum = new_pendulum(center_x, center_y, 100, .theta = PI / 2);
	Vector2List *points = v2list_new();

	return (Game){.pendulum = pendulum, .points = points};
}

void draw_trace(const Vector2List *points, Color color) {
	if (points->size < 2) {
		return;
	}

	f32 alpha = (f32)color.a;
	f32 diff = alpha / (f32)points->size;

	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	for (Vector2Node *p = points->head; p && p->next; p = p->next) {
		rlColor4ub(color.r, color.g, color.b, (unsigned char)alpha);

		rlVertex2f(p->value.x, p->value.y);
		rlVertex2f(p->next->value.x, p->next->value.y);

		alpha -= diff;
	}

	rlEnd();
}

void main_loop(void *arg) {
	const usize n = 100;
	Game *game = arg;

	bool is_dragging = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

	if (is_dragging) {
		handle_input(&game->pendulum);
		v2list_clear(game->points);
	} else {
		update_pendulum(&game->pendulum);
	}

	v2list_push_front(game->points, pendulum_end(game->pendulum));
	if (game->points->size > n) {
		v2list_pop_back(game->points);
	}

	BeginDrawing();
	ClearBackground(BLACK);

	draw_trace(game->points, BLUE);
	draw_pendulum(game->pendulum, BLUE);

	if (!is_dragging) {
		draw_text_tr("Click to drag the pendulum", WIDTH - 5, 5, 20, RAYWHITE);
	}

	f32 theta = game->pendulum.theta / PI;
	DrawText(TextFormat("theta: %+.2f PI", theta), 5, 5, 20, LIME);

	EndDrawing();
}

int main() {
	Game game = new_game();

	InitWindow(WIDTH, HEIGHT, "Simple Pendulum");

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(main_loop, (void *)&game, 0, true);
#else
	SetTargetFPS(FPS);

	while (!WindowShouldClose()) {
		main_loop(&game);
	};
#endif // __EMSCRIPTEN__

	CloseWindow();
}
