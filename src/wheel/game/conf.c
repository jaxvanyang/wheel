#include "conf.h"
#include "../path.h"
#include "../sys.h"

FILE *open_config(const char *mode) {
	Str *path = get_config_dir();
	path_join(path, "jaxvanyang");
	if (!is_dir(path->data)) {
		make_dir(path->data, 0755);
	}
	path_join(path, "poker.conf");

	if (!is_file(path->data)) {
		FILE *f = fopen(path->data, "w");
		if (f != NULL) {
			fclose(f);
		}
	}

	FILE *f = fopen(path->data, mode);
	str_free(path);

	return f;
}
