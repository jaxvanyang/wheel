#include <assert.h>
#include <string.h>
#include <wheel/path.h>

int main() {
	{
		const char *raw_path = "/usr/bin/wc";
		char *path = os_path(raw_path);
#ifdef _WIN32
		assert(strcmp(path, "\\usr\\bin\\wc") == 0);
#else
		assert(strcmp(path, "/usr/bin/wc") == 0);
#endif
		free(path);
	}

	{
		Str *path = str_from("a");
		path_join(path, "b");

#ifdef _WIN32
		assert(strcmp(path->data, "a\\b") == 0);
#else
		assert(strcmp(path->data, "a/b") == 0);
#endif
	}

#ifndef _WIN32
	{
		Str *home_dir = get_home_dir();
		Str *config_dir = get_config_dir();
		Str *cache_dir = get_cache_dir();
		Str *data_dir = get_data_dir();

		printf("home_dir: %s\n", home_dir->data);
		printf("config_dir: %s\n", config_dir->data);
		printf("cache_dir: %s\n", cache_dir->data);
		printf("data_dir: %s\n", data_dir->data);

		str_free(home_dir);
		str_free(config_dir);
		str_free(cache_dir);
		str_free(data_dir);
	}
#endif // _WIN32
}
