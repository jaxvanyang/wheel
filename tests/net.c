#include <assert.h>
#include <string.h>
#include <wheel/net.h>

int main() {
	{
		SockAddr sa = {.addr = 0x01020304, .port = 1234};
		char *addr = format_sa(sa);
		assert(strcmp("1.2.3.4:1234", addr) == 0);
		FREE(addr);
	}
}
