//! Inspect IEEE 754 float number.
//!
//! Usage: hex2float <32-bit hex ...>

#include <wheel.h>

int main(int argc, char **argv) {
	if (argc <= 1) {
		return 1;
	}

	for (int i = 1; i < argc; ++i) {
		char *hex = argv[i];
		f32 number;

		sscanf(hex, "%X", (u32 *)&number);

		u32 *hex_number = (u32 *)&number;
		Float f = decode_f32(number);

		printf(
			"%X: %hhd x %lf x 2^%" PRId64 " = %f\n",
			*hex_number,
			f.sign,
			f.m,
			f.exponent,
			number
		);
	}
}
