#include <raylib.h>
#include <wheel.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

typedef struct {
	Color color;
	char *name;
} NamedColor;

static NamedColor colors[] = {
	{LIGHTGRAY, "LIGHTGRAY"}, {GRAY, "GRAY"},					{DARKGRAY, "DARKGRAY"},
	{YELLOW, "YELLOW"},				{GOLD, "GOLD"},					{ORANGE, "ORANGE"},
	{PINK, "PINK"},						{RED, "RED"},						{MAROON, "MAROON"},
	{GREEN, "GREEN"},					{LIME, "LIME"},					{DARKGREEN, "DARKGREEN"},
	{SKYBLUE, "SKYBLUE"},			{BLUE, "BLUE"},					{DARKBLUE, "DARKBLUE"},
	{PURPLE, "PURPLE"},				{VIOLET, "VIOLET"},			{DARKPURPLE, "DARKPURPLE"},
	{BEIGE, "BEIGE"},					{BROWN, "BROWN"},				{DARKBROWN, "DARKBROWN"},
	{WHITE, "WHITE"},					{RAYWHITE, "RAYWHITE"}, {MAGENTA, "MAGENTA"},
};

const static usize len = sizeof(colors) / sizeof(NamedColor);
const static int width = 200, height = 60;
const static int rows = 6;
const static int cols = (len + rows - 1) / rows;
const static int WIDTH = width * cols, HEIGHT = height * rows;

void update_draw() {
	BeginDrawing();
	ClearBackground(BLACK);

	for (usize i = 0; i < len; ++i) {
		int row = i % rows;
		int col = i / rows;
		int x = width * col, y = height * row;
		NamedColor c = colors[i];

		DrawRectangle(x, y, width, height, c.color);
		DrawText(c.name, x + 4, y + 4, 20, BLACK);
		DrawText(TextFormat("0x%08X", c.color), x + 4, y + 24, 15, BLACK);
	}

	EndDrawing();
}

int main() {

	InitWindow(WIDTH, HEIGHT, "Palette");

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(update_draw, 0, true);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update_draw();
	};
#endif // __EMSCRIPTEN__

	CloseWindow();
}
