#include <stdio.h>

int main() {
#ifdef _WIN32
	printf("Hello, Windows!\n");
#elif defined(__APPLE__)
#if TARGET_OS_IPHONE
	printf("Hello, iOS!\n");
#elif TARGET_OS_MAC
	printf("Hello, macOS!\n");
#endif
#else
	printf("Hello, Linux (probably)!\n");
#endif
}
