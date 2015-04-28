#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

void strclear(char *str){
	char have_space = 0;
	int rem = 0;
	for (size_t i = 0; i <= strlen(str); i++){
		if (isspace(str[i])){
			if (have_space){
				rem++;
			}
			else{
				have_space = 1;
				str[i - rem] = str[i];
			}
		}
		else{
			str[i - rem] = str[i];
			have_space = 0;
		}
	}
}

bool check_one(char *original, char *expected) {
	char *str = _strdup(original);
	strclear(str);
	return (strcmp(expected, str) == 0);
}

void test(){
	assert(check_one(" a b c ", " a b c ") == 1);
	assert(check_one("      abc ", " abc ") == 1);
	assert(check_one("\t\na b c\t\n", "\ta b c\t") == 1);
	assert(check_one("  \t  gh   \t\n ", " gh ") == 1);
	assert(check_one("  \t  gh   \t\n ", "AXAZAz") == 0);
	assert(check_one("", "") == 1);
	assert(check_one("     ", " ") == 1);
}

int main(){
	test();
	char str[] = "\t\na b c\t\n";
	printf("%s", str);
	strclear(str);
	printf("\n---\n");
	printf("%s", str);

}