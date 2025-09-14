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

		assert(b.size == a.rows);
		assert(vec_get(b, 0) == 14);
		assert(vec_get(b, 1) == 32);

		unload_mat(a);
		unload_vec(x);
		unload_vec(b);
	}
}
