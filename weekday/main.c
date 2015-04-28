#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

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

char is_leap(int y){
	return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
}

char check_date(int d, int m, int y){
	int usual_year[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int leap_year[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int* year = is_leap(y) ? leap_year : usual_year;
	return !((y < 0) || (m < 1) || (m > 12) || (d<1) || (d>year[m - 1]));
}

int day_number(int d, int m, int y)
{
	int usual_year[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int leap_year[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int* year = is_leap(y) ? leap_year : usual_year;

	if ((y < 0) || (m < 1) || (m > 12) || (d<1) || (d>year[m - 1]))
		return -1;

	int result = 0;
	int cur_d = 0;
	int cur_m = 0;
	while ((d != (cur_d + 1)) || (m != (cur_m + 1))){
		result++;
		cur_d++;

		int x = year[cur_m];
		cur_m += cur_d / x;
		cur_d = cur_d % x;
	}
	return result;
}

int days_btw(int d1, int m1, int y1, int d2, int m2, int y2){
	int usual_year_size = 365;
	int leap_year_size = 366;
	
	//date1 should be less than date2
	if ((y1 > y2) || ((y1 == y2)&&(m1 > m2)) || ((y1 == y2)&&(m1 == m2)&&(d1 > d2))){
		//return -(d2, m2, y2, d1, m1, y1);
		return -days_btw(d2, m2, y2, d1, m1, y1);
	}

	int result = 0;
	for (int y = y1; y < y2; y++){
		int year_size = is_leap(y) ? leap_year_size : usual_year_size;
		result += year_size;
	}

	result += day_number(d2, m2, y2) - day_number(d1, m1, y1);
	
	return result;
}

char* week_day(int d, int m, int y){
	char* week[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	if (check_date(d, m, y))
		return week[((days_btw(16, 10, 1995, d, m, y) % 7) + 7) % 7];
	else
		return "!@#$%";
}

void test(){
	assert(days_btw(5, 3, 2015, 6, 3, 2015) == 1);
	assert(days_btw(14, 7, 2017, 25, 8, 2019) == 772);
	assert(days_btw(25, 8, 2019, 14, 7, 2017) == -772);
	assert(days_btw(16, 10, 1995, 05, 03, 2015) == 7080);
	assert(strcmp(week_day(01, 01, 1900) == "Monday") == 0);
	assert(strcmp(week_day(16, 10, 1995) == "Monday") == 0);
	assert(strcmp(week_day(06, 03, 2015), "Friday") == 0);
}

int main(){
	test();
	printf("Number of day\n");
	int day = read_int("Day is ");
	int month = read_int("Month is ");
	int year = read_int("Year is ");
	printf("%.2d.%.2d.%.4d is %s\n", day, month, year, week_day(day, month, year));
	return 0;
}