#include <stdio.h>
#include <wheel/sys.h>

int main() {
	usize n = nproc();
	printf("%" USIZE_FMT "\n", n);
}
