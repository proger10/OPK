#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#define MAX_STR 1024

int check_brackets(char *exp)
{
	char stack[MAX_STR] = { 0 };
	int head = 0;
	for (size_t i = 0; i < strlen(exp); i++){
		if ((exp[i] == '(') || (exp[i] == '[') || (exp[i] == '<') || (exp[i] == '{')){
			stack[head] = exp[i];
			head++;
		}
		if ((exp[i] == ')') || (exp[i] == ']') || (exp[i] == '>') || (exp[i] == '}')){
			if (head < 1)
				return 0;
			if ((exp[i] == ')') && ('(' != stack[head - 1]))
				return 0;
			if ((exp[i] == ']') && ('[' != stack[head - 1]))
				return 0;
			if ((exp[i] == '>') && ('<' != stack[head - 1]))
				return 0;
			if ((exp[i] == '}') && ('{' != stack[head - 1]))
				return 0;
			head--;
		}
	}
	return (head == 0);
}

void test(){
	assert(check_brackets("{()[()]}<<<>>>") == 1);
	assert(check_brackets("{}()<({}<>)>") == 1);
	assert(check_brackets("") == 1);
	assert(check_brackets("<>") == 1);
	assert(check_brackets("({<[]>})") == 1);

	assert(check_brackets("[(])") == 0);
	assert(check_brackets("{{[]]}}") == 0);
	assert(check_brackets("[") == 0);
	assert(check_brackets("(") == 0);
	assert(check_brackets("<") == 0);
	assert(check_brackets(">") == 0);
	assert(check_brackets(")))") == 0);
	assert(check_brackets("(((") == 0);
	assert(check_brackets("><") == 0);
	assert(check_brackets(")|(") == 0);
}

int main(){
	test();
	return 0;
}