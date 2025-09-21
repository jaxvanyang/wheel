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
}
