#include <assert.h>
#include <basics/core.h>

int main() {
	{
		assert(sizeof(usize) == sizeof(isize));
		assert(sizeof(float) == 4);
		assert(sizeof(double) == 8);
	}
}
