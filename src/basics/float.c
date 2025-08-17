#include "float.h"

// S: 1, E: 8, M: 23
const u32 F32_M_MASK = (1u << 23) - 1;
const u32 F32_E_MASK = (1u << 31) - 1 - F32_M_MASK;

// S: 1, E: 11, M: 52
const u64 F64_M_MASK = (1lu << 52) - 1;
const u64 F64_E_MASK = (1lu << 63) - 1 - F64_M_MASK;

Float decode_f32(f32 number) {
	u32 *u32_number = (u32 *)&number;
	Float ret;

	ret.m = (f64)(*u32_number & F32_M_MASK) / (1u << 23);
	ret.exponent = (*u32_number & F32_E_MASK) >> 23;
	if (ret.exponent) {
		ret.m += 1;
	}
	ret.exponent -= 127;
	ret.sign = *u32_number >> 31 ? -1 : 1;

	return ret;
}

Float decode_f64(f64 number) {
	u64 *u64_number = (u64 *)&number;
	Float ret;

	ret.m = (f64)(*u64_number & F64_M_MASK) / (1lu << 52);
	ret.exponent = (*u64_number & F64_E_MASK) >> 52;
	if (ret.exponent) {
		ret.m += 1;
	}
	ret.exponent -= 1023;
	ret.sign = *u64_number >> 63 ? -1 : 1;

	return ret;
}
