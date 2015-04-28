#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 


void skip_garbage() {
	int ch;
	do {
		ch = getchar();
	} while (ch != '\n');
}

int read_int(const char * request) {
	for (;;) {
		printf("%s", request);
		int num;
		int res = scanf("%d", &num);
		skip_garbage();
		if (res == 1) {
			return num;
		}
	}
}

int main(){
	printf("Petya and The Ring\n");
	int n = read_int("N = ");
	int k = read_int("K = ");
	if ((n < 1) || (k < 1) || (n>100000)){
		printf("WAT??\n");
		return 0;
	}

	char arr[100000] = { 0 };
	int pointer = 0;
	int counter = 0;
	int rem = 0;

	while (rem != n){
		if (!arr[pointer]){
			counter++;
		}
		if (counter == k){
			printf("%d ", pointer+1);
			arr[pointer] = 1;
			counter = 0;
			rem++;
		}
		pointer = (pointer+1) % n;
	}

	//printf("%d ", pointer + 1);
	return 0;
}