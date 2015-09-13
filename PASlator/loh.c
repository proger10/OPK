#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "console.h"

short PASlator_bg = CON_COLOR_BLACK;
short PASlator_fg = CON_COLOR_WHITE;
void textcolor(int fg){
    if (fg == 0) PASlator_fg = CON_COLOR_BLACK;
    if (fg == 1) PASlator_fg = CON_COLOR_BLUE;
    if (fg == 2) PASlator_fg = CON_COLOR_GREEN;
    if (fg == 4) PASlator_fg = CON_COLOR_RED;
    if (fg == 15) PASlator_fg = CON_COLOR_WHITE;
    con_initPair(0, PASlator_fg, PASlator_bg);
    con_setColor(0);
}

void textbackground(int bg){
    if (bg == 0) PASlator_bg = CON_COLOR_BLACK;
    if (bg == 1) PASlator_bg = CON_COLOR_BLUE;
    if (bg == 2) PASlator_bg = CON_COLOR_GREEN;
    if (bg == 4) PASlator_bg = CON_COLOR_RED;
    if (bg == 15) PASlator_bg = CON_COLOR_WHITE;
    con_initPair(0, PASlator_fg, PASlator_bg);
    con_setColor(0);
}

int ord(char c){
    return c;
}

char readkey(){
    int result = con_getKey();
    if ((result<0)||(result>255)) exit(1);
    return (char) result;
}

void clrscr(){
    con_clearScr();
}

void gotoxy(int x, int y){
    con_gotoXY(x, y);
}

void frame(int x1, int y1, int x2, int y2) {
int i;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;for (i = x1;
 i <= x2;
 i ++) {
gotoxy(i 
, y1 
);
con_outTxt("%c\n", '#' 
);
;gotoxy(i 
, y2 
);
con_outTxt("%c\n", '#' 
);
;}
;
;for (i = y1;
 i <= y2;
 i ++) {
gotoxy(x1 
, i 
);
con_outTxt("%c", '#' 
);
;gotoxy(x2 
, i 
);
con_outTxt("%c", '#' 
);
;}
;
;}

;
int x;
int y;
char c;
int main(){
    con_init();clrscr();
textcolor(4 
);
frame((6 
)-(1 
) 
, (4 
)-(1 
) 
, (74 
)+(1 
) 
, (19 
)+(1 
) 
);
(x 
)=(10 
);
(y 
)=(10 
);
textcolor(2 
);
gotoxy(x 
, y 
);
con_outTxt("%c", '*' 
);
;(c 
)=(readkey() 
);
while ((ord(c 
) 
)!=(27 
) 
){
if ((c 
)==('w' 
) 
){
if ((y 
)>(4 
) 
){
(y 
)=((y 
)-(1 
) 
);
}
;
}
;
if ((c 
)==('a' 
) 
){
if ((x 
)>(6 
) 
){
(x 
)=((x 
)-(1 
) 
);
}
;
}
;
if ((c 
)==('s' 
) 
){
if ((y 
)<(19 
) 
){
(y 
)=((y 
)+(1 
) 
);
}
;
}
;
if ((c 
)==('d' 
) 
){
if ((x 
)<(74 
) 
){
(x 
)=((x 
)+(1 
) 
);
}
;
}
;
gotoxy(x 
, y 
);
con_outTxt("%c", '*' 
);
;(c 
)=(readkey() 
);
}
;
;    con_deinit();
    return 0;
}