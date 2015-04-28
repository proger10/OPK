#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include "console.h"

#define MAP_SIZE 8
#define COLOR_HORSE 1
#define COLOR_CHECKED 2
#define COLOR_BORDER 3
#define COLOR_NORMAL 4

static void init_colors() {
	//con_initPair(COLOR_HORSE, CON_COLOR_BLUE, CON_COLOR_BLACK);
	con_initPair(COLOR_HORSE, CON_COLOR_WHITE, CON_COLOR_BLUE);
	con_initPair(COLOR_CHECKED, CON_COLOR_BLUE, CON_COLOR_BLUE);
	con_initPair(COLOR_BORDER, CON_COLOR_WHITE, CON_COLOR_BLACK);
	con_initPair(COLOR_NORMAL, CON_COLOR_WHITE, CON_COLOR_BLACK);
}

typedef struct _point{
	int x;
	int y;
} point;

char check_point(int x, int y){
	if ((x > MAP_SIZE - 1) || (y > MAP_SIZE - 1) || (x < 0) || (y < 0))
		return 0;
	return 1;
}


char solve(int x, int y, char *map, int *empty, point **result){
	point moves[] = { { -2, 1 }, { -1, 2 }, { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 } };
	map[MAP_SIZE * x + y] = 1;
	char success = 0;
	(*empty)--;

	if (*empty != 0){
		for (int i = 0; i<sizeof(moves) / sizeof(point); i++){
			if ((x + moves[i].x > MAP_SIZE - 1) || (y + moves[i].y > MAP_SIZE - 1) || (x + moves[i].x < 0) || (y + moves[i].y < 0))
				continue;
			if (map[MAP_SIZE * (x + moves[i].x) + y + moves[i].y])
				continue;

			if (solve(x + moves[i].x, y + moves[i].y, map, empty, result)){
				success = 1;
				break;
			}
		}
	}
	else{
		*result = (point *)malloc(MAP_SIZE * MAP_SIZE * sizeof(point));
		if (*result == NULL)
			return 0;
	}

	if (*result != NULL){
		(*result)[(*empty)].x = x;
		(*result)[(*empty)].y = y;
		success = 1;
	}

	(*empty)++;
	map[MAP_SIZE * x + y] = 0;
	return success;
}

void print_horse(point *result){
#define CORNER_POS_X  3
#define CORNER_POS_Y  3
#define PIXEL_RATIO_X 2
#define PIXEL_RATIO_Y 1
	con_init();
	con_hideCursor();
	con_clearScr();
	init_colors();
	con_setColor(COLOR_BORDER);
	for (int i = 0; i < (8 + 2)*PIXEL_RATIO_X; i++){
		for (int j = 0; j < PIXEL_RATIO_Y; j++){
			con_gotoXY(CORNER_POS_X + i, CORNER_POS_Y + j);
			con_outTxt("#");
			con_gotoXY(CORNER_POS_X + i, CORNER_POS_Y + j + 9 * PIXEL_RATIO_Y);
			con_outTxt("#");
		}

	}
	for (int i = 0; i < (8 + 2)*PIXEL_RATIO_Y; i++){
		for (int j = 0; j < PIXEL_RATIO_X; j++){
			con_gotoXY(CORNER_POS_X + j, CORNER_POS_Y + i);
			con_outTxt("#");
			con_gotoXY(CORNER_POS_X + j + 9 * PIXEL_RATIO_X, CORNER_POS_Y + i);
			con_outTxt("#");
		}

	}

	for (int step = MAP_SIZE*MAP_SIZE - 1; step >= 0; step--){

		con_setColor(COLOR_HORSE);
		for (int i = 0; i < PIXEL_RATIO_X; i++){
			for (int j = 0; j < PIXEL_RATIO_Y; j++){
				con_gotoXY(CORNER_POS_X + PIXEL_RATIO_X + result[step].x * PIXEL_RATIO_X + i, CORNER_POS_Y + PIXEL_RATIO_Y + result[step].y * PIXEL_RATIO_Y + j);
				con_outTxt("!");
			}
		}

		Sleep(500);

		con_setColor(COLOR_CHECKED);
		for (int i = 0; i < PIXEL_RATIO_X; i++){
			for (int j = 0; j < PIXEL_RATIO_Y; j++){
				con_gotoXY(CORNER_POS_X + PIXEL_RATIO_X + result[step].x * PIXEL_RATIO_X + i, CORNER_POS_Y + PIXEL_RATIO_Y + result[step].y * PIXEL_RATIO_Y + j);
				con_outTxt(" ");
			}
		}
	}
	con_gotoXY(0, CORNER_POS_Y + PIXEL_RATIO_Y * 10);
	con_setColor(COLOR_NORMAL);
	con_deinit();
}

void horse(int x, int y){
	if (!check_point(x - 1, y - 1))
		return;
	char *map = (char *)calloc(MAP_SIZE * MAP_SIZE, sizeof(char));
	if (map == NULL)
		return;
	int empty = MAP_SIZE * MAP_SIZE;
	point *result = NULL;
	solve(x - 1, y - 1, map, &empty, &result);
	free(map);
	if (result == NULL){
		printf("FAIL!!!\n");
		return;
	}

	printf("SOLVED!\n");
	Sleep(500);
	print_horse(result);
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

	horse(x, y);
	return 0;
}