#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include "console.h"

#define MAP_SIZE 8

typedef struct _point{
	int x;
	int y;
} point;

char check_point(int x, int y){
	if ((x > MAP_SIZE - 1) || (y > MAP_SIZE-1) || (x < 0) || (y < 0))
		return 0;
	return 1;
}


char solve(int x, int y, char *map, int *empty, point **result){
	point moves[] = { { -2, 1 }, { -1, 2 }, { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, -1 } };
	map[MAP_SIZE * x + y] = 1;
	char success = 0;
	(*empty)--;

	if (*empty != 0){
		for (int i = 0; i<sizeof(moves)/sizeof(point); i++){
			if ((x + moves[i].x > MAP_SIZE - 1) || (y + moves[i].y > MAP_SIZE-1) || (x + moves[i].x < 0) || (y + moves[i].y < 0))
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
	con_init();
	con_hideCursor();
	con_clearScr();
	for (int i = 0; i < 8 + 2; i++){
		con_gotoXY(i, 0);
		con_outTxt("#");
		con_gotoXY(i, 9);
		con_outTxt("#");

	}
	for (int i = 0; i < 8 + 2; i++){
		con_gotoXY(0, i);
		con_outTxt("#");
		con_gotoXY(9, i);
		con_outTxt("#");

	}

	for (int i = MAP_SIZE*MAP_SIZE - 1; i >= 0; i--){
		con_gotoXY(result[i].x + 1, result[i].y + 1);
		con_setColor(CON_COLOR_WHITE);
		con_outTxt("@");
		Sleep(500);
		con_gotoXY(result[i].x + 1, result[i].y + 1);
		//con_setColor(CON_COLOR_BLUE);
		con_outTxt(" ");
	}
	getc(stdin);
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
		//con_gotoXY(0, 0);
		//con_outTxt("Cant solve =(\n");
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


	//con_init();
	//con_hideCursor();
	//con_clearScr();
	//init_colors();

	horse(x, y);
	//con_deinit();
	return 0;
}
