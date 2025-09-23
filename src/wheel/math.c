#include "math.h"
#include <assert.h>
#include <math.h>
#include <string.h>

bool f32_equal(f32 x, f32 y) {
	bool ret = (fabsf(x - y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));

	return ret;
}

bool f64_equal(f64 x, f64 y) {
	bool ret = (fabs(x - y)) <= (EPSILON * fmax(1.0f, fmax(fabs(x), fabs(y))));

	return ret;
}

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

void free_vec(Vec *v) {
	unload_vec(*v);
	v->data = NULL;
	v->size = 0;
	free(v);
}

void print_vec(const Vec v) {
	putchar('[');
	if (v.size) {
		printf("%f", v.data[0]);
	}
	for (usize i = 1; i < v.size; ++i) {
		printf(", %f", v.data[i]);
	}
	printf("]\n");
}

bool vec_equal(const Vec a, const Vec b) {
	assert(a.size == b.size);

	for (usize i = 0; i < a.size; ++i) {
		if (!f32_equal(a.data[i], b.data[i])) {
			return false;
		}
	}

	return true;
}

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

Mat mat_hilb(usize rows) {
	Mat ret = mat_one(rows, rows);

	for (usize i = 0; i < rows; ++i) {
		for (usize j = 0; j < rows; ++j) {
			// 1-based hilb(i, j) = 1 / (i + j - 1)
			ret.data[i * rows + j] /= (i + j + 1);
		}
	}

	return ret;
}

void unload_mat(const Mat m) { free(m.data); }

void free_mat(Mat *m) {
	unload_mat(*m);
	m->data = NULL;
	m->rows = m->cols = 0;
	free(m);
}

void print_mat(const Mat m) {
	for (usize i = 0; i < m.rows; ++i) {
		putchar('|');
		if (m.cols) {
			printf("%f", mat_get(m, i, 0));
		}
		for (usize j = 1; j < m.cols; ++j) {
			printf(", %f", mat_get(m, i, j));
		}
		printf("|\n");
	}
}

usize mat_size(const Mat m) { return m.rows * m.cols; }

bool mat_equal(const Mat a, const Mat b) {
	assert(a.rows == b.rows);
	assert(a.cols == b.cols);

	usize size = mat_size(a);

	for (usize i = 0; i < size; ++i) {
		if (!f32_equal(a.data[i], b.data[i])) {
			return false;
		}
	}

	return true;
}

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

Vec *solve_linear(const Mat a, const Vec b) {
	assert(a.rows == b.size);

	// FIXME: try to solve n x m equation
	if (a.rows != a.cols) {
		return NULL;
	}

	Mat m = mat_clone(a);
	Vec v = vec_clone(b);
	Vec tmp = vec_zero(a.cols);

	Vec *ret = malloc(sizeof(Vec));
	ret->size = tmp.size;
	ret->data = tmp.data;

	if (a.cols == 0) {
		return ret;
	}

	// make m upper-triangle matrix
	for (usize j = 0; j + 1 < m.cols; ++j) {
		// make m[j][j] non-zero
		if (f32_equal(mat_get(m, j, j), 0.0)) {
			for (usize i = j + 1; i < m.rows; ++i) {
				if (!f32_equal(mat_get(m, i, j), 0.0)) {
					// switch m[i] & [j]
					memswap(m.data + i * m.cols, m.data + j * m.cols, sizeof(f32) * m.cols);

					// switch v[i] & v[j]
					memswap(v.data + i, v.data + j, sizeof(f32));

					break;
				}
			}
		}

		f32 m_jj = mat_get(m, j, j);

		if (f32_equal(m_jj, 0.0)) {
			free(ret);
			return NULL;
		}

		f32 v_j = vec_get(v, j);

		// make m[j+1..rows][j] = 0
		for (usize i = j + 1; i < m.rows; ++i) {
			f32 m_ij = mat_get(m, i, j);

			if (f32_equal(m_ij, 0.0)) {
				continue;
			}

			f32 t = m_ij / m_jj;

			mat_set(m, i, j, 0.0);

			// update m[i][j+1..cols]
			for (usize k = j + 1; k < m.cols; ++k) {
				f32 val = mat_get(m, i, k) - t * mat_get(m, j, k);
				mat_set(m, i, k, val);
			}

			// update the v[i]
			f32 val = vec_get(v, i) - t * v_j;
			vec_set(v, i, val);
		}
	}

	// make m diagonal matrix
	for (usize j = m.cols - 1; j > 0; --j) {
		f32 m_jj = mat_get(m, j, j);
		f32 v_j = vec_get(v, j);

		// make m[0..j][j] = 0
		for (usize i = 0; i < j; ++i) {
			f32 m_ij = mat_get(m, i, j);
			f32 t = m_ij / m_jj;

			mat_set(m, i, j, 0.0);

			// update v[i]
			f32 val = vec_get(v, i) - t * v_j;
			vec_set(v, i, val);
		}
	}

	for (usize i = 0; i < m.cols; ++i) {
		f32 m_ii = mat_get(m, i, i);
		f32 v_i = vec_get(v, i);
		f32 val = v_i / m_ii;
		vec_set(*ret, i, val);
	}

	return ret;
}

f32 get_det(const Mat matrix) {
	assert(matrix.rows == matrix.cols);

	// TBD: what's the definition of empty matrix's determinant?
	if (matrix.rows == 0) {
		return 0.0;
	}

	Mat m = mat_clone(matrix);
	f32 ret = 1.0;

	// make m upper-triangle matrix
	for (usize j = 0; j + 1 < m.cols; ++j) {
		// try to make m[j][j] non-zero
		if (f32_equal(mat_get(m, j, j), 0.0)) {
			for (usize i = j + 1; i < m.rows; ++i) {
				if (!f32_equal(mat_get(m, i, j), 0.0)) {
					// switch m[i] & [j]
					memswap(m.data + i * m.cols, m.data + j * m.cols, sizeof(f32) * m.cols);
					ret = -ret;

					break;
				}
			}
		}

		f32 m_jj = mat_get(m, j, j);

		if (f32_equal(m_jj, 0.0)) {
			unload_mat(m);
			return 0.0;
		}

		// make m[j+1..rows][j] = 0
		for (usize i = j + 1; i < m.rows; ++i) {
			f32 m_ij = mat_get(m, i, j);

			if (f32_equal(m_ij, 0.0)) {
				continue;
			}

			f32 t = m_ij / m_jj;

			mat_set(m, i, j, 0.0);

			// update m[i][j+1..cols]
			for (usize k = j + 1; k < m.cols; ++k) {
				f32 val = mat_get(m, i, k) - t * mat_get(m, j, k);
				mat_set(m, i, k, val);
			}
		}
	}

	// check if the m becomes upper-triangle matrix
	for (usize i = 1; i < m.rows; ++i) {
		for (usize j = 0; j < i; ++j) {
			assert(f32_equal(mat_get(m, i, j), 0.0));
		}
	}

	for (usize i = 0; i < m.rows; ++i) {
		ret *= mat_get(m, i, i);
	}

	return ret;
}
