#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

	if ((y1 > y2) || ((y1 == y2) && (m1 > m2)) || ((y1 == y2) && (m1 == m2) && (d1 > d2))){
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

int week_day(int d, int m, int y){
	if (check_date(d, m, y))
		return ((days_btw(16, 10, 1995, d, m, y) % 7) + 7) % 7;
	else
		return -1;
}

char **cal_month(unsigned year, unsigned month){
	if ((year < 0) || (year>9999) || (month < 1) || (month>12)){
		char **nope = (char **)calloc(1, sizeof(char**));
		nope[0] = NULL;
		return nope;
	}
	month--;
	char *month_names[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	char *header = "Mo Tu We Th Fr Sa Su";
	int usual_year[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int leap_year[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int* year_days = is_leap(year) ? leap_year : usual_year;

	int firstday = week_day(1, month + 1, year);
	int res_size = ((year_days[month] + firstday) / 7) + (((year_days[month] + firstday) % 7 == 0 )?0:1) + 3;

	char **result = (char **)calloc(res_size, sizeof(char *));
	result[res_size - 1] = NULL;

	result[0] = (char *)calloc(21, sizeof(char));
	memset(result[0], ' ', 20);
	result[0][10 - (strlen(month_names[month]) + 5) / 2] = '\0';
	sprintf(result[0], "%s%s %4d", result[0], month_names[month], year);
	memset(result[0] + strlen(result[0]) , ' ', 20 - strlen(result[0]));

	result[1] = (char *)calloc(21, sizeof(char));
	sprintf(result[1], "%s", header);

	int p_day = 1 - firstday;
	for (int i = 0; i < res_size - 3; i++){
		result[2+i] = (char *)calloc(21, sizeof(char));
		sprintf(result[2 + i], "%2d %2d %2d %2d %2d %2d %2d",
			(p_day - 1) % year_days[month] + 1,
			(p_day + 0) % year_days[month] + 1,
			(p_day + 1) % year_days[month] + 1,
			(p_day + 2) % year_days[month] + 1,
			(p_day + 3) % year_days[month] + 1,
			(p_day + 4) % year_days[month] + 1,
			(p_day + 5) % year_days[month] + 1);
		if (p_day < 1)
			memset(result[2 + i], ' ', (1 - p_day) * 3);
		if (p_day + 7 > year_days[month])
			memset(result[2 + i] + (1 - p_day + year_days[month]) * 3 -1 , ' ', (p_day +6 - year_days[month])*3);
		p_day += 7;
	}
	return result;
}

int cal_lines(char **calendar){
	int result = 0;
	for (; *calendar; calendar++){
		result++;
	}
	return result;
}

void cal_free_result(char **calendar){
	for (char **i = calendar; *i; i++){
		free(*i);
	}
	free(calendar);

}

char **cal_year(unsigned year){
	if ((year < 0) || (year>9999)){
		char **nope = (char **)calloc(1, sizeof(char**));
		nope[0] = NULL;
		return nope;
	}
	char **months[12];
	int res_size = 6;
	int max_lines[4] = {0};
	for (int i = 0; i < 12; i++){
		months[i] = cal_month(year, i+1);
		int t_size = cal_lines(months[i]);
		if (max_lines[i / 3] < t_size){
			max_lines[i / 3] = t_size;
		}
		if ((i % 3 == 0) && (i / 3 - 1 >= 0)){
			res_size += max_lines[i / 3 - 1];
		}
	}
	res_size += max_lines[3];
	char **result = (char **)calloc(res_size, sizeof(char *));
	result[res_size - 1] = NULL;

	result[0] = (char *)calloc(65, sizeof(char));
	memset(result[0], ' ', 30);
	sprintf(result[0], "%s%4d", result[0], year);

	result[1] = (char *)calloc(2, sizeof(char));
	result[1][0] = ' ';
	char *magicstr = "                    ";

	int res_shift = 2;
	for (int i = 0; i < 4; i++){
		char **a = cal_month(year, i * 3 + 1);
		char **b = cal_month(year, i * 3 + 2);
		char **c = cal_month(year, i * 3 + 3);
		char **t_a = a;
		char **t_b = b;
		char **t_c = c;
		for (int j = 0; j < max_lines[i]; j++){
			result[res_shift + j] = (char *)calloc(65, sizeof(char));
			sprintf(result[res_shift + j], "%s  %s  %s", (*a) ? (*a) : magicstr, (*b) ? (*b) : magicstr, (*c) ? (*c) : magicstr);
			if (*a) 
				a++;
			if (*b)
				b++;
			if (*c)
				c++;
		}
		cal_free_result(t_a);
		cal_free_result(t_b);
		cal_free_result(t_c);
		res_shift += max_lines[i];
		if (i != 3){
			result[res_shift] = (char *)calloc(2, sizeof(char));
			result[res_shift][0] = ' ';
			res_shift++;
		}
	}

	return result;
}

void cal_print(char **calendar){
	for (; *calendar; calendar++){
		printf("%s\n", *calendar);
	}
}


int main(){
	char **t = cal_year(1971);
	//char **t = cal_month(2011, 1);
	cal_print(t);
	return 0;
}