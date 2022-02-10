#define _INCLUDE

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <mem.h>
#include <alloc.h>
#include <dos.h>
#include <dir.h>
#include <stdarg.h>
#include "est.h"

#include "egi.p"
#include "graph.h"
#include "setpd.h"
#include "sethan.h"

void error2(BYTE *str);


extern BYTE first_sound[8][19][32];
extern BYTE mid_sound[2][26][32];
extern BYTE last_sound[4][27][32];
extern BYTE ascii[95][16];

extern BYTE far *vid_addr;
extern WORD offsetx[];
extern int MaxX, MaxY;
extern int bminx, bminy, bmaxx, bmaxy;
extern int graph_mode;
extern BYTE color_mode;

#define BIT_0		0x80;
#define bioskey(a) wGetKey(a)

BYTE est_icon[32]={
	0x00,0x00,0x00,0x00,0x00,0x00,0x79,0xFF,0x7B,0xFF,0x63,0x0C,
	0x63,0x0C,0x73,0x8C,0x71,0xCC,0x60,0xCC,0x60,0xCC,0x7F,0xCC,
	0x7F,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,
};

#define NORM_VID 0
#define REV_VID  1
#define ON      1
#define OFF     0
#define ESC    27
#define ENTER  '\r'
#define SPACE  32
#define UP     72
#define DOWN   80
#define LEFT   75
#define RIGHT  77
#define HOME   71
#define END    79
#define PGUP   73
#define PGDN   81
#define ALT_X  45

#define SPAN_1   0
#define SPAN_2   1
#define SPAN_3   2
#define SPAN_4   3
#define SPAN_5   4
#define SPAN_6   5

#define SPAN_16  6
#define SPAN_31  7
#define SPAN_32  8
#define SPAN_41  9
#define SPAN_42  10
#define SPAN_51  11
#define SPAN_52  12
#define SPAN_61  13
#define SPAN_53  14
#define SPAN_54  15

#define SPAN_317 16

struct menu_panel {
	int x,y,endx,endy;
	BYTE **menu;
int count,last_menu,active;
} panel[17];

BYTE *our_buf;
BYTE top_choice=0;



char line_gap=6;
char indent=0;
char lmargin=1;
BYTE rmargin=78;
char align_way=5;

char exec_dir[80]="C:\\21";
char font_dir[80]="C:\\21\\FONT";
char swap_dir[80]="C:\\21\\SWP";

char word_gap=0;
char u_fspace=20;
char d_fspace=20;
char l_fspace=20;
char prn_kind=0;
char paper_kind=0;
char prn_port=0;
char paper_supply=0;

char m_kind=0;
char m_port=0;
char mc_color=15;

char menu_hfont=0;
char menu_efont=0;
char han_kbd=1;
char eng_kbd=0;
char rev_flag=1;

BYTE psort=6;
WORD papwids[10]={1440, 1440, 1440, 1440, 1440, 1440, 1440, 1440, 1440, 1440};
WORD paplens[10]={1890, 1890, 1890, 1890, 1890, 1890, 1890, 1890, 1890, 1890};


BYTE C_cnt=2;
BYTE C_key[10]={0,1};
BYTE *key_name[]={
	"µw¢… £¡Š‚  ", //0
	"Ðe‹i 2¤é¯¢ ", //1
	"...........", //2
	"...........", //3
	"...........", //4
	"...........", //5
	"...........", //6
	"...........", //7
	"...........", //8
	"...........", //9
	"...........", //10
};



BYTE White=15;

static int fontc[3]={63, 63, 63};
static int curc[3]={0, 21, 42};

BYTE cfg_data[]={
	"Word Processor 21st Century Config File V1.3\x1a\x3\x2\x1"
};

typedef struct {
	BYTE bit0 : 1 ;
	BYTE bit1 : 1 ;
	BYTE bit2 : 1 ;
	BYTE bit3 : 1 ;
	BYTE bit4 : 1 ;
	BYTE bit5 : 1 ;
	BYTE bit6 : 1 ;
	BYTE bit7 : 1 ;
} BITS;

typedef union {
	BITS b;
	BYTE c;
} BIT;

WORD k_buf[16];
int k_buf_loc=0;
int k_buf_loc2=0;
void init_tick(void);
void deinit_tick(void);

int wGetKey(int func)
{
	union REGS r;

	if(k_buf_loc) {
		func=k_buf[k_buf_loc2++];
		if(k_buf_loc2==k_buf_loc) k_buf_loc2=k_buf_loc=0;
		return func;
	}
	func+=0x10;
	r.h.ah=func;
	int86(0x16, &r, &r);
	if(func==0x10) {
		if(r.h.al) {
			if(r.h.al==0xe0) r.h.al=0;
			else r.h.ah=0;
		}
	}
	return r.x.ax;
}

void vResetKeyBuffer(void)
{
	int far *kbs = (int far *)0x0040001al;
	int far *kbe = (int far *)0x0040001cl;

	if(*kbs==*kbe) return;
	asm mov ah, 0;
	asm int 0x16;
}


void vPutKey(char scan_code, char ascii_code)
{
	k_buf[k_buf_loc]=(((WORD)scan_code)<<8) | ascii_code;
	k_buf_loc++;
}

int wCheckKeyHit(void)
{
	int rtn=0;

	if(k_buf_loc>0) return 1;
	asm MOV AH, 0x11;
	asm INT 0x16;
	asm JZ NOKEY;
	asm MOV rtn, AX;
NOKEY:
	return rtn;
}

void set_palette_int(BYTE s, int *pal)
{
	union REGS rr;
	int p[3];

	memcpy(p, pal, 6);
	if(graph_mode<EGA350) return;
	rr.h.ah=0x10;
	if(graph_mode<VGA640) {
		p[0]/=21;
		p[1]/=21;
		p[2]/=21;
		rr.h.al=0;
		rr.h.bl=s;
		rr.h.bh=((p[0]&1)<<5) | ((p[1]&1)<<4) | ((p[2]&1)<<3) |
						((p[0]&2)<<1) | ((p[1]&2)) | ((p[2]&2)>>1);
	} else {
		rr.h.al=0x10;
		rr.x.bx=s;
		rr.h.dh=p[0];  //r
		rr.h.ch=p[1];  //g
		rr.h.cl=p[2];  //b
	}
	int86(0x10, &rr, &rr);
}

void vGetImage(int x, int y, int endx, int endy, BYTE *buf)
{
	register WORD i, j;
	char far *offset;
	BYTE *temp;

	temp=buf;
	for(i=x ; i<endx+1 ;i++) {
		offset=vid_addr+offsetx[i]+y;
		for(j=y; j<endy+1 ; j++) *temp++=*offset++;
	}
}

void vPutImage(int x, int y, int endx, int endy, BYTE *buf)
{
	register WORD i, j;
	char far *offset;
	BYTE *temp;

	temp=buf;
	for(i=x ;i<endx+1;i++) {
		offset=vid_addr+offsetx[i]+y;
		for(j=y ; j<endy+1 ; j++) {
			*offset++ = *temp++;
		}
	}
}

void iInitHangul(BYTE gmode)
{
	if(iInitGraphMode(gmode, 0)!=1) {
		printf("\nGraphics setup error!! ");
		exit(0);
	}
}

void vDeinitHangul(void)
{
	vDeinitGraphMode();
}


