#include "widget.h"
#include "utils.h"

void draw_dialogue(
	const char *text, Rectangle dest, f32 font_size, Color fg_color, Color bg_color
) {
	Vector2 radius = {
		.x = dest.width / 2,
		.y = dest.height / 2,
	};
	Vector2 center = {
		.x = dest.x + radius.x,
		.y = dest.y + radius.y,
	};

	DrawEllipse(center.x, center.y, radius.x, radius.y, bg_color);
	DrawEllipseLines(center.x, center.y, radius.x, radius.y, BLACK);
	draw_text_center(text, center.x, center.y, font_size, fg_color);
}
