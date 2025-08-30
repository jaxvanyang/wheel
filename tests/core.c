#include <assert.h>
#include <wheel/core.h>

int main() {
	{
		assert(sizeof(usize) == sizeof(isize));
		assert(sizeof(float) == 4);
		assert(sizeof(double) == 8);
	}
}