void near vPutImage16x16(int x, int y, WORD *image_16x16, BYTE mode)
{
	register int i;
	WORD far *offset;
	BYTE far *offsetb;
	BYTE s=0, e=16;
	WORD image[16];
	WORD timage[16];


	if(bminx>x) s=bminx-x+1;
	if(bmaxx<x+16) e=bmaxx+1-x;

	if(mode==0) {
		if(bminy>>3>y-1) {
			for(i=s ; i<e ; i++) {
				offsetb=(BYTE far *)(vid_addr+offsetx[x+i]+y+1);
				*offsetb = (BYTE)(image_16x16[i]>>8);
			}
		} else if(bmaxy>>3<y+2) {
			for(i=s ; i<e ; i++) {
				offsetb=(BYTE far *)(vid_addr+offsetx[x+i]+y);
				*offsetb = (BYTE)image_16x16[i];
			}
		} else {
			for(i=s ; i<e ; i++) {
				offset=(WORD far *)(vid_addr+offsetx[x+i]+y);
				*offset = image_16x16[i];
			}
		}
		return;
	}
	for(i=0 ; i<16; i++) timage[i]=image[i]=image_16x16[i];

	if (mode & BIT_7) { 
		for(i=1 ; i<15 ; i++) {
			timage[i+1] |= image[i];
			timage[i+1] |= (image[i]>>1);
			timage[i] ^= image[i];
		}
	}
//	if(mode & BIT_8) timage[15] = 0xffff;
	if(mode & BIT_4)  
		for(i=0 ; i<16 ; i++) timage[i] = timage[i] ^ 0xffff;
	if(bminy>>3>y-1) {
		for(i=s ; i<e ; i++) {
			offsetb=(BYTE far *)(vid_addr+offsetx[x+i]+y+1);
			*offsetb = (BYTE)(timage[i]>>8);
		}
	} else if(bmaxy>>3<y+2) {
		for(i=s ; i<e ; i++) {
			offsetb=(BYTE far *)(vid_addr+offsetx[x+i]+y);
			*offsetb = (BYTE)timage[i];
		}
	} else {
		for(i=s ; i<e ; i++) {
			offset=(WORD far *)(vid_addr+offsetx[x+i]+y);
			*offset = timage[i];
		}
	}
}


void near vPutImage16x8(int x, int y, BYTE *image_16x8, BYTE mode)
{
	register int i;
	char far *offset;
	char s=0, e=16;
	BYTE image[16];
	BYTE timage[16];

	if(bminx>x) s=bminx-x+1;
	if(bmaxx<x+16) e=bmaxx+1-x;

	if(mode==0) {
		for(i=s ; i<e ; i++) {
			offset=vid_addr+offsetx[x+i]+y;
			*offset = image_16x8[i];
		}
		return;
	}
	for(i=0 ; i<16; i++) timage[i]=image[i]=image_16x8[i];

	 if (mode & BIT_7) { 
		for(i=1 ; i<15 ; i++) {
			timage[i+1] |= image[i];
			timage[i+1] |= (image[i]>>1);
			timage[i] ^= image[i];
		}
	}
//	if(mode & BIT_8) timage[15] = 0xff;
	if(mode & BIT_4)
		for(i=0 ; i<16 ; i++) timage[i] = timage[i] ^ 0xff;
	for(i=s ; i<e ; i++) {
		offset=vid_addr+offsetx[x+i]+y;
		*offset = timage[i];
	}
}

void vPFF(int x, int y, BYTE *b5, BYTE effect)
{
	BYTE image[32];
	BYTE cho, jung, jong;
	BYTE cho_style, jung_style, jong_style;
	register int i;
	CODE2 c;

	c.c2[1]=*b5;
	c.c2[0]=*(b5+1);
	cho = table[0][c.c3.code1];
	jung = table[1][c.c3.code2];
	jong = table[2][c.c3.code3];
	if(jong==0xff) {
		cho_style = cho_type_1[jung];
		if(cho!=0 && cho!=15 && 7<jung && 13>jung) jung+=13;
		jung_style=0;
	} else {
		cho_style = cho_type_2[jung];
		jong_style = jong_type[jung];
		if(cho!=0 && cho!=15 && 7<jung && 13>jung) jung+=13;
		jung_style=1;
	}
	if(jung==0xff) {
		if(jong == 0xff) cho_style = 0;
		else	cho_style=jong_style=0;
	}
	if (cho != 0xff)
		for(i=0;i<32;i++)
			image[i]=first_sound[cho_style][cho][i];
	else for(i=0;i<16;i++) image[i]=0;
	if (jung != 0xff)
		for(i=0;i<32;i++)
			image[i]|=mid_sound[jung_style][jung][i];
	if (jong != 0xff)
		for(i=0;i<32;i++)
			image[i]|=last_sound[jong_style][jong][i];
	vPutImage16x16(x, y, (WORD *)image, effect);
}

void vPHF(int x, int y, BYTE code, BYTE effect)
{
	WORD loc;
	BYTE *image;

	if(code>31) {
		loc=code-32;
		image=ascii[loc];
		vPutImage16x8(x, y, image, effect);
	}
}

void vClipOff(void)
{
	bminx=-1;
	bminy=-1;
	bmaxx=MaxX;
	bmaxy=MaxY;
}

void vClipOn(int x, int y, int ex, int ey)
{
	bminx=x;
	bminy=y;
	bmaxx=ex;
	bmaxy=ey;
}

void vPutString(WORD x, WORD y, BYTE *str, BYTE mode)
{
	while(*str) {
		if(*str>127) {
			vPFF(x, y, str, mode);
			y+=2;
			str+=2;
		} else {
			vPHF(x, y++, *(str++), mode);
		}
	}
}

int iPrintf(int x, int y, BYTE mode, BYTE *fmt, ...)
{
	 va_list argptr;
	 int cnt;
	 char buffer[80];

	 va_start(argptr, fmt);
	 cnt = vsprintf(buffer, fmt, argptr);
	 va_end(argptr);
	 vPutString(x, y, buffer, mode);
	 return(cnt);
}

#define GREEN  0x02   // background is black foreground is green

char vmode;
char bar_attr=0xa0;

void write_char(BYTE x, BYTE y, BYTE code, BYTE color)
{
	asm {
		mov ah, 2;
		mov bh, 0;
		mov dh, x;
		mov dl, y;
		int 10h;
		mov ah, 9;
		mov al, code;
		xor bh, bh;
		mov bl, color;
		mov cx, 1;
		int 10h;
	}
}

void write_string(int x, int y, BYTE *str, BYTE color)
{
	register int i;

	for(i=y; *str; i++) {
		write_char(x, i, *str++, color);
	}
}

void draw_border(int x, int y, int ex, int ey, int color)
{
	register int i;

	for(i = x+1; i<ex; i++) {
		write_char(i, y, 186, color);
		write_char(i, ey, 186, color);
	}
	for(i =y+1; i<ey; i++) {
		write_char(x, i, 205, color);
		write_char(ex, i, 205, color);
	}
	write_char(x, y ,201  , color);
	write_char(x, ey, 187  , color);
	write_char(ex, y, 200  , color);
	write_char(ex, ey, 188  , color);
}

int video_mode(void)
{
	union REGS r;

	r.h.ah = 15;
	return int86(0x10, &r, &r) & 255;
}

void set_video(void)
{
	vmode=video_mode();
}

void clr_scr(int color)
{
	union REGS r;

	r.h.ah =6;
	r.h.al =0;
	r.h.ch =0;
	r.h.cl =0;
	r.h.dh =24;
	r.h.dl =79;
	r.h.bh =color;
	int86(0x10, &r, &r);
}

void text_bar(int x, int y, int ex, int ey, char code, char color)
{
	int i, j;

	for(i=x; i<ex+1; i++) {
		for(j=y; j<ey+1; j++) {
			write_char(i, j, code, color);
		}
	}
}

void put_text_panel(int x, int y, int count, int color, char **str)
{
	int i, width;

	width=strlen(str[0])+1;
	text_bar(x, y+2, x+count+2, y+width+2, 32, 0);
	text_bar(x, y, x+count+1, y+width, 32, color);
	draw_border(x, y, x+count+1, y+width, color | 0x0f);
	for(i=0; i<count; i++) {
		write_string(x+1+i, y+1, str[i], color);
	}
}


char *T_menu[]={
	" Hercules      720x348 ",
	" Hercules      640x400 ",
	" EGA           640x350 ",
//	" EGAmono       640x350 ",
	" VGA           640x480 ",
	" VGAmono       640x480 ",
  " VGA(ET3000)   800x600 ",
	" VGA(TRIDENT)  800x600 ",
	" VGA(REALTEK)  800x600 ",
  " VGA(ATI)      800x600 ",
	" SPC           640x400 ",
	" Corona        640x400 ",
	" Mighty        640x400 ",
	" Partner       640x400 ",
	" Televideo     640x400 ",
	" TOSHIBA       640x400 ",
	" PC24          640x400 ",
	" AT&T          640x400 ",
	"      <<Cancel>>       ",
};

