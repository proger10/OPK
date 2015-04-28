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

int day_number(int d, int m, int y)
{
	int usual_year[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int leap_year[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	unsigned char is_leap = ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
	int* year = is_leap ? leap_year : usual_year;

	if ((y < 0) || (m < 1) || (m > 12) || (d<1) || (d>year[m-1]))
		return -1;

	int result = 0;
	int cur_d = 0;
	int cur_m = 0;
	while ((d != (cur_d+1)) || (m != (cur_m+1))){
		result++;
		cur_d++;

		int x = year[cur_m];
		cur_m += cur_d / x;
		cur_d = cur_d % x;
	}
	return result;
}

int main(){
	printf("Number of day\n");
	int day = read_int("Day is ");
	int month = read_int("Month is ");
	int year = read_int("Year is ");
	int result = day_number(day, month, year);
	if (result < 0){
		printf("WAT??\n");
		return 0;
	}
	printf("%.2d.%.2d.%.4d is %dth day of %.4d\n", day, month, year, result+1, year);
	
	return 0;
}