#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#define MAX_STR 128

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

void getstr(int n, char* result){
	for (int i = 0; i < n; i++){
		result[i] = (char)('a' + 1.0*rand() / (RAND_MAX + 1) * ('z' - 'a' + 1));
	}
	result[n] = '\0';
}

int main(){
	srand((unsigned)time(NULL));
	printf("Keyboard trainer.\n");
	int n;
	do {
		n = read_int("Difficulty (1-79): ");
	} while ((n < 1) || (n>79));
	char str[MAX_STR];
	getstr(n, str);
	printf("%s\n", str);

	int errors = 0;
	time_t t = 0;
	for (int i = 0; i < n; i++){
		while (_getch() != str[i]){
			errors++;
			if (t == 0)
				t = time(NULL);
		}
		if (t == 0)
			t = time(NULL);
		printf("%c", str[i]);
	}

	t = time(NULL) - t;
	printf("\nDone! Errors: %d. Time: %llu. Speed: %.2lf chars/s.\n", errors, t, 1.0*n/t);
	return 0;
}