int choice;

int get_text_resp(int x, int y, int count, char color, char **str)
{
	union {
		char ch[2];
		int i;
	} c;

	write_string(x+1+choice, y+1, str[choice], bar_attr);
	for(;;) {
		while(!wCheckKeyHit());
		c.i=bioskey(0);
		write_string(x+1+choice, y+1, str[choice], color);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case '\r' :
					return choice;
				case  27  :
					return -1;
			}
		}
		else {
			switch(c.ch[1]) {
				case  72 :
					choice--;
					if(choice==-1) choice=count-1;
					break;
				case  80 :
					choice++;
					if(choice==count) choice=0;
					break;
				case  71 :  case 73 :
					choice=0;
					break;
				case  79 :  case 81 :
					choice=count-1;
					break;
			}
		}
		write_string(x+1+choice, y+1, str[choice], bar_attr);
	}
}

void set_cursor(char st, char vExit)
{
	union REGS r;
	r.h.ah=0x01;
	r.h.ch=st;
	r.h.cl=vExit;
	int86(0x10, &r, &r);
}

int get_mode(int i)
{
	choice=i;
	set_cursor(0x30,0);
	text_bar(0, 0, 24, 79, 177, 0x03);
	text_bar(0, 0, 0, 79, 32, 0x70);
	write_string(0, 18, " Hangul Word Processor 21st Century Setup !!", 0x7e);

	text_bar(24, 0, 24, 79, 32, 0x70);
	write_string(24, 21, "Copyright (c) 1993-1995  ESTsoft Corp.", 0x7e);
	write_string(24, 46, "ESTsoft", 0x71);

	write_string(2, 27, "   Select Graphic Card!  ", 0x3f);
	put_text_panel(3, 27, 18, 0x7f, T_menu);
	i= get_text_resp(3, 27, 18, 0x7f, T_menu);
	clr_scr(0x7);
	set_cursor(15, 16);
	return i;
}

BYTE cur_hsize=0x10;
BYTE cursor_flag=0;
BYTE on_off=OFF;
int iCX, iCY;

#include "settc.c"

void vPutStringLength(int x, int y, BYTE *str, BYTE attr, BYTE sloc, BYTE len)
{
	BYTE temp[0xff];

	vClipOn(-1, bminy=(y<<3)-1, (y+len)<<3, MaxX);
	strcpy(temp, str+sloc);
	temp[len]=0;
	vPutString(x, y, temp, attr);
	if(strlen(temp)<len) vBarByte(x, y+strlen(temp), x+15, y+len, 0);
	vClipOff();
}

BYTE iInsertByte(BYTE *buf, BYTE loc, BYTE len, BYTE flag)
{
	if(strlen(buf)+flag>=len) return 0;
	memmove(buf+loc+flag, buf+loc, len-loc-flag);
	return 1;
}

int iGetString(int x, int y, BYTE len, BYTE scr_len, BYTE *str)
{
	INKEY ch;
	BYTE buf[80];
	BYTE scr_c=0;
	BYTE loc;
  int i;

	strcpy(buf, str);
	cur_hsize=0x10;
	loc=strlen(buf);
	iCX=x;
	iCY=y+loc;
	vBarByte(x, y, x+15, y+scr_len, 0);
	vPutStringLength(x, y, buf, 0, 0, scr_len);
	if(loc>scr_len) {
		vXORBarBytex16(x, y, scr_len);
	}
	else {
		vXORBarBytex16(x, y, loc);
	}
	vTCON();
	while(!wCheckKeyHit()) if(loc<scr_len) vTextCursorOperation();
	ch.i=bioskey(0);
	vPutKey(ch.ch[1], ch.ch[0]);
	vTCOFF();
	if(ch.ch[0] && ch.ch[0]!=BS) {
		if(ch.ch[0]!=RETURN && ch.ch[0]!=ESC) {
			loc=0;
			buf[loc]=0;
			iCY=y;
			setmem(buf, len, 0);
		}
	}
	for(;;) {
		vTCON();
		while(!wCheckKeyHit()) vTextCursorOperation();
		ch.i=bioskey(0);
		vTCOFF();
		if(!ch.ch[0]) {
			switch(ch.ch[1]) {
				case RIGHT :
					if(loc==strlen(buf)) break;
					loc++;
					iCY++;
					break;
				case LEFT :
					if(loc==0) break;
					loc--;
					iCY--;
					if(iCY<y) {
						scr_c--;
						iCY++;
					}
					break;
				case HOME :
					iCY=y;
					loc=0;
					scr_c=0;
					break;
				case END :
					loc=strlen(buf);
					if(loc>scr_len) scr_c=loc-scr_len;
					iCY=y+loc-scr_c;
					break;
				case DELETE :
				 if(buf[loc]==0) break;
				 memmove(buf+loc, buf+loc+1, len-loc-1);
				 break;
			}
		}
		else {
			ch.ch[0]=toupper(ch.ch[0]);
			switch(ch.ch[0]) {
				case ESC :
					return -1;
				case BS :
					vPutKey(LEFT, 0);
					vPutKey(DELETE, 0);
					break;
				case RETURN :
					i=strlen(buf);
					if(i>0) {
						i--;
						while(buf[i]==32) {
							buf[i]=0;
							i--;
							if(i<0) break;
						}
					}
					strcpy(str, buf);
RN:				return 1;
				default :
					if(!iInsertByte(buf, loc, len, 1)) break;
					iCY++;
					buf[loc++]=ch.ch[0];
					break;
			}
		}
		if(iCY>y+scr_len) {
			scr_c+=iCY-y-scr_len;
			iCY-=iCY-y-scr_len;
		}
		vPutStringLength(x, y, buf, 0, scr_c, scr_len);
	}
}

BYTE full_vector=1;
BYTE disp_return=0;
BYTE disp_newline=0;
BYTE disp_space=0;
BYTE scr_saver_num=2;
BYTE no_work_time=3;
BYTE m_rate=5;
BYTE show_rod=0;
BYTE View_time=0;
BYTE mk_bak=1;
BYTE ins_lock=0;
BYTE asave=0;
BYTE as_time=30;
BYTE MRfunc1=7;
BYTE MRfunc2=6;

BYTE data_to_cfg(BYTE *fname)
{
	FILE *fp;
	int i;

	if((fp=fopen(fname, "wb+"))==NULL) {
		error2("Cannot make config file! Disk full?");
		return 0;
	}
	fwrite(cfg_data, 1, 48, fp);
	fputc(graph_mode, fp);
	fputc(m_kind, fp);
	fputc(m_port+1, fp);
	fputc(menu_hfont, fp);
	fputc(menu_efont, fp);
	fwrite(exec_dir, 80, 1, fp);
	fwrite(font_dir, 80, 1, fp);
	fwrite(swap_dir, 80, 1, fp);

	fputc(line_gap , fp);
	fputc(indent , fp);
	fputc(lmargin	, fp);
	fputc(rmargin	, fp);
	fputc(align_way	, fp);

	fputc(word_gap , fp);
	fputc(l_fspace , fp);
	fputc(u_fspace , fp);
	fputc(d_fspace , fp);

	fputc(prn_kind , fp);
	fputc(paper_kind , fp);
	fputc(paper_supply , fp);
	fputc(prn_port , fp);
	fputc(rev_flag, fp);
	fputc(han_kbd, fp);
	if(eng_kbd) fputc(eng_kbd+2, fp);
	else fputc(eng_kbd, fp);
	fputc(mc_color, fp);
	fputc(fontc[0], fp);
	fputc(fontc[1], fp);
	fputc(fontc[2], fp);
	fputc(curc[0], fp);
	fputc(curc[1], fp);
	fputc(curc[2], fp); // this is int

	fputc(psort, fp);
	for(i=6; i<10; i++) putw(papwids[i], fp);
	for(i=6; i<10; i++) putw(paplens[i], fp);
	for(i=6; i<10; i++) fwrite(paper[i]+2, 12, 1, fp);

// Statement...
	fputc(full_vector, fp);
	fputc(disp_return, fp);
	fputc(disp_newline, fp);
	fputc(disp_space, fp );
	fputc(scr_saver_num, fp);
	fputc(no_work_time, fp);
	fputc(m_rate, fp);
	fputc(show_rod, fp);
	fputc(View_time, fp);
	fputc(mk_bak, fp);
	fputc(ins_lock, fp);
	fputc(asave, fp);
	fputc(as_time, fp);
	fputc(MRfunc1, fp);
	fputc(MRfunc2, fp);

	fputc(C_cnt, fp);
	fwrite(C_key, 10, 1, fp);
	for(i=0; i<10; i++) fwrite(key_name[i], 11, 1, fp);
	for(i=0; i<128; i++) fputc(0, fp);

	fclose(fp);
	return 1;
}

