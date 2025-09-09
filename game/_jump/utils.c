#include "utils.h"

ResourceManager *new_resource_manager() {
	ResourceManager *manager = malloc(sizeof(ResourceManager));

	manager->platform =
		load_texture("assets/brackeys_platformer_assets/sprites/platforms.png");
	manager->player =
		load_texture("assets/brackeys_platformer_assets/sprites/knight.png");
	manager->tileset =
		load_texture("assets/brackeys_platformer_assets/sprites/world_tileset.png");
	manager->jump = load_sound("assets/brackeys_platformer_assets/sounds/jump.wav");
	manager->explosion = load_sound("assets/brackeys_platformer_assets/sounds/explosion.wav");
	manager->hurt = load_sound("assets/brackeys_platformer_assets/sounds/hurt.wav");
	manager->pixel_operator8 =
		load_font("assets/brackeys_platformer_assets/fonts/PixelOperator8.ttf");

	return manager;
}
