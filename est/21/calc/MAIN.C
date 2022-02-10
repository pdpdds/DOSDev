#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <conio.h>

#include <ctype.h>

#include "est.h"
//#include "..\21lib\21_all.h"
#include "..\21lib\init_66.h"

#define CALC_WID  48
#define CALC_LEN  32

extern char far *vid_addr;
extern byte far *our_buf;
extern word *offsetx;
extern int MaxX, MaxY;
extern double Result;
extern byte error_flag;

int Cx, Cy, Clx, Cly;
byte far *calc_buf;
byte Cstr[105];

byte *Butt[]={
	"PI",  "SQRT", "EXP",	 "ABS",   "SQR",
	"SIN", "ASIN", "SINH", "LN",    "TOUP",
	"COS", "ACOS", "COSH", "LOG",   "POW10",
	"TAN", "ATAN", "TANH", "LOG10", "TODOWN",
};

byte *Sym[]={
	 "1", "2", "3", "4", "5", "+", "-", "*", "/",
	 "6", "7", "8", "9", "0", "E", "^", "(", ")",
};

byte *Else[]={
  "Enter",
  "¢…¬á¡",
	"AC",
};

/* CALC_DX.c */
int ch_from_buf ( void );
void loc_back ( void );
void readch ( void );
double power ( double x );
int what_func ( char *str );
double math_func ( void );
double number ( void );
double factor ( void );
double term ( void );
double expression ( void );
//int matherr ( struct exception *a );
int calc_main ( char *str );

void Button(int flag, int x, int y, int ly, byte *str, byte mode)
{
	int t;

	mc_off();
	if(flag==0) {
		t=strlen(str);
		t=(ly-1)/2-t/2;
		gbar(x-2, y*8-2, x+18, (y+ly)*8-6, mode);
		put_string(x, y+t, str, mode);
		gbox(x-2, y*8-2, x+18, (y+ly)*8-6, mode^1);
	}
	else {
		gbar(x-2, y*8-2, x+18, (y+ly)*8-6, -1);
	}
}

byte Rstr[30];

void Cerror(char *s)
{
	mc_off();
	strcpy(Rstr, s);
	error_flag=1;
}

void put_result(void)
{
	int i;
	byte str[100];

	mc_off();
	if(!error_flag) sprintf(Rstr, "%17.17g", Result);
	gprintf(Cx+63, Cy+10, 0, "%27s", Rstr);
	gbox(Cx+60, (Cy+10)*8-2, Cx+60+22, (Cy+37)*8+2, 1);
}

void put_butt(void)
{
	int i, j, x, y;

	x=Cx+128+24; y=Cy+2;
	for(i=0; i<4; i++) {
		for(j=0; j<5; j++) Button(0, x+i*24, y+9*j, 9, Butt[i*5+j], 0);
	}
}

byte *get_butt(int mx, int my)
{
	int x, y, i, j;

	x=Cx+128+24; y=Cy+2;
	if(my<y*8 || my>(y+44)*8) return 0;
	if(mx<x || mx>x+24*4) return 0;

	i=(mx-x)/24;
	j=(my-y*8)/(9*8);
	Button(1, x+i*24, y+9*j, 9, Butt[i*5+j], 0);
	mc_on();
	while(lbp || rbp);
	Button(0, x+i*24, y+9*j, 9, Butt[i*5+j], 0);
	mc_on();
	return Butt[i*5+j];
}

void put_sym(void)
{
	int i, j, x, y;

	x=Cx+74+24; y=Cy+2;

	for(j=0; j<2; j++) {
		for(i=0; i<9; i++) Button(0, x+j*24, y+4*i, 4, Sym[j*9+i], 0);
	}
	Button(0, x,   y+36,  9,  Else[0], 0);
	Button(0, x+24, y+36, 9,  Else[1], 0);
	Button(0, Cx+63, y+36, 9, Else[2], 0);
}

byte get_else(int mx, int my)
{
	int x, y;

	x=Cx+74+24; y=Cy+2;

	if(mx>x && mx<x+20 && my>(y+36)*8 && my<(y+36+10)*8) {
		Button(1, x, y+36, 9, Else[0], 0);
		while(lbp ||rbp);
		Button(0, x, y+36, 9, Else[0], 0);
		return 1;
	}
	else if(mx>x+24 && mx<x+24+20 && my>(y+36)*8 && my<(y+36+10)*8) {
		Button(1, x+24, y+36, 9, Else[1], 0);
		while(lbp ||rbp);
		Button(0, x+24, y+36, 9, Else[1], 0);
		return 2;
	}
	else if(mx>Cx+63 && mx<Cx+63+20 && my>(y+36)*8 && my<(y+36+10)*8) {
		Button(1, Cx+63, y+36, 9, Else[2], 0);
		while(lbp || rbp);
		Button(0, Cx+63, y+36, 9, Else[2], 0);
		return 0;
	}
	else {
		while(lbp || rbp);
		return -1;
	}
}

