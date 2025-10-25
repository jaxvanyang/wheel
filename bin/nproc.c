#include <stdio.h>
#include <wheel/sys.h>

int main() {
	usize n = get_nproc();
	printf("%" USIZE_FMT "\n", n);
}
