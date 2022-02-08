#include <stdio.h>
#include <dir.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

char *string[3]  = { "Input", "Delete", "End" };
int current;
char key;
FILE *fp;
char filename[128];

long getfilesize(char fname[]) {
	struct ffblk found;

	findfirst(fname, &found, 0);
	return found. ff_fsize;
}

void realdelete(int num) {
	int i = 0;
	char idata[9];
	FILE *imsi = fopen("IMSI.DAT", "w");

	while(1) {
		if ( i != num ) {
			fseek(fp, i * 8, SEEK_SET);
			fgets(idata, 9, fp);
			fprintf(imsi, "%-8s", idata);
			if ( i > num ) fseek(imsi, i * 8, SEEK_SET);
			else fseek(imsi, ( i + 1 ) * 8, SEEK_SET);
		}
		i++;
		if ( i > getfilesize(filename) / 8 - 1 ) break;
	}
	fclose(imsi);
}

void input(void) {
	int i = 0, n = 0;
	char data[9];
	char idata[9];
	char olddata[9];

	clrscr_han();
	set_color_han(GREEN);
	draw_box_han(35, 0, 35, 30, BOX_H2V2);
	set_color_han(WHITE);

	while(1) {
		if ( i == getfilesize(filename) / 8 ) break;
		fseek(fp, i * 8, SEEK_SET);
		fgets(idata, 9, fp);
		xprintf_han(40 + 10 * ( i / 25 ), i % 25, "%-8s", idata);
		i++;
	}

	fseek(fp, i * 8, SEEK_SET);

	n = getfilesize(filename) / 8;
	if ( n == 100 ) {
		xprintf_han(1, 20, "Sorry. But you can't add more.");
		getch_han();
		return;
	}

	while(1) {
		gotoxy_han(0, 0);
		printf_han("?...");
		set_max_strlen_han(8);
		scanf_han("%s", data);
		if ( strcmp(data, olddata) == 0 ) return;
		strcpy(olddata, data);
		n++;
		fprintf(fp, "%-8s", data);
		xprintf_han(40 + 10 * ( i / 25 ), i % 25, "%-8s", data);
		i++;
		xprintf_han(0, 0, "                  ");
		if ( n > 99 ) {
			xprintf_han(1, 20, "Sorry. But 100 data is limit...");
			getch_han();
			return;
		}
	}

	set_bkcolor_han(BLACK);
}

void delete(void) {
	int key;
	int num = 0;
	char data[9];

	while(1) {
		fseek(fp, num * 8, SEEK_SET);
		fgets(data, 9, fp);
		printf_han("%-8s\n", data);
		key = getch();
		switch(key) {
			case 43 : if ( getfilesize(filename) / 8 - 1 > num ) num++;
					  break;
			case 45 : if ( num > 0 ) num--;
					  break;
			case DEL : realdelete(num);
					   unlink(filename);
					   rename("IMSI.DAT", filename);
					   return;
			case _ESC : return;
		}

		clrscr_han();
	}
}

void exitfunc(void) {
	int a;
	char newname[13];

	clrscr_han();

	a = getfilesize("NEWFILE.DAT");

	if ( a != 0 ) {
		xprintf_han(20, 10, "You Made New File.");
		xprintf_han(20, 11, "So Please type New File's name.");
		xprintf_han(20, 12, "You'd better type *.DAT.");
		xprintf_han(44, 12, "?");
		gotoxy_han(46, 12);
		set_max_strlen_han(8);
		scanf_han("%s", newname);
		rename("NEWFILE.DAT", newname);
	}

	exit(0);
}

int main(void)
{
	int keycode;
	FILE *imsi;

	imsi = fopen("NEWFILE.DAT", "w");
	fclose(imsi);

	if(init_han() != OK_HAN) exit(1);

	set_bkcolor_han(LIGHTBLUE);

	keycode = dir_box_short(10, 10, "*.DAT", filename);

	if ( keycode == ESC ) exit(3);

	fp = fopen(filename, "a+");

	while(1) {
		clrscr_han();

		do {
			key = popup_han(33, 8, string, 3, &current);
		} while(key == LEFT || key == RIGHT);

		clrscr_han();
		switch(key)
		{
			case ESC   : exitfunc();
			case ENTER :
			switch(current)
			{
				case 0 : input(); fclose(fp); break;
				case 1 :
					 delete();
					 fclose(fp);
					 fp = fopen(filename, "a+");
					 break;
				case 2 : exitfunc();
			 }
		}
	}

	return(0);
}