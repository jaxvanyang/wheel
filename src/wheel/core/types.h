#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

typedef size_t usize;
typedef ptrdiff_t isize;

typedef float f32;
typedef double f64;

extern const usize USIZE_MAX;

// Platform-appropriate format specifiers
#ifdef _WIN32
#define USIZE_FMT "llu"
#define ISIZE_FMT "lld"
#else
#define USIZE_FMT "zu"
#define ISIZE_FMT "zd"
#endif
