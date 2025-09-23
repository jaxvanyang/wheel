// max-det - find the max determinant of n x n matrix of 1 and -1s

#include <math.h>
#include <wheel.h>

void print_progress(usize i, usize n) {
	f32 progress = (f32)i * 100.0 / (f32)n;
	printf("[%" USIZE_FMT "/%" USIZE_FMT "] %.3f%%\n", i, n, progress);
}

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		error("expected 1 argument");
	}

	u64 n;
	sscanf(argv[1], "%lld", &n);

	u64 limit = 1ull << (n * n);
	f32 max_det = -INFINITY;
	Mat m = mat_zero(n, n);

	print_progress(0, limit);

	for (u64 flags = 0; flags < limit; ++flags) {
		for (usize i = 0; i < n; ++i) {
			for (usize j = 0; j < n; ++j) {
				bool is_one = (flags >> (i * n + j)) & 1;
				mat_set(m, i, j, is_one ? 1.0 : -1.0);
			}
		}

		term_clear_line();

		f32 det = get_det(m);
		if (det > max_det) {
			max_det = det;

			putchar('\n');
			print_mat(m);
			printf("\nmax det: %.0f\n\n", max_det);
		}

		print_progress(flags + 1, limit);
	}

	unload_mat(m);
}
