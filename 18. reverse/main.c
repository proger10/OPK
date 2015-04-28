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

void reverse_array(int arr[], int n){
	for (int i = 0; i < n/2; i++){
		int t = arr[i];
		arr[i] = arr[n-1-i];
		arr[n-1-i] = t;
	}
}

int main(){
	srand((time(NULL)) % INT_MAX);
	printf("Randomize array of integers\n");
	int n = read_int("Array length: ");
	if (n < 1){
		printf("WAT???\n");
		return 0;
	}

	//int* arr = (int*)calloc(n, sizeof(int));
	int arr[256] = { 0 };
	printf("Scanning for array\n");
	for (int i = 0; i < n; i++){
		arr[i] = read_int("Next element: ");
	}

	reverse_array(arr, n);

	printf("Reversed array: ");
	for (int i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}