BYTE cfg_to_data(BYTE *fname)
{
	FILE *fp;
	BYTE choice;
	int i;

	if((fp=fopen(fname, "rb"))==NULL) {
		printf("\nCannot open %s file! Make it? (Y/N)", fname);
		choice=getch();
		if(choice=='Y' || choice=='y') {
			unlink(fname);
			return 1;
		}
		return 0;
	}
	fread(our_buf, 1, 48, fp);
	our_buf[48]=0;
	if(strcmp(our_buf, cfg_data)!=NULL) {
		printf("\nConfig file is defective! Make it again? (Y/N)");
		choice=getch();
		if(choice=='Y' || choice=='y') {
			return 1;
		}
		return 0;
	}
	graph_mode=fgetc(fp);
	m_kind=fgetc(fp);
	m_port=fgetc(fp)-1;
	menu_hfont=fgetc(fp);
	menu_efont=fgetc(fp);
	fread(exec_dir, 80, 1, fp);
	fread(font_dir, 80, 1, fp);
	fread(swap_dir, 80, 1, fp);
	line_gap		=fgetc(fp);
	indent			=fgetc(fp);
	lmargin			=fgetc(fp);
	rmargin			=fgetc(fp);
	align_way		=fgetc(fp);

	word_gap		=fgetc(fp);
	l_fspace		=fgetc(fp);
	u_fspace		=fgetc(fp);
	d_fspace		=fgetc(fp);

	prn_kind		=fgetc(fp);
	paper_kind	=fgetc(fp);
	paper_supply=fgetc(fp);
	prn_port		=fgetc(fp);

	rev_flag = fgetc(fp);
	han_kbd = fgetc(fp);
	eng_kbd = fgetc(fp);
	if(eng_kbd) eng_kbd-=2;
	mc_color = fgetc(fp);
  fontc[0] = fgetc(fp);
	fontc[1] = fgetc(fp);
	fontc[2] = fgetc(fp);
	curc[0] = fgetc(fp);
	curc[1] = fgetc(fp);
	curc[2] = fgetc(fp);

	psort=fgetc(fp);
	for(i=6; i<10; i++) papwids[i]=getw(fp);
	for(i=6; i<10; i++) paplens[i]=getw(fp);
	for(i=6; i<10; i++) fread(paper[i]+2, 12, 1, fp);

// Statement...
	full_vector = fgetc(fp);
	disp_return = fgetc(fp);
	disp_newline = fgetc(fp);
	disp_space = fgetc(fp);
	scr_saver_num = fgetc(fp);
	no_work_time = fgetc(fp);
	m_rate = fgetc(fp);
	show_rod = fgetc(fp);
	View_time = fgetc(fp);
	mk_bak = fgetc(fp);
	ins_lock = fgetc(fp);
	asave = fgetc(fp);
	as_time = fgetc(fp);
	MRfunc1 = fgetc(fp);
	MRfunc2 = fgetc(fp);

	C_cnt=fgetc(fp);
	fread(C_key, 10, 1, fp);
	for(i=0; i<10; i++) fread(key_name[i], 11, 1, fp);

	fclose(fp);
	return 1;
}

void put_set_board(void)
{
	WORD i;
	WORD sx, ex;
	WORD center;

	sx=(MaxX-250)/2;
	ex=sx+265;
	center=((MaxY>>3)+1-52)/2;        // center , center +52  33
	vDrawPanelByte(20, 1, 44, MaxY/8, 0);
  vPutString(24, center, "Ðe‹i¶¡—aÏa¡­A¬á 21­A‹¡ v1.41 ·©¤e¶w ÑÅ‰w¬é¸÷ Ïa¡‹aœ‘", 0);
	vDrawPanelByte(sx, 1, ex, MaxY>>3, 0);
	center=((MaxY>>3)+1-33)/2;
	vDrawPanelByte(MaxX-28, 1, MaxX-5, MaxY/8, 0);
	vPutString(MaxX-24, center+1, "Copyright (c) 1993,1994 ESTsoft Corp.", 0);
	for(i=0;i<15;i++) {
		vPutString(sx+5+i*17, 2, setting[i], 0x0);
	}
}

void put_cur_set(BYTE num)
{
	WORD sx;

	sx=(MaxX-250)/2;
	if(num!=100 && num<11) {
		vBBarBit(sx+5+17*num, 16*8, sx+5+17*num+16, 60*8);
	}
	if(num==100 || num==1) {
		vBarByte(sx+5+17, 16, sx+5+17*4, 60, 0);
		iPrintf(sx+5+17*1, 16, 0x0, "%s", exec_dir);
		iPrintf(sx+5+17*2, 16, 0x0, "%s", font_dir);
		iPrintf(sx+5+17*3, 16, 0x0, "%s", swap_dir);
	}
	if(num==100 || num==5) {
		iPrintf(sx+5+17*5, 16, 0x0, "%s", panel[SPAN_32].menu[prn_kind]+2);
	}
	if(num==100 || num==6) {
		iPrintf(sx+5+17*6, 16, 0x0, "%s", panel[SPAN_317].menu[paper_kind]+2);
	}

	if(num==100 || num==7) {
    iPrintf(sx+5+17*7, 16, 0x0, "%s", (paper_supply==0) ? "µe­¢‰·‹s" : "{¸w‰·‹s");
	}
	if(num==100 || num==8) {
		iPrintf(sx+5+17*8, 16, 0x0, "%s", (!prn_port) ? "LPT1" : "LPT2");
	}
	if(num==100 || num==10) {
		iPrintf(sx+5+17*10, 16, 0x0, "%s", panel[SPAN_41].menu[m_kind]+2);
	}
	if(num==100 || num==11) {
		iPrintf(sx+5+17*11, 16, 0x0, "%s", (!m_port) ? "COM1" : "COM2");
		iPrintf(sx+5+17*11, 42, 0x0, "%s", panel[SPAN_42].menu[mc_color]+2);
	}
	if(num==100 || num==12) {
		iPrintf(sx+5+17*12, 16, 0x0, "%s", panel[SPAN_51].menu[menu_hfont]+2);
	}
	if(num==100 || num==13) {
		iPrintf(sx+5+17*12, 42, 0x0, "%s", panel[SPAN_52].menu[menu_efont]+2);
	}
	if(num==100 || num==14) {
		iPrintf(sx+5+14*17, 12, 0x0, "R=%2d G=%2d B=%2d", curc[0], curc[1], curc[2]);
		iPrintf(sx+5+14*17, 42, 0x0, "R=%2d G=%2d B=%2d", fontc[0], fontc[1], fontc[2]);
	}
	if(num==100 || num==15) {
		iPrintf(sx+5+17*13, 16, 0x0, "%s", panel[SPAN_53].menu[han_kbd-1]+2);
		iPrintf(sx+5+17*13, 42, 0x0, "%s", panel[SPAN_54].menu[eng_kbd]+2);
	}
}

