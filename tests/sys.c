#include <stdio.h>
#include <wheel/sys.h>

int main() {
	{
		OSType os = get_os();

		switch (os) {
		case OS_LINUX:
			printf("Hello, Linux\n");
			break;
		case OS_MACOS:
			printf("Hello, macOS\n");
			break;
		case OS_IOS:
			printf("Hello, iOS\n");
			break;
		case OS_WINDOWS:
			printf("Hello, Windows\n");
			break;
		default:
			printf("Hello, unknown OS\n");
		}
	}

	{
		usize n = nproc();
		printf("nproc: %" USIZE_FMT "\n", n);
	}
}
