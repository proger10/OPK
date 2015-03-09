#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

long long parse_number(char *str, int base, int *is_ok)
{
	long long result = 0;
	if (strlen(str) == 0){
		*is_ok = true;
		return 0;
	}
	int lshift = 0;
	while(isspace(str[lshift]))
		lshift++;
	int sign = 1;
	if ((str[lshift] == '+') || (str[lshift] == '-')){
		sign = (str[lshift] == '+') ? 1 : -1;
		lshift++;
	}
	for (size_t i = lshift; i < strlen(str); i++){
		char c = str[i];
		if ((str[i] >= 'A') && (str[i] <= 'Z'))
			c += -'A' + 'a';
		if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'a' + base - 11))){
			result = result*base + c - (((c >= '0') && (c <= '9'))?'0':('a'-10));
		}
		else
		{
			*is_ok = false;
			return sign*result;
		}
		
	}
	*is_ok = true;
	return sign*result;
}

void test(){
	int is_ok = 0;
	assert((parse_number(_strdup("123"), 10, &is_ok) == 123) && is_ok);
	assert((parse_number(_strdup("+123"), 10, &is_ok) == 123) && is_ok);
	assert((parse_number(_strdup("-123"), 10, &is_ok) == -123) && is_ok);
	assert((parse_number(_strdup("123t"), 10, &is_ok) == 123) && !is_ok);
	assert((parse_number(_strdup(""), 10, &is_ok) == 0) && is_ok);
	assert((parse_number(_strdup("az"), 36, &is_ok) == 395) && is_ok);
	assert((parse_number(_strdup("		 -AbAd1dEa"), 16, &is_ok) == -2880249322LL) && is_ok);
}

int main(){
	test();
	return 0;
};