#include <assert.h>
#include <stdio.h>
#include <wheel/core.h>
#include <wheel/list.h>

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
	usize n = 20;

	for (usize i = 1; i <= n; ++i) {
		TimeVal t0 = time_now();
		usize fib_result = fib(i);
		f64 dt1 = elapsed(t0);

		t0 = time_now();
		usize fib_recur_result = fib_recur(i);
		f64 dt2 = elapsed(t0);

		assert(fib_result == fib_recur_result);

		printf("fib(%zu) = %zu, dt1 = %.3lf ms, dt2 = %.3lf ms\n", i, fib_result, dt1, dt2);
	}
}
