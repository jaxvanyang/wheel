#pragma once

#include "core.h"

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

typedef struct {
	usize size;
	f32 *data;
} Vec;

typedef struct {
	usize rows;
	usize cols;
	f32 *data;
} Mat;

// Check whether two given floats are almost equal
bool f32_equal(f32 x, f32 y);
bool f64_equal(f64 x, f64 y);
usize usize_log2(usize n);

f64 factorial(usize n);
f64 binom(usize n, usize k);
f64 arrangement(usize n, usize k);

// NOTE: returned vector needs to be unloaded.
Vec vec(usize size, const f32 *data);
Vec vec_zero(usize size);
Vec vec_one(usize size);
Vec vec_clone(const Vec v);
void unload_vec(const Vec v);
void free_vec(Vec *v);

void print_vec(const Vec v);

bool vec_equal(const Vec a, const Vec b);
f32 vec_get(const Vec v, usize i);
void vec_set(const Vec v, usize i, f32 val);
Vec vec_add(const Vec a, const Vec b);
Vec vec_sub(const Vec a, const Vec b);
Vec vec_mul(const Vec v, f32 k);
Vec vec_div(const Vec v, f32 k);
f32 vec_dot(const Vec a, const Vec b);

// NOTE: returned matrix needs to be unloaded.
Mat mat(usize rows, usize cols, const f32 *data);
Mat mat_zero(usize rows, usize cols);
Mat mat_one(usize rows, usize cols);
Mat mat_clone(const Mat m);
// Return a Hilbert matrix.
Mat mat_hilb(usize rows);
void unload_mat(const Mat m);
void free_mat(Mat *m);

void print_mat(const Mat m);

usize mat_size(const Mat m);
bool mat_equal(const Mat a, const Mat b);
f32 mat_get(const Mat m, usize row, usize col);
void mat_set(const Mat m, usize row, usize col, f32 val);
Mat mat_add(const Mat a, const Mat b);
Mat mat_sub(const Mat a, const Mat b);
Mat mat_scale(const Mat m, f32 k);
Mat mat_mul(const Mat a, const Mat b);
Mat mat_div(const Mat m, f32 k);

Mat mat_trans(const Mat m);
f32 get_cofactor(const Mat m, usize row, usize col);
// Return a cofactor matrix of `m`.
Mat mat_cofactors(const Mat m);
// Return the inverse matrix of `m` if it exists, `NULL` otherwise.
Mat *mat_inverse(const Mat m);

Vec mat_x_vec(const Mat m, const Vec v);
// Solve linear equation, return x in Ax = b.
// Return NULL if no solution.
Vec *solve_linear(const Mat a, const Vec b);

f32 get_det(const Mat matrix);