void vSetMenu(int num,int x, int y, int count,BYTE **menu)
{
	WORD width=0;
	int endx,endy;

	width = strlen(menu[0])-1;
	endx = x+3+((count)*17);
	endy = y+width+1;

	panel[num].x = x;
	panel[num].y = y;
	panel[num].endx = endx;
	panel[num].endy = endy;
	panel[num].menu = (BYTE **)menu;
	panel[num].count = count;
	panel[num].last_menu = 1;
	panel[num].active = OFF;
}

void vInitMenu(void)
{
	int x =0 , y =0;

	vSetMenu(SPAN_1  , x+16, y, 5 , input);
	vSetMenu(SPAN_2  , x+16, y+10, 3 , directory);
	vSetMenu(SPAN_3  , x+16, y+20, 5 , pchoice);
	vSetMenu(SPAN_4  , x+16, y+30, 3 , vMouseOptDlg);
	vSetMenu(SPAN_5  , x+16, y+40, 4 , fonts);
	vSetMenu(SPAN_6  , x+16, y+50, 5,  custom);

	vSetMenu(SPAN_61  , x+16+4*18, y+53, 3,  color_set);

	vSetMenu(SPAN_16 , x+66, y+5   , 7 , align);
	vSetMenu(SPAN_31 , x+36, y+20+5, 4 , poption);
	vSetMenu(SPAN_32 , x+50, y+20+5, 1 , pkind);
	vSetMenu(SPAN_41 , x+34, y+30+5, 3 , mouse_sort);
	vSetMenu(SPAN_42 , x+18, y+30+5, 16, mouse_colour);
	vSetMenu(SPAN_51 , x+34, y+40+5, 4 , hfont);
	vSetMenu(SPAN_52 , x+51, y+40+5, 4 , efont);
	vSetMenu(SPAN_53 , x+68, y+40+5, 2, hangul_kbd);
	vSetMenu(SPAN_54 , x+68+17, y+40+5, 13, english_kbd);

	vSetMenu(SPAN_317, x+66, y+20+5, 6, paper);
}

#define XORLB	{vMCOFF(); vXORBarBit(x+37, y*8+27, x+53, y*8+48); vMCON();}
#define XORRB	{vMCOFF(); vXORBarBit(x+37, y*8+62, x+53, y*8+83); vMCON();}
#define PUTNUM {if(mousex!=oldx && mousex !=oldy) {vMCOFF();\
									iPrintf(x+120, y+2, 0x10, "x=%3d y=%3d",\
													mousex-x, mousey-y*8);	vMCON();\
									oldx=mousex;oldy=mousey;}}



void vOpenDlgWindow(int lx, int ly, BYTE *head)
{
	int x, y, ex, ey;

	x=MaxX/2-lx*4;
	y=MaxY/16-ly/2;
	ex=x+lx*8;
	ey=y+ly;

	vGetImage(x, y, ex+2, ey+2, our_buf+27000);
	vDrawPanelByte(x, y, ex, ey,0);
	vPutString(x+2, y+(ly-strlen(head))/2, head, 0x0);
	vWLineYBit(x+16+3, y*8, ey*8);
}

void vCloseDlgWindow(int lx, int ly)
{
	int x, y;

	x=MaxX/2-lx*4;
	y=MaxY/16-ly/2;
	vPutImage(x, y, x+lx*8+2, y+ly+2, our_buf+27000);
}


void vRevTopMenu(int n)
{
	vXORBarBytex16(0, n*10, 10);
}

void vSaveMenuBk(int num)
{
	if (panel[num].active==OFF) {
		if(num <SPAN_16) {
			vGetImage((panel[num].x),(panel[num].y),
			(panel[num].endx)+30,(panel[num].endy)+4, our_buf);
		}
		else if(num <SPAN_317) {
			vGetImage((panel[num].x),(panel[num].y),
			(panel[num].endx)+30,(panel[num].endy)+4, our_buf+10000);
		}
		else {
			vGetImage((panel[num].x),(panel[num].y),
			(panel[num].endx)+30,(panel[num].endy)+4, our_buf+20000);
		}
		panel[num].active = ON;
	}
}

void vRestoreMenuBk(int num)
{
	if (panel[num].active==ON) {
		if(num <SPAN_16) {
			vPutImage((panel[num].x),(panel[num].y),
			(panel[num].endx)+30,(panel[num].endy)+4, our_buf);
		}
		else if(num <SPAN_317) {
			vPutImage((panel[num].x),(panel[num].y),
			(panel[num].endx)+30,(panel[num].endy)+4, our_buf+10000);
		}
		else {
			vPutImage((panel[num].x),(panel[num].y),
			(panel[num].endx)+30,(panel[num].endy)+4, our_buf+20000);
		}
		panel[num].active = OFF;
	}
}

void vPutMenu(int n)
{
	register int i;
	int choice;
	int x = panel[n].x;
	int y = panel[n].y;
	int endx = panel[n].endx;
	int endy = panel[n].endy;
	int attr;

	vDrawPanelByte(x, y, endx, endy, rev_flag);
	for(i=0; i<panel[n].count; i++) {
		if(panel[n].menu[i][0]=='0') attr=0x0;
		else attr=0x2;
		if(rev_flag) attr|=0x10;
		vPutString(x+2+i*17, y+1, panel[n].menu[i]+2, attr);
	}
	choice=panel[n].last_menu;
	vXORBarBit(panel[n].x+2+(choice-1)*17,panel[n].y*8+2,panel[n].x+18+(choice-1)*17,panel[n].endy*8-2);
}

void vExit(void)
{
	free(our_buf);
	vDeinitHangul();
	deinit_tick();
	exit(0);
}

int iGetUserResponse(int num)
{
	union inkey {
		char ch[2];
		int  i;
	} c;
	int x = panel[num].x;
	int y = panel[num].y;
	int endy=panel[num].endy;
	int choice=panel[num].last_menu;   

	for(;;) {
		while(!wCheckKeyHit());
		c.i=bioskey(0);
		vXORBarBit(x+2+(choice-1)*17,y*8+2,x+18+(choice-1)*17,endy*8-2);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case '\r' :
					if(panel[num].menu[choice-1][0]!='0') break;
					vXORBarBit(x+2+(choice-1)*17,y*8+2,x+18+(choice-1)*17,endy*8-2);
					panel[num].last_menu = choice;
					return choice;
				case SPACE :
					panel[num].last_menu = choice;
					choice++;
					if(choice>panel[num].count) choice=1;
					break;
				case ESC :
					panel[num].last_menu = choice;
					return -1;
			}
		} else {
			switch(c.ch[1]) {
				case HOME :
					panel[num].last_menu = choice;
					choice=1;
					break;
				case END  :
					panel[num].last_menu = choice;
					choice=panel[num].count;
					break;
				case DOWN :
					panel[num].last_menu = choice;
					choice++;
					if(choice>panel[num].count) choice=1;
					break;
				case UP :
					panel[num].last_menu = choice;
					choice--;
					if(choice<1) choice=panel[num].count;
					break;
				case LEFT :
					if(num<SPAN_16) {
						vRevTopMenu(top_choice);
						if(top_choice==0) {
							top_choice=7;
						}
						else top_choice--;
						vXORBarBit(x+2+(choice-1)*17,y*8+2,x+18+(choice-1)*17,endy*8-2);
						panel[num].last_menu = choice;
						vRevTopMenu(top_choice);
						return -2;
					}
					break;
				case RIGHT :
					if(num<SPAN_16) {
						vRevTopMenu(top_choice);
						if(top_choice==7) {
							top_choice=0;
						}
						else top_choice++;
						vXORBarBit(x+2+(choice-1)*17,y*8+2,x+18+(choice-1)*17,endy*8-2);
						panel[num].last_menu = choice;
						vRevTopMenu(top_choice);
						return -2;
					}
					break;
				case  PGUP:
					if(top_choice!=0) {
						vRevTopMenu(top_choice);
						top_choice=0;
						vRevTopMenu(top_choice);
						panel[num].last_menu = choice;
						return-2;
					}
					break;
				case  PGDN :
					if(top_choice!=6) {
						vRevTopMenu(top_choice);
						top_choice=6;
						vRevTopMenu(top_choice);
						panel[num].last_menu = choice;
						return-2;
					}
					break;
				case ALT_X :
					return -1;
			}
		}
		vXORBarBit(x+2+(choice-1)*17,y*8+2,x+18+(choice-1)*17,endy*8-2);
	}
}

