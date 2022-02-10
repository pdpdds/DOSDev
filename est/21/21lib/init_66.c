// version 1992.7.23

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <stdarg.h>
#include <string.h>
#include <conio.h>
#include <alloc.h>
#include <est.h>

#define mousex *mouse_x
#define mousey *mouse_y
#define lbp *lb
#define rbp *rb

byte far *vid_addr;
word far *offsetx;
int MaxX, MaxY;

int far *mouse_x;
int far *mouse_y;
byte far *lb;
byte far *rb;

byte far *our_buf;
byte far *exe_path;
byte far *font_path;
byte far *swap_path;
//unsigned long CURSORSPEED=30000l;

union REGS R;
struct SREGS SR;

byte set_ext_data();
byte far *int_loc;

int is_in_21()
{
	if((int_loc=(byte far *)getvect(0x66))) {
		if(int_loc[20]==50 && int_loc[22]==49 && int_loc[24]==67) {
			return set_ext_data();
		}
	}
	return 0;
}

byte set_ext_data()
{
	union REGS in, out;

	in.x.ax=0;
	int86(0x66, &in, &out);
	if(out.x.ax!=21) return 0;

	in.x.ax=1;
	int86(0x66, &in, &out);
	MaxX=out.x.cx;
	MaxY=out.x.dx;

	in.x.ax=2;
	int86(0x66, &in, &out);
	vid_addr=(byte far *)MK_FP(out.x.cx, out.x.dx);

	in.x.ax=3;
	int86(0x66, &in, &out);
	offsetx=(word far *)MK_FP(out.x.cx, out.x.dx);

	in.x.ax=4;
	int86(0x66, &in, &out);
	mouse_x=(int far *)MK_FP(out.x.ax, out.x.bx);
	mouse_y=(int far *)MK_FP(out.x.cx, out.x.dx);

	in.x.ax=5;
	int86(0x66, &in, &out);
	lb=(byte far *)MK_FP(out.x.ax, out.x.bx);
	rb=(byte far *)MK_FP(out.x.cx, out.x.dx);

	in.x.ax=12;
	int86(0x66, &in, &out);
	our_buf=MK_FP(out.x.ax, out.x.bx);
/*
	CURSORSPEED=(unsigned int)out.x.cx;
	CURSORSPEED<<=16;
	CURSORSPEED+=(unsigned int)out.x.dx;
*/
	in.x.ax=13;
	int86(0x66, &in, &out);
	exe_path=MK_FP(out.x.ax, out.x.bx);

	in.x.ax=14;
	int86(0x66, &in, &out);
	font_path=MK_FP(out.x.ax, out.x.bx);

	in.x.ax=15;
	int86(0x66, &in, &out);
	swap_path=MK_FP(out.x.ax, out.x.bx);
	return 1;
}

void edelay(int time)
{
	R.x.ax=17;
	R.x.bx=time;
	int86(0x66, &R, &R);
}

void mc_off()
{
	union REGS in, out;
	in.x.ax=6;
	int86(0x66, &in, &out);
}

void mc_on()
{
	union REGS in, out;
	in.x.ax=7;
	int86(0x66, &in, &out);
}

void clip_range(int x, int y, int ex, int ey)
{
	union REGS in, out;
	in.x.ax=8;
	in.x.bx=x;
	in.x.cx=y;
	in.x.dx=ex;
	in.x.si=ey;
	int86(0x66, &in, &out);
}

void rest_range()
{
	union REGS in, out;
	in.x.ax=9;
	int86(0x66, &in, &out);
}

int get_fname(byte far *src, byte far *fname, byte code)
{
	R.x.ax=10;
	R.x.bx=code;
	R.x.cx=FP_SEG(src);
	R.x.dx=FP_OFF(src);
	R.x.si=FP_SEG(fname);
	R.x.di=FP_OFF(fname);
	int86(0x66, &R, &R);
	return R.x.bx;
}

void put_string(int x, int y, byte far *str, int attr)
{
	union REGS in, out;
	in.x.ax=11;
	in.x.bx=x;
	in.x.cx=y;
	in.x.dx=FP_SEG(str);
	in.x.si=FP_OFF(str);
	in.x.di=attr;
	int86(0x66, &in, &out);
}

int get_string(byte flag, int x, int y, int len, int scr_len, byte far *buf)
{
	R.x.ax=16;
	R.x.bx=flag;
	R.x.cx=x;
	R.x.dx=y;
	R.x.si=len;
	R.x.di=scr_len;
	SR.es=FP_SEG(buf);
	SR.ds=FP_OFF(buf);
	int86x(0x66, &R, &R, &SR);
	return R.x.ax;
}

void set_ext(byte far *name)
{
  R.x.ax=20;
  R.x.cx=FP_SEG(name);
  R.x.dx=FP_OFF(name);
  int86(0x66, &R, &R);
}

