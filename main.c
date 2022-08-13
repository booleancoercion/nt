#include "nt.h"
#include "bigint.h"
#include <stdio.h>

#define assert(cond) if(!(cond)){ \
		printf("assertion failed: %s on line %d\n", \
			   #cond, \
			   __LINE__); \
		return 1; }

#define assert_eq(a,b) if((a) != (b)){ \
		printf("assertion failed: %s == %s on line %d\n", \
			   #a, #b, \
			   __LINE__); \
		return 1; }


static int egcd_test(void) {
	long a = 25421;
	long b = 8492;
	egcd_t res = extended_gcd(a, b);
	assert_eq(res.gcd, 11);
	assert_eq(res.x * a + res.y * b, res.gcd);

	return 0;
}

static int rem_euclid_test(void) {
	assert_eq(rem_euclid(264, 98), 68);
	assert_eq(rem_euclid(-264, 98), 30);
	
	return 0;
}

static int mod_inv_test(void) {
	long a = 25421 / 11;
	long b = 8492 / 11;

	long ainv = mod_inverse(a, b);
	assert_eq(rem_euclid(a * ainv, b), 1);

	return 0;
}

static int bigint_test_neg_idempotent(void) {
	bigint *a = bi_init();

	a->chunks[0] = -1;
	a->len = 1;

	bi_negate(a);
	bi_negate(a);

	assert_eq(a->chunks[0], -1);
	assert_eq(a->len, 1);
	assert_eq(bi_sign(a), 0);

	return 0;
}

static int bigint_test_add_simple(void) {
	bigint *a = bi_init();
	bigint *b = bi_init();

	a->chunks[3] = 50;
	a->len = 4;
	bi_negate(a);

	b->chunks[3] = 10;
	b->len = 4;

	bi_add_assign(a, b);

	bi_negate(a);
	assert_eq(a->chunks[3], 40);
	assert_eq(a->len, 4);
	assert_eq(bi_sign(a), 0);

	bi_free(b);
	bi_free(a);

	return 0;
}

static int bigint_test_add_carry(void) {
	bigint *a = bi_init();

	a->chunks[0] = -1;
	a->len = 1;

	bi_add_assign(a, a);

	assert_eq(a->chunks[0], (bic)-2);
	assert_eq(a->chunks[1], 1);
	assert_eq(a->len, 2);
	assert_eq(bi_sign(a), 0);

	bi_free(a);

	return 0;
}

static int bigint_test_add_cap(void) {
	bigint *a = bi_init();
	bigint *b = bi_init();
	
	a->chunks[3] = ((bic) -1) >> 1;
	a->len = 4;

	b->chunks[3] = 1;
	b->len = 4;

	bi_add_assign(a, b);

	assert(a->cap > 4);
	assert_eq(a->len, 4);

	bi_free(b);
	bi_free(a);

	return 0;
}

#define test(f) \
		printf("Running test %s:\n", #f); \
		acc += 1 - (f)(); \
		num += 1; \
		puts("");

int main(void) {
	int acc = 0;
	int num = 0;

	test(egcd_test);
	test(rem_euclid_test);
	test(mod_inv_test);
	test(bigint_test_neg_idempotent);
	test(bigint_test_add_simple);
	test(bigint_test_add_carry);
	test(bigint_test_add_cap);

	puts("-------------");
	printf("Successful tests: %d/%d.\n", acc, num);

	return 0;
}

