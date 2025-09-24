// max-det - find the max determinant of n x n matrix of 1 and -1s

#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <wheel.h>

typedef struct {
	u64 n;
	u64 divider;
	u64 rem;
	u64 *cnt;
	f32 *max_det;
	pthread_mutex_t *lock;
} Arg;

void print_progress(usize cnt, usize max_cnt) {
	f32 progress = (f32)cnt * 100.0 / (f32)max_cnt;
	printf("[%" USIZE_FMT "/%" USIZE_FMT "] %.3f%%\n", cnt, max_cnt, progress);
}

void *calc(void *argument) {
	Arg *arg = argument;
	u64 n = arg->n;
	u64 max_cnt = 1ull << (n * n);
	Mat m = mat_zero(n, n);
	u64 cnt = 0;
	// merely 1e6 computations per second
	u64 threshold = 1000000;

	pthread_mutex_lock(arg->lock);
	f32 max_det = *arg->max_det;
	pthread_mutex_unlock(arg->lock);

	for (u64 flags = arg->rem; flags < max_cnt; flags += arg->divider) {
		for (usize i = 0; i < n; ++i) {
			for (usize j = 0; j < n; ++j) {
				bool is_one = (flags >> (i * n + j)) & 1;
				mat_set(m, i, j, is_one ? 1.0 : -1.0);
			}
		}

		f32 det = get_det(m);

		++cnt;
		if (det > max_det) {
			max_det = det;
		}

		if (cnt >= threshold) {
			pthread_mutex_lock(arg->lock);
			*arg->cnt += cnt;
			cnt = 0;
			if (max_det > *arg->max_det) {
				*arg->max_det = max_det;

				term_clear_line();
				print_mat(m);
				printf("\nmax det: %.0f\n\n", *arg->max_det);
				print_progress(*arg->cnt, max_cnt);
			} else {
				max_det = *arg->max_det;
			}
			pthread_mutex_unlock(arg->lock);
		}
	}

	pthread_mutex_lock(arg->lock);
	*arg->cnt += cnt;
	cnt = 0;
	if (max_det > *arg->max_det) {
		*arg->max_det = max_det;

		term_clear_line();
		putchar('\n');
		print_mat(m);
		printf("\nmax det: %.0f\n\n", *arg->max_det);
		print_progress(*arg->cnt, max_cnt);
	} else {
		max_det = *arg->max_det;
	}
	pthread_mutex_unlock(arg->lock);

	unload_mat(m);

	return NULL;
}

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		error("expected 1 argument");
	}

	f32 max_det = -INFINITY;
	u64 cnt = 0;
	u64 n;
	sscanf(argv[1], "%lld", &n);

	u64 divider = 8;
	pthread_mutex_t lock;
	pthread_mutex_init(&lock, NULL);
	pthread_t *threads = malloc(sizeof(pthread_t) * divider);
	Arg *args = malloc(sizeof(Arg) * divider);

	u64 max_cnt = 1ull << (n * n);
	print_progress(cnt, max_cnt);

	for (u64 i = 0; i < divider; ++i) {
		args[i].n = n;
		args[i].divider = divider;
		args[i].rem = i;
		args[i].cnt = &cnt;
		args[i].max_det = &max_det;
		args[i].lock = &lock;

		pthread_create(threads + i, NULL, calc, args + i);
	}

	while (cnt < max_cnt) {
		pthread_mutex_lock(&lock);
		term_clear_line();
		print_progress(cnt, max_cnt);
		pthread_mutex_unlock(&lock);

		sleep(1);
	}

	for (u64 i = 0; i < divider; ++i) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&lock);

	term_clear_line();
	print_progress(cnt, max_cnt);

	FREE(args);
	FREE(threads);
}
