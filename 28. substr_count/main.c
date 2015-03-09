#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int substring_count(char *str, char *substr)
{
	int result = 0;
	for (size_t i = 0; i + strlen(substr) <= strlen(str); i++){
		if (memcmp(str + i, substr, strlen(substr)) == 0){
			result++;
		}
	}
	return result;
}

void test(){
	assert(substring_count("abcabc", "ab") == 2);
	assert(substring_count("abcabcd", "d") == 1);
	assert(substring_count("abcabcd", "q") == 0);
	assert(substring_count("sss", "s") == 3);
	assert(substring_count("sss", "ss") == 2);
	assert(substring_count("sss", "sss") == 1);
}

int main(){
	test();
	return 0;
};