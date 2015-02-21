#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

void randomize_array(int arr[], int n){
	for (int i = 0; i < n; i++){
		for (int j = i + 1; j < n; j++){
//			if (arr[i] < arr[j]){
			if (rand() % 2 == 0){
				int t = arr[j];
				arr[j] = arr[i];
				arr[i] = t;
			}
		}
	}
}

int main(){
	srand((time(NULL)) % INT_MAX);
	printf("Randomize array of integers\n");
	printf("Array length: ");
	int n;
	scanf("%d", &n);
	if (n < 1){
		printf("WAT???\n");
		return 0;
	}
	int* arr = (int*)calloc(n, sizeof(int));
	printf("Scanning for array: ");
	for (int i = 0; i < n; i++){
		scanf("%d", &arr[i]);
	}

	randomize_array(arr, n);

	printf("Mixed array: ");
	for (int i = 0; i < n; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}