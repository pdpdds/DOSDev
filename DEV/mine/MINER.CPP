#include <io.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

#include "button.h"
#include "mouse.h"

void draw_panel_up(void);
void draw_panel_down(int _ns_bomb,int _time,int _set_bomb);

void draw_num(int num,int xx,int yy);
void _draw_num(int num_,int xx,int yy);
void __draw_num(int xx,int yy,int color,int x0_x1_y2_y3);

int show_time(int set);
int show_ns_bomb(int set);

void seek(int m_x,int m_y);
void _seek_boom(int xx,int yy);
void _seek_ns_bomb(int xx,int yy);
void __seek_ns_bomb(int xx,int yy,unsigned char *buff_x,unsigned char *buff_y,int *ne);
int ___seek_ns_bomb(int xx,int yy);
void flag(int m_x,int m_y);
void set_bombs(void);
void push_new(int _key);
int push_set(void);
void _push_set_pr_data(void);
void _push_set_player(int new_mode,int _key);
int _push_set_player_custom(void);
void _push_set_help(int _key);

void outtextxy_3d(int x,int y,char *str,int color,int ucolor);
void outtextxy_outline(int x,int y,char *str,int color,int out_color);

void success(void);
unsigned x_getch(void);
void clear_key_buff(void);

int _gets_eng(int xx,int yy,int color,int bkcolor,int len,char *buff);
void save(void);
int load(void);
void _save_load(struct ss *buff[5]);
void show_score(int set,int _time);
int _show_score(struct sc *score_1,struct sc *score_2,struct sc *score_3,int _time);

/*-------------------------------------------------------------------*/
void get_dac(unsigned char *rgb);
void set_dac(unsigned char *rgb);
void fade_in(unsigned char *rgb);
void fade_out(void);

/*-------------------------------------------------------------------*/

/**/
extern void EGAVGA_driver(void);
/*
#define Beginner     { mode = 0; x = 11; y = 9 ; bombs = 1 ; SIZE = 20; }
#define Intermediate { mode = 1; x = 24; y = 24; bombs = 1 ; SIZE = 16; }
#define Expert       { mode = 2; x = 52; y = 35; bombs = 1;  SIZE = 12; }
*/

#define Beginner     { mode = 0; x = 11; y = 9 ; bombs = 15 ; SIZE = 20; }
#define Intermediate { mode = 1; x = 24; y = 24; bombs = 90 ; SIZE = 16; }
#define Expert       { mode = 2; x = 52; y = 35; bombs = 400; SIZE = 12; }

#define F1    	  0x3b00
#define ALT_B     0x3000
#define ALT_C     0x2e00
#define ALT_E     0x1200
#define ALT_I     0x1700
#define ALT_L     0x2600
#define ALT_N     0x3100
#define ALT_R     0x1300
#define ALT_S     0x1f00
#define ALT_X     0x2d00

#define DEL   0x5300
#define UP    0x4800
#define DOWN  0x5000
#define LEFT  0x4b00
#define RIGHT 0x4d00
#define INS   0x5200
#define HOME  0x4700
#define END   0x4f00
#define ESC   0x001b
#define BS    0x0008
#define ENTER 0x000d

int x=11, y=9, start_x,start_y,bombs=15, mode =0;
int SIZE=20, CHK_TIME=0, CHK_MOUSE=0;

unsigned char bomb_xy[62][42];
//	DCBA 4321	=> 0 - NO, 1 - bomb, 2 -  , 4 - 	//				   A - push, B - flag, C - ?
struct ss
{	char name[30], size, mode, x, y, board[62][42];
	int bombs, time, ns_bombs;
};
struct sc
{	char name[30];
	int time;
};

const unsigned char
	fl[36] =
	{	0x07,0x00,0x07,0x00,0x00,0xFF,0xEF,0xEF,0x00,0xFF,0xE7,0xE7,0x00,0xFF,0xE3,0xE3,
		0x18,0xE7,0xE7,0xE7,0x18,0xE7,0xE7,0xE7,0x18,0xE7,0xE7,0xE7,0x3C,0xC3,0xC3,0xC3,
		0x3C,0xC3,0xC3,0xC3 },
	que[36] =
	{ 0x07,0x00,0x07,0x00,0x3C,0xC3,0xC3,0xC3,0x66,0x81,0x81,0x81,0x06,0xC8,0xC8,0xC8,
		0x0C,0xF0,0xF0,0xF0,0x18,0xE1,0xE1,0xE1,0x00,0xF3,0xF3,0xF3,0x18,0xE7,0xE7,0xE7,
		0x00,0xF3,0xF3,0xF3 },
	bm[36] =
	{	0x07,0x00,0x07,0x00,0x00,0xB5,0xB5,0xB5,0x00,0x42,0x42,0x42,0x30,0xB1,0xB1,0xB1,
		0x20,0x21,0x21,0x21,0x00,0x80,0x80,0x80,0x00,0x81,0x81,0x81,0x00,0x42,0x42,0x42,
		0x00,0xAD,0xAD,0xAD },
	numa[8][36] =
	{	0x07,0x00,0x07,0x00,0x18,0xFF,0xFF,0xFF,0x3C,0xFB,0xFB,0xFB,0x1C,0xFB,0xFB,0xFB,
		0x1C,0xFB,0xFB,0xFB,0x1C,0xFB,0xFB,0xFB,0x1C,0xFB,0xFB,0xFB,0x7E,0xFF,0xFF,0xFF,
		0x3F,0xC0,0xC0,0xC0,
		0x07,0x00,0x07,0x00,0x7C,0xFF,0xFF,0x83,0xFE,0xC7,0xC7,0x01,0x67,0x9E,0x9E,0x98,
		0x1F,0xFC,0xFC,0xE0,0x7E,0xF1,0xF1,0x81,0xFE,0xC7,0xC7,0x01,0xFF,0xFE,0xFE,0x00,
		0x7F,0x80,0x80,0x80,
		0x07,0x00,0x07,0x00,0x7C,0xFF,0x83,0xFF,0xFE,0xC7,0x01,0xC7,0x67,0x9E,0x98,0x9E,
		0x1F,0xFC,0xE0,0xFC,0x0E,0xF7,0xF1,0xF7,0xC7,0xFE,0x38,0xFE,0x7F,0xFC,0x80,0xFC,
		0x3E,0xC1,0xC1,0xC1,
		0x07,0x00,0x07,0x00,0x1C,0xFF,0xE3,0xE3,0x3E,0xFD,0xC1,0xC1,0x7E,0xED,0x81,0x81,
		0xFE,0xCD,0x01,0x01,0xFE,0xFF,0x01,0x01,0x7F,0x8C,0x80,0x80,0x0E,0xFD,0xF1,0xF1,
		0x06,0xF9,0xF9,0xF9,
		0x07,0x00,0x07,0x00,0xFE,0x01,0xFF,0xFF,0xFF,0x00,0xC0,0xC0,0xFC,0x03,0xFF,0xFF,
		0x7E,0x81,0x87,0x87,0x07,0xF8,0xFE,0xFE,0xC7,0x38,0xFE,0xFE,0x7F,0x80,0xFC,0xFC,
		0x3E,0xC1,0xC1,0xC1,
		0x07,0x00,0x07,0x00,0x3C,0xC3,0xFF,0xC3,0x7E,0x81,0xE1,0x81,0xF0,0x0F,0xCF,0x0F,
		0xFC,0x03,0xFF,0x03,0xFE,0x01,0xC7,0x01,0xE7,0x18,0xDE,0x18,0x7F,0x80,0xFC,0x80,
		0x3E,0xC1,0xC1,0xC1,
		0x07,0x00,0x07,0x00,0xFE,0x01,0x01,0xFF,0xFF,0x00,0x00,0xC6,0x6F,0x90,0x90,0x9C,
		0x1E,0xE1,0xE1,0xF9,0x3C,0xC3,0xC3,0xF3,0x38,0xC7,0xC7,0xF7,0x38,0xC7,0xC7,0xF7,
		0x18,0xE7,0xE7,0xE7,
		0x07,0x00,0x07,0x00,0x00,0xFF,0xFF,0x83,0x38,0xC7,0xC7,0x01,0x21,0xDE,0xDE,0x18,
		0x03,0xFC,0xFC,0x80,0x38,0xC7,0xC7,0x01,0x21,0xDE,0xDE,0x18,0x03,0xFC,0xFC,0x80,
		0x3E,0xC1,0xC1,0xC1 };

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

