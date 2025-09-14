#include "math.h"
#include <assert.h>
#include <string.h>

usize usize_log2(usize n) {
	if (n == 0) {
		error("expected n != 0\n");
	}

	usize c = 0;
	while (n != 1) {
		n >>= 1;
		++c;
	}

	return c;
}

Vec vec(usize size, const f32 *data) {
	Vec v = {size, malloc(sizeof(f32) * size)};
	memcpy(v.data, data, sizeof(f32) * size);

	return v;
}

Vec vec_zero(usize size) {
	Vec v = {size, malloc(sizeof(f32) * size)};
	memset(v.data, 0, sizeof(f32) * size);

	return v;
}

Vec vec_one(usize size) {
	Vec v = {size, malloc(sizeof(f32) * size)};

	for (usize i = 0; i < size; ++i) {
		v.data[i] = 1.0;
	}

	return v;
}

Vec vec_clone(const Vec v) { return vec(v.size, v.data); }

void unload_vec(const Vec v) { free(v.data); }

f32 vec_get(const Vec v, usize i) {
	if (i >= v.size) {
		error("expected i < %zu\n", v.size);
	}

	return v.data[i];
}

void vec_set(const Vec v, usize i, f32 val) {
	if (i >= v.size) {
		error("expected i < %zu\n", v.size);
	}

	v.data[i] = val;
}

Vec vec_add(const Vec a, const Vec b) {
	assert(a.size == b.size);

	Vec ret = vec_clone(a);

	for (usize i = 0; i < a.size; ++i) {
		ret.data[i] += b.data[i];
	}

	return ret;
}

Vec vec_sub(const Vec a, const Vec b) {
	assert(a.size == b.size);

	Vec ret = vec_clone(a);

	for (usize i = 0; i < a.size; ++i) {
		ret.data[i] -= b.data[i];
	}

	return ret;
}

Vec vec_mul(const Vec v, f32 k) {
	Vec ret = vec_clone(v);

	for (usize i = 0; i < v.size; ++i) {
		ret.data[i] *= k;
	}

	return ret;
}

Vec vec_div(const Vec v, f32 k) {
	Vec ret = vec_clone(v);

	for (usize i = 0; i < v.size; ++i) {
		ret.data[i] /= k;
	}

	return ret;
}

f32 vec_dot(const Vec a, const Vec b) {
	assert(a.size == b.size);

	f32 ret = 0.0;

	for (usize i = 0; i < a.size; ++i) {
		ret += a.data[i] * b.data[i];
	}

	return ret;
}

Mat mat(usize rows, usize cols, const f32 *data) {
	usize size = rows * cols;
	Mat ret = {rows, cols, malloc(sizeof(f32) * size)};
	memcpy(ret.data, data, sizeof(f32) * size);

	return ret;
}

Mat mat_zero(usize rows, usize cols) {
	usize size = rows * cols;
	Mat ret = {rows, cols, malloc(sizeof(f32) * size)};
	memset(ret.data, 0, sizeof(f32) * size);

	return ret;
}

Mat mat_one(usize rows, usize cols) {
	usize size = rows * cols;
	Mat ret = {rows, cols, malloc(sizeof(f32) * size)};

	for (usize i = 0; i < size; ++i) {
		ret.data[i] = 1.0;
	}

	return ret;
}

Mat mat_clone(const Mat m) { return mat(m.rows, m.cols, m.data); }

void unload_mat(const Mat m) { free(m.data); }

usize mat_size(const Mat m) { return m.rows * m.cols; }

f32 mat_get(const Mat m, usize row, usize col) {
	if (row >= m.rows) {
		error("expected row < %zu\n", m.rows);
	} else if (col >= m.cols) {
		error("expected row < %zu\n", m.rows);
	}

	return m.data[row * m.cols + col];
}

void mat_set(const Mat m, usize row, usize col, f32 val) {
	if (row >= m.rows) {
		error("expected row < %zu\n", m.rows);
	} else if (col >= m.cols) {
		error("expected row < %zu\n", m.rows);
	}

	m.data[row * m.cols + col] = val;
}

Mat mat_add(const Mat a, const Mat b) {
	assert(a.rows == b.rows);
	assert(a.cols == b.cols);

	Mat ret = mat_clone(a);
	usize size = mat_size(ret);

	for (usize i = 0; i < size; ++i) {
		ret.data[i] += b.data[i];
	}

	return ret;
}

Mat mat_sub(const Mat a, const Mat b) {
	assert(a.rows == b.rows);
	assert(a.cols == b.cols);

	Mat ret = mat_clone(a);
	usize size = mat_size(ret);

	for (usize i = 0; i < size; ++i) {
		ret.data[i] -= b.data[i];
	}

	return ret;
}

Mat mat_mul(const Mat m, f32 k) {
	Mat ret = mat_clone(m);
	usize size = mat_size(ret);

	for (usize i = 0; i < size; ++i) {
		ret.data[i] *= k;
	}

	return ret;
}

Mat mat_div(const Mat m, f32 k) {
	Mat ret = mat_clone(m);
	usize size = mat_size(ret);

	for (usize i = 0; i < size; ++i) {
		ret.data[i] /= k;
	}

	return ret;
}

Vec mat_x_vec(const Mat m, const Vec v) {
	assert(m.cols == v.size);

	Vec ret = vec_zero(m.rows);

	for (usize i = 0; i < m.rows; ++i) {
		for (usize j = 0; j < m.cols; ++j) {
			ret.data[i] += mat_get(m, i, j) * v.data[j];
		}
	}

	return ret;
}