int iPullDownMenu(int num)
{

	vSaveMenuBk(num);
	vPutMenu(num);
	return iGetUserResponse(num);
}

void vDrawTopMenu(void)
{
	int i;

	vBarByte(0, 0, 16, MaxY/8+7, rev_flag);
	if(rev_flag) {
		for(i=0; i<8; i++) {
			vPutString(0, i*10, main_menu[i], 0x10);
		}
		vBLineYBit(16, 0, MaxY);
	} else {
		for(i=0; i<8; i++) {
			vPutString(0, i*10, main_menu[i], 0x0);
		}
		vWLineYBit(16, 0, MaxY);
	}
}




void vPutMenuObj(int n, int choice, int y, BYTE *menu)
{
	memcpy(panel[n].menu[choice]+y, menu, strlen(menu));
}

void put_num(int n, int choice, int y, int num)
{
	sprintf(panel[n].menu[choice]+y, "%3d", num);
}

int get_num(const int i)
{
	return i;
}

int get_data(int def, int start, int vExit, char *unit, BYTE *head)
{
	int i,x, y;

	union inkey {
		BYTE ch[2];
		int  i;
	} c;

	i=def;
	x=MaxX/2-8*8+34;
	y=MaxY/16-15;
	vOpenDlgWindow(14, 30, head);
	vWLineYBit(x+32+15, y*8, (y+30)*8);
	vPutString(x+35+16, y+2, "(<-)   = -1      (->) = +1", 0x0);
	vPutString(x+50+16, y+2, "Ctrl <-= -10   Ctrl-> =+10",0x0);
	iPrintf(x+20, y+2, 0x0, "%3d ¦Èá %3dŒa»¡", start, vExit);
	iPrintf(x, y+2, 0, "%3d %s", i, unit);
	for(;;) {
		c.i=bioskey(0);
		if(c.ch[0]) {
			switch(c.ch[0])  {
				case ESC :
					vBarByte(x, y+1, x+15, y+5, 0);
					iPrintf(x, y+2,0x0, "%3d %s", i, unit);
					vCloseDlgWindow(14,30);
					return def;
				case RETURN :
					vBarByte(x, y+1, x+15, y+5, 0);
					iPrintf(x, y+2,0x0, "%3d %s", i, unit);
					vCloseDlgWindow(14,30);
					return i;
			}
		} else if(c.ch[1]) {
			switch(c.ch[1]) {
				case CTRL_RARROW :
					i+=10;
					if(i>vExit) i=vExit;
					break;
				case CTRL_LARROW :
					i-=10;
					if(i<start) i=start;
					break;
				case LEFT :
					i-=1;
					if(i<start) i=start;
					break;
				case RIGHT :
					i+=1;
					if(i>vExit) i=vExit;
					break;
				case CTRL_PGUP : case HOME :
					i=start;
					break;
				case CTRL_PGDN : case END :
					i=vExit;
					break;
			}
		}
		vBarByte(x, y+1, x+15, y+5, 0);
		iPrintf(x, y+2,0x0, "%3d %s", i, unit);
	}
}

void iInputMenuDrv(void)
{
	int choice, code;
	BYTE l, i, r;
	int s, e;

	i=indent; l=lmargin; r=rmargin;
	put_num(SPAN_1, 0, 14, line_gap);
	put_num(SPAN_1, 1, 14, indent);
	put_num(SPAN_1, 2, 14, lmargin);
	put_num(SPAN_1, 3, 14, rmargin);
	vPutMenuObj(SPAN_1, 4, 11, (BYTE *)panel[SPAN_16].menu[align_way]+2);
	for(;;) {
		choice=iPullDownMenu(SPAN_1);
		switch(choice) {
			case -1 :
				vExit();
				break;
			case -2 :
				vRestoreMenuBk(SPAN_1);
				return;
			case  1 :
				line_gap=get_data((int)line_gap, 0, 20, "•¡Ëa",(BYTE *) "º‰ˆe‰b");
				put_num(SPAN_1, 0, 14, line_gap);
				break;
			case  2 :
				s=1-l; e=r-8;
				if(s<-120) s=-120;
				if(e>120) e=120;
				i=indent=(char)get_data((int)(char)i,  s, e, "Äéœñ", (BYTE *)"—iµa/´á³a‹¡");
				put_num(SPAN_1, 1, 14, indent);
				break;
			case  3 :
				if((int)((char)i)>0) {
					s=1; e=r-10-(int)((char)i);
				}
				else {s=1-(int)((char)i); e=r-10;}
				l=lmargin=get_data((int)l, s, e, "µi", (BYTE *)"¶E½¢{");
				put_num(SPAN_1, 2, 14, lmargin);
				break;
			case  4 :
				if((int)((char)i) > 0) {
					s=l+10+i; e=250;
				}
				else {s=l+10; e=250;}
				r=rmargin=get_data((int)r, s, e, "µi", (BYTE *)"µ¡Ÿe½¢{");
				put_num(SPAN_1, 3, 14, rmargin);
				break;
			case  5 :
				code=iPullDownMenu(SPAN_16);
				vRestoreMenuBk(SPAN_16);
				if(code!=-1) {
					align_way=code-1;
					vPutMenuObj(SPAN_1, 4, 11, (BYTE *)panel[SPAN_16].menu[align_way]+2);
				}
				break;
		}
		vPutMenu(SPAN_1);
	}
}

void dir_drv(void)
{
	int choice;
	int x, y;

	x=MaxX/2-6*4;
	y=MaxY/16-40/2;

	for(;;) {
		choice=iPullDownMenu(SPAN_2);
		switch(choice) {
			case -1 :
				vExit();
				break;
			case -2 :
				vRestoreMenuBk(SPAN_2);
				return;
			case  1 :
				vOpenDlgWindow(6, 40, "¯©Ð—Ìa·© —¡BÉ¡Ÿ¡");
				iGetString(x+27, y+1, 78, 38, exec_dir);
				vCloseDlgWindow(6, 40);
				if(exec_dir[strlen(exec_dir)-1]=='\\') {
					exec_dir[strlen(exec_dir)-1]=0;
				}
				strcpy(font_dir, exec_dir);
				strcpy(swap_dir, exec_dir);
				strcat(font_dir, "\\FONT");
				strcat(swap_dir, "\\SWP");
				vRestoreMenuBk(SPAN_2);
				put_cur_set(1);
				break;
			case  2 :
				vOpenDlgWindow(6, 40, "¸aÑwÌa·© —¡BÉ¡Ÿ¡");
				iGetString(x+27, y+1, 78, 38, font_dir);
				vCloseDlgWindow(6, 40);
				if(font_dir[strlen(font_dir)-1]=='\\') {
					font_dir[strlen(font_dir)-1]=0;
				}
				vRestoreMenuBk(SPAN_2);
				put_cur_set(1);
				break;
			case  3 :
				vOpenDlgWindow(6, 40, "¯aµÓÌa·© —¡BÉ¡Ÿ¡");
				iGetString(x+27, y+1, 78, 38, swap_dir);
				vCloseDlgWindow(6, 40);
				if(swap_dir[strlen(swap_dir)-1]=='\\') {
					swap_dir[strlen(swap_dir)-1]=0;
				}
				vRestoreMenuBk(SPAN_2);
				put_cur_set(1);
				break;
		}
	}
}

int iGetPrinterDriver(BYTE *fname)
{
	FILE *fp;
	BYTE buf[80];
	int i, len;

	if((fp=fopen(fname, "rt"))==NULL) return -1;
	for(i=0;i<20;i++) {
		fgets(buf, 80, fp);
		if(buf[0]==':') break;
		sscanf(buf, "%s", pkind[i]+2);
		len=strlen(pkind[i]+2);
		if(len!=12) pkind[i][len+2]=' ';
	}
	fclose(fp);
	return i;
}