int main(void)
{   int gd=VGA,gm=VGAHI, m_x,m_y,m_l,m_r,chk=0,tmp,i,j;
	unsigned char *rgb,*rgb2;;
	char *text_buff;
	struct text_info t_i;

	text_buff = (char *)malloc(4000);
	rgb = (unsigned char *)malloc(256*3);
	gettext(1,1,80,25,text_buff);
	gettextinfo(&t_i);

	detectgraph(&gd,&gm);	// check VGA
	if (gd != VGA)
	{	cputs("Error : This program must be run in VGA.\n\r\a");
		return(1);
	}

/*closegraph();
printf("GD = %d , GM = %d\n",gd,gm);return;*/
//bombs=1;mode=3;SIZE=10;x=62;y=42;

	registerbgidriver(EGAVGA_driver);
	initgraph(&gd,&gm,"");

	get_dac(rgb);
	rgb2 = (unsigned char *)malloc(256*3);
	for(i=0;i<256*3;i++)
		rgb2[i] = 0;
	set_dac(rgb2);
	free(rgb2);

//	if (mouse_initialize() != 0) CHK_MOUSE = 1;
	if (mouse_initialize() == 0)
	  {	draw_panel_up();
		draw_panel_down(-1,-1,YES);
fade_in(rgb);
		box_fill_pattern(173,223,477,287,8,0,0xaa);
		box(168,218,472,282,1);
		button_no_fill(169,219,471,281,UP);
		box_fill_pattern(170,220,470,280,9,7,0xaa);
		outtextxy_outline(180,240,"Error  :",12,8);
		outtextxy_outline(252,240,"Mouse Driver Not Found.....",14,8);
		setcolor(8);
		outtextxy(250,260,"Push Any Button...");
		getch();
	  } else
	  {	mouse_range(0,0,631,471);

		draw_panel_up();
		draw_panel_down(-1,-1,YES);
/**/
fade_in(rgb);
/**/

//show_score();
//return;

		mouse_show_cursor();
		do
		{	mouse_information(&m_l,&m_r,&m_x,&m_y);
			if (CHK_TIME == 1) show_time(-2);
			chk = x_getch();
			if (chk != 0)
			  {	if (chk == ALT_X) break;
				else if (chk == ALT_N) push_new(YES);
				else if (chk == ALT_B && mode != 0)
				{   _push_set_player(0,YES);
					draw_panel_up();
					draw_panel_down(-1,-1,YES);
					CHK_TIME = 0;
				}
				else if (chk == ALT_I && mode != 1)
				{   _push_set_player(1,YES);
					draw_panel_up();
					draw_panel_down(-1,-1,YES);
					CHK_TIME = 0;
				}
				else if (chk == ALT_E && mode != 2)
				{   _push_set_player(2,YES);
					draw_panel_up();
					draw_panel_down(-1,-1,YES);
					CHK_TIME = 0;
				}
				else if (chk == ALT_C)
				{   int o_b=bombs,o_x=x,o_y=y;

					_push_set_player(3,YES);
					if (o_b != bombs || o_x != x || o_y != y)
					{	draw_panel_up();
						draw_panel_down(-1,-1,YES);
						CHK_TIME = 0;
					}
				}
				else if (chk == ALT_S) save();
				else if (chk == ALT_L)
				{	tmp = load();
					if (tmp == 2)		// no saved
					{   mouse_show_cursor();
						draw_panel_up();
						draw_panel_down(-1,-1,YES);
						CHK_TIME = 0;
					}
					if (tmp == 3)
					{   mouse_show_cursor();
						draw_panel_up();
						draw_panel_down(-5,-4,NO);
						mouse_hide_cursor();

						for(i=0;i<y;i++)
							for(j=0;j<x;j++)
							{	if (bomb_xy[j][i] == 0x10)
								{	bomb_xy[j][i] = 0;
									___seek_ns_bomb(j,i);
								}
								if ((bomb_xy[j][i]&0x20) == 0x20)
									putimage(start_x+(j*SIZE)+((SIZE-8)>>1),start_y+(i*SIZE)+((SIZE-8)>>1),fl,COPY_PUT);
								if ((bomb_xy[j][i]&0x40) == 0x40)
									putimage(start_x+(j*SIZE)+((SIZE-8)>>1),start_y+(i*SIZE)+((SIZE-8)>>1),que,COPY_PUT);
							}
						mouse_show_cursor();
					}
				}
				else if (chk == ALT_R) show_score(0,0);
				else if (chk == F1) _push_set_help(YES);
				chk = 0;
				clear_key_buff();
			  }
			  else
				chk = 0;
			if (m_l == 0 && m_r == 0) continue;

			if (CHK_TIME != -1 && m_l == 1 && (m_x >= start_x && m_y >= start_y)
						&& (m_x < start_x+(x*SIZE) && m_y < start_y+(y*SIZE)) ) seek(m_x,m_y);
			if (CHK_TIME != -1 && m_r == 1 && (m_x >= start_x && m_y >= start_y)
						&& (m_x < start_x+(x*SIZE) && m_y < start_y+(y*SIZE)) ) flag(m_x,m_y);
			if ((m_r == 1 || m_l == 1) &&
				(m_x >= start_x+x*SIZE-100 && m_y >= start_y-37 &&
				 m_x <= start_x+x*SIZE-63 && m_y <= start_y-16)) push_new(NO);
			if ((m_r == 1 || m_l == 1) &&
				(m_x >= start_x && m_y >= start_y-37 &&
				 m_x <= start_x+44 && m_y <= start_y-16)) chk = push_set();

		} while(chk == 0);
	  }

fade_out();
free(rgb);
box_fill_x8(1,1,80,60,0);
//set_dac(rgb);

	closegraph();
	puttext(1,1,80,25,text_buff);
	free(text_buff);
	gotoxy(2,t_i.cury);
	textattr(0x71);
	cputs("   Miner  v1.01       1993.10.27        Lee Kyoung Su    (HiTel ID: ttagai)   ");
	textattr(t_i.attribute);
	gotoxy(t_i.curx,t_i.cury);

	return(0);
}
/*-------------------------------------------------------------------*/
void draw_panel_up(void)	// max => 52,35 // min => 11,1
{	start_x = ( 640-(x*SIZE)) >>1;
	start_y = ((480-(y*SIZE)) >>1) + 19;

	mouse_hide_cursor();

	box_fill_pattern(0,0,639,479,0,7,0xaa);
	panel_s(start_x-7,start_y-44,start_x+x*SIZE+7,start_y-9,7);

	button_no_fill(start_x,start_y-37,start_x+44,start_y-16,UP);
	outtextxy_3d(start_x+11,start_y-29,"SET",9,15);

	button_down_s(start_x+x*SIZE-163,start_y-37,start_x+x*SIZE-103,start_y-16,0,NO);

	button_no_fill(start_x+x*SIZE-100,start_y-37,start_x+x*SIZE-63,start_y-16,UP);
	outtextxy_3d(start_x+x*SIZE-93,start_y-29,"NEW",9,15);

	button_down_s(start_x+x*SIZE-60,start_y-37,start_x+x*SIZE,start_y-16,0,NO);

	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void draw_panel_down(int _ns_bomb,int _time ,int _set_bomb)
{   int i,j, xy,xy2;

	mouse_hide_cursor();
	panel_s(start_x-7,start_y-8,start_x+x*SIZE+7,start_y+y*SIZE+8,7);

	for(i=0,xy2=start_y+y*SIZE-1,xy=start_x+(SIZE-1);i<x;i++,xy+=SIZE)
		line_y(start_y+1,xy2,xy,8);
	for(j=0,xy2=start_x+x*SIZE-1,xy=start_y+(SIZE-1);j<y;j++,xy+=SIZE)
		line_x(start_x+1,xy2,xy,8);

	for(i=0,xy2=start_y+y*SIZE-1,xy=start_x;i<x;i++,xy+=SIZE)
		line_y(start_y,xy2,xy,15);
	for(j=0,xy2=start_x+x*SIZE-1,xy=start_y;j<y;j++,xy+=SIZE)
		line_x(start_x,xy2,xy,15);

	show_ns_bomb(_ns_bomb);
	show_time(_time);
	if (_set_bomb == YES) set_bombs();
	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void draw_num(int num,int xx,int yy)
{   int cur = mouse_get_cursor();
    if (cur == 1) mouse_hide_cursor();

    _draw_num(num/1000,xx+13,yy);	num -= num/1000*1000;
    _draw_num(num/100,xx+26,yy); 	num -= num/100*100;
    _draw_num(num/10,xx+39,yy);  	num -= num/10*10;
    _draw_num(num,xx+52,yy);

    if (cur == 1) mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void _draw_num(int num_,int xx,int yy)
{   static const int col[10][7] =
	{ 14,14,14,8,14,14,14, 8,8,14,8,8,14,8,   14,8,14,14,14,8,14,
	  14,8,14,14,8,14,14,  8,14,14,14,8,14,8, 14,14,8,14,8,14,14,
	  14,14,8,14,14,14,14, 14,8,14,8,8,14,8,  14,14,14,14,14,14,14,
	  14,14,14,14,8,14,14 };

	__draw_num(xx+2,yy   ,col[num_][0],0);
	__draw_num(xx  ,yy+1 ,col[num_][1],2);
	__draw_num(xx+9,yy+1 ,col[num_][2],3);
	__draw_num(xx+2,yy+8 ,col[num_][3],0);
	__draw_num(xx  ,yy+9 ,col[num_][4],2);
	__draw_num(xx+9,yy+9 ,col[num_][5],3);
	__draw_num(xx+2,yy+15,col[num_][6],1);
}
/*-------------------------------------------------------------------*/
void __draw_num(int xx,int yy,int color,int x0_x1_y2_y3)
{   switch(x0_x1_y2_y3)
	{	case 0 :
			line_x(xx,xx+6,yy,color);
			line_x(xx+1,xx+5,yy+1,color);
			break;
		case 1 :
			line_x(xx+1,xx+5,yy,color);
			line_x(xx,xx+6,yy+1,color);
			break;
		case 2 :
			line_y(yy,yy+6,xx,color);
			line_y(yy+1,yy+5,xx+1,color);
			break;
		case 3 :
			line_y(yy+1,yy+5,xx,color);
			line_y(yy,yy+6,xx+1,color);
			break;
		default :
			break;
	}
}
/*-------------------------------------------------------------------*/
int show_time(int set)	//	-1 => reset,   -2 => go,  -3 => return time,  (set >=0)  => set
{	static int t_time;
	static long sec,psec;

	if (set == -1)
	  {	t_time = 0;
		time(&psec);
	  }
	if (set == -2)
	{	time(&sec);
		if (sec == psec) return t_time;
		t_time += (int)(sec-psec);
		if (t_time > 9999 || t_time < 0) t_time = 0;
		psec = sec;
	}
	if (set >= 0)
	{   t_time = set;
	    time(&psec);
	}
	if (set != -3 && set < 0)
	{	mouse_hide_cursor();
		draw_num(t_time,start_x+x*SIZE-68,start_y-34);
		mouse_show_cursor();
	}
	return t_time;
}
/*-------------------------------------------------------------------*/
int show_ns_bomb(int set)	// -1 => reset, -2 => ns_bombs++, -3 => ns_bombs--, -4 => return ns_bombs, (set >=0) => set
{	static int ns_bomb;

	if (set == -1) ns_bomb = bombs;
	if (set >= 0) ns_bomb = set;
	if (set == -2 || set == -3)
	{	ns_bomb += (set == -2)? 1 : -1;
		if (ns_bomb < 0)
		{	ns_bomb = 0;
			return -1;
		}
	}
	if (set != -4 && set < 0)
	{	mouse_hide_cursor();
		draw_num(ns_bomb,start_x+x*SIZE-171,start_y-34);
		mouse_show_cursor();
	}
	return ns_bomb;
}
/*-------------------------------------------------------------------*/
void seek(int m_x,int m_y)
{	int i,j, xx,yy,chk;

	xx = (m_x-start_x)/SIZE;
	yy = (m_y-start_y)/SIZE;

	if (bomb_xy[xx][yy] == 0x10 || (bomb_xy[xx][yy]&0x20) == 0x20 || (bomb_xy[xx][yy]&0x40) == 0x40) return;
	mouse_hide_cursor();
	button_no_fill(start_x+(xx*SIZE),start_y+(yy*SIZE),
				   start_x+(xx*SIZE)+(SIZE-1),start_y+(yy*SIZE)+(SIZE-1),DOWN);
	if (mouse_check_click(start_x+(xx*SIZE),start_y+(yy*SIZE),
						  start_x+(xx*SIZE)+(SIZE-1),start_y+(yy*SIZE)+(SIZE-1)) == 0)
	{	button_no_fill(start_x+(xx*SIZE),start_y+(yy*SIZE),
				   start_x+(xx*SIZE)+(SIZE-1),start_y+(yy*SIZE)+(SIZE-1),UP);
		mouse_show_cursor();
		return;
	}

	if ((bomb_xy[xx][yy]&0x01) == 0x01)
		_seek_boom(xx,yy);
	  else
	  {	_seek_ns_bomb(xx,yy);
		if (CHK_TIME == 0)
		{	CHK_TIME = 1;
			show_time(-1);
		}
		for(i=0,chk=0;i<y && chk==0;i++)
			for(j=0;j<x && chk==0;j++)
		if (bomb_xy[j][i] != 0x10 && bomb_xy[j][i] != 0x21) chk = 1;
		if (chk == 0) success();
	  }

	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void _seek_boom(int xx,int yy)
{	int i,j, xxx,yyy;

	cputs("\a");
	CHK_TIME = -1;

	box_fill(start_x+xx*SIZE+1,start_y+yy*SIZE+1,
			 start_x+xx*SIZE+(SIZE-1)-1,start_y+yy*SIZE+(SIZE-1)-1,12);
	xxx = start_x+(SIZE-8)/2;
	yyy = start_y+(SIZE-8)/2;
	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
		{	if ((bomb_xy[j][i]&0x01) == 0x01) putimage(xxx+j*SIZE,yyy+i*SIZE,bm,COPY_PUT);
			if (bomb_xy[j][i] == 0x20)
			{	setcolor(12);
				line(xxx+j*SIZE,yyy+i*SIZE,xxx+j*SIZE+7,yyy+i*SIZE+7);
				line(xxx+j*SIZE+7,yyy+i*SIZE,xxx+j*SIZE,yyy+i*SIZE+7);
			}
		}
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if (getpixel(xxx+xx*SIZE+i,yyy+yy*SIZE+j) == 7) putpixel(xxx+xx*SIZE+i,yyy+yy*SIZE+j,12);

}
/*-------------------------------------------------------------------*/
void _seek_ns_bomb(int xx,int yy)
{   static unsigned char buff_x[3000],buff_y[3000];
	int ns=0,ne=1;

	mouse_hide_cursor();
	if (___seek_ns_bomb(xx,yy) == 0)
	{   buff_x[0] = xx;
		buff_y[0] = yy;
		do
		{   __seek_ns_bomb(buff_x[ns],buff_y[ns],buff_x,buff_y,&ne);
		} while (++ns < ne);
	}
	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void __seek_ns_bomb(int xx,int yy,unsigned char *buff_x,unsigned char *buff_y,int *ne)
{	if (xx-1 >= 0 && yy-1 >= 0 && bomb_xy[xx-1][yy-1] == 0 && ___seek_ns_bomb(xx-1,yy-1) == 0)
	{   buff_x[*ne    ] = xx-1;
	    buff_y[(*ne)++] = yy-1;
	}
	if (             yy-1 >= 0 && bomb_xy[xx  ][yy-1] == 0 && ___seek_ns_bomb(xx  ,yy-1) == 0)
	{   buff_x[*ne    ] = xx;
	    buff_y[(*ne)++] = yy-1;
	}
	if (xx+1 < x  && yy-1 >= 0 && bomb_xy[xx+1][yy-1] == 0 && ___seek_ns_bomb(xx+1,yy-1) == 0)
	{   buff_x[*ne    ] = xx+1;
	    buff_y[(*ne)++] = yy-1;
	}
	if (xx-1 >= 0              && bomb_xy[xx-1][yy  ] == 0 && ___seek_ns_bomb(xx-1,yy  ) == 0)
	{   buff_x[*ne    ] = xx-1;
	    buff_y[(*ne)++] = yy;
	}
	if (xx+1 < x               && bomb_xy[xx+1][yy  ] == 0 && ___seek_ns_bomb(xx+1,yy  ) == 0)
	{   buff_x[*ne    ] = xx+1;
	    buff_y[(*ne)++] = yy;
	}
	if (xx-1 >= 0 && yy+1 <  y && bomb_xy[xx-1][yy+1] == 0 && ___seek_ns_bomb(xx-1,yy+1) == 0)
	{   buff_x[*ne    ] = xx-1;
	    buff_y[(*ne)++] = yy+1;
	}
	if (             yy+1 <  y && bomb_xy[xx  ][yy+1] == 0 && ___seek_ns_bomb(xx  ,yy+1) == 0)
	{   buff_x[*ne    ] = xx;
	    buff_y[(*ne)++] = yy+1;
	}
	if (xx+1 < x  && yy+1 <  y && bomb_xy[xx+1][yy+1] == 0 && ___seek_ns_bomb(xx+1,yy+1) == 0)
	{   buff_x[*ne    ] = xx+1;
	    buff_y[(*ne)++] = yy+1;
	}
}
/*-------------------------------------------------------------------*/
int ___seek_ns_bomb(int xx,int yy)
{	int ret=0;

	if (xx-1 >= 0              && (bomb_xy[xx-1][yy  ]&0x01) == 0x01) ret++;
	if (             yy+1 <  y && (bomb_xy[xx  ][yy+1]&0x01) == 0x01) ret++;
	if (xx+1 < x               && (bomb_xy[xx+1][yy  ]&0x01) == 0x01) ret++;
	if (             yy-1 >= 0 && (bomb_xy[xx  ][yy-1]&0x01) == 0x01) ret++;
	if (xx-1 >= 0 && yy-1 >= 0 && (bomb_xy[xx-1][yy-1]&0x01) == 0x01) ret++;
	if (xx-1 >= 0 && yy+1 <  y && (bomb_xy[xx-1][yy+1]&0x01) == 0x01) ret++;
	if (xx+1 < x  && yy+1 <  y && (bomb_xy[xx+1][yy+1]&0x01) == 0x01) ret++;
	if (xx+1 < x  && yy-1 >= 0 && (bomb_xy[xx+1][yy-1]&0x01) == 0x01) ret++;

	if (bomb_xy[xx][yy] == 0)
	{   bomb_xy[xx][yy] = 0x10;
		button_no_fill(start_x+(xx*SIZE),start_y+(yy*SIZE),start_x+(xx*SIZE)+(SIZE-1),start_y+(yy*SIZE)+(SIZE-1),DOWN);
		if (ret != 0)
			putimage(start_x+(xx*SIZE)+((SIZE-8)>>1),start_y+(yy*SIZE)+((SIZE-8)>>1),&numa[ret-1][0],COPY_PUT);
	}
	return ret;
}
/*-------------------------------------------------------------------*/
void flag(int m_x,int m_y)
{	int i,j, xx,yy,xxx,yyy, chk;

	xx = (m_x-start_x)/SIZE;
	yy = (m_y-start_y)/SIZE;
	if ((bomb_xy[xx][yy]&0x10) == 0x10) return;
	xxx = start_x+xx*SIZE+(SIZE-8)/2;
	yyy = start_y+yy*SIZE+(SIZE-8)/2;

	mouse_hide_cursor();
	if ((bomb_xy[xx][yy]&0x20) == 0x20)
	  {	if (mouse_check_click(start_x+xx*SIZE,start_y+yy*SIZE,
							  start_x+xx*SIZE+(SIZE-1),start_y+yy*SIZE+(SIZE-1)) == 1)
		{	putimage(xxx,yyy,que,COPY_PUT);
			bomb_xy[xx][yy] += 0x40-0x20;
			show_ns_bomb(-2);
		}
	  } else
	  { if ((bomb_xy[xx][yy]&0x40) == 0x40)
		  {	if (mouse_check_click(start_x+xx*SIZE,start_y+yy*SIZE,
								  start_x+xx*SIZE+(SIZE-1),start_y+yy*SIZE+(SIZE-1)) == 1)
			{	box_fill(xxx,yyy,xxx+7,yyy+7,7);
				bomb_xy[xx][yy] -= 0x40;
			}
		  } else
		  {	putimage(xxx,yyy,fl,COPY_PUT);

			if (mouse_check_click(start_x+xx*SIZE,start_y+yy*SIZE,
								  start_x+xx*SIZE+(SIZE-1),start_y+yy*SIZE+(SIZE-1)) == 0)
				box_fill(xxx,yyy,xxx+7,yyy+7,7);
			  else
			  { chk = show_ns_bomb(-3);
				if (chk < 0)
				  {	cputs("\a");
					box_fill(xxx,yyy,xxx+7,yyy+7,7);
				  } else
				  {	bomb_xy[xx][yy] += 0x20;
					if (CHK_TIME == 0)
					{	CHK_TIME = 1;
						show_time(-1);
					}
					if (chk == 0)
					{	for(i=0;i<y && chk==0;i++)
							for(j=0;j<x && chk==0;j++)
								if (bomb_xy[j][i] != 0x10 && bomb_xy[j][i] != 0x21) chk = 1;
						if (chk == 0) success();
					}
				  }
			  }
		  }
	  }
	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void set_bombs(void)
{	int xx,yy,i;

	for(xx=0;xx<x;xx++)
		for(yy=0;yy<y;yy++)
			bomb_xy[xx][yy] = 0x00;

	randomize();
	for(i=0;i<bombs;i++)
	{   do
		{	xx = random(x);
			yy = random(y);
		} while(bomb_xy[xx][yy] == 0x01);

		bomb_xy[xx][yy] = 0x01;
	}
}
/*-------------------------------------------------------------------*/
void push_new(int _key)
{	mouse_hide_cursor();
	button_down_s(start_x+x*SIZE-100,start_y-37,start_x+x*SIZE-63,start_y-16,7,YES);
	if (mouse_check_click(start_x+x*SIZE-100,start_y-37,start_x+x*SIZE-63,start_y-16) == 1 || _key == YES)
	{	CHK_TIME = 0;
		draw_panel_down(-1,-1,YES);
	}
	mouse_hide_cursor();
	button_up_s(start_x+x*SIZE-100,start_y-37,start_x+x*SIZE-63,start_y-16,7,YES);
	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
int push_set(void)
{   int m_x,m_y,m_l,m_r, chk=0,ret=0,i,j,
		old_mode=mode,old_x=x,old_y=y,old_bombs=bombs,old_size=SIZE;
	char *buff;

	mouse_hide_cursor();
	button_down_s(start_x,start_y-37,start_x+44,start_y-16,7,YES);
	if (mouse_check_click(start_x,start_y-37,start_x+44,start_y-16) == 1)
	{   buff = (char *)malloc(24804);
		get_image_x8(14,80,44,279,buff);

		box_fill_pattern(109,85,351,279,8,0,0xaa);
		box(104,80,348,276,1);
		button_up_s(105,81,347,275,7,NO);

		button_no_fill(114,90,338,109,DOWN);
		box_fill_pattern(115,91,337,108,9,7,0xaa);
		outtextxy_3d(180,96,"P L A Y E R",15,8);

		button_no_fill(114,110,260,206,DOWN);
		box_fill_pattern(115,111,259,205,8,7,0xaa);
		button_up_s(124,120,238,136,7,NO);
		outtextxy_3d(133,124,"Beginner",15,9);
		button_up_s(124,140,238,156,7,NO);
		outtextxy_3d(133,144,"Intermediate",15,9);
		button_up_s(124,160,238,176,7,NO);
		outtextxy_3d(133,164,"Expert",15,9);
		button_up_s(124,180,238,196,7,NO);
		outtextxy_3d(133,184,"Custom",15,9);

		button_no_fill(261,110,338,206,DOWN);
		box_fill_pattern(262,111,337,205,8,7,0xaa);
		setcolor(8);
		button_down_s(272,120,327,133,7,NO);
		outtextxy(281,123,"Width");
		button_no_fill(272,133,327,144,DOWN);
		button_down_s(272,146,327,159,7,NO);
		outtextxy(277,149,"Height");
		button_no_fill(272,159,327,170,DOWN);
		button_down_s(272,172,327,189,7,NO);
		outtextxy(281,175,"Mines");
		button_no_fill(272,185,327,196,DOWN);

		button_no_fill(114,220,166,240,UP);
		outtextxy_outline(128,226,"O K",15,9);
		button_no_fill(114,245,166,265,UP);
		outtextxy_outline(117,251,"Cancel",15,9);

		button_no_fill(170,220,222,240,UP);
		outtextxy_outline(181,226,"Save",15,4);
		button_no_fill(170,245,222,265,UP);
		outtextxy_outline(181,251,"Load",15,4);
		button_no_fill(230,220,282,240,UP);
		outtextxy_outline(238,226,"Score",15,8);
		button_no_fill(230,245,282,265,UP);
		outtextxy_outline(240,251,"Help",15,8);
//		button_no_fill(286,220,338,240,UP);
//		outtextxy_outline(296,226,"Quit",15,9);
		button_no_fill(286,245,338,265,UP);
		outtextxy_outline(296,251,"Quit",15,2);

		button_down_s(124,120+mode*20,238,136+mode*20,7,YES);
		_push_set_pr_data();

		mouse_show_cursor();
		do
		{ 	mouse_information(&m_l,&m_r,&m_x,&m_y);
			if (m_l == 0 && m_r == 0) continue;

			if (mode != 0 && m_x >= 124 && m_y >= 120 && m_x <= 238 && m_y <= 136) _push_set_player(0,NO);
			if (mode != 1 && m_x >= 124 && m_y >= 140 && m_x <= 238 && m_y <= 156) _push_set_player(1,NO);
			if (mode != 2 && m_x >= 124 && m_y >= 160 && m_x <= 238 && m_y <= 176) _push_set_player(2,NO);
			if (m_x >= 124 && m_y >= 180 && m_x <= 238 && m_y <= 196) _push_set_player(3,NO);

			if (m_x >= 114 && m_y >= 220 && m_x <= 166 && m_y <= 240)
			{   mouse_hide_cursor();
				button_down_s(114,220,166,240,7,YES);
				if (mouse_check_click(114,220,166,240) == 1) chk = 1;
				button_up_s(114,220,166,240,7,YES);
				mouse_show_cursor();
			}
			if (m_x >= 114 && m_y >= 245 && m_x <= 166 && m_y <= 265)
			{   mouse_hide_cursor();
				button_down_s(114,245,166,265,7,YES);
				if (mouse_check_click(114,245,166,265) == 1) chk = 1;
				button_up_s(114,245,166,265,7,YES);
				mouse_show_cursor();
				mode = old_mode;
				x = old_x;
				y = old_y;
				bombs = old_bombs;
				SIZE = old_size;
			}
			if (m_x >= 170 && m_y >= 220 && m_x <= 222 && m_y <= 240)
			{	mouse_hide_cursor();
				button_down_s(170,220,222,240,7,YES);
				if (mouse_check_click(170,220,222,240) == 1)
				{	mouse_show_cursor();
					save();
					mouse_hide_cursor();
				}
				button_up_s(170,220,222,240,7,YES);
				mouse_show_cursor();
			}
			if (m_x >= 170 && m_y >= 245 && m_x <= 222 && m_y <= 265)
			{	mouse_hide_cursor();
				button_down_s(170,245,222,265,7,YES);
				if (mouse_check_click(170,245,222,265) == 1)
				{	mouse_show_cursor();
					chk = load();
					mouse_hide_cursor();
				}
				button_up_s(170,245,222,265,7,YES);
				mouse_show_cursor();
			}
			if (m_x >= 230 && m_y >= 220 && m_x <= 282 && m_y <= 240)
			{   mouse_hide_cursor();
				button_down_s(230,220,282,240,7,YES);
				if (mouse_check_click(230,220,282,240) == 1)
				{	mouse_show_cursor();
					show_score(0,0);
					mouse_hide_cursor();
				}
				button_up_s(230,220,282,240,7,YES);
				mouse_show_cursor();
			}
			if (m_x >= 230 && m_y >= 245 && m_x <= 282 && m_y <= 265) _push_set_help(NO);
			if (m_x >= 286 && m_y >= 245 && m_x <= 338 && m_y <= 265)
			{   mouse_hide_cursor();
				button_down_s(286,245,338,265,7,YES);
				if (mouse_check_click(286,245,338,265) == 1) chk = ret = 1;
				button_up_s(286,245,338,265,7,YES);
				mouse_show_cursor();
				mode = old_mode;
				x = old_x;
				y = old_y;
				bombs = old_bombs;
			}
		} while(chk == 0);
		mouse_hide_cursor();

		put_image_x8(14,80,buff,COPY_PUT);
		free(buff);
	}
	button_up_s(start_x,start_y-37,start_x+44,start_y-16,7,YES);
	if (chk == 2 || (chk == 1 && (old_x != x || old_y != y || old_bombs != bombs)))
	{   mouse_show_cursor();
		draw_panel_up();
		draw_panel_down(-1,-1,YES);
		mouse_hide_cursor();
		CHK_TIME = 0;
	}
	if (chk == 3)
	{   mouse_show_cursor();
		draw_panel_up();
		draw_panel_down(-5,-4,NO);
		mouse_hide_cursor();

		for(i=0;i<y;i++)
			for(j=0;j<x;j++)
			{	if (bomb_xy[j][i] == 0x10)
				{	bomb_xy[j][i] = 0;
					___seek_ns_bomb(j,i);
				}
				if ((bomb_xy[j][i]&0x20) == 0x20)
					putimage(start_x+(j*SIZE)+((SIZE-8)>>1),start_y+(i*SIZE)+((SIZE-8)>>1),fl,COPY_PUT);
				if ((bomb_xy[j][i]&0x40) == 0x40)
					putimage(start_x+(j*SIZE)+((SIZE-8)>>1),start_y+(i*SIZE)+((SIZE-8)>>1),que,COPY_PUT);
			}
	}
	mouse_show_cursor();
	return ret;
}
/*-------------------------------------------------------------------*/
void _push_set_pr_data(void)
{   char num[5];

	setcolor(0);	        box_fill_pattern(273,134,326,143,8,7,0xaa);
	sprintf(num,"%04d",x);
	outtextxy(285,135,num);	box_fill_pattern(273,160,326,169,8,7,0xaa);
	sprintf(num,"%04d",y);
	outtextxy(285,161,num);	box_fill_pattern(273,186,326,195,8,7,0xaa);
	sprintf(num,"%04d",bombs);
	outtextxy(285,187,num);
}
/*-------------------------------------------------------------------*/
void _push_set_player(int new_mode,int _key)
{   mouse_hide_cursor();

	if ((new_mode != 3 || mode != 3) && _key == NO) button_down_s(124,120+new_mode*20,238,136+new_mode*20,7,YES);
	if (mouse_check_click(124,120+new_mode*20,238,136+new_mode*20) == 1 || _key == YES)
	  {	if ((new_mode != 3 || mode != 3) && _key == NO) button_up_s(124,120+mode*20,238,136+mode*20,7,YES);
		if (new_mode == 0) Beginner;
		if (new_mode == 1) Intermediate;
		if (new_mode == 2) Expert;
		if (new_mode == 3)
			if (_push_set_player_custom() == 1 && _key == NO)
			{ 	button_up_s(124,120+new_mode*20,238,136+new_mode*20,7,YES);
				button_down_s(124,120+mode*20,238,136+mode*20,7,YES);
			}

		if (_key == NO) _push_set_pr_data();
	  } else
		if ((new_mode != 3 || mode != 3) && _key == NO) button_up_s(124,120+new_mode*20,238,136+new_mode*20,7,YES);

	mouse_show_cursor();
}

/*-------------------------------------------------------------------*/
int _push_set_player_custom(void)
{	int old_x=x,old_y=y,old_bombs=bombs,old_size=SIZE, chk=0,ret=0, m_l,m_r,m_x,m_y;
	char* buff = (char *)malloc(14620);

	get_image_x8(31,150,59,275,buff);
	box_fill_pattern(246,156,468,275,8,0,0xaa);
	box(240,150,459,271,1);

	button_up_s(241,151,463,270,7,NO);
	button_no_fill(250,160,382,261,DOWN);
	box_fill_pattern(251,161,381,260,7,8,0xaa);
	outtextxy_outline(260,170,"Width",9,15);
	button_up_s(259,181,282,191,7,NO);
	button_up_s(285,181,308,191,7,NO);
	button_down_s(312,170,372,191,0,NO);
	draw_num(x,305,172);
	outtextxy_outline(260,200,"Height",9,15);
	button_up_s(259,211,282,221,7,NO);
	button_up_s(285,211,308,221,7,NO);
	button_down_s(312,200,372,221,0,NO);
	draw_num(y,305,202);
	outtextxy_outline(260,230,"Mines",9,15);
	button_up_s(259,241,282,251,7,NO);
	button_up_s(285,241,308,251,7,NO);
	button_down_s(312,230,372,251,0,NO);
	draw_num(bombs,305,232);

	setcolor(0);
	outtextxy(267,183,"+");
	outtextxy(293,183,"-");
	outtextxy(267,213,"+");
	outtextxy(293,213,"-");
	outtextxy(267,243,"+");
	outtextxy(293,243,"-");

	button_no_fill(397,160,454,201,UP);
	outtextxy_3d(410,176,"O  K",15,9);
	button_no_fill(397,220,454,261,UP);
	outtextxy_3d(402,236,"Cancel",15,9);

	mouse_show_cursor();
	do
	{ 	mouse_information(&m_l,&m_r,&m_x,&m_y);
		if (m_l == 0 && m_r == 0) continue;

		if (x < 62 && m_x >= 259 && m_y >= 181 && m_x <= 282 && m_y <= 191)
		{	mouse_hide_cursor();
			button_down_s(259,181,282,191,7,YES);
			draw_num(++x,305,172);
			mouse_show_cursor();
			delay(70);
			mouse_hide_cursor();
			button_up_s(259,181,282,191,7,YES);
			mouse_show_cursor();
		}
		if (x > 11 && m_x >= 285 && m_y >= 181 && m_x <= 308 && m_y <= 191)
		{   if (x <= 21 && y > 35) continue;
			if (x <= 18 && y > 26) continue;
			if (x <= 13 && y > 20) continue;
			mouse_hide_cursor();
			button_down_s(285,181,308,191,7,YES);
			draw_num(--x,305,172);
			mouse_show_cursor();
			delay(70);
			mouse_hide_cursor();
			button_up_s(285,181,308,191,7,YES);
			mouse_show_cursor();
		}
		if (y < 42 && m_x >= 259 && m_y >= 211 && m_x <= 282 && m_y <= 221)
		{	if (y >= 35 && x < 21) continue;
			if (y >= 26 && x < 18) continue;
			if (y >= 20 && x < 13) continue;
			mouse_hide_cursor();
			button_down_s(259,211,282,221,7,YES);
			draw_num(++y,305,202);
			mouse_show_cursor();
			delay(70);
			mouse_hide_cursor();
			button_up_s(259,211,282,221,7,YES);
			mouse_show_cursor();
		}
		if (y >  1 && m_x >= 285 && m_y >= 211 && m_x <= 308 && m_y <= 221)
		{   mouse_hide_cursor();
			button_down_s(285,211,308,221,7,YES);
			draw_num(--y,305,202);
			mouse_show_cursor();
			delay(70);
			mouse_hide_cursor();
			button_up_s(285,211,308,221,7,YES);
			mouse_show_cursor();
		}
		if (bombs < x*y && m_x >= 259 && m_y >= 241 && m_x <= 282 && m_y <= 251)
		{   mouse_hide_cursor();
			button_down_s(259,241,282,251,7,YES);
			draw_num(++bombs,305,232);
			mouse_show_cursor();
			delay(30);
			mouse_hide_cursor();
			button_up_s(259,241,282,251,7,YES);
			mouse_show_cursor();
		}
		if (bombs >   1 && m_x >= 285 && m_y >= 241 && m_x <= 308 && m_y <= 251)
		{   mouse_hide_cursor();
			button_down_s(285,241,308,251,7,YES);
			draw_num(--bombs,305,232);
			mouse_show_cursor();
			delay(30);
			mouse_hide_cursor();
			button_up_s(285,241,308,251,7,YES);
			mouse_show_cursor();
		}
		if (bombs > x*y)
		{	bombs = x*y;
			mouse_hide_cursor();
			draw_num(bombs,305,232);
			mouse_show_cursor();
		}

		if (m_x >= 397 && m_y >= 160 && m_x <= 454 && m_y <= 201)
		{   mouse_hide_cursor();
			button_down_s(397,160,454,201,7,YES);
			if (mouse_check_click(397,160,454,201) == 1)
			{	SIZE = 20;
				if (x > 30 || y > 20) SIZE = 16;
				if (x > 36 || y > 26) SIZE = 12;
				if (x > 52 || y > 35) SIZE = 10;
				chk = 1;
				mode = 3;
			}
			button_up_s(397,160,454,201,7,YES);
			mouse_show_cursor();
		}
		if (m_x >= 397 && m_y >= 220 && m_x <= 454 && m_y <= 261)
		{   mouse_hide_cursor();
			button_down_s(397,220,454,261,7,YES);
			if (mouse_check_click(397,220,454,261) == 1)
			{	x = old_x;
				y = old_y;
				bombs = old_bombs;
				SIZE = old_size;
				chk = ret = 1;
			}
			button_up_s(397,220,454,261,7,YES);
			mouse_show_cursor();
		}

	} while(chk == 0);

	mouse_hide_cursor();
	put_image_x8(31,150,buff,COPY_PUT);
	free(buff);
	return ret;
}

/*-------------------------------------------------------------------*/
void _push_set_help(int _key)
{	char *buff;
	int m_l,m_r,m_x,m_y;

	mouse_hide_cursor();
	if (_key == NO)	button_down_s(230,245,282,265,7,YES);
	if (mouse_check_click(230,245,282,265) == 1 || _key == YES)
	{	buff = (char *)malloc(24184);

		get_image_x8(23,165,61,319,buff);
		box_fill_pattern(180,169,479,319,8,0,0xaa);
		box(176,165,475,315,1);
		button_up_s(177,166,474,293,7,NO);
		button_up_s(177,294,474,314,7,NO);

		outtextxy_outline(200,175,"F1      :",15,8);
		outtextxy_3d(290,175,"Help",15,9);
		outtextxy_outline(200,185,"Alt + X :",15,8);
		outtextxy_3d(290,185,"Quit",15,9);
		outtextxy_outline(200,195,"Alt + N :",15,8);
		outtextxy_3d(290,195,"New Game",15,9);
		outtextxy_outline(200,205,"Alt + B :",15,8);
		outtextxy_3d(290,205,"Setting Beginner",15,9);
		outtextxy_outline(200,215,"Alt + I :",15,8);
		outtextxy_3d(290,215,"Setting Intermediate",15,9);
		outtextxy_outline(200,225,"Alt + E :",15,8);
		outtextxy_3d(290,225,"Setting Expert",15,9);
		outtextxy_outline(200,235,"Alt + C :",15,8);
		outtextxy_3d(290,235,"Setting Custom",15,9);
		outtextxy_outline(200,245,"Alt + S :",15,8);
		outtextxy_3d(290,245,"Save Game",15,9);
		outtextxy_outline(200,255,"Alt + L :",15,8);
		outtextxy_3d(290,255,"Load Game",15,9);
		outtextxy_outline(200,265,"Alt + R :",15,8);
		outtextxy_3d(290,265,"Show Score",15,9);

		setcolor(8);
		outtextxy(230,280,"( Push Mouse Button... )");
		outtextxy_outline(191,300,"Lee Kyoung Su (HiTel ID :  ttagai)",7,8);

		mouse_show_cursor();
		for( ; ; )
		{	mouse_information(&m_l,&m_r,&m_x,&m_y);
			if (m_l == 1 || m_r == 1)
			{	mouse_check_click(0,0,639,479);
				break;
			}
		}
		mouse_hide_cursor();
		put_image_x8(23,165,buff,COPY_PUT);
		free(buff);
	}
	if (_key == NO)	button_up_s(230,245,282,265,7,YES);
	mouse_show_cursor();
}
/*-------------------------------------------------------------------*/
void success(void)
{	char *buff,*buff2;
	int m_l,m_r,m_x,m_y, m_cur,_time;
	struct sc *score_b[3],*score_i[3],*score_e[3];

	_time = show_time(-3);
	m_cur = mouse_get_cursor();
	cputs("\a");
	buff = (char *)malloc(12140);

	if (m_cur == 1) mouse_hide_cursor();
	get_image_x8(19,230,59,303,buff);
	box_fill_pattern(163,233,471,303,8,0,0xaa);
	box(160,230,468,300,1);
	button_no_fill(161,231,467,299,UP);
	box_fill_pattern(162,232,466,298,7,9,0xaa);
	outtextxy_outline(179,255,"C O N G R A T U L A T I O N  !!!!!",15,8);
	setcolor(8);
	outtextxy(232,280,"Push Mouse Button....");

	mouse_show_cursor();
	for( ; ; )
	{	mouse_information(&m_l,&m_r,&m_x,&m_y);
		if (m_l == 1 || m_r == 1)
		{	mouse_check_click(0,0,639,479);
			break;
		}
	}

	if (mode == 0 || mode == 1 || mode == 2) show_score(1,_time);

	mouse_hide_cursor();
	put_image_x8(19,230,buff,COPY_PUT);
	free(buff);

	CHK_TIME = 0;
	draw_panel_down(-1,-1,YES);
	if (m_cur == 0) mouse_hide_cursor();
}

/*-------------------------------------------------------------------*/
void show_score(int set, int _time)	// 0 => show score,  1 => write score
{       int i,file,m_l,m_r,m_x,m_y,num;
	long flen;
	char header[14], *buff;
	struct sc *score_b[3],*score_i[3],*score_e[3];

	for(i=0;i<3;i++)
	{	score_b[i] = (struct sc *)malloc(32);
		score_i[i] = (struct sc *)malloc(32);
		score_e[i] = (struct sc *)malloc(32);
	}
	file = open("miner.scr",0x8001);
	if (file != -1)
	{	flen = filelength(file);
		if (flen != 302L)
		  {	file = -1;
			close(file);
		  }
		  else
		  {	read(file,header,14);
			for(i=0;i<3;i++)
			{	read(file,(void *)score_b[i],32);
				read(file,(void *)score_i[i],32);
				read(file,(void *)score_e[i],32);
			}
			close(file);
			if (strncmp("LEE MINER V1.0",header,14) != 0) file = -1;
		  }
	}
	if (file == -1)
	{	for(i=0;i<3;i++)
		{	strcpy(score_b[i]->name,"No Name");
			strcpy(score_i[i]->name,"No Name");
			strcpy(score_e[i]->name,"No Name");

			score_b[i]->time = score_i[i]->time = score_e[i]->time = 9999;
		}
	}

	if (set == 1)
	{	if (mode == 0)
		{   num = _show_score(score_b[0],score_b[1],score_b[2],_time);
			if (num == -1)
			{	for(i=0;i<3;i++)
				{	free(score_b[i]);
					free(score_i[i]);
					free(score_e[i]);
				}
				return;
			}
		}
		if (mode == 1)
		{   num = _show_score(score_i[0],score_i[1],score_i[2],_time);
			if (num == -1)
			{	for(i=0;i<3;i++)
				{	free(score_b[i]);
					free(score_i[i]);
					free(score_e[i]);
				}
				return;
			}
		}
		if (mode == 2)
		{   num = _show_score(score_e[0],score_e[1],score_e[2],_time);
			if (num == -1)
			{	for(i=0;i<3;i++)
				{	free(score_b[i]);
					free(score_i[i]);
					free(score_e[i]);
				}
				return;
			}
		}
	}
	mouse_hide_cursor();

	if (set == 0)
	  {	buff = (char *)malloc(40236);
		get_image_x8(24,149,70,362,buff);
		box_fill_pattern(189,154,556,362,0,8,0xaa);
		box(184,149,553,359,9);
		button_up_s(185,150,552,358,7,NO);
	  } else
	  {	buff = (char *)malloc(35724);
		get_image_x8(24,149,70,338,buff);
		box_fill_pattern(189,154,556,338,0,8,0xaa);
		box(184,149,553,335,9);
		button_up_s(185,150,552,334,7,NO);
	  }

	outtextxy_outline(300,156,"S   C   O   R   E",9,15);
	outtextxy_outline(194,170,"Beginner     :",15,8);
	button_no_fill(200,181,492,217,DOWN);
	button_no_fill(494,181,542,217,DOWN);
	outtextxy_outline(194,225,"Intermediate :",15,8);
	button_no_fill(200,236,492,272,DOWN);
	button_no_fill(494,236,542,272,DOWN);
	outtextxy_outline(194,280,"Expert       :",15,8);
	button_no_fill(200,291,492,327,DOWN);
	button_no_fill(494,291,542,327,DOWN);

	header[1] = '.';
	header[2] = 0;
	for(i=0;i<3;i++)
	{	setcolor(1);
		header[0] = i+'1';
		outtextxy(210,183+i*12,header);
		outtextxy(210,238+i*12,header);
		outtextxy(210,293+i*12,header);

		setcolor(0);
		outtextxy(234,183+i*12,score_b[i]->name);
		outtextxy(234,238+i*12,score_i[i]->name);
		outtextxy(234,293+i*12,score_e[i]->name);

		setcolor(15);
		sprintf(&header[3],"%04d",score_b[i]->time);
		outtextxy(502,183+i*12,&header[3]);
		sprintf(&header[3],"%04d",score_i[i]->time);
		outtextxy(502,238+i*12,&header[3]);
		sprintf(&header[3],"%04d",score_e[i]->time);
		outtextxy(502,293+i*12,&header[3]);
	}

	if (set == 0)
	  {	outtextxy_3d(293,340,"Push Mouse Button...",15,8);

		mouse_show_cursor();
		for( ; ; )
		{	mouse_information(&m_l,&m_r,&m_x,&m_y);
			if (m_l == 1 || m_r == 1)
			{	mouse_check_click(0,0,639,479);
				break;
			}
		}
	  } else
	  {	setcolor(8);
		if (mode == 0)
		{	sprintf(&header[3],"%04d",score_b[num]->time);
			outtextxy(502,183+num*12,&header[3]);
		}
		if (mode == 1)
		{	sprintf(&header[3],"%04d",score_i[num]->time);
			outtextxy(502,238+num*12,&header[3]);
		}
		if (mode == 2)
		{	sprintf(&header[3],"%04d",score_e[num]->time);
			outtextxy(502,293+num*12,&header[3]);
		}

		if (mode == 0) _gets_eng(234,183+num*12,15,7,29,score_b[num]->name);
		if (mode == 1) _gets_eng(234,238+num*12,15,7,29,score_i[num]->name);
		if (mode == 2) _gets_eng(234,293+num*12,15,7,29,score_e[num]->name);

		unlink("miner.scr");
		file = open("miner.scr",0x8100, 0x0080);
		write(file,"LEE MINER V1.0",14);
		for(i=0;i<3;i++)
		{	write(file,(void *)score_b[i],32);
			write(file,(void *)score_i[i],32);
			write(file,(void *)score_e[i],32);
		}
		close(file);
	  }

	mouse_hide_cursor();

	put_image_x8(24,149,buff,COPY_PUT);
	mouse_show_cursor();
	free(buff);

	for(i=0;i<3;i++)
	{	free(score_b[i]);
		free(score_i[i]);
		free(score_e[i]);
	}
}

/*-------------------------------------------------------------------*/
int _show_score(struct sc *score_1,struct sc *score_2,struct sc *score_3,int _time)
{   int num;

	if (score_1->time > _time)
	  {	num = 0;
		strcpy(score_3->name,score_2->name); score_3->time = score_2->time;
		strcpy(score_2->name,score_1->name); score_2->time = score_1->time;
		score_1->name[0] = 0;
		score_1->time = _time;
	  } else
		if (score_2->time > _time)
		  {	num = 1;
			strcpy(score_3->name,score_2->name);
			score_3->time = score_2->time;
			score_2->name[0] = 0;
			score_2->time = _time;
		  } else
		  if (score_3->time > _time)
			{	num = 2;
				score_3->name[0] = 0;
				score_3->time = _time;
			}
			else
				num = -1;

	return num;
}
/*-------------------------------------------------------------------*/
void outtextxy_3d(int x,int y,char *str,int color,int ucolor)
{	int old_color = getcolor();
	setcolor(ucolor);
	outtextxy(x+1,y+1,str);
	setcolor(color);
	outtextxy(x,y,str);
	setcolor(old_color);
}
/*-------------------------------------------------------------------*/
void outtextxy_outline(int x,int y,char *str,int color,int out_color)
{	int old_color = getcolor();
	setcolor(out_color);
	outtextxy(x-1,y,str);
	outtextxy(x+1,y,str);
	outtextxy(x,y-1,str);
	outtextxy(x,y+1,str);
	setcolor(color);
	outtextxy(x,y,str);
	setcolor(old_color);
}

/*-------------------------------------------------------------------*/
unsigned x_getch(void)
{   unsigned hl;

	if (kbhit() == 0) return 0;
	hl = getch();
	hl += (hl == 0)? ((unsigned)getch()<<8) : 0;
	return hl;
}

/*------------------------------------------------------------------------------------*/
void clear_key_buff(void){ bdos(0x0c,0,2); }

/*------------------------------------------------------------------------------------*/
int _gets_eng(int xx, int yy, int color, int bkcolor, int len, char *buff)
{	int m_x,m_y,m_l,m_r;
	int chk=0, num=0,cur=0,ret,ins=1,tchk=0;
	long ti1=0,ti2;
	char bu[36];
	static const char chkstr[] =
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890!@#$%^&*()-=\\_+|`~,./<>?[]{}";

	if (*buff != 0) num = strlen(buff);
	mouse_hide_cursor();
	box_fill(xx,yy,xx+len*8+7,yy+7,bkcolor);
	setcolor(color);
	outtextxy(xx,yy,buff);
	mouse_show_cursor();

	do
	{	ti2 = clock();
		if (ti1+6 < ti2)
		{	ti1  = ti2;
			tchk = (tchk == 0)? 1: 0;
			mouse_hide_cursor();
			getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
			putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
			mouse_show_cursor();
		}
		mouse_information(&m_l,&m_r,&m_x,&m_y);
		chk = x_getch();
		if (chk != 0)
		{   if (tchk == 1)
			{	tchk = 0;
				getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
				putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
			}
			if (chk <= 0x00ff && strchr(chkstr,(char)chk) != 0L)
			{	if (ins == 1)
				  {	if (num < len)
					{   strcpy(bu,buff+cur);
					    strcpy(buff+cur+1,bu);
					    *(buff+cur) = (char)chk;
					    cur++;
					    num++;
					}
				  } else
				  {	if (cur == num && num < len)
					{   strcpy(bu,buff+cur);
					    strcpy(buff+cur+1,bu);
					    *(buff+cur) = (char)chk;
					    num++;
					}
					if (cur < len)
					{   *(buff+cur) = (char)chk;
					    cur++;
					}
				  }
			}
			if (chk == BS && cur > 0)
			{   strcpy(buff+cur-1,buff+cur);
				cur--;
				num--;
			}
			if (chk == HOME && cur > 0)    cur = 0;
			if (chk == END  && cur <= num) cur = num;
			if (chk == DEL  && cur <= num)
			{	strcpy(buff+cur,buff+cur+1);
				if (num > 0) num--;
			}
			if (chk == INS)  ins = (ins == 0)? 1: 0;
			if (chk == LEFT  && cur > 0)   cur--;
			if (chk == RIGHT && cur < num) cur++;
			if (chk == ENTER)
			{	ret = 1; break;
			}
			if (chk == ESC)
			{	ret = 0; break;
			}

			mouse_hide_cursor();
			if (chk!=INS && chk!=HOME && chk!=END && chk!=LEFT && chk!=RIGHT)
			{	box_fill(xx,yy,xx+(len<<3)+7,yy+7,bkcolor);
				outtextxy(xx,yy,buff);
			}
			tchk = 1;
			getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
			putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
			mouse_show_cursor();
		}
		if (m_l == 0 && m_r == 0) continue;

		if (m_x >= xx && m_y >= yy && m_x <= xx+(len<<3)+7 && m_y <= yy+8)
			if (mouse_check_click(xx,yy,xx+(len<<3)+7,yy+8) == 1)
			{	if (tchk == 1)
				{	tchk = 0;
					mouse_hide_cursor();
					getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
					putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
					mouse_show_cursor();
				}
				ret = 1;
				break;
			}
	} while (1);

	return ret;
}

/*-------------------------------------------------------------------*/
void save(void)
{   int i,file, m_x,m_y,m_l,m_r,ret=-1,num=-1,tmp;
	char header[14], *buff,name_buff[30];
	struct ss *save_buff[5];

	for(i=0;i<5;i++) save_buff[i] = (struct ss *)malloc(2644);
	_save_load(save_buff);

	mouse_hide_cursor();
	buff = (char *)malloc(18724);
	get_image_x8(10,186,54,289,buff);
	box_fill_pattern(82,190,431,289,0,8,0xaa);
	box(78,186,427,285,9);
	button_up_s(79,187,426,284,7,NO);
	outtextxy_outline(152,193,"S  A  V  E        G a m e",15,9);
	button_no_fill(364,208,417,238,UP);
	outtextxy_3d(375,219,"O  K",15,9);
	button_no_fill(364,246,417,276,UP);
	outtextxy_3d(367,257,"Cancel",15,9);

	header[1] = 0;
	for (i=0;i<5;i++)
	{	button_no_fill(88,207+i*14,110,220+i*14,UP);
		setcolor(8);
		header[0] = i+'1';
		outtextxy(96,210+i*14,header);
		button_no_fill(114,207+i*14,357,220+i*14,DOWN);
		setcolor(15);
		outtextxy(116,210+i*14,save_buff[i]->name);
	}

	mouse_show_cursor();
	do
	{ 	mouse_information(&m_l,&m_r,&m_x,&m_y);
		if (m_l == 0 && m_r == 0) continue;

		for(i=0;i<5;i++)
			if (m_x >= 88 && m_y >= 207+i*14 && m_x <= 110 && m_y <= 220+i*14)
			{	mouse_hide_cursor();
				if (num != i) button_no_fill(88,207+i*14,110,220+i*14,DOWN);
				if (mouse_check_click(88,207+i*14,110,220+i*14) == 1)
				  { strcpy(name_buff,save_buff[i]->name);
					mouse_show_cursor();
					tmp = _gets_eng(116,210+i*14,0,7,29,name_buff);
					mouse_hide_cursor();
					if (tmp == 1)
					  { if (num != -1 && num != i)
						{	button_no_fill(88,207+num*14,110,220+num*14,UP);
								setcolor(15);
							outtextxy(116,210+num*14,save_buff[num]->name);
						}
						strcpy(save_buff[i]->name,name_buff);
						num = i;
					  }
					  else
					  { box_fill(115,208+i*14,356,219+i*14,7);
						setcolor((num == i)? 0:15);
						outtextxy(116,210+i*14,save_buff[i]->name);
						if (num != i) button_no_fill(88,207+i*14,110,220+i*14,UP);
					  }
				  }
				  else
					if (num != i) button_no_fill(88,207+i*14,110,220+i*14,UP);
				mouse_show_cursor();
			}
		if (m_x >= 364 && m_y >= 208 && m_x <= 417 && m_y <= 238)
		{   mouse_hide_cursor();
			button_down_s(364,208,417,238,7,YES);
			if (mouse_check_click(364,208,417,238) == 1) ret = 1;
			button_up_s(364,208,417,238,7,YES);
			mouse_show_cursor();
		}
		if (m_x >= 364 && m_y >= 246 && m_x <= 417 && m_y <= 276)
		{   mouse_hide_cursor();
			button_down_s(364,246,417,276,7,YES);
			if (mouse_check_click(364,246,417,276) == 1) ret = 0;
			button_up_s(364,246,417,276,7,YES);
			mouse_show_cursor();
		}
	} while(ret == -1);

	if (ret == 1)
	{   save_buff[num]->size     = SIZE;
		save_buff[num]->mode     = mode;
		save_buff[num]->x	     = x;
		save_buff[num]->y        = y;
		memcpy((void *)save_buff[num]->board,(void *)bomb_xy,62*42);
		save_buff[num]->bombs    = bombs;

		save_buff[num]->time     = show_time(-3);
		save_buff[num]->ns_bombs = show_ns_bomb(-4);

		unlink("miner.sav");
		file = open("miner.sav",0x8100, 0x0080);
		write(file,"LEE MINER V1.0",14);
		for(i=0;i<5;i++)
			write(file,(void *)save_buff[i],2644);
		close(file);
	}

	mouse_hide_cursor();
	put_image_x8(10,186,buff,COPY_PUT);
	mouse_show_cursor();
	free (buff);

	for(i=0;i<5;i++)
		free((void *)save_buff[i]);
}

/*-------------------------------------------------------------------*/
int load(void)
{   int i, m_x,m_y,m_l,m_r,ret=-1,num=-1;
	char header[14], *buff;
	struct ss *load_buff[5];

	for(i=0;i<5;i++)
		load_buff[i] = (struct ss *)malloc(2644);
	_save_load(load_buff);

	mouse_hide_cursor();
	buff = (char *)malloc(18724);
	get_image_x8(27,236,71,339,buff);
	box_fill_pattern(212,240,561,339,0,8,0xaa);
	box(208,236,557,335,1);
	button_up_s(209,237,556,334,7,NO);
	outtextxy_outline(282,243,"L  O  A  D        G a m e",15,1);
	button_no_fill(494,258,547,288,UP);
	outtextxy_3d(505,269,"O  K",15,1);
	button_no_fill(494,296,547,326,UP);
	outtextxy_3d(497,307,"Cancel",15,1);

	header[1] = 0;
	for (i=0;i<5;i++)
	{	button_no_fill(218,257+i*14,240,270+i*14,UP);
		setcolor(8);
		header[0] = i+'1';
		outtextxy(226,260+i*14,header);
		button_no_fill(244,257+i*14,487,270+i*14,DOWN);
		setcolor(15);
		outtextxy(246,260+i*14,load_buff[i]->name);
	}

	mouse_show_cursor();
	do
	{ 	mouse_information(&m_l,&m_r,&m_x,&m_y);
		if (m_l == 0 && m_r == 0) continue;

		for(i=0;i<5;i++)
			if (m_x >= 218 && m_y >= 257+i*14 && m_x <= 240 && m_y <= 270+i*14)
			{	mouse_hide_cursor();
				if (num != i) button_no_fill(218,257+i*14,240,270+i*14,DOWN);
				if (mouse_check_click(218,257+i*14,240,270+i*14) == 1)
				  {	if (num != -1 && num != i)
					{   setcolor(15);
						outtextxy(246,260+num*14,load_buff[num]->name);
						button_no_fill(218,257+num*14,240,270+num*14,UP);
					}
					num = i;
					setcolor(0);
					outtextxy(246,260+i*14,load_buff[i]->name);
				  }
				  else
					if (num != i) button_no_fill(218,257+i*14,240,270+i*14,UP);
				mouse_show_cursor();
			}
		if (m_x >= 494 && m_y >= 258 && m_x <= 547 && m_y <= 288)
		{   mouse_hide_cursor();
			button_down_s(494,258,547,288,7,YES);
			if (mouse_check_click(494,258,547,288) == 1) ret = 2;
			button_up_s(494,258,547,288,7,YES);
			mouse_show_cursor();
		}
		if (m_x >= 494 && m_y >= 296 && m_x <= 547 && m_y <= 326)
		{   mouse_hide_cursor();
			button_down_s(494,296,547,326,7,YES);
			if (mouse_check_click(494,296,547,326) == 1) ret = 0;
			button_up_s(494,296,547,326,7,YES);
			mouse_show_cursor();
		}
	} while(ret == -1);

	if (ret == 2)
	{   if (num != -1)
		  {	if (load_buff[num]->size == -1)
				_push_set_player(0,YES);
			  else
			  {	SIZE  = load_buff[num]->size;
				mode  = load_buff[num]->mode;
				x     = load_buff[num]->x;
				y     = load_buff[num]->y;
				memcpy((void *)bomb_xy,(void *)load_buff[num]->board,62*42);
				bombs = load_buff[num]->bombs;
				show_time(load_buff[num]->time);
				show_ns_bomb(load_buff[num]->ns_bombs);
				ret = 3;
			  }
			CHK_TIME = 1;
		  }
		  else
			ret = 0;
	}

	mouse_hide_cursor();
	put_image_x8(27,236,buff,COPY_PUT);
	mouse_show_cursor();
	free(buff);
	for(i=0;i<5;i++)
		free((void *)load_buff[i]);
	return ret;
}

/*-------------------------------------------------------------------*/
void _save_load(struct ss *buff[5])
{	int file,i;
	long flen;
	char header[14];

	file = open("miner.sav",0x8001);
	if (file != -1)
	{	flen = filelength(file);
		if (flen != 13234L)
		  {	file = -1;
			close(file);
		  }
		  else
		  {	read(file,header,14);
			for(i=0;i<5;i++)
				read(file,(void *)buff[i],2644);
			close(file);
			if (strncmp("LEE MINER V1.0",header,14) != 0) file = -1;
		  }
	}
	if (file == -1)
	{	for(i=0;i<5;i++)
		{	strcpy(buff[i]->name,"No Name");
			buff[i]->size     = -1;
		}
	}
}

/*-------------------------------------------------------------------*/

void get_dac(unsigned char *rgb)
{   unsigned char *prgb;

	prgb = rgb+256*3;
	outportb(0x03c7,0x00);
	for( ;rgb<prgb; )
		*rgb++ = inportb(0x03c9);
	rgb -= 256*3;
}

void set_dac(unsigned char *rgb)
{   unsigned char *prgb;

	prgb = rgb+256*3;
	outportb(0x03c8,0x00);
	for( ;rgb<prgb; )
		outportb(0x03c9,*rgb++);
	rgb -= 256*3;
}

void fade_out(void)
{	unsigned char *rgb;
	int i,j, *rgb2,*prgb2;

	rgb  = (unsigned char *)malloc(256*3);
	rgb2 = (int *)malloc((256*2)*3);
	prgb2 = rgb2+256*3;

	get_dac(rgb);
	for( ;rgb2<prgb2; )
		*rgb2++ = (unsigned)(*rgb++)<<6;
	rgb  -= 256*3;
	rgb2 -= 256*3;

	outportb(0x03c8,0x00);
	for(i=0;i<64/4;i++)
	{   for( ;rgb2<prgb2;rgb++ )
			*rgb2++ -= (unsigned)*rgb+(unsigned)*rgb+(unsigned)*rgb+(unsigned)*rgb;
		rgb  -= 256*3;
		rgb2 -= 256*3;

		for( ;rgb2<prgb2; )
		{	{	asm mov dx, 0x03da
			go:	asm in  al, dx
				asm and al, 0x08
				asm jz  go
			}
			for(j=0;j<64*3;j++)
				outportb(0x03c9,(unsigned char)((unsigned)(*rgb2++)>>6));
		}
		rgb2 -= 256*3;
	}

	free(rgb);
	free(rgb2);
}

void fade_in(unsigned char *rgb)
{	int i,j, *rgb2,*prgb2;

	rgb2 = (int *)malloc((256*2)*3);
	prgb2 = rgb2+256*3;

	for( ;rgb2<prgb2; )
		*rgb2++ = 0;
	rgb2 -= 256*3;

	outportb(0x03c8,0x00);
	for(i=0;i<64/4;i++)
	{   for( ;rgb2<prgb2;rgb++)
			*rgb2++ += (unsigned)*rgb+(unsigned)*rgb+(unsigned)*rgb+(unsigned)*rgb;
		rgb  -= 256*3;
		rgb2 -= 256*3;

		for( ;rgb2<prgb2; )
		{	{	asm mov dx, 0x03da
			go:	asm in  al, dx
				asm and al, 0x08
				asm jz  go
			}
			for(j=0;j<64*3;j++)
				outportb(0x03c9,(unsigned char)((unsigned)(*rgb2++)>>6));
		}
		rgb2 -= 256*3;
	}
	free(rgb2);
	set_dac(rgb);
}

