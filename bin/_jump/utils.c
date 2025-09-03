#include "utils.h"

ResourceManager *new_resource_manager() {
	ResourceManager *manager = malloc(sizeof(ResourceManager));

	manager->platform =
		load_texture("assets/brackeys_platformer_assets/sprites/platforms.png");
	manager->player =
		load_texture("assets/brackeys_platformer_assets/sprites/knight.png");
	manager->jump = load_sound("assets/brackeys_platformer_assets/sounds/jump.wav");

	return manager;
}
