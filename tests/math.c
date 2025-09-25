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
		Vec *answer1 = solve_linear(a, b);

		assert(answer1 != NULL);
		assert(b.size == a.rows);
		assert(vec_get(b, 0) == 6);
		assert(vec_get(b, 1) == 9);
		assert(vec_get(b, 2) == 8);
		assert(vec_equal(*answer1, x));

		Mat *inverse = mat_inverse(a);
		assert(inverse != NULL);
		Vec answer2 = mat_x_vec(*inverse, b);
		assert(vec_equal(answer2, x));

		unload_mat(a);
		unload_vec(x);
		unload_vec(b);
		free_vec(answer1);
		free_mat(inverse);
		unload_vec(answer2);
	}

	{
		// clang-format off
		Mat a = mat(2, 2, (f32[]){
			1, 2,
			3, 4,
		});
		// clang-format on
		f32 det = get_det(a);
		f32 answer = 1 * 4 - 2 * 3;

		assert(det == answer);

		unload_mat(a);
	}

	{
		// clang-format off
		Mat a = mat(3, 3, (f32[]){
			0, 1, 2,
			3, 0, 4,
			5, 6, 0,
		});
		// clang-format on
		f32 det = get_det(a);
		f32 answer = 56;

		assert(det == answer);

		unload_mat(a);
	}

	{
		// clang-format off
		Mat a = mat(2, 3, (f32[]){
			0, 1, 2,
			3, 0, 4,
		});
		Mat b = mat(3, 2, (f32[]){
			0, 3,
			1, 0,
			2, 4,
		});
		// clang-format on
		Mat x = mat_trans(a);

		assert(mat_equal(b, x));

		unload_mat(a);
		unload_mat(b);
		unload_mat(x);
	}
}
