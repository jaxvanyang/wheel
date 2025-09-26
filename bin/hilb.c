#include <wheel.h>

int main() {
	for (usize i = 1; i <= 10; ++i) {
		Mat hilb = mat_hilb(i);
		f32 det = get_det(hilb);
		print_mat(hilb);
		printf("\ndet(hilb(%" USIZE_FMT ")) = %.100f\n\n", i, det);
		unload_mat(hilb);
	}
}
