#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <wheel.h>
#include <wheel/xray.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

static const i32 WIDTH = 800;
static const i32 HEIGHT = 450;
static const i32 FPS = 60;
static const f32 TICK = 1.0 / (f32)FPS;
static const f32 GRAVITY = 10;

typedef struct {
	Vector2 position;
	f32 length;
	f32 theta;
	f32 v; // angular velocity
} Pendulum;

typedef struct {
	f32 theta;
	f32 v; // angular velocity
} PendulumArg;

void draw_pendulum(Pendulum pendulum, Color color) {
	Vector2 diff = {
		pendulum.length * sinf(pendulum.theta), pendulum.length * cosf(pendulum.theta)
	};
	Vector2 end = Vector2Add(pendulum.position, diff);
	f32 thick = pendulum.length / 40;

	DrawLineEx(pendulum.position, end, thick, RAYWHITE);
	DrawCircle(end.x, end.y, pendulum.length / 16, color);
}

#define new_pendulum(x, y, length, ...) \
	_new_pendulum(x, y, length, (PendulumArg){__VA_ARGS__})
inline Pendulum _new_pendulum(f32 x, f32 y, f32 length, PendulumArg arg) {
	return (Pendulum){
		.position = (Vector2){x, y}, .length = length, .theta = arg.theta, .v = arg.v
	};
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

static Pendulum pendulum;

void main_loop() {
	BeginDrawing();
	ClearBackground(BLACK);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		handle_input(&pendulum);

	} else {
		update_pendulum(&pendulum);

		draw_text_tr("Click to drag the pendulum", WIDTH - 5, 5, 20, RAYWHITE);
	}

	draw_pendulum(pendulum, BLUE);

	f32 theta = pendulum.theta / PI;
	DrawText(TextFormat("theta: %+.2f PI", theta), 5, 5, 20, LIME);

	EndDrawing();
}

int main() {
	f32 center_x = (f32)WIDTH / 2;
	f32 center_y = (f32)HEIGHT / 2;
	pendulum = new_pendulum(center_x, center_y, 100, .theta = PI / 2);

	InitWindow(WIDTH, HEIGHT, "Simple Pendulum");

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, true);
#else
	SetTargetFPS(FPS);

	while (!WindowShouldClose()) {
		main_loop();
	};
#endif // __EMSCRIPTEN__

	CloseWindow();
}
