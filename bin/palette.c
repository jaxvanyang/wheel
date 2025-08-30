#include <raylib.h>
#include <wheel.h>

typedef struct {
	Color color;
	char *name;
} NamedColor;

int main() {
	NamedColor colors[] = {
		{LIGHTGRAY, "LIGHTGRAY"}, {GRAY, "GRAY"},					{DARKGRAY, "DARKGRAY"},
		{YELLOW, "YELLOW"},				{GOLD, "GOLD"},					{ORANGE, "ORANGE"},
		{PINK, "PINK"},						{RED, "RED"},						{MAROON, "MAROON"},
		{GREEN, "GREEN"},					{LIME, "LIME"},					{DARKGREEN, "DARKGREEN"},
		{SKYBLUE, "SKYBLUE"},			{BLUE, "BLUE"},					{DARKBLUE, "DARKBLUE"},
		{PURPLE, "PURPLE"},				{VIOLET, "VIOLET"},			{DARKPURPLE, "DARKPURPLE"},
		{BEIGE, "BEIGE"},					{BROWN, "BROWN"},				{DARKBROWN, "DARKBROWN"},
		{WHITE, "WHITE"},					{RAYWHITE, "RAYWHITE"}, {MAGENTA, "MAGENTA"},
	};

	const usize len = sizeof(colors) / sizeof(NamedColor);
	const int width = 200, height = 60;
	const int rows = 6;
	const int cols = (len + rows - 1) / rows;
	const int WIDTH = width * cols, HEIGHT = height * rows;

	InitWindow(WIDTH, HEIGHT, "Palette");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
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
	};

	CloseWindow();
}
