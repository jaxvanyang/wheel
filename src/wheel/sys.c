#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "sys.h"

OSType get_os() {
#ifdef __linux__
	return OS_LINUX;
#elif TARGET_OS_MAC
	return OS_MACOS;
#elif TARGET_OS_IPHONE
	return OS_IOS;
#elif defined(_WIN32)
	return OS_WINDOWS;
#else
	return OS_UNKNOWN;
#endif
}

usize get_nproc() {
#ifdef _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return sysinfo.dwNumberOfProcessors;
#else
	return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
