#include <assert.h>
#include <string.h>
#include <wheel/net.h>

int main() {
	{
		UDPServer server = udp_server(0x01020304, 1234);
		char *addr = format_sa(server.sa);
		assert(strcmp("1.2.3.4:1234", addr) == 0);
		FREE(addr);
	}
}