byte *get_sym(int mx, int my)
{
	int x, y, i, j;

	x=Cx+74+24; y=Cy+2;
	if(my<y*8 || my>(y+36)*8) return 0;
	if(mx>x && mx<x+16) i=0;
	else if(mx>x+24 && mx<x+24+16) i=1;
	else return 0;
	j=(my-y*8)/(4*8);
	Button(1, x+i*24, y+4*j, 4, Sym[i*9+j], 0);
	mc_on();
	while(lbp || rbp);
	Button(0, x+i*24, y+4*j, 4, Sym[i*9+j], 0);
	mc_on();
	return Sym[i*9+j];
}

void off_calc(int x, int y);
void put_calc(void);

int calc_mouse(void)
{
	int i, mx, my, tx, ty;
	byte *str=0, flag=0;

	getpos(mx, my);
	if(mx>Cx && mx<Cx+24 && my>Cy*8 && my<(Cy+Cly)*8) {
		if(my<Cy*8+24) {
			while(lbp || rbp); return -1;
		}
		tx=Cx; ty=Cy;
		i=move_window(&tx, &ty, &Clx, &Cly);
		if(i) {
			mc_off();
			off_calc(Cx, Cy);
			Cx=tx; Cy=ty;
			put_calc();
		}
		return 10;
	}
	str=get_sym(mx, my);
	if(str) goto OUT;
	str=get_butt(mx, my);
	if(str) {
		flag=1; goto OUT;
	}
	i=get_else(mx, my);
	if(i!=-1) {
		return i;
	}
	else return 10;
OUT :
	if(strcmp(str, "PI")==0) flag=0;
/*
	while(*str) {
		key_input(RIGHT, 0);
		key_input(0, tolower(*str));
		str++;
	}
*/
	if(strlen(Cstr)<75) {
		strcat(Cstr, str);
		if(flag) {
//		key_input(0, '(');
			strcat(Cstr, "(");
		}
		key_input(END, 0);
	}
	return 10;
}

void put_calc(void)
{
	get_image(Cx, Cy, Cx+Clx*8+10, Cy+Cly+2, calc_buf);
  put_ewin(Cx, Cy, Clx, Cly, 0, "‰¬e‹¡");
	put_sym();
	put_butt();
	gbox(Cx+30, (Cy+10)*8-2, Cx+30+22, (Cy+46)*8+2, 1);
  put_string(Cx+33, (Cy+2), "·³b :", 0);
  put_string(Cx+63, (Cy+2), "‰i‰Á :", 0);
	put_result();
}

void off_calc(int x, int y)
{
	put_image(x, y, Cx+Clx*8+10, Cy+Cly+2, calc_buf);
}

void calc_driver(void)
{
	INKEY c;
	int i, j, flag=0;

	setmem(Cstr, 105, 0);
	setmem(Rstr, 30, 0);
	put_calc();
	mc_on();
	for(;;) {
		i=get_string(0, Cx+33, Cy+10, 100, 36, (byte far *)Cstr);
		if(i==1 || i==1000) {
			if(Cstr[0]==NULL) {
				error_flag=0; break;
			}
			if(!calc_main(Cstr)) Result=0;
			if(i==1000) {
				if(Result) {
					flag=1; break;
				}
				else Cerror("Nothing to document!!");
			}
			put_result();
		}
		else if(i==-1) break;
		else if(i==-4) {
			j=calc_mouse();
			switch(j) {
				case -1 : goto OUT;
				case  0 :
					setmem(Cstr,  100, 0);
					error_flag=Result=0;
					put_result();
					break;
				case  1 :
					if(Cstr[0]==NULL) {
						error_flag=0; break;
					}
					if(!calc_main(Cstr)) Result=0;
					put_result();
					break;
				case  2 :
					if(error_flag==0) {
						flag=1; goto OUT;
					}
					break;
				default : break;
			}
		}
	}
OUT:
	mc_off();
	off_calc(Cx, Cy);
	if(flag) {
		dat2mother(Rstr);
		set_flag(1);
	}
	else set_flag(0);
	exit_21();
}

void main(void)
{
	if(is_in_21()==0) {
		printf("Must executed in 21!!\n");
		exit(-1);
	}
	set_ext_data();
	calc_buf=(byte far *)our_buf;
	Clx=CALC_LEN;
	Cly=CALC_WID;
	Cx=MaxX/2 - Clx*4;
	Cy=MaxY/16- Cly/2;
	calc_driver();
}

