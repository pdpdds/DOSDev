#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <alloc.h>
typedef unsigned char byte;
typedef unsigned int word;

#include "init_66.h"
//#include "..\\21lib\\21_all.h"
#include "nums.h"

#define ON   1
#define OFF  0

#define bioskey(n) bios_key(n)

#define RETURN   '\r'
#define BS        8
#define TAB       9
#define SPACE    32
#define ESC      27

#define UP       72
#define DOWN     80
#define RIGHT    77
#define LEFT     75
#define HOME     71
#define END      79
#define PGDN     81
#define PGUP     73
#define INSERT   82
#define DELETE   83

typedef union {
	byte ch[2];
	word i;
} INKEY;

byte far big_buf[10000];

extern word MaxX, MaxY;
extern int graph_mode;
extern byte far *vid_addr;
extern word *offsetx;

struct date cd;  // current date

byte cwinicon[]={
0x60,0x00,0x00,0x6F,0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,
0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,
0xFE,0x6E,0x00,0x0E,0x6E,0xFF,0xE6,0x6E,0xFF,0xE6,
0x6E,0x00,0x06,0x6F,0x00,0x06,0x6F,0xFF,0xFE,0x6F,
0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,
0xFE,0x6F,0xFF,0xFE};

int px, py, plx, ply;

void end()
{
	setdate(&cd);
	exit(0);
}

void cput_popup()
{
	byte center;

	mc_off();
	get_image(px, py, px+plx*8, py+ply, big_buf);
	put_ewin(px, py, plx, ply, 0, "”ib");
	mc_on();
}

void coff_popup()
{
	mc_off();
	put_image(px, py, px+plx*8, py+ply, big_buf);
	mc_on();
}


int get_day(int year, int month)
{
	struct date d;
	union REGS r;

	if(year<1980) {
		d.da_year=1980+(28-(1980-year)%28);
	}
	else {
		d.da_year=year;
	}
	d.da_mon=month;
	d.da_day=1;
	setdate(&d);
	r.h.ah=0x2a;
	int86(0x21, &r, &r);
	return r.h.al;
}

byte *W_dat[]={
	"\x01·©",
	"\x0¶©",
	"\x0ÑÁ",
	"\x0®",
	"\x0¡¢",
	"\x0‹q",
	"\x0É¡",
};

char mon_data[12]={31, 28, 31, 30, 31, 30, 31, 31, 30 ,31, 30, 31};

extern int ly;

void put_luner(int x, int y, int year, int month, int n)
{
	byte far *offset;
	int i;

	gprintf(px+22, py+13, 0,"%4d", ly);
	for(i=0; i<5; i++) {
		offset=vid_addr+offsetx[x+i]+y;
		*offset=nums[n-1][i];
	}
	if(n==1) {
		for(i=0; i<5; i++) {
			offset=vid_addr+offsetx[x+i-6]+y;
			*offset=nums[month-1][i];
		}
	}
}

void put_datum(int year, int month, int day)
{
	int i;
	int val;
	int attr=0;
	struct date d;

	mc_off();
	barb(px+22, py+2, px+plx*8-4, py+ply-2, 0);
	if((year%4)==0) {
		if(year%100==0 && year%400!=0) {
			mon_data[1]=28;
		}
		else mon_data[1]=29;
	}
	else mon_data[1]=28;
	gprintf(px+22, py+8, 0,"%4d(    )‘e %2d¶©", year, month);
	for(i=0; i<7; i++) {
		put_string(px+22+20, py+3+i*4, W_dat[i]+1, W_dat[i][0]);
	}
	val=get_day(year, month);
	for(i=0; i<mon_data[month-1] ; i++) {
		if(i==day-1) {
			if(year==cd.da_year && month==cd.da_mon) attr=0x04;
		}
		else attr=0x00;
		if((i+val)%7==0) attr|=0x01;
		else attr|=0;
		calc_luner(px+22+50+((i+val)/7)*16, py+5+4*((i+val)%7), year, month, i+1);
		gprintf(px+22+40+((i+val)/7)*16, py+3+4*((i+val)%7), attr, "%2d", i+1);
	}
	mc_on();
}

void calender_drv()
{
	INKEY c;
	int year, month, day, i;
	int tx, ty, ox, oy, oldx, oldy;
	struct date d;

	getdate(&d);
	getdate(&cd);
	year=d.da_year;
	month=d.da_mon;
	day=d.da_day;

	px=MaxX/2-10*8;
	py=MaxY/16-16;
	plx=20;
	ply=32;
	cput_popup();
	put_datum(year, month, day);
	for(;;) {
		mc_on();
		while(!key_hit()) {
			if(lbp) {
				if(mousex<px+24 && mousex>px && mousey>py*8+24 && mousey<(py+ply)*8) {
					tx=px;
					ty=py;
					i=move_window(&tx, &ty, &plx, &ply);
					if(i==1) {
						 coff_popup();
						 px=tx; py=ty;
						 cput_popup();
						 put_datum(year, month, day);
					 }
					 mc_on();
				}
				else if(mousex<px+24 && mousex>px && mousey>py*8 && mousey<(py+ply)*8) {
					mc_off();
					coff_popup();
					end();
				}
			}
		}
		c.i=bioskey(0);
		mc_off();
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC :  case RETURN :
					coff_popup();
					setdate(&cd);
					return;
			}
		}
		else {
			switch(c.ch[1]) {
				case PGUP : case UP :
					if(c.ch[1]==UP) year--;
					else year-=10;
					if(year<1) year=1;
					put_datum(year, month, day);
					break;
				case PGDN : case DOWN :
					if(c.ch[1]==DOWN)	year++;
					else year+=10;
					if(year>2099) year=2099;
					put_datum(year, month, day);
					break;
				case LEFT :
					month--;
					if(month<1) {
						if(year!=1) year--;
						month=12;
					}
					put_datum(year, month, day);
					break;
				case RIGHT :
					month++;
					if(month==13) {
						if(year!=2099) year++;
						month=1;
					}
					put_datum(year, month, day);
					break;
			}
		}
	}
}

void main()
{
	int num;

	if(is_in_21()==0) {
		printf("Must executed in 21!!!\n");
		exit(0);
	}
	set_ext_data();
	calender_drv();
	end();
}

