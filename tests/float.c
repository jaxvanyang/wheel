#include <assert.h>
#include <basics/float.h>

int main() {
	{
		Float f = decode_f32(0.25);
		assert(f.sign == 1);
		assert(f.m == 1.0);
		assert(f.exponent == -2);
	}

	{
		Float f = decode_f32(-0.25);
		assert(f.sign == -1);
		assert(f.m == 1.0);
		assert(f.exponent == -2);
	}

	{
		Float f = decode_f64(0.25);
		assert(f.sign == 1);
		assert(f.m == 1.0);
		assert(f.exponent == -2);
	}

	{
		Float f = decode_f64(-0.25);
		assert(f.sign == -1);
		assert(f.m == 1.0);
		assert(f.exponent == -2);
	}
}
