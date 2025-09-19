#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <wheel.h>
#include <wheel/xray.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

static const i32 WIDTH = 1080;
static const i32 HEIGHT = 720;
static const i32 FPS = 60;
static const f32 TICK = 1.0 / (f32)FPS;
static const usize STEPS = 6;
static const f32 GRAVITY = 10;

typedef struct {
	f32 theta;
	f32 w; // reprecent omega, angular velocity
	Color color;
	f32 mass;
} PendulumArg;

typedef struct {
	Vector2List *points;
	Vector2 position;
	f32 length;
	f32 theta;
	f32 w; // reprecent omega, angular velocity
	Color color;
	f32 mass;
} Pendulum;

typedef struct {
	f32 a_theta;
	f32 a_w; // angular velocity
	Color a_color;
	f32 a_mass;
	f32 b_theta;
	f32 b_w; // angular velocity
	Color b_color;
	f32 b_mass;
} DoublePendulumArg;

typedef struct {
	Pendulum a;
	Pendulum b;
} DoublePendulum;

typedef struct {
	DoublePendulum pendulum;
	f32 energy;
} Game;

Vector2 pendulum_end(Pendulum pendulum) {
	Vector2 diff = {
		pendulum.length * sinf(pendulum.theta), pendulum.length * cosf(pendulum.theta)
	};
	Vector2 end = Vector2Add(pendulum.position, diff);

	return end;
}

#define new_pendulum(x, y, length, color, ...) \
	_new_pendulum(x, y, length, color, (PendulumArg){.mass = 1.0, __VA_ARGS__})
Pendulum _new_pendulum(f32 x, f32 y, f32 length, Color color, PendulumArg arg) {
	Vector2List *points = v2list_new();
	return (Pendulum){
		.points = points,
		.position = (Vector2){x, y},
		.length = length,
		.color = color,
		.theta = arg.theta,
		.w = arg.w,
		.mass = arg.mass,
	};
}

#define new_double_pendulum(x, y, a_length, a_color, b_length, b_color, ...) \
	_new_double_pendulum( \
		x, \
		y, \
		a_length, \
		a_color, \
		b_length, \
		b_color, \
		(DoublePendulumArg){.a_mass = 1.0, .b_mass = 1.0, __VA_ARGS__} \
	)
DoublePendulum _new_double_pendulum(
	f32 x, f32 y, f32 a_length, Color a_color, f32 b_length, Color b_color,
	DoublePendulumArg arg
) {
	Pendulum a = new_pendulum(
		x, y, a_length, a_color, .theta = arg.a_theta, .w = arg.a_w, .mass = arg.a_mass
	);
	Vector2 end = pendulum_end(a);
	Pendulum b = new_pendulum(
		end.x,
		end.y,
		b_length,
		b_color,
		.theta = arg.b_theta,
		.w = arg.b_w,
		.mass = arg.b_mass
	);

	return (DoublePendulum){.a = a, .b = b};
}

void handle_input(Game *game) {
	Pendulum *a = &game->pendulum.a;
	Pendulum *b = &game->pendulum.b;

	Vector2 mouse = GetMousePosition();
	Vector2 diff = Vector2Subtract(mouse, a->position);

	f32 total_length = a->length + b->length;
	f32 d = Vector2Length(diff);

	if (total_length <= d) {
		a->theta = b->theta = atan2f(diff.x, diff.y);
		b->position = pendulum_end(*a);
	} else {
		f32 cos_alpha =
			(a->length * a->length + d * d - b->length * b->length) / 2 / a->length / d;
		f32 alpha = acosf(cos_alpha);
		f32 beta = atan2f(-diff.y, diff.x) + PI / 2;
		f32 cos_gamma = (a->length * a->length + b->length * b->length - d * d) / 2 /
										a->length / b->length;
		f32 gamma = acosf(cos_gamma);

		a->theta = beta - alpha;
		b->theta = PI + a->theta - gamma;
		b->position = pendulum_end(*a);
	}

	a->w = b->w = 0.0;
	game->energy = 0.0;
}

void update_trace(DoublePendulum *pendulum) {
	// trace point counts
	const usize n1 = 100;
	const usize n2 = n1 * 10;

	Vector2 a_end = pendulum_end(pendulum->a);
	Vector2 b_end = pendulum_end(pendulum->b);

	v2list_push_front(pendulum->a.points, a_end);
	v2list_push_front(pendulum->b.points, b_end);

	if (pendulum->a.points->size > n1) {
		v2list_pop_back(pendulum->a.points);
	}

	if (pendulum->b.points->size > n2) {
		v2list_pop_back(pendulum->b.points);
	}
}

