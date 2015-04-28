#ifndef RATIONAL_H
#define RATIONAL_H

#include <stdio.h>

typedef struct _Rational {
	int numer;  /* numerator, ��������� */
	int denom; /* denominator, ����������� */
} Rational;

void rat_add(Rational *result, Rational *a, Rational *b);
void rat_sub(Rational *result, Rational *a, Rational *b);
void rat_mul(Rational *result, Rational *a, Rational *b);
void rat_div(Rational *result, Rational *a, Rational *b);

/* ���������� � ������������� �������. power ����� ���� �������������! */
void rat_power(Rational *result, Rational *r, int power);
void rat_create(Rational *res, int a, int b);

/* ���������� �� ����. ������ */
int rat_to_i(Rational *a);
double rat_to_d(Rational *a); /* �������������� � ����� � ����. ������ */
void rat_print(Rational *a, FILE *fp);  /*������� � ������� p/q */
int rat_compare(Rational *a, Rational *b);

#endif