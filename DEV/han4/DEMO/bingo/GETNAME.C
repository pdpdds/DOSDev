#include <stdio.h>
#include "extendio.h"
#include "hanio.h"
#include "keydef.h"

char name[11][11] = { "aa", "bb", "cc", "dd", "ee", "ff", "gg", "ff", "hh", "ii", "End" };
char key;
int current = 0;

void readname(void) {
	int i;
	FILE *fp = fopen("BINGO.NAM", "r+");

    for ( i = 0 ; i < 11 ; i++ ) {
		fgets(name[i], 11, fp);
		fseek(fp, (i + 1) * 10, SEEK_SET);
	}

	fclose(fp);
}

void savename(void) {
	int i;
	FILE *fp = fopen("BINGO.NAM", "w+");

	for ( i = 0 ; i < 11 ; i++ ) {
		fprintf(fp, "%-10s", name[i]);
	}

	fclose(fp);
}

void firstscreen(void) {
	int i;

	set_bkcolor_han(LIGHTBLUE);
	clrscr_han();
	draw_box_han(20, 4, 60, 16, BOX_H2V2);
	draw_box_han(27, 4, 27, 16, BOX_H2V2);
	put_eng_font(27, 4, 203);
	put_eng_font(27, 16, 202);

	xprintf_han(22, 5, "USER");
	xprintf_han(22, 6, "Com");

	for ( i = 0 ; i < 11 ; i++ ) {
		xprintf_han(30, 5 + i, "%s", name[i]);
	}
}

int main(void) {
	int y = 0, oldy = 0;
	int in = 0;
	int key;

	if (init_han() != OK_HAN) exit(1);
	set_max_strlen_han(10);
	readname();

	firstscreen();

	gotoxy_han(30, 5);
	while(1) {
		set_vbcolor_han(WHITE, LIGHTGREEN);
		xprintf_han(30, 5 + y, "%-10s", name[y]);

		oldy = y;

		key = getch_han();

		if ( key == _ESC ) exit(1);
		if ( key == 83 || key == 115 ) savename();

		if ( key == _ENTER ) {
			if ( y == 10 ) exit(1);

			set_vbcolor_han(WHITE, LIGHTBLUE);
			gotoxy_han(40, 22);
			printf_han("?");
			in = gets_han( name[y] );
			if ( in == ESC ) exit(2);
			set_vbcolor_han(WHITE, LIGHTBLUE);
			xprintf_han(40, 22, "                                      ");
		}

		if ( key == 0 ) {
			key = getch_han();
			switch ( key ) {
				case DOWN : if ( y == 10 ) y = 0;
							else y++;
							break;
				case UP	  : if ( y == 0 ) y = 10;
							else y--;
							break;
			}
		}

		set_vbcolor_han(WHITE, LIGHTBLUE);
		xprintf_han(30, 5 + oldy, "%-10s", name[oldy]);
	}

	getch_han();
	close_han();

	return(0);
}