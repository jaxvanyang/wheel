#include <assert.h>
#include <basics/core.h>
#include <basics/list.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

usize fib(usize n) {
	if (n <= 1) {
		return n;
	}

	Ulist *list = ulist_new();
	ulist_insert(list, 0, 0);
	ulist_insert(list, 1, 1);

	for (usize i = 2; i <= n; ++i) {
		ulist_insert(list, i, ulist_get(list, i - 1) + ulist_get(list, i - 2));
	}

	usize ret = ulist_get(list, n);

	ulist_free(list);
	list = NULL;

	return ret;
}

usize fib_recur(usize n) {
	if (n < 2) {
		return n;
	}

	return fib_recur(n - 1) + fib_recur(n - 2);
}

int main() {
	usize n = 40;

	for (usize i = 1; i <= n; ++i) {
		struct timeval t0, t;

		gettimeofday(&t0, NULL);
		usize fib_result = fib(i);
		gettimeofday(&t, NULL);
		f64 dt1 = elapsed(t0, t);

		gettimeofday(&t0, NULL);
		usize fib_recur_result = fib_recur(i);
		gettimeofday(&t, NULL);
		f64 dt2 = elapsed(t0, t);

		assert(fib_result == fib_recur_result);

		printf("fib(%llu) = %llu, dt1 = %.3lf ms, dt2 = %.3lf ms\n", i, fib_result, dt1, dt2);
	}
}