void error(byte far *fname)
{
	R.x.ax=50;
	R.x.bx=FP_SEG(fname);
	R.x.cx=FP_OFF(fname);
	int86(0x66, &R, &R);
}

int user_choice(byte far *str)
{
	R.x.ax=51;
	R.x.bx=FP_SEG(str);
	R.x.cx=FP_OFF(str);
	int86(0x66, &R, &R);
	return R.x.bx;
}

word key_hit()
{
	R.x.ax=52;
	int86(0x66, &R, &R);
	return R.x.bx;
}

int bios_key(int num)
{
	R.x.ax=53;
	R.x.bx=num;
	int86(0x66, &R, &R);
	return R.x.bx;
}

/*
void set_mpos(int x, int y)
{
	R.x.ax=55;
	R.x.bx=x;
	R.x.cx=y;
	int86(0x66, &R, &R);
}
*/

byte direc[80];

void save_screen(byte *fname)
{
	FILE *out;

	sprintf(direc, "%s\\%s", swap_path, fname);
	if((out=fopen(direc, "wb+"))==0) {
		error("Cannot Open Screen file!");
		return;
	}
	fwrite(vid_addr, (word)((MaxX+1)*((MaxY+1)/8)), 1, out);
	fclose(out);
}

void load_screen(byte *fname)
{
	FILE *in;

	sprintf(direc, "%s\\%s", swap_path, fname);
	if((in=fopen(direc,"rb"))==0) {
		error("Cannot Open Screen file!");
		return;
	}
	fread(vid_addr, (word)((MaxX+1)*((MaxY+1)/8)),1,in);
	fclose(in);
}

int gprintf(int x, int y, byte mode, byte far *fmt, ...)
{
	 va_list argptr;
	 int cnt;
	 byte buffer[80];

	 va_start(argptr, fmt);
	 cnt = vsprintf(buffer, fmt, argptr);
	 va_end(argptr);
	 put_string(x, y, buffer, mode);
	 return(cnt);
}

// FROM 100 GRAPHIC FUNCTION

void gclip_off()
{
	R.x.ax=94;
	int86(0x66, &R, &R);
}

void gclip_on(int x, int y, int ex, int ey)
{
	R.x.ax=95;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	R.x.si=ey;
	int86(0x66, &R, &R);
}

void glinex(int x, int ex, int y, byte mode)
{
	R.x.ax=96;
	R.x.bx=x;
	R.x.cx=ex;
	R.x.dx=y;
	R.x.si=mode;
	int86(0x66, &R, &R);
}

void gliney(int x, int y, int ey, byte mode)
{
	R.x.ax=97;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ey;
	R.x.si=mode;
	int86(0x66, &R, &R);
}

void gbox(int x, int y, int ex, int ey, byte mode)
{
	R.x.ax=98;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	R.x.si=ey;
	R.x.di=mode;
	int86(0x66, &R, &R);
}

void gbar(int x, int y, int ex, int ey, byte mode)
{
	R.x.ax=99;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	R.x.si=ey;
	R.x.di=mode;
	int86(0x66, &R, &R);
}

void linexy(int x, int y, int ex, int ey, int mode)
{
	R.x.ax=106;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	R.x.si=ey;
	R.x.di=mode;
	int86(0x66, &R, &R);
}

void xorbox_byte(int x, int y, int len)
{
	R.x.ax=113;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=len;
	int86(0x66, &R, &R);
}

void barb(int x, int y, int ex, int ey, int mode)
{
	R.x.ax=114;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	R.x.si=ey;
	R.x.di=mode;
	int86(0x66, &R, &R);
}

void put_free_image(int x, int y, byte xsize, byte ysize, byte far *offset, byte mode)
{
	R.x.ax=120;
	R.x.bx=x;
	R.x.cx=y;
	R.h.dh=xsize;
	R.h.dl=ysize;
	R.x.si=FP_SEG(offset);
	R.x.di=FP_OFF(offset);
	SR.ds=mode;
	int86x(0x66, &R, &R, &SR);
}

void get_image(int x, int y, int ex, int ey, byte far *offset)
{
	R.x.ax=121;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	SR.ds=ey;
	R.x.si=FP_SEG(offset);
	R.x.di=FP_OFF(offset);
	int86x(0x66, &R, &R, &SR);
}

void put_image(int x, int y, int ex, int ey, byte far *offset)
{
	R.x.ax=122;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	SR.ds=ey;
	R.x.si=FP_SEG(offset);
	R.x.di=FP_OFF(offset);
	int86x(0x66, &R, &R, &SR);
}

void  pwi(int x, int y, byte far *buf)
{
	R.x.ax=123;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=FP_SEG(buf);
	R.x.si=FP_OFF(buf);
	int86(0x66, &R, &R);
}

void put_panel(int x, int y, int ex, int ey, byte mode)
{
	R.x.ax=124;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ex;
	R.x.si=ey;
	R.x.di=mode;
	int86(0x66, &R, &R);
}

// FROM 200 Window Function

