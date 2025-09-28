#pragma once

#include "core.h"
#include "list.h"

// NOTE: remember to unload created instance.
typedef struct {
	Ilist *data;
	bool sign; // whether it's negative
} BigInt;

// NOTE: you need to unload the created BigInt.
BigInt new_bigint(isize num);
BigInt bigint_clone(const BigInt num);
void unload_bigint(BigInt num);

// NOTE: you have to free the returned string.
char *bigint_display(const BigInt num);

bool bigint_is_zero(const BigInt num);
bool bigint_equal(const BigInt a, const BigInt b);
// Return if a is less than b.
bool bigint_lt(const BigInt a, const BigInt b);
// Return if a is greater than b.
bool bigint_gt(const BigInt a, const BigInt b);

BigInt bigint_add(const BigInt a, const BigInt b);
BigInt bigint_sub(const BigInt a, const BigInt b);
BigInt bigint_mul(const BigInt a, const BigInt b);
BigInt bigint_div(const BigInt a, const BigInt b);

// Add BigInt and normal integer (isize).
BigInt bigint_add_i(const BigInt a, isize b);
// Subtract BigInt and normal integer (isize).
BigInt bigint_sub_i(const BigInt a, isize b);
// Multiply BigInt and normal integer (isize).
BigInt bigint_mul_i(const BigInt a, isize b);
// Divide BigInt by normal integer (isize).
BigInt bigint_div_i(const BigInt a, isize b);
