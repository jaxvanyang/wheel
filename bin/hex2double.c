//! Inspect IEEE 754 float number.
//!
//! Usage: hex2double <64-bit hex ...>

#include <wheel.h>

int main(int argc, char **argv) {
	if (argc <= 1) {
		return 1;
	}

	for (int i = 1; i < argc; ++i) {
		char *hex = argv[i];
		f64 number;

		sscanf(hex, "%" PRIx64 "", (u64 *)&number);

		u64 *hex_number = (u64 *)&number;
		Float f = decode_f64(number);

		printf(
			"%" PRIx64 ": %hhd x %lf x 2^%" PRId64 " = %f\n",
			*hex_number,
			f.sign,
			f.m,
			f.exponent,
			number
		);
	}
}