void iPrintDrv(void)
{
	int choice1, choice2;
	for(;;) {
ST:
		choice1=iPullDownMenu(SPAN_3);
		switch(choice1) {
			case -1 :
				vExit();
				break;
			case -2 :
				vRestoreMenuBk(SPAN_3);
				return;
			case  1 :
				put_num(SPAN_31, 0, 13, word_gap);
				put_num(SPAN_31, 1, 13, u_fspace);
				put_num(SPAN_31, 2, 13, d_fspace);
				put_num(SPAN_31, 3, 13, l_fspace);
				for(;;) {
					choice2=iPullDownMenu(SPAN_31);
					switch(choice2) {
						case -1 :
							vRestoreMenuBk(SPAN_31);
							goto ST;
						case  1 :
							word_gap=get_data(word_gap, 0, 50, "•¡Ëa", (BYTE *)"‹i¸aˆe‰b");
							put_num(SPAN_31, 0, 13, word_gap);
							break;
						case  2 :
							u_fspace=get_data(u_fspace,0 ,50, "Ð—", (BYTE *)"¶áµa¤‚");
							put_num(SPAN_31, 1, 13, u_fspace);
							break;
						case  3 :
							d_fspace=get_data(d_fspace, 0, 50, "Ð—", (BYTE *)"´aœµa¤‚");
							put_num(SPAN_31, 2, 13, d_fspace);
							break;
						case  4 :
							l_fspace=get_data(l_fspace, 0, 50, "Äéœñ", (BYTE *) "¶E½¢µa¤‚");
							put_num(SPAN_31, 3, 13, l_fspace);
							break;
					}
					vPutMenu(SPAN_31);
				}
			case 2 :
				choice2=iPullDownMenu(SPAN_32);
				vRestoreMenuBk(SPAN_32);
				if(choice2!=-1) {
					prn_kind=choice2-1;
					put_cur_set(5);
					break;
				}
				break;
			case 3 :
				choice2=iPullDownMenu(SPAN_317);
				vRestoreMenuBk(SPAN_317);
				if(choice2!=-1) {
					paper_kind=choice2-1;
					put_cur_set(6);
				}
				break;
			case 4 :
				paper_supply^=1;
				put_cur_set(7);
				break;
			case 5 :
				prn_port^=0x01;
				put_cur_set(8);
				break;
		}
	}
}

void mouse_drv(void)
{
	int choice, code;
	for(;;) {
		choice=iPullDownMenu(SPAN_4);
		switch(choice) {
			case -1 :
				vExit();
				break;
			case -2 :
				vRestoreMenuBk(SPAN_4);
				return;
			case  1 :
				code=iPullDownMenu(SPAN_41);
				vRestoreMenuBk(SPAN_41);
				if(code != -1) {
					m_kind=code-1;
					put_cur_set(10);
				}
				break;
			case 2 :
				m_port^=0x01;
				put_cur_set(11);
				break;
			case 3 :
				code=iPullDownMenu(SPAN_42);
				vRestoreMenuBk(SPAN_42);
				if(code != -1) {
					mc_color=code-1;
					put_cur_set(11);
				}
				break;
		}
	}
}

void font_drv(void)
{
	int choice, code;
	for(;;) {
		choice=iPullDownMenu(SPAN_5);
		switch(choice) {
			case -1 :
				vExit();
				break;
			case -2 :
				vRestoreMenuBk(SPAN_5);
				return;
			case  1 :
				code=iPullDownMenu(SPAN_51);
				vRestoreMenuBk(SPAN_51);
				if(code !=-1) {
					menu_hfont=code-1;
					put_cur_set(12);
				}
				break;
			case  2 :
				code=iPullDownMenu(SPAN_52);
				vRestoreMenuBk(SPAN_52);
				if(code!=-1) {
					menu_efont=code-1;
					put_cur_set(13);
				}
				break;
			case 3 :
				code=iPullDownMenu(SPAN_53);
				vRestoreMenuBk(SPAN_53);
				if(code!=-1) {
					han_kbd=code;     // start from '1'
					put_cur_set(15);
				}
				break;
			case 4 :
				code=iPullDownMenu(SPAN_54);
				vRestoreMenuBk(SPAN_54);
				if(code!=-1) {
					eng_kbd=code-1;  // start form '0'
					put_cur_set(15);
				}
				break;
		}
	}
}

void put_bar(int x, int y, int ly, int tot, int cur)
{
	int len, i;

	len=((ly*8)*(cur))/tot;
	vWBoxBit(x-2, y*8-2, x+18, (y+ly)*8+2);
	vBBarBit(x, y*8, x+16, (y+ly)*8);
	if(len>8) {
		vWBarBit(x, y*8, x+16, y*8+len);
	}
	else {
		for(i=0; i<len; i++) vWLineXBit(x, x+16, y*8+i);
	}
	iPrintf(x, y+ly+1, 0, "%2d", cur);
}

BYTE *Pmenu[]={
	"¨iˆe¬‚(R)",
	"Á¡¢¬‚(G)",
	"Ìaœe¬‚(B)",
};

void pal_menu(int cho, int flag)
{
	int x, y;

	x=MaxX/2-18*4;
	y=MaxY/16-32/2;
	if(cho < 3) {
		if(flag) vPutString(x+32+cho*30, y+1, ">", 0);
		else vPutString(x+32+cho*30, y+1, " ", 0);
	}
	else {
		if(flag) vPutString(x+18*8-20, y+1, ">", 0);
		else vPutString(x+18*8-20, y+1, " ", 0);
	}
}

int Pal_max=63;

void pal_volume(BYTE cnum, int *val)
{
	int x, y, lx=18, ly=32, i;
	int inc;
	char cho=0;
	int tval[3];
	INKEY c;

	if(graph_mode<VGA640) inc=21;
	else inc=1;
	x=MaxX/2-lx*4;
	y=MaxY/16-ly/2;
	vOpenDlgWindow(lx, ly, "ÌiAËa¹¡¸÷");
	for(i=0; i<3; i++) {
		put_bar(x+32+i*30, y+12, 16, Pal_max, val[i]);
		vPutString(x+32+i*30, y+2, Pmenu[i], 0);
	}
	vWLineYBit(x+lx*8-25, y*8, (y+ly)*8);
	vPutString(x+lx*8-20, y+14, "µÅža", 0);
	pal_menu(cho, 1);
	memcpy(tval, val, 6);
	for(;;) {
		while(!wCheckKeyHit());
		pal_menu(cho, 0);
		c.i=bioskey(0);
		if(c.ch[1]) {
			switch(c.ch[1]) {
				case UP :
					cho--; if(cho<0) cho=3; break;
				case DOWN :
					cho++; if(cho==4) cho=0; break;
				case LEFT : case CTRL_LARROW : case PGDN :
					if(cho!=3) {
						if(c.ch[1]==LEFT) val[cho]-=inc;
						else val[cho]-=inc*10;
						if(val[cho]<0) val[cho]=0;
						put_bar(x+32+cho*30, y+12, 16, Pal_max, val[cho]);
					}
					else {
						for(i=0;i<3;i++) {
							if(c.ch[1]==LEFT) val[i]-=inc;
							else val[i]-=inc*10;
							if(val[i]<0) val[i]=0;
							put_bar(x+32+i*30, y+12, 16, Pal_max, val[i]);
						}
					}
					break;
				case RIGHT :  case CTRL_RARROW : case PGUP :
					if(cho!=3) {
						if(c.ch[1]==RIGHT) val[cho]+=inc;
						else val[cho]+=inc*10;
						if(val[cho]>Pal_max) val[cho]=Pal_max;
						put_bar(x+32+cho*30, y+12, 16, Pal_max, val[cho]);
					}
					else {
						for(i=0;i<3;i++) {
							if(c.ch[1]==RIGHT) val[i]+=inc;
							else val[i]+=inc*10;
							if(val[i]>Pal_max) val[i]=Pal_max;
							put_bar(x+32+i*30, y+12, 16, Pal_max, val[i]);
						}
					}
					break;
				case HOME :
					if(cho!=3) {
						val[cho]=0;
						put_bar(x+32+cho*30, y+12, 16, Pal_max, val[cho]);
					}
					break;
				case END :
					if(cho!=3) {
						val[cho]=Pal_max;
						put_bar(x+32+cho*30, y+12, 16, Pal_max, val[cho]);
					}
					break;
			}
		} else if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC :
					memcpy(val, tval, 6);
					set_palette_int(cnum, val);
					vCloseDlgWindow(lx, ly);
					return;
				case RETURN :
					vCloseDlgWindow(lx, ly);
					return;
			}
		}
		pal_menu(cho, 1);
		set_palette_int(cnum, val);
	}
}

