#include <assert.h>
#include <wheel/float.h>

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

	{
		u32 hex = 0x00400000;
		Float f = decode_f32(*(f32 *)&hex);
		assert(f.sign == 1);
		assert(f.m == 0.5);
		assert(f.exponent == -127);
	}

	{
		u32 hex = 0x80400000;
		Float f = decode_f32(*(f32 *)&hex);
		assert(f.sign == -1);
		assert(f.m == 0.5);
		assert(f.exponent == -127);
	}

	{
		u64 hex = 0x0008000000000000;
		Float f = decode_f64(*(f64 *)&hex);
		assert(f.sign == 1);
		assert(f.m == 0.5);
		assert(f.exponent == -1023);
	}

	{
		u64 hex = 0x8008000000000000;
		Float f = decode_f64(*(f64 *)&hex);
		assert(f.sign == -1);
		assert(f.m == 0.5);
		assert(f.exponent == -1023);
	}
}
