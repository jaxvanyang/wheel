//! Inspect IEEE 754 float number.
//!
//! Usage: hex2double <64-bit hex ...>

#include <basics.h>

int main(int argc, char **argv) {
	if (argc <= 1) {
		return 1;
	}

	for (usize i = 1; i < argc; ++i) {
		char *hex = argv[i];
		f64 number;

		sscanf(hex, "%llX", (u64 *)&number);

		u64 *hex_number = (u64 *)&number;
		Float f = decode_f64(number);

		printf(
			"%llX: %hhd x %lf x 2^%lld = %f\n", *hex_number, f.sign, f.m, f.exponent, number
		);
	}
}
