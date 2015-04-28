#ifndef RATIONAL_H
#define RATIONAL_H

#include <stdio.h>

typedef struct _Rational {
	int numer;  /* numerator, числитель */
	int denom; /* denominator, знаменатель */
} Rational;

void rat_add(Rational *result, Rational *a, Rational *b);
void rat_sub(Rational *result, Rational *a, Rational *b);
void rat_mul(Rational *result, Rational *a, Rational *b);
void rat_div(Rational *result, Rational *a, Rational *b);

/* возведение в целочисленную степень. power может быть отрицательным! */
void rat_power(Rational *result, Rational *r, int power);
void rat_create(Rational *res, int a, int b);

/* округление до ближ. целого */
int rat_to_i(Rational *a);
double rat_to_d(Rational *a); /* преобразование в число с плав. точкой */
void rat_print(Rational *a, FILE *fp);  /*выводит в формате p/q */
int rat_compare(Rational *a, Rational *b);

#endif