#define _STR_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

typedef struct _roman {
	int value;
	char sym;
} roman;

int rome_char_to_dec(char rome){
	roman vals[] = { { 1, 'I' }, { 5, 'V' }, { 10, 'X' }, { 50, 'L' }, { 100, 'C' }, { 500, 'D' }, { 1000, 'M' } };
	for (int i = 0; i < sizeof(vals); i++){
		if (vals[i].sym == rome){
			return vals[i].value;
		}
	}
	return -1;
}

int roman_to_decimal(char *str_roman)
{
	int last = 0;
	int result = 0;
	int cur = 0;
	for (size_t i = 0; i < strlen(str_roman); i++){
		cur = rome_char_to_dec(str_roman[i]);
		if (cur == -1){
			return 0;
		}
		if ((last != 0) && (last < cur)){
			result -= last;
		}
		if ((last != 0) && (last >= cur)){
			result += last;
		}
		last = cur;
	}
	result += cur;
	return result;
}

char try_add(char ch, int count, char *buf, size_t maxlen){
	for (int i = 0; i < count; i++){
		size_t buf_size = strlen(buf);
		if (buf_size + 1 < maxlen){
			buf[buf_size] = ch;
			buf[buf_size + 1] = '\0';
		}
		else
			return 0;
	}
	return 1;
}


int decimal_to_roman(int number, char *string, int maxlen)
{
	memset(string, '\0', maxlen);
	if ((number<1) || (number>3999)){
		return 0;
	}
	roman vals[] = { { 1, 'I' }, { 5, 'V' }, { 10, 'X' }, { 50, 'L' }, { 100, 'C' }, { 500, 'D' }, { 1000, 'M' } };
	while (number != 0){
		int cur = number;
		int pw = 0;
		while (cur != (cur % 10)){
			cur /= 10;
			pw++;
		}
		number -= cur*((int)pow(10, pw));
		switch (cur){
		case 1:
		case 2:
		case 3:
			try_add(vals[pw * 2].sym, cur, string, maxlen);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			if (!try_add(vals[pw * 2].sym, 5 - cur, string, maxlen))
				return 1;
			try_add(vals[pw * 2 + 1].sym, 1, string, maxlen);
			if (!try_add(vals[pw * 2].sym, cur - 5, string, maxlen))
				return 1;
			break;
		case 9:
			if (!try_add(vals[pw * 2].sym, 1, string, maxlen))
				return 1;
			if (!try_add(vals[pw * 2 + 2].sym, 1, string, maxlen))
				return 1;
			break;
		default:
			break;
		}
	}
	return 0;
}

void test(){
	assert(roman_to_decimal("I") == 1);
	assert(roman_to_decimal("II") == 2);
	assert(roman_to_decimal("III") == 3);
	assert(roman_to_decimal("IV") == 4);
	assert(roman_to_decimal("V") == 5);
	assert(roman_to_decimal("VI") == 6);
	assert(roman_to_decimal("VII") == 7);
	assert(roman_to_decimal("VIII") == 8);
	assert(roman_to_decimal("IX") == 9);
	assert(roman_to_decimal("X") == 10);
	assert(roman_to_decimal("XI") == 11);
	assert(roman_to_decimal("XII") == 12);
	assert(roman_to_decimal("XIII") == 13);
	assert(roman_to_decimal("XIV") == 14);
	assert(roman_to_decimal("XV") == 15);
	assert(roman_to_decimal("CCVII") == 207);
	assert(roman_to_decimal("MLXVI") == 1066);
	assert(roman_to_decimal("MCMIV") == 1904);
	assert(roman_to_decimal("MCMLIV") == 1954);
	assert(roman_to_decimal("MCMXC") == 1990);
	assert(roman_to_decimal("MMXIV") == 2014);
	//assert(roman_to_decimal("MMMDDDCCCLLLXXXVVVIII") == 4998);
	assert(roman_to_decimal("MMMCMXCIX") == 3999);
	assert(roman_to_decimal("MMMCMXCIdX") == 0);
	assert(roman_to_decimal("") == 0);

	char *buf = (char *)calloc(100500, sizeof(char));
	if (buf == NULL)
		return;

	decimal_to_roman(1, buf, 100500);
	assert(strcmp(buf, "I") == 0);
	decimal_to_roman(2, buf, 100500);
	assert(strcmp(buf, "II") == 0);
	decimal_to_roman(3, buf, 100500);
	assert(strcmp(buf, "III") == 0);
	decimal_to_roman(4, buf, 100500);
	assert(strcmp(buf, "IV") == 0);
	decimal_to_roman(5, buf, 100500);
	assert(strcmp(buf, "V") == 0);
	decimal_to_roman(6, buf, 100500);
	assert(strcmp(buf, "VI") == 0);
	decimal_to_roman(7, buf, 100500);
	assert(strcmp(buf, "VII") == 0);
	decimal_to_roman(8, buf, 100500);
	assert(strcmp(buf, "VIII") == 0);
	decimal_to_roman(9, buf, 100500);
	assert(strcmp(buf, "IX") == 0);
	decimal_to_roman(10, buf, 100500);
	assert(strcmp(buf, "X") == 0);
	decimal_to_roman(11, buf, 100500);
	assert(strcmp(buf, "XI") == 0);
	decimal_to_roman(12, buf, 100500);
	assert(strcmp(buf, "XII") == 0);
	decimal_to_roman(13, buf, 100500);
	assert(strcmp(buf, "XIII") == 0);
	decimal_to_roman(14, buf, 100500);
	assert(strcmp(buf, "XIV") == 0);
	decimal_to_roman(15, buf, 100500);
	assert(strcmp(buf, "XV") == 0);

	decimal_to_roman(207, buf, 100500);
	assert(strcmp(buf, "CCVII") == 0);
	decimal_to_roman(1066, buf, 100500);
	assert(strcmp(buf, "MLXVI") == 0);
	decimal_to_roman(1904, buf, 100500);
	assert(strcmp(buf, "MCMIV") == 0);
	decimal_to_roman(1954, buf, 100500);
	assert(strcmp(buf, "MCMLIV") == 0);
	decimal_to_roman(1990, buf, 100500);
	assert(strcmp(buf, "MCMXC") == 0);
	decimal_to_roman(2014, buf, 100500);
	assert(strcmp(buf, "MMXIV") == 0);
	decimal_to_roman(3999, buf, 100500);
	assert(strcmp(buf, "MMMCMXCIX") == 0);


	free(buf);
}

int main(){
	test();
	return 0;
}