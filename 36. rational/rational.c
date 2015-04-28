#include "rational.h"
#include <math.h>

static int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a % b);
}

static int nanalize(Rational *res){
	if ((res->denom == 0)){
		res->denom = 0;
		res->numer = 0;
		return 1;
	}
	return 0;
}

static void normalize(Rational *res){
	// todo
	int g = gcd(abs(res->numer), abs(res->denom));
	if (res->denom < 0)
		g = -g;
	res->numer /= g;
	res->denom /= g;
}

static int fastpow(int x, int n){
	if (x == 0){
		return 0;
	}
	if (n < 0){
		return 0;
	}
	int result = 1;
	while (n != 0){
		if (n % 2 == 1){
			result *= x;
		}
		x *= x;
		n /= 2;
	}
	return result;
}

void rat_create(Rational *res, int a, int b){
	if (b == 0){
		a = 0;
	}
	res->numer = a;
	res->denom = b;
	normalize(res);
}

void rat_print(Rational *a, FILE *fp){
	fprintf(fp, "%d/%d", a->numer, a->denom);
}

int rat_compare(Rational *a, Rational *b) {
	int test = (a->numer)*(b->denom) - (a->denom)*(b->numer);
	if (test == 0)
		return 0;
	return test > 0 ? 1 : -1;
}

int rat_to_i(Rational *a){
	int i = a->numer / a->denom;
	double f = rat_to_d(a) - i;
	if (f < 0.5)
		return i;
	else
		return i + 1;
}

double rat_to_d(Rational *a){
	return 1.0*(a->numer) / (a->denom);
}

void rat_add(Rational *result, Rational *a, Rational *b){
	if (nanalize(a) || nanalize(b)){
		return;
	}
	result->numer = (a->numer)*(b->denom) + (a->denom)*(b->numer);
	result->denom = (b->denom)*(a->denom);
	normalize(result);
}

void rat_sub(Rational *result, Rational *a, Rational *b){
	if (nanalize(a) || nanalize(b)){
		return;
	}
	result->numer = (a->numer)*(b->denom) - (a->denom)*(b->numer);
	result->denom = (b->denom)*(a->denom);
	normalize(result);
}

void rat_mul(Rational *result, Rational *a, Rational *b){
	if (nanalize(a) || nanalize(b)){
		return;
	}

	result->numer = (a->numer)*(b->numer);
	result->denom = (b->denom)*(a->denom);
	normalize(result);
}

void rat_div(Rational *result, Rational *a, Rational *b){
	if (nanalize(a) || nanalize(b)){
		return;
	}

	result->numer = (a->numer)*(b->denom);
	result->denom = (a->denom)*(b->numer);
	normalize(result);
}

void rat_power(Rational *result, Rational *r, int power){
	if (power == 0){
		Rational zero = { 0, 1 };
		if (rat_compare(result, &zero) == 0){
			result->denom = 0;
			result->numer = 0;
			return;
		}
		else{
			result->denom = 1;
			result->numer = 1;
			return;
		}
	}
	if (power > 0){
		result->numer = fastpow(r->numer, power);
		result->denom = fastpow(r->denom, power);
	}
	if (power < 0){
		result->numer = fastpow(r->denom, -power);
		result->denom = fastpow(r->numer, -power);
	}
	normalize(result);
}