// Calculate radius acceleration
Vector2 get_acceleration(const DoublePendulum *pendulum) {
	Pendulum a = pendulum->a;
	Pendulum b = pendulum->b;

	f32 mass_sum = a.mass + b.mass;
	f32 delta_theta = a.theta - b.theta;
	f32 cos_delta_theta = cosf(delta_theta);
	f32 sin_delta_theta = sinf(delta_theta);

	f32 v1 = -b.mass * b.length * powf(b.w, 2.0) * sin_delta_theta;
	v1 -= mass_sum * GRAVITY * sinf(a.theta);
	f32 v2 = a.length * powf(a.w, 2.0) * sin_delta_theta - GRAVITY * sinf(b.theta);

	// solve x in matrix equation mx = v

	// clang-format off
	Mat m = mat(2, 2, (f32 *)(f32[][2]){
		{mass_sum * a.length, b.mass * b.length * cos_delta_theta},
		{a.length * cos_delta_theta, b.length},
	});
	// clang-format on
	Vec v = vec(2, (f32[]){v1, v2});

	Vec *x = solve_linear(m, v);
	assert(x != NULL);

	Vector2 ret = {vec_get(*x, 0), vec_get(*x, 1)};

	free_vec(x);
	unload_vec(v);
	unload_mat(m);

	return ret;
}

void update_double_pendulum(DoublePendulum *pendulum) {
	usize n = 1;
	f32 dt = TICK / (f32)n;

	for (usize i = 0; i < n; ++i) {
		Pendulum *a = &pendulum->a;
		Pendulum *b = &pendulum->b;
		Vector2 accel = get_acceleration(pendulum);

		// use Verlet algorithm

		a->w += 0.5 * accel.x * dt;
		a->theta += a->w * dt;
		a->theta = fmodf(a->theta, PI * 2);

		b->position = pendulum_end(*a);

		b->w += 0.5 * accel.y * dt;
		b->theta += b->w * dt;
		b->theta = fmodf(b->theta, PI * 2);

		accel = get_acceleration(pendulum);

		a->w += 0.5 * accel.x * dt;
		b->w += 0.5 * accel.y * dt;
	}
}

void clear_points(DoublePendulum *pendulum) {
	v2list_clear(pendulum->a.points);
	v2list_clear(pendulum->b.points);
}

Game new_game() {
	f32 center_x = (f32)WIDTH / 2;
	f32 center_y = (f32)HEIGHT / 2;
	DoublePendulum pendulum = new_double_pendulum(
		center_x,
		center_y,
		100,
		BLUE,
		100,
		RED,
		.a_mass = 1e-3,
		.a_theta = 2.453,
		.b_mass = 1e-3,
		.b_theta = -2.7727,
	);

	return (Game){.pendulum = pendulum, .energy = 0.0};
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

void draw_pendulum(Pendulum pendulum) {
	Vector2 end = pendulum_end(pendulum);
	f32 thick = pendulum.length / 40;

	draw_trace(pendulum.points, pendulum.color);
	DrawLineEx(pendulum.position, end, thick, RAYWHITE);
	DrawCircle(end.x, end.y, thick * 2, pendulum.color);
}

void draw_double_pendulum(DoublePendulum pendulum) {
	draw_pendulum(pendulum.b);
	draw_pendulum(pendulum.a);
}

void update_draw_info(Game *game) {
	Pendulum a = game->pendulum.a;
	Pendulum b = game->pendulum.b;

	f32 delta_theta = a.theta - b.theta;
	f32 cos_delta_theta = cosf(delta_theta);

	f32 t1 = 0.5 * a.mass * powf(a.length * a.w, 2.0);
	f32 t2 = 0.5 * b.mass *
					 (powf(a.length * a.w, 2.0) + powf(b.length * b.w, 2.0) +
						2.0 * a.length * b.length * a.theta * b.theta * cos_delta_theta);
	f32 t = t1 + t2;

	f32 v1 = -a.mass * GRAVITY * a.length * cosf(a.theta);
	f32 v2 = -b.mass * GRAVITY * (a.length * cosf(a.theta) + b.length * cosf(b.theta));
	f32 v = v1 + v2;

	f32 e = t + v;
	f32 error = e - game->energy;
	game->energy = e;

	Str *text = str_new();
	str_push_str(text, TextFormat("a_theta: %+.2f PI\n", a.theta));
	str_push_str(text, TextFormat("b_theta: %+.2f PI\n", b.theta));
	str_push_str(text, TextFormat("T: %.2f\n", t));
	str_push_str(text, TextFormat("V: %.2f\n", v));
	str_push_str(text, TextFormat("E: %.2f\n", e));
	str_push_str(text, TextFormat("Error: %.2f\n", error));

	DrawText(text->data, 5, 5, 20, LIME);

	str_free(text);
}

void main_loop(void *arg) {
	Game *game = arg;

	bool is_dragging = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

	if (is_dragging) {
		handle_input(game);
		clear_points(&game->pendulum);
	} else {
		for (usize i = 0; i < STEPS; ++i) {
			update_double_pendulum(&game->pendulum);
		}
		update_trace(&game->pendulum);
	}

	BeginDrawing();
	ClearBackground(BLACK);

	draw_double_pendulum(game->pendulum);

	if (!is_dragging) {
		draw_text_tr("Click to drag the pendulum", WIDTH - 5, 5, 20, RAYWHITE);
	}

	update_draw_info(game);

	EndDrawing();
}

int main() {
	Game game = new_game();

	InitWindow(WIDTH, HEIGHT, "Double Pendulum");

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
