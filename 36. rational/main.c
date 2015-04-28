#include "rational.h"
#include <assert.h>

void test(){
	Rational a, b, c, d;
	
	rat_create(&a, 1, 1);
	rat_print(&a, stdout);
	printf("\n");
	rat_create(&b, 1, 1);
	rat_mul(&c, &a, &b);
	assert(rat_compare(&a, &b) == 0);
	assert(rat_compare(&a, &c) == 0);
	assert(rat_compare(&c, &b) == 0);

	rat_create(&b, 10, 10);
	assert(rat_compare(&a, &b) == 0);

	rat_create(&b, 10, 3);
	rat_add(&c, &a, &b);
	rat_create(&d, 13, 3);
	assert(rat_compare(&c, &d) == 0);

	rat_sub(&c, &b, &a);
	rat_create(&d, 7, 3);
	assert(rat_compare(&c, &d) == 0);

	rat_create(&a, 30, 13);
	rat_mul(&c, &a, &b);
	rat_create(&d, 30 * 10, 13 * 3);
	assert(rat_compare(&c, &d) == 0);


	rat_div(&c, &a, &b);
	rat_create(&d, 30*3, 10*13);
	assert(rat_compare(&c, &d) == 0);

	rat_create(&a, 13, 7);
	rat_power(&b, &a, 3);
	rat_create(&c, 13 * 13 * 13, 7 * 7 * 7);
	assert(rat_compare(&b, &c) == 0);


	rat_create(&a, 13, 7);
	rat_power(&b, &a, -3);
	rat_create(&c, 7 * 7 * 7, 13 * 13 * 13);
	assert(rat_compare(&b, &c) == 0);


	rat_create(&a, 13, 7);
	rat_power(&b, &a, 0);
	rat_create(&c, 1, 1);
	assert(rat_compare(&b, &c) == 0);


	rat_create(&a, -1, 3);
	rat_create(&b, 1, 2);
	assert(rat_compare(&a, &b) == -1);

	rat_create(&a, 1, 3);
	rat_create(&b, 1, 2);
	assert(rat_compare(&a, &b) == -1);
}

int main(){
	test();
	return 0;
}