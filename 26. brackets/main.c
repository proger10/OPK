#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <assert.h>
#include <stdio.h>

int check_parentheses(char *exp)
{
	int stack = 0;
	for (size_t i = 0; i < strlen(exp); i++){
		if (exp[i] == '(')
			stack++;
		if (exp[i] == ')')
			stack--;
		if (stack < 0){
			return 0;
		}
	}
	return stack==0;
}

void test(){
	 assert(check_parentheses("") == 1);
	 assert(check_parentheses("()") == 1);
	 assert(check_parentheses("((()))()(())") == 1);
	 assert(check_parentheses(")(") == 0);
	 assert(check_parentheses("((())))") == 0);
	 assert(check_parentheses("(()") == 0);
	 assert(check_parentheses(")") == 0);
	 assert(check_parentheses("(") == 0);
}


int main(){
	test();


	printf("Bracket balance\n");
	printf("I do nothing =(\n");

	return 0;
}