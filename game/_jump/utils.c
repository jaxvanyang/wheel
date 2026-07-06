#include "utils.h"
#include <wheel/path.h>
#include <wheel/sys.h>

ResourceManager *new_resource_manager() {
	ResourceManager *manager = malloc(sizeof(ResourceManager));

	Str *path = path_new("assets/brackeys_platformer_assets");
	if (!is_dir(path->data)) {
		str_free(path);
		path = get_assets_dir();
		path_join(path, "brackeys_platformer_assets");
	}
	path_join(path, "sprites/platforms.png");
	manager->platform = load_texture(path->data);
	path_strip_and_join(path, 1, "knight.png");
	manager->player = load_texture(path->data);
	path_strip_and_join(path, 1, "world_tileset.png");
	manager->tileset = load_texture(path->data);
	path_strip_and_join(path, 2, "music/time_for_adventure.mp3");
	manager->bgm = load_music_stream(path->data);
	path_strip_and_join(path, 2, "sounds/jump.wav");
	manager->jump = load_sound(path->data);
	path_strip_and_join(path, 1, "explosion.wav");
	manager->explosion = load_sound(path->data);
	path_strip_and_join(path, 1, "hurt.wav");
	manager->hurt = load_sound(path->data);
	path_strip_and_join(path, 2, "fonts/PixelOperator8.ttf");
	manager->pixel_operator8 = load_font(path->data);
	str_free(path);

	return manager;
}
