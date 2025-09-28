#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <wheel/bigint.h>

int main() {
	{
		BigInt num = new_bigint(-1234567890);
		char *s = bigint_display(num);

		assert(strcmp(s, "-1234567890") == 0);

		FREE(s);
		unload_bigint(num);
	}

	{
		// -2^63
		BigInt num = new_bigint(-9223372036854775808ull);
		char *s = bigint_display(num);

		assert(strcmp(s, "-9223372036854775808") == 0);

		FREE(s);
		unload_bigint(num);
	}

	{
		BigInt fib[2] = {
			new_bigint(0),
			new_bigint(1),
		};

		for (usize i = 2; i <= 100; ++i) {
			usize prev2 = i % 2;
			usize prev = prev2 ^ 1;

			BigInt num = bigint_add(fib[prev], fib[prev2]);
			char *s = bigint_display(num);
			printf("fib(%" USIZE_FMT ") = %s\n", i, s);
			FREE(s);

			BigInt diff = bigint_sub(num, fib[prev]);
			assert(bigint_equal(diff, fib[prev2]));

			unload_bigint(fib[prev2]);
			fib[prev2] = num;
		}

		unload_bigint(fib[0]);
		unload_bigint(fib[1]);
	}

	{
		BigInt a = new_bigint(-5);
		BigInt b = new_bigint(4);
		BigInt c = new_bigint(-1);

		BigInt sum = bigint_add(a, b);
		assert(bigint_equal(sum, c));
		unload_bigint(sum);

		sum = bigint_add(b, a);
		assert(bigint_equal(sum, c));
		unload_bigint(sum);

		assert(bigint_lt(a, b));
		assert(bigint_lt(a, c));
		assert(bigint_lt(c, b));

		assert(bigint_gt(b, a));
		assert(bigint_gt(c, a));
		assert(bigint_gt(b, c));
	}
}