void put_hbar(int x, int ex, int y)
{
	R.x.ax=201;
	R.x.bx=x;
	R.x.cx=ex;
	R.x.dx=y;
	int86(0x66, &R, &R);
}

void put_vbar(int x, int y, int ey)
{
	R.x.ax=202;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ey;
	int86(0x66, &R, &R);
}

void loc_hbar(word x, word ex, word y, word total, int sub, int old_sub)
{
	R.x.ax=203;
	R.x.bx=x;
	R.x.cx=ex;
	R.x.dx=y;
	R.x.si=total;
	R.x.di=sub;
	SR.es=old_sub;
	int86x(0x66, &R, &R, &SR);
}

void loc_vbar(word x, word y, word ey, word total, int sub, int old_sub)
{
	R.x.ax=204;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ey;
	R.x.si=total;
	R.x.di=sub;
	SR.es=old_sub;
	int86x(0x66, &R, &R, &SR);
}

int set_hbar(x, ex, y, total, sub, old_sub, home, end)
int x, ex, y, total, sub, old_sub, home, end;
{
	R.x.ax=199;      // Setting variables C1, C2, C3
	R.x.bx=x;
	R.x.cx=ex;
	R.x.dx=y;
	int86(0x66, &R, &R);

	R.x.ax=205;
	R.x.bx=total;
	R.x.cx=sub;
	R.x.dx=old_sub;
	R.x.si=home;
	R.x.di=end;
	int86(0x66, &R, &R);
	return R.x.bx;
}

int set_vbar(x, y, ey, total, sub, old_sub, home, end)
int x, y, ey, total, sub, old_sub, home, end;
{
	R.x.ax=199;      // Setting variables C1, C2, C3
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=ey;
	int86(0x66, &R, &R);

	R.x.ax=206;
	R.x.bx=total;
	R.x.cx=sub;
	R.x.dx=old_sub;
	R.x.si=home;
	R.x.di=end;
	int86(0x66, &R, &R);
	return R.x.bx;
}

void put_ewin(int x, int y, int lx, int ly, byte code, byte far *str)
{
	R.x.ax=207;
	R.x.bx=x;
	R.x.cx=y;
	R.x.dx=lx;
	R.x.si=ly;
	R.x.di=code;
	SR.es=FP_SEG(str);
	SR.ds=FP_OFF(str);
	int86x(0x66, &R, &R, &SR);
}

int move_window(int far *x, int far *y, int far *lx, int far *ly)
{
	R.x.ax=200;
	R.x.bx=FP_SEG(x);
	R.x.cx=FP_OFF(x);
	R.x.dx=FP_SEG(y);
	R.x.si=FP_OFF(y);
	int86(0x66, &R, &R);

	R.x.ax=208;
	R.x.bx=FP_SEG(lx);
	R.x.cx=FP_OFF(lx);
	R.x.dx=FP_SEG(ly);
	R.x.si=FP_OFF(ly);
	int86(0x66, &R, &R);
	return R.x.bx;
}

int resize_window(int far *x, int far *y, int far *lx, int far *ly)
{
	R.x.ax=200;
	R.x.bx=FP_SEG(x);
	R.x.cx=FP_OFF(x);
	R.x.dx=FP_SEG(y);
	R.x.si=FP_OFF(y);
	int86(0x66, &R, &R);

	R.x.ax=209;
	R.x.bx=FP_SEG(lx);
	R.x.cx=FP_OFF(lx);
	R.x.dx=FP_SEG(ly);
	R.x.si=FP_OFF(ly);
	int86(0x66, &R, &R);
	return R.x.bx;
}

void set_win_min(int x, int y)
{
	R.x.ax=210;
	R.x.bx=x;
	R.x.cx=y;
	int86(0x66, &R, &R);
}

void dat2mother(byte *s)
{
	byte *str;

	R.x.ax=220;
	int86(0x66, &R, &R);
	str=MK_FP(R.x.bx, R.x.cx);
	strcpy(str, s);
}

void set_flag(byte c)
{
	R.x.ax=221;
	R.x.bx=c;
	int86(0x66, &R, &R);
}

void save_buf(byte far *fname, byte far *buf, word len)
{
	FILE *out;

	if((out=fopen(fname,"wb+"))==0) {
		return;
	}
	fwrite(buf, len, 1, out);
	fclose(out);
}

void load_buf(byte *fname, byte far *buf, word len)
{
	FILE *in;

	if((in=fopen(fname,"rb"))==0) {
		return;
	}
	fread(buf, len, 1, in);
	fclose(in);
	remove(fname);
}

void key_input(char scan_code, char ascii_code)
{
	_AH=5;
	_CH=scan_code;
	_CL=ascii_code;
	geninterrupt(0x16);
}

/*
void test()
{
	gprintf(0, MaxY/8-3, 0x05, "%3luK", farcoreleft()/1024);
}
*/

void exit_21()
{
	exit(0);
}