void color_drv(void)
{
	int i;
	int temp[3];

	if(!color_mode) return;
	for(;;) {
		i=iPullDownMenu(SPAN_61);
		if(i>0) {
			switch(i) {
				case 1:
					pal_volume(0, curc);
					break;
				case 2:
					pal_volume(White, fontc);
					break;
				case 3:
					memcpy(temp, curc, 6);
					memcpy(curc, fontc, 6);
					memcpy(fontc, temp, 6);
					set_palette_int(0, curc);
					set_palette_int(White, fontc);
					break;
			}
			if(curc[0]+curc[1]+curc[2]>fontc[0]+fontc[1]+fontc[2]) rev_flag=0;
			else rev_flag=1;
			put_cur_set(14);
			vRestoreMenuBk(SPAN_61);
			vRestoreMenuBk(SPAN_6);
			vDrawTopMenu();
			vSaveMenuBk(SPAN_6);
			vPutMenu(SPAN_6);
			vRevTopMenu(top_choice);
		}
		else if(i<0) {
			vRestoreMenuBk(SPAN_61);
			return;
		}
	}
}

void colour_drv(void)
{
	int i;
	int c_tbl[4][6]={
		{0, 0, 0, 63, 63, 63},
		{0, 21, 42, 63, 63, 63},
		{63, 63, 63, 0, 21, 0},
		{42, 42, 42, 0, 0, 0},
	};

	for(;;) {
		i=iPullDownMenu(SPAN_6);
		if(i>0 && i<5) {
			i--;
			memcpy(curc, c_tbl[i], 6);
			memcpy(fontc, c_tbl[i]+3, 6);
			set_palette_int(0, curc);
			set_palette_int(White, fontc);
			if(curc[0]+curc[1]+curc[2]>fontc[0]+fontc[1]+fontc[2]) rev_flag=0;
			else rev_flag=1;
			put_cur_set(14);
			vRestoreMenuBk(SPAN_6);
			vDrawTopMenu();
			vRevTopMenu(top_choice);
		}
		else if(i==5) {
			color_drv();
			vRestoreMenuBk(SPAN_6);
			vDrawTopMenu();
			vRevTopMenu(top_choice);
		}
		else if(i==-1) {
			vExit();
		}
		else if(i<-1) {
			vRestoreMenuBk(SPAN_6);
			return;
		}
	}
}

int get_key(void)
{
	INKEY c;

	for(;;) {
		while(!wCheckKeyHit());
		c.i=bioskey(0);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case '\r' :
					return 1;
				case SPACE :
					return 2;
				case ESC   :
					return -1;
			}
		} else if(c.ch[1]) {
			switch(c.ch[1]) {
				case LEFT :
					vRevTopMenu(top_choice);
					if(top_choice==0) {
						top_choice=7;
					}
					else top_choice--;
					vRevTopMenu(top_choice);
					return -2;
				case RIGHT :
					vRevTopMenu(top_choice);
					if(top_choice==7) {
						top_choice=0;
					}
					else top_choice++;
					vRevTopMenu(top_choice);
					return -3;
				case HOME :  case  PGUP:
					if(top_choice!=0) {
						vRevTopMenu(top_choice);
						top_choice=0;
						vRevTopMenu(top_choice);
						return -10;
					}
					break;
				case ALT_X :
					return -1;
			}
		}
	}
}

void error2(BYTE *str)
{
  vOpenDlgWindow(6, 30, "µAœá");
	vPutString(MaxX/2-2*8+20, MaxY/16-14, str, 0);
	getch();
	vCloseDlgWindow(6, 30);
}

void save_drv(void)
{
	int i;

	for(;;) {
		i=get_key();
		if(i==-10) return;
		if(i==-1) vExit();
		if(i>0) {
			if(!data_to_cfg("21.CNF")) {
				vOpenDlgWindow(4, 30, "Error");
				vPutString(MaxX/2-2*8, MaxY/16-14, " Cannot make 21.CNF!! ", 0);
				getch();
				vCloseDlgWindow(4, 30);
			}
			vExit();
		} else if(i<0) {
			return;
		}
	}
}

void quit_drv(void)
{
	int i;

	for(;;) {
		i=get_key();
		if(i>-2) {
			vExit();
		} else if(i<-1) {
			return;
		}
	}
}

void setup_drv(void)
{
	vDrawTopMenu();
	vRevTopMenu(0);
	for(;;) {
		switch(top_choice) {
			case -1 :
				vExit();
				break;
			case  0 :
				iInputMenuDrv();
				break;
			case  1 :
				dir_drv();
				break;
			case  2 :
				iPrintDrv();
				break;
			case  3 :
				mouse_drv();
				break;
			case  4 :
				font_drv();
				break;
			case  5 :
				colour_drv();
				// set screen default color
				break;
			case  6 :
				save_drv();
				break;
			case  7 :
				quit_drv();
				break;
		}
	}
}

extern int oldx, oldy;




int iCBreakHandler(void)
{
	return(1);
}

void cover_ctrlc(void)
{
	union REGS r;

	r.h.ah=0x33;
	r.h.al=1;
	r.h.dl=0;
	int86(0x21, &r, &r);
}

BYTE bCheckFile(BYTE *buf)
{
  struct ffblk ffblk;
	return findfirst(buf, &ffblk, FA_ARCH | FA_RDONLY);
}

BYTE g_drive[MAXDRIVE];
BYTE g_dir[80];
BYTE g_file[MAXFILE];
BYTE g_ext[MAXEXT];

void main(int argc, char *argv[])
{
	int num;
	BYTE *cnf_path;
	BYTE inf_path[80];

	ctrlbrk(iCBreakHandler);
	cover_ctrlc();
	graph_mode=1;
	our_buf=(BYTE *)malloc(38400l);
	if(!our_buf) {
		printf("Not enough memory!!\n");
		exit(-1);
	}
	if(bCheckFile("PRNDRV.INF")!=0) {
		fnsplit(argv[0], g_drive, g_dir, g_file, g_ext);
		if(*g_dir) {
			cnf_path=malloc(80);
			sprintf(cnf_path, "%s%sPRNDRV.INF", g_drive, g_dir);
		} else cnf_path=searchpath("PRNDRV.INF");
		strcpy(inf_path, cnf_path);
	} else {
		getcwd(g_dir, 80);
		sprintf(inf_path, "%s\\PRNDRV.INF", g_dir);
	}
	if(!cfg_to_data("21.CNF")) {
		printf("\nHave a nice dos!\n");
		exit(-1);
	}
	num=get_mode(graph_mode-1)+1;
	if(num==0 || num==11) exit(0);
	iInitHangul((BYTE)num);
	if(color_mode) {
		if(graph_mode<VGA640) White=15;
		else White=63;
		set_palette_int(0, curc);
		set_palette_int(White, fontc);
	} else {
		setmem(curc, 6, 0);
		fontc[0]=fontc[1]=fontc[2]=63;
	}
	vFillScreen(0xaaaa, 0x5555);
	put_set_board();
	vInitMenu();
	num=iGetPrinterDriver(inf_path);
	if(num==-1) {
		error2(" PRNDRV.INFÌa·©·¡ ´ô¯s“¡”a!");
	} else vSetMenu(SPAN_32 , 50, 20+5, num, pkind);
	if(psort>6) {
		vSetMenu(SPAN_317, 66, 20+5, psort, paper);
	}
	put_cur_set(100);

	init_tick();
	setup_drv();
}

