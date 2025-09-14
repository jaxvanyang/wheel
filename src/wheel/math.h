#pragma once

#include "core.h"

typedef struct {
	usize size;
	f32 *data;
} Vec;

typedef struct {
	usize rows;
	usize cols;
	f32 *data;
} Mat;

usize usize_log2(usize n);

Vec vec(usize size, const f32 *data);
Vec vec_zero(usize size);
Vec vec_one(usize size);
Vec vec_clone(const Vec v);
void unload_vec(const Vec v);

f32 vec_get(const Vec v, usize i);
void vec_set(const Vec v, usize i, f32 val);
Vec vec_add(const Vec a, const Vec b);
Vec vec_sub(const Vec a, const Vec b);
Vec vec_mul(const Vec v, f32 k);
Vec vec_div(const Vec v, f32 k);
f32 vec_dot(const Vec a, const Vec b);

Mat mat(usize rows, usize cols, const f32 *data);
Mat mat_zero(usize rows, usize cols);
Mat mat_one(usize rows, usize cols);
Mat mat_clone(const Mat m);
void unload_mat(const Mat m);

usize mat_size(const Mat m);
f32 mat_get(const Mat m, usize row, usize col);
void mat_set(const Mat m, usize row, usize col, f32 val);
Mat mat_add(const Mat a, const Mat b);
Mat mat_sub(const Mat a, const Mat b);
Mat mat_mul(const Mat m, f32 k);
Mat mat_div(const Mat m, f32 k);

Vec mat_x_vec(const Mat m, const Vec v);
