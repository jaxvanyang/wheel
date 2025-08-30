#include <wheel/path.h>

int main() {
	{
		const char *raw_path = "/usr/bin/wc";
		char *path = os_path(raw_path);
		printf("os_path: %s -> %s\n", raw_path, path);
		free(path);
	}
}
