#include "utils.h"
#include "../path.h"
#include <raymath.h>

Vector2 get_screen_size() {
	return (Vector2){
		GetScreenWidth(),
		GetScreenHeight(),
	};
}

Texture2D load_texture(const char *path) {
	char *p = os_path(path);
	Texture2D ret = LoadTexture(p);

	free(p);
	p = NULL;

	return ret;
}

Sound load_sound(const char *path) {
	char *p = os_path(path);
	Sound ret = LoadSound(p);

	free(p);
	p = NULL;

	return ret;
}

Music load_music_stream(const char *path) {
	char *p = os_path(path);
	Music ret = LoadMusicStream(p);

	free(p);
	p = NULL;

	return ret;
}

Font load_font(const char *path) {
	char *p = os_path(path);
	Font ret = LoadFont(p);

	free(p);
	p = NULL;

	return ret;
}

float get_default_font_spacing(int font_size) {
	int default_font_size = 10; // Default Font chars height in pixel
	if (font_size < default_font_size)
		font_size = default_font_size;
	int spacing = font_size / default_font_size;

	return (float)spacing;
}

void draw_text_tr(const char *text, int x, int y, int font_size, Color color) {
	int width = MeasureText(text, font_size);
	x -= width;

	DrawText(text, x, y, font_size, color);
}

void draw_text_ex_tr(
	Font font, const char *text, Vector2 position, float font_size, float spacing,
	Color tint
) {
	Vector2 size = MeasureTextEx(font, text, font_size, spacing);
	position.x -= size.x;

	DrawTextEx(font, text, position, font_size, spacing, tint);
}

void draw_text_center(const char *text, int x, int y, int font_size, Color color) {
	int spacing = get_default_font_spacing(font_size);
	Vector2 size = MeasureTextEx(GetFontDefault(), text, font_size, spacing);
	x -= size.x / 2;
	y -= size.y / 2;

	DrawText(text, x, y, font_size, color);
}

void draw_text_ex_center(
	Font font, const char *text, Vector2 position, float font_size, float spacing,
	Color tint
) {
	Vector2 size = MeasureTextEx(font, text, font_size, spacing);
	position.x -= size.x / 2;
	position.y -= size.y / 2;

	DrawTextEx(font, text, position, font_size, spacing, tint);
}

void draw_texture_rec_scale(
	Texture2D texture, Rectangle source, Vector2 position, Color tint, f32 scale
) {
	Rectangle dest = {
		position.x, position.y, source.width * scale, source.height * scale
	};
	DrawTexturePro(texture, source, dest, Vector2Zero(), 0, tint);
}
