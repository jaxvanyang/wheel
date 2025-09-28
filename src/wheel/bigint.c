#include "bigint.h"
#include <assert.h>

BigInt new_bigint(isize num) {
	BigInt ret = {
		.data = ilist_new(),
		.sign = false,
	};

	if (num == 0) {
		ilist_push(ret.data, 0);
		return ret;
	}

	// absolute value
	usize val = num;

	if (num < 0) {
		ret.sign = true;
		val = -num;
	}

	while (val > 0) {
		ilist_push(ret.data, val % 10);
		val /= 10;
	}

	return ret;
}

BigInt bigint_clone(const BigInt num) {
	BigInt ret = {
		.data = ilist_clone(num.data),
		.sign = num.sign,
	};

	return ret;
}

void unload_bigint(BigInt num) { ilist_free(num.data); }

char *bigint_display(const BigInt num) {
	if (bigint_is_zero(num)) {
		char *ret = malloc(sizeof(char) * 2);
		ret[0] = '0';
		ret[1] = '\0';
		return ret;
	}

	Str *buffer = str_new();

	for (usize i = 0; i < num.data->length; ++i) {
		str_push(buffer, '0' + ilist_get(num.data, i));
	}
	if (num.sign) {
		str_push(buffer, '-');
	}

	str_reverse(buffer);

	char *ret = buffer->data;
	FREE(buffer);

	return ret;
}

bool bigint_is_zero(const BigInt num) {
	return num.data->length == 1 && ilist_get(num.data, 0) == 0;
}

bool bigint_equal(const BigInt a, const BigInt b) {
	if (bigint_is_zero(a) && bigint_is_zero(b)) {
		return true;
	}

	if (a.sign != b.sign || a.data->length != b.data->length) {
		return false;
	}

	for (usize i = 0; i < a.data->length; ++i) {
		if (ilist_get(a.data, i) != ilist_get(b.data, i)) {
			return false;
		}
	}

	return true;
}

bool bigint_lt(const BigInt a, const BigInt b) {
	if (a.sign && !b.sign) {
		return true;
	} else if (!a.sign && b.sign) {
		return false;
	}

	bool ret = false;

	// compare absolute value first
	if (a.data->length < b.data->length) {
		ret = true;
	} else if (a.data->length == b.data->length) {
		for (isize i = a.data->length - 1; i >= 0; --i) {
			if (ilist_get(a.data, i) < ilist_get(b.data, i)) {
				ret = true;
				break;
			} else if (ilist_get(a.data, i) < ilist_get(b.data, i)) {
				ret = false;
				break;
			}
		}
	}

	return ret ^ a.sign;
}

bool bigint_gt(const BigInt a, const BigInt b) {
	return !(bigint_equal(a, b) || bigint_lt(a, b));
}

BigInt bigint_add(const BigInt a, const BigInt b) {
	BigInt x, y;

	// make len(x) >= y
	if (a.data->length >= b.data->length) {
		x = a;
		y = b;
	} else {
		x = b;
		y = a;
	}

	if (x.sign != y.sign) {
		y.sign = !y.sign;
		return bigint_sub(x, y);
	}

	BigInt ret = bigint_clone(x);
	isize carry = 0;

	for (usize i = 0; i < y.data->length; ++i) {
		ret.data->data[i] += carry + ilist_get(y.data, i);
		carry = ilist_get(ret.data, i) / 10;
		ret.data->data[i] %= 10;
	}

	for (usize i = y.data->length; i < x.data->length; ++i) {
		ret.data->data[i] += carry;
		carry = ilist_get(ret.data, i) / 10;
		if (carry == 0) {
			break;
		}
		ret.data->data[i] %= 10;
	}

	if (carry != 0) {
		ilist_push(ret.data, carry);
	}

	return ret;
}

BigInt bigint_sub(const BigInt a, const BigInt b) {
	assert(a.data->length > 0);
	assert(b.data->length > 0);

	BigInt x, y;

	// make |x| >= |y|, if not the same sign, use the original order
	if (a.sign != b.sign || a.data->length > b.data->length) {
		x = a;
		y = b;
	} else if (a.data->length < b.data->length) {
		x = b;
		x.sign = !x.sign;
		y = a;
		y.sign = !y.sign;
	} else {
		bool is_equal = true;

		for (isize i = a.data->length - 1; i >= 0; --i) {
			if (ilist_get(a.data, i) > ilist_get(b.data, i)) {
				x = a;
				y = b;
				is_equal = false;
				break;
			} else if (ilist_get(a.data, i) < ilist_get(b.data, i)) {
				x = b;
				x.sign = !x.sign;
				y = a;
				y.sign = !y.sign;
				is_equal = false;
				break;
			}
		}

		if (is_equal) {
			x = a;
			y = b;
		}
	}

	if (a.sign != b.sign) {
		y.sign = !y.sign;
		return bigint_add(x, y);
	}

	BigInt ret = bigint_clone(x);
	isize borrow = 0;

	for (usize i = 0; i < y.data->length; ++i) {
		ret.data->data[i] -= borrow + ilist_get(y.data, i);
		if (ilist_get(ret.data, i) >= 0) {
			borrow = 0;
		} else {
			borrow = 1;
			ret.data->data[i] += 10;
		}
	}

	for (usize i = y.data->length; i < x.data->length; ++i) {
		ret.data->data[i] -= borrow;
		if (ilist_get(ret.data, i) >= 0) {
			borrow = 0;
		} else {
			borrow = 1;
			ret.data->data[i] += 10;
		}
	}

	assert(borrow == 0);

	for (usize i = ret.data->length - 1; i > 0; --i) {
		if (ilist_get(ret.data, i) == 0) {
			ilist_pop(ret.data);
		} else {
			break;
		}
	}

	return ret;
}

BigInt bigint_mul(const BigInt a, const BigInt b);
BigInt bigint_div(const BigInt a, const BigInt b);

BigInt bigint_add_i(const BigInt a, isize b);
BigInt bigint_sub_i(const BigInt a, isize b);
BigInt bigint_mul_i(const BigInt a, isize b);
BigInt bigint_div_i(const BigInt a, isize b);
