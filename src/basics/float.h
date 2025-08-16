#pragma once

#include "core.h"

extern const u32 F32_M_MASK;
extern const u32 F32_E_MASK;
extern const u64 F64_M_MASK;
extern const u64 F64_E_MASK;

typedef struct {
	f64 m;
	i64 exponent;
	i8 sign;
} Float;

Float decode_f32(f32 number);
Float decode_f64(f64 number);
