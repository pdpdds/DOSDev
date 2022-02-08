#include <io.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

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


int _gets_eng(int xx,int yy,int color,int bkcolor,int len,char *buff);

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
{	int m_x,m_y,m_l,m_r, tcolor;
	int chk=0, num=0,cur=0,ret,ins=1,tchk=0;
	long ti1=0,ti2;
	char bu[36];
	static const char chkstr[] =
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890!@#$%^&*()-=\\_+|`~,./<>?[]{}";

	if (*buff != 0) num = strlen(buff);
//	mouse_hide_cursor();
//	box_fill(xx,yy,xx+len*8+7,yy+7,bkcolor);
	setcolor(color); tcolor = color;
	outtextxy(xx,yy,buff);
//	mouse_show_cursor();
	do
	{	ti2 = clock();
		if (ti1+6 < ti2)
		{	ti1  = ti2;
			tchk = (tchk == 0)? 1: 0;
//			mouse_hide_cursor();
			getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
			putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
//			mouse_show_cursor();
		}
//		mouse_information(&m_l,&m_r,&m_x,&m_y);
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

//			mouse_hide_cursor();
			if (chk!=INS && chk!=HOME && chk!=END && chk!=LEFT && chk!=RIGHT)
			{
//				box_fill(xx,yy,xx+(len<<3)+7,yy+7,bkcolor);
				outtextxy(xx,yy,buff);
			}
			tchk = 1;
			getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
			putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
//			mouse_show_cursor();
		}
#if 0
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
#endif
#if 0
		if (tchk == 1)
		{	tchk = 0;
	//		mouse_hide_cursor();
			getimage(xx+(cur<<3),yy+(ins<<2),xx+(cur<<3)+7,yy+7,bu);
			putimage(xx+(cur<<3),yy+(ins<<2),bu,NOT_PUT);
	//		mouse_show_cursor();
		}
#endif

	} while (1);

	return ret;
}

extern void EGAVGA_driver(void);

int main(void)
{
	int gd=VGA,gm=VGAHI, m_x,m_y,m_l,m_r,chk=0,tmp,i,j;
	unsigned char *rgb,*rgb2;;
	char *text_buff;
	struct text_info t_i;
	const int BUF_SIZE = 30;
	char name_buff[BUF_SIZE] = "-- Text Input Test --\n";

	text_buff = (char *)         malloc(4000);
	      rgb = (unsigned char *)malloc(256*3);
	gettext(1,1,80,25,text_buff);
	gettextinfo(&t_i);
	detectgraph(&gd,&gm);	// check VGA
	if (gd != VGA)
	{	cputs("Error : This program must be run in VGA.\n\r\a");
		return(1);
	}

	registerbgidriver(EGAVGA_driver);
	initgraph(&gd,&gm,"");

get_dac(rgb);
rgb2 = (unsigned char *)malloc(256*3);
for(i=0;i<256*3;i++) rgb2[i] = 0;

//set_dac(rgb2);
free(rgb2);
fade_in(rgb);

     for(i=0;i<5;i++)
	tmp = _gets_eng(116, 210 + i * 14, 1, 3, (BUF_SIZE -1), name_buff);

fade_out();
free(rgb);
//box_fill_x8(1,1,80,60,0);
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
