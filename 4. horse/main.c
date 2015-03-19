#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include "console.h"

#define COLOR_BORDER 1
#define COLOR_FIELD  2
#define COLOR_POINT  3

static void init_colors() {
/*	con_initPair(COLOR_BORDER, CON_COLOR_BLACK, CON_COLOR_BLUE);
	con_initPair(COLOR_FIELD, CON_COLOR_GREEN, CON_COLOR_GREEN);
	con_initPair(COLOR_POINT, CON_COLOR_RED, CON_COLOR_GREEN);*/
}

typedef struct _point{
	int x;
	int y;
} point;

char check_point(int x, int y){
	if ((x > 7) || (y > 7) || (x < 0) || (y < 0))
		return 0;
	return 1;
}

int get_empty(int x, int y, int *map){
	int result = 0;
	if (!check_point(x + 2, y + 1))
		if (!check_point(x + 1, y + 2, map, empty, result))
			if (!check_point(x + 2, y - 1, map, empty, result))
				if (!check_point(x + 1, y - 2, map, empty, result))
					if (!check_point(x - 2, y + 1, map, empty, result))
						if (!check_point(x - 1, y + 2, map, empty, result))
							if (!check_point(x - 2, y - 1, map, empty, result))
								if (!check_point(x - 1, y - 2, map, empty, result))
}

char solve(int x, int y, char *map, int *empty, point **result){
	if (!check_point(x, y))
		return 0;
	if (map[8 * x + y])
		return 0;
	map[8 * x + y] = 1;
	(*empty)--;
	if (*empty != 0){
		if (!solve(x + 2, y + 1, map, empty, result))
			if (!solve(x + 1, y + 2, map, empty, result))
				if (!solve(x + 2, y - 1, map, empty, result))
					if (!solve(x + 1, y - 2, map, empty, result))
						if (!solve(x - 2, y + 1, map, empty, result))
							if (!solve(x - 1, y + 2, map, empty, result))
								if (!solve(x - 2, y - 1, map, empty, result))
									if (!solve(x - 1, y - 2, map, empty, result))
									{
										(*empty)++;
										map[8 * x + y] = 0;
										return 0;
									}
	}
	else{
		*result = (point *)calloc(8 * 8, sizeof(point));
		if (*result == NULL)
			return 0;
	}

	if (*result != NULL){
		point p = { x, y };
		(*result)[(*empty)] = p;
	}
	(*empty)++;
	map[8 * x + y] = 0;
	return 1;
}

void horse(int x, int y){
	if (!check_point(x - 1, y - 1))
		return;
	char *map = (char *)calloc(8 * 8, sizeof(char));
	if (map == NULL)
		return;
	point *result = NULL;
	int empty = 8 * 8;
	solve(x - 1, y - 1, map, &empty, &result);
	free(map);
	if (result == NULL){
		con_gotoXY(0, 0);
		con_outTxt("Cant solve =(\n");
		return;
	}
	for (int i = 8 * 8-1; i >= 0; i++){
		con_gotoXY(result[i].x, result[i].y);
		con_outTxt("@");
		Sleep(500); 
		con_gotoXY(result[i].x, result[i].y);
		con_outTxt(" ");
	}
	free(result);
}

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

int main() {
	int x = 0; 
	int y = 0; 

	while (!check_point(x - 1, 1)){
		x = read_int("Horse x [1-8]: ");
	}
	while (!check_point(1, y - 1)){
		y = read_int("Horse y [1-8]: ");
	}

	
	con_init();
	con_hideCursor();
	con_clearScr();
	init_colors();

	horse(x, y);

	con_deinit();
	return 0;
}
