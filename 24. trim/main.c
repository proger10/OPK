#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

int index(int x){
	if (x > 0)
		return x;
	else
		return 0;
}

void strtrim(char *str){
	size_t lrem = 0;
	size_t rrem = strlen(str);
	if (rrem == 0)
		return;
	while (isspace(str[lrem])){
		if (lrem + 1 < rrem){
			lrem++;
		}
		else{
			break;
		}
	}

	while (isspace(str[rrem - 1])){
		if (rrem - 1 >= lrem){
			rrem--;
		}
		else{
			break;
		}
	}

	for (size_t i = lrem; i < rrem; i++){
		str[i - lrem] = str[i];
	}
	str[rrem - lrem] = '\0';

}

bool check_strtrim_one(char *original, char *expected) {
	char *str = _strdup(original);
	strtrim(str);
	return (strcmp(expected, str) == 0);
}

void test(){
	assert(check_strtrim_one(" a b c ", "a b c") == 1);
	assert(check_strtrim_one("      abc ", "abc") == 1);
	assert(check_strtrim_one("\t\na b c\t\n", "a b c") == 1);
	assert(check_strtrim_one("      ", "") == 1);
	assert(check_strtrim_one("", "") == 1);
}

int main(){
	printf("I do nothing =(\n");
	test();
	return 0;
}