#include <assert.h>
#include <wheel/math.h>

int main() {
	{
		// clang-format off
		Mat a = mat(2, 3, (f32[]){
			1, 2, 3,
			4, 5, 6,
		});
		// clang-format on
		Vec x = vec(3, (f32[]){1, 2, 3});
		Vec b = mat_x_vec(a, x);
		// Vec *x_ = solve_linear(a, b);

		// assert(x_ != NULL);
		assert(b.size == a.rows);
		assert(vec_get(b, 0) == 14);
		assert(vec_get(b, 1) == 32);

		unload_mat(a);
		unload_vec(x);
		unload_vec(b);
		// free_vec(x_);
	}

	{
		// clang-format off
		Mat a = mat(3, 3, (f32[]){
			1, 1, 1,
			1, 1, 2,
			1, 2, 1,
		});
		// clang-format on
		Vec x = vec(3, (f32[]){1, 2, 3});
		Vec b = mat_x_vec(a, x);
		Vec *x_ = solve_linear(a, b);

		assert(x_ != NULL);
		assert(b.size == a.rows);
		assert(vec_get(b, 0) == 6);
		assert(vec_get(b, 1) == 9);
		assert(vec_get(b, 2) == 8);
		assert(vec_equal(*x_, x));

		unload_mat(a);
		unload_vec(x);
		unload_vec(b);
		free_vec(x_);
	}
}
