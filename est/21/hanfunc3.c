#define _HANFUNC3
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <conio.h>
#include <mem.h>
#include <alloc.h>
#include <dos.h>
#include <dir.h>
#include <stdarg.h>
#include "est.h"
#include "21.h"
#include "proto.h"
#include "hanfunc1.h"
#include "snum.h"
#include "kstbl.h"
#include "numbers.h"
#include "hidden.h"

#define EF_SIZE 1520
#define HF_SIZE 4992*2
#define JF_SIZE 5408
#define LOC_JUNG 2432
#define LOC_JONG 3264
#define MAXEFONT 96
#define MAXHFONT 256
#define MAXJFONT 169
#define FONTCACHESIZE 10240
#define HGEFTCACHESIZE 20480
#define CACHEMIN 5
#define JUL      7*19
#define JOL      7*19+21*2
#define BIT_0   0x80;
#define TT 100
#define T  1000
int maxjhan=4;
int maxchan=4;
#define maxeng  6
#define maxjp   2
#define maxhalf 5
#define maxfull 9


BYTE c_buf[16]={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x06,0x06,0x26,0x7E,0xFE,0x7C,0x20,0x00
};
BYTE l_buf[16]={
	0x00,0x00,0x02,0x02,0x06,0x06,0x0E,0x1E,
	0x3E,0x1E,0x0E,0x06,0x06,0x02,0x02,0x00,
};

BYTE *lf;
BYTE *cr;

BYTE space[]={44, 22, 46, 22};

BYTE char_icon[][32]={
 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x18,0x31,0xB8,
	0x01,0x98,0x03,0x18,0x0E,0x18,0x18,0x18,0x30,0x18,0x3F,0xBC,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0xCC,
	0x18,0xCC,0x38,0xDE,0x2C,0xCC,0x7C,0xCC,0x46,0xCC,0xC6,0xC6,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0x03,
	0xDB,0x03,0xC7,0xB7,0xC3,0x3B,0xC3,0x33,0xDB,0x33,0x71,0xB3,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6C,0x1E,
	0xCC,0x36,0xCC,0x36,0xEF,0x7F,0x6D,0xB6,0x6D,0xB6,0xCD,0xB3,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
 {0x00,0x00,0x00,0x00,0x00,0x00,0x79,0xFF,0x7B,0xFF,0x63,0x0C,
  0x63,0x0C,0x73,0x8C,0x71,0xCC,0x60,0xCC,0x60,0xCC,0x7F,0xCC,
  0x7F,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,}
};
BYTE char_icons[][16]={
{ 0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0C,
	0xD8,0x70,0x20,0x00,0x00,0x00,0x00,0x00},
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3C,
	0x3C,0x18,0x00,0x00,0x00,0x00,0x00,0x00},
{ 0x00, 0xFB, 0xDB, 0xDF, 0xDB, 0xFB, 0x00, 0xFB,
	0x1B, 0xFB, 0xC3, 0xFB, 0x00, 0x42, 0xFF, 0x42},
{ 0x00, 0xFB, 0xDB, 0xDF, 0xDB, 0xFB, 0x00, 0xFB,
	0x1B, 0xFB, 0xC3, 0xFB, 0x00, 0x40, 0xFF, 0x40},
{ 0x00, 0xFB, 0xDB, 0xDF, 0xDB, 0xFB, 0x00, 0xFB,
	0x1B, 0xFB, 0xC3, 0xFB, 0x00, 0x02, 0xFF, 0x02},
{ 0x00, 0xFF, 0x33, 0x33, 0x0C, 0xFF, 0x00, 0xFB,
	0x1B, 0xFB, 0xC3, 0xFB, 0x00, 0x42, 0xFF, 0x42},
{ 0x00, 0xFF, 0x33, 0x33, 0x0C, 0xFF, 0x00, 0xFB,
	0x1B, 0xFB, 0xC3, 0xFB, 0x00, 0x40, 0xFF, 0x40},
{ 0x00, 0xFF, 0x33, 0x33, 0x0C, 0xFF, 0x00, 0xFB,
	0x1B, 0xFB, 0xC3, 0xFB, 0x00, 0x02, 0xFF, 0x02},
};

BYTE bpage_no[32]={
	0xF8, 0x00, 0xCC, 0x00, 0xCC, 0x00, 0xFB, 0x77,
	0xC1, 0x54, 0xC7, 0x57, 0xC5, 0x54, 0xC7, 0x77,
	0x00, 0x10, 0xCC, 0x70, 0xCC, 0x00, 0xEC, 0x00,
	0xFD, 0xF0, 0xDD, 0xB0, 0xCD, 0xB6, 0xCD, 0xF6,
};

BYTE vpage_no[173]={
  0x56,
	0x84, 0x84, 0x04, 0x1B, 0x08,	0x1F, 0x13, 0x1F, 0x17, 0x1B, 0x17, 0x0B, 0x2B,	0x0B, 0x2B,	0x04,
	0x04, 0x04, 0x87, 0x0B, 0x07, 0x18, 0x09, 0x1A, 0x12, 0x1A, 0x14, 0x18, 0x14,	0x0B, 0x07, 0x0B,
	0xB4, 0x84, 0x34, 0x0B, 0x39, 0x0B, 0x50, 0x18, 0x34, 0x18, 0x34, 0x1F, 0x5B,	0x1F, 0x5B, 0x18,
	0x57, 0x18, 0x3F, 0x0B, 0x5B, 0x0B, 0x5B, 0x04, 0x34, 0x04, 0x9C, 0xA0, 0x1C,	0x2C, 0x17, 0x2C,
	0x17, 0x24, 0x14, 0x24, 0x14, 0x2F, 0x2B, 0x2F, 0x2B, 0x20, 0x1C, 0x20, 0x9F, 0x23, 0x1F,	0x2C,
	0x28, 0x2C, 0x28, 0x23, 0x1F, 0x23, 0x94, 0xB4, 0x14, 0x43, 0x37, 0x43, 0x37, 0x34, 0x34,	0x34,
	0x34, 0x40, 0x2B, 0x40, 0x2B, 0x34, 0x14, 0x34, 0x97, 0x37, 0x17, 0x40, 0x28,	0x40, 0x28,	0x37,
	0x17, 0x37, 0x94, 0xC8, 0x14, 0x57, 0x23, 0x57, 0x23, 0x4B, 0x28, 0x4B, 0x28,	0x57, 0x2B,	0x57,
	0x2B, 0x48, 0x14, 0x48, 0x97, 0x4B, 0x17, 0x54, 0x20, 0x54, 0x20, 0x4B, 0x17, 0x4B, 0xC4,	0xA8,
	0x44, 0x3F, 0x5B, 0x3F, 0x5B, 0x28, 0x44, 0x28, 0xC7, 0x2F, 0x47, 0x38, 0x58,	0x38, 0x58,	0x2F,
	0x47, 0x2F, 0xD4, 0xCC, 0x54, 0x53, 0x5B, 0x53, 0x5B, 0x4C, 0x54, 0x4C,
};



BYTE near vcho_type_1[]={0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
									 3, 3, 1, 2, 3, 3, 3, 2, 1, 3, 0};
BYTE near vcho_type_2[]={4, 4, 4, 4, 4, 4, 4, 4, 5, 6,
									 6, 6, 5, 5, 6, 6, 6, 5, 5, 6, 4};
BYTE near vjong_type[]={0, 1, 0, 1, 1, 1, 1, 1, 2, 0,
									 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1};
BYTE near vcho_tbl1[21]={
	0, 1, 0, 1, 0, 1, 0, 1, 2, 4, 4, 4, 2, 3, 5, 5, 5, 3, 2, 4, 0};
BYTE near vcho_tbl2[21]={
	6, 8, 6, 8, 6, 8, 6, 8, 9,11,11,11, 9,10,12,12,12,10, 9, 11, 6};
BYTE near vcho_tbl3[21]={
	7, 8, 7, 8, 7, 8, 7, 8, 9,11,11,11, 9,10,12,12,12,10, 9, 11, 7};
BYTE near vjung_tbl[11]={
	21, 0, 0, 0, 22, 23, 0, 0, 0, 24, 25};
BYTE near vjong_tbl[21]={
	0, 1, 0, 1, 0, 1, 0, 1, 2, 0, 1, 0, 2, 2, 0, 1, 0, 2, 2, 0, 0};
BYTE near vjong_tbl2[21]={
	0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 1, 0, 0, 0, 1, 1, 0, 0};

//BYTE near line_char[96][16];

extern BYTE *vid_addr;
extern WORD offsetx[];
extern WORD MaxX, MaxY;
extern int bminx, bminy, bmaxx, bmaxy;
extern int jminx, jminy, jmaxx, jmaxy;
extern text_window tw[];
extern BYTE cur_w;
extern BYTE key_flag;
extern BYTE g_dir[];
extern BYTE main_arr[];
extern BYTE exe_path[];
extern BYTE hanja_dic_name[];

BYTE disp_return=0;
BYTE disp_newline=0;
BYTE disp_space=0;
BYTE full_vector=0;
BYTE old_full=0;
BYTE jopaning=0;
BYTE printing=0;

BYTE *efont_name[]={
	"ASCIIM",
	"ASCIIG",
	"ASCIII",
	"ASCIIP",
	"ASCIIU",
	"ASCIID"
};

BYTE *hfont_name[]={
	"HANGULM",
  "HANGULG",
	"HANGULS",
	"HANGULP",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
};

BYTE hgeft_kind[14]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
WORD svfsize[6]={5200, 4800, 6000, 7100, 4800, 4800};
WORD wvfsize[5]={12200, 10200, 10000, 13500, 12000};
BYTE svfrent=0;
BYTE sfrent=0;
BYTE wvfrent=0;
BYTE wfrent=0;

BYTE *sf=NULL;
WORD *wf=NULL;

BYTE *svf=NULL;
WORD *svftbl;
WORD *gsvftbl[6];

BYTE *wvf=NULL;
WORD *wvftbl;
WORD *gwvftbl[5];

BYTE *gsf[6]={0, 0, 0, 0, 0, 0} ;
WORD *gwf[5]={0, 0, 0, 0, 0};

BYTE *gsvf[6]={0, 0, 0, 0, 0, 0};
BYTE *gwvf[5]={0, 0, 0, 0, 0};


BYTE *font_cache=0;
int fc_max=0;
WORD fc_loc=0;
BYTE *bfc;

BYTE *hfc=0;
int hfc_loc=0;
int hfc_max=0;
BYTE hfc_found=0;

BYTE *hgjeft=0;
WORD *hgjeft_tbl=0;
BYTE hgjeft_flag=1;

BYTE font_path[80];


BYTE xs=1;
BYTE ys=1;
int xunit=16;
int yunit=16;
int sx=0;
int xlength=96;
int bufwid=13;
int bufhigh=16;

int iPrintf(int x, int y, BYTE mode, BYTE *fmt, ...);
static void near (*vPutWordImage)(int x, int y, WORD *image, BYTE mode);
static void near (*vPutByteImage)(int x, int y, BYTE *image, BYTE mode);
static void near (*vPutFontImage)(int x, int y, BYTE eff);

int iReadBitmapFont(BYTE *font, char *fname, WORD size)
{
	int fi;

	sprintf(g_dir, "%s\\%s.FNT", font_path, fname);
	if((fi = open(g_dir, O_BINARY | O_RDONLY)) == -1) {
		vErrorMsgfDlg(0, 1, fname);
		return 0;
	}
	read(fi, font, size);
	close(fi);
	return 1;
}

int iReadAsciiVectorFont(BYTE *efont, WORD *eftbl, char *e_name)
{
	int fi;
	WORD i, sum;
	BYTE c;

	sprintf(g_dir, "%s\\%s.VFT", font_path, e_name);
	if((fi = open(g_dir, O_BINARY | O_RDONLY)) == -1) {
		vErrorMsgfDlg(0, 1, e_name);
		return 0;
	}
	eftbl[0]=0;
	read(fi, &i, 2);
	for(i=1;i<MAXEFONT+1;i++) {
		read(fi, &c, 1);
		eftbl[i]=c;
	}
	for(i=1, sum=0;i<MAXEFONT;i++) {
		read(fi, efont+sum, eftbl[i]*2);
		sum+=eftbl[i]*2;
		eftbl[i]=sum;
	}
	close(fi);
	return 1;
}

int iReadHanJohapVectorFont(BYTE *efont, WORD *eftbl, char *e_name )
{
	int fi;
	WORD i, sum;
	BYTE c;

	sprintf(g_dir, "%s\\%s.VFT", font_path, e_name);
	if((fi = open(g_dir, O_BINARY | O_RDONLY)) == -1) {
		vErrorMsgfDlg(0, 1, e_name);
		return 0;
	}
	eftbl[0]=0;
	for(i=1;i<MAXHFONT+1;i++) {
		read(fi, &c, 1);
		eftbl[i]=c;
	}
	for(i=1, sum=0;i<MAXHFONT+1;i++) {
		read(fi, efont+sum, eftbl[i]*2);
		sum+=eftbl[i]*2;
		eftbl[i]=sum;
	}
	close(fi);
	return 1;
}

static int near iTG2KSIndex(WORD x)
{
	int mid, vLeftKey=0, vRightKey=2350;

	while(vRightKey - vLeftKey > 1) {
		mid=(vRightKey+vLeftKey)>>1;
		if(x <= KStbl[mid]) vRightKey = mid;
		else vLeftKey=mid;
	}
	if(x!=KStbl[vRightKey]) return 0;
	return vRightKey;
}

#include "vechan2.c"

BYTE near old_code2=0;
BYTE near old_vcode2=0;
static void near vPFF(int x, int y, BYTE *b5)
{
	WORD image[16];
	BYTE cho, jung, jong;
	BYTE cho_style, jung_style, jong_style;
	register WORD i;
	WORD *s1, *s2, *s3;
	BYTE effect, code2;
	CODE2 c;

	effect=*(b5+2) & 0x7f;
	code2=*(b5+4);
	c.c2[1]=*b5;
	c.c2[0]=*(b5+1);
	xs=*(b5+3)>>4;
	ys=*(b5+3) & 0x0f;
	if(xs>6 || ys>6) return;
	bufwid=((yunit*ys+7)>>3)+1;
	bufhigh=xunit*xs;

	if(code2 & BIT_2) { 
		if(code2 & BIT_3) { 
			if(code2 & BIT_4) { 
				hfcnum=code2 & 0x0f;
				if(hfcnum>=maxchan || full_vector==2 || hgeft_kind[hfcnum+maxjhan]==3)
					goto CANNOT;
				hfcode=iTG2KSIndex(c.i);
				if(!hfcode) {
CANNOT:
					hfcnum=0;
					code2=0;
					goto JOHAP;
				}
				hfcode--;
				if(xs+ys<CACHEMIN && !printing) hfcnum|=(xs<<6) | (ys<<4);
				if(!(bfc=(BYTE *)pGetHanFontCacheImage())) goto CANNOT;
				if(hfc_found) memcpy(main_arr, bfc, hfc_found);
				else {
					if(hfcnum>>4) {
						code2=full_vector;
						full_vector=1;
						vNewRasterize(bfc, hfcsize);
						full_vector=code2;
					}
					else vNewRasterize(bfc, hfcsize);
				}
				(*vPutFontImage)(x, y, effect);
				hfcnum=0;
				return;
			}
			else { 
JOHAP:
				code2 &= 0x0f;
				if(code2>=maxjhan) return;
				cho = table_0[c.c3.code1];
				jung = table_1[c.c3.code2];
				jong = table_2[c.c3.code3];
				if(xs+ys+xunit>18 && full_vector<2) {
					if(printing && code2==0 && yunit*ys>39 && hgjeft_flag) {
//					if(code2==0 && hgjeft_flag) {
						if(!hgjeft) {
							iFreeFontBuffer(1);
							hgjeft=(BYTE *)pAllocUMB((WORD)53000L);
							if(!hgjeft) {
								vErrorMsgfDlg(1, 0, 0);
								hgjeft_flag=0;
								goto VJOHAP;
							}
							hgjeft_tbl=(WORD *)pAllocUMB(500*2);
							if(!hgjeft_tbl) {
								vErrorMsgfDlg(1, 0, 0);
LOADERROR:
								vFreeUMB(hgjeft);
								hgjeft_flag=0;
								goto VJOHAP;
							}
							if(!iReadHanWansungVectorfont("HGMJ.EFT")) {
								vErrorMsgfDlg(0, 1, "HGMJ.EFT");
								vFreeUMB(hgjeft_tbl);
								goto LOADERROR;
							}
						}
						if(jung==0xff) {
							if(jong == 0xff) cho_style = 0;
							else	{
								cho_style=9;
								jong_style=3;
							}
						}
						else {
							if(jong==0xff) {
								cho_style=vcho_tbl1[jung];
								jung_style=0;
							}
							else {
								if(jong==3) {
									cho_style=vcho_tbl3[jung];
									jung_style=2;
								}
								else {
									cho_style=vcho_tbl2[jung];
									jung_style=1;
								}
								jong_style=vjong_tbl[jung];
							}
							if(vjong_tbl2[jung]) {
								if (cho==0 || cho==15 || (cho==1 && vjong_tbl2[jung]==1))
									jung=vjung_tbl[jung-8];
							}
						}
						if (cho != 0xff) {
							vNewRasterize(hgjeft+hgjeft_tbl[cho*13+cho_style],
													hgjeft_tbl[cho*13+cho_style+1]-
													hgjeft_tbl[cho*13+cho_style]);
						}
						if (jung != 0xff) {
							vNewRasterize(hgjeft+hgjeft_tbl[19*13+jung*3+jung_style],
													hgjeft_tbl[19*13+jung*3+jung_style+1]-
													hgjeft_tbl[19*13+jung*3+jung_style]);
						}
						if (jong != 0xff) {
							vNewRasterize(hgjeft+hgjeft_tbl[19*13+26*3+jong*3+jong_style],
													hgjeft_tbl[19*13+26*3+jong*3+jong_style+1]-
													hgjeft_tbl[19*13+26*3+jong*3+jong_style]);
						}
					}
					else {
VJOHAP:
						if(code2 != old_vcode2) {
							old_vcode2=code2;
							if(!gwvf[code2]) {
								gwvf[code2]=(BYTE *)pAllocUMB(wvfsize[code2]);
								if(!gwvf[code2]) {
									gwvf[code2]=gwvf[wvfrent];
									gwvftbl[code2]=gwvftbl[wvfrent];
									gwvf[wvfrent]=0;
									gwvftbl[wvfrent]=0;
									wvfrent=code2;
								}
								else {
									gwvftbl[code2]=(WORD *)pAllocUMB(MAXHFONT*2+2);
									if(!gwvftbl[code2]) {
										vFreeUMB(gwvf[code2]);
										gwvf[code2]=gwvf[wvfrent];
										gwvftbl[code2]=gwvftbl[wvfrent];
										gwvf[wvfrent]=0;
										gwvftbl[wvfrent]=0;
										wvfrent=code2;
									}
								}
								if(!iReadHanJohapVectorFont(gwvf[code2], gwvftbl[code2], hfont_name[code2])) return;
							}
							wvf=gwvf[code2];
							wvftbl=gwvftbl[code2];
						}
						if(key_flag && wCheckKeyHit()) return;
						if(jung==0xff) {
							if(jong == 0xff) cho_style = 0;
							else  cho_style=jong_style=0;
						}
						else {
							if(jong==0xff) {
								cho_style = vcho_type_1[jung];
								jung_style=0;
							}
							else {
								cho_style = vcho_type_2[jung];
								jong_style = vjong_type[jung];
								jung_style=1;
							}
						}
						if (cho != 0xff)
							vRasterize(wvf+wvftbl[cho_style*19+cho],
								wvftbl[cho_style*19+cho+1]-wvftbl[cho_style*19+cho]);
						if (jung != 0xff)
							vRasterize(wvf+wvftbl[JUL+jung_style*21+jung],
								wvftbl[JUL+jung_style*21+jung+1]-
								wvftbl[JUL+jung_style*21+jung]);
						if (jong != 0xff)
							vRasterize(wvf+wvftbl[JOL+jong_style*27+jong],
								wvftbl[JOL+jong_style*27+jong+1]-
								wvftbl[JOL+jong_style*27+jong]);
					}
					(*vPutFontImage)(x, y, effect);
				}
				else {
					if(code2 != old_code2) {
						old_code2=code2;
						if(!gwf[code2]) {
							gwf[code2]=(WORD *)pAllocUMB(HF_SIZE);
							if(!gwf[code2]) {
								gwf[code2]=gwf[wfrent];
								gwf[wfrent]=0;
								wfrent=code2;
							}
							if(!iReadBitmapFont((BYTE *)gwf[code2], hfont_name[code2], HF_SIZE)) return;
						}
						wf=gwf[code2];
					}
					if(jung==0xff) {
						if(jong == 0xff) cho_style = 0;
						else  cho_style=jong_style=0;
					}
					else {
						if(jong==0xff) {
							cho_style = cho_type_1[jung];
							if(cho!=0 && cho!=15 && 7<jung && 13>jung) jung+=13;
							jung_style=0;
						}
						else {
							cho_style = cho_type_2[jung];
							jong_style = jong_type[jung];
							if(cho!=0 && cho!=15 && 7<jung && 13>jung) jung+=13;
							jung_style=1;
						}
					}
					if(xs+ys<3) {
						if (cho != 0xff) s1=(wf+cho_style*19*16+cho*16);
						else s1=null_wimage;
						if (jung != 0xff) s2=(wf+LOC_JUNG+jung_style*26*16+jung*16);
						else s2=null_wimage;
						if (jong != 0xff) s3=(wf+LOC_JONG+jong_style*27*16+jong*16);
						else s3=null_wimage;
						for(i=0;i<16;i++) image[i]=s1[i] | s2[i] | s3[i];
						(*vPutWordImage)(x, y, image, effect);
					}
					else {
						if (cho != 0xff)
							for(i=0;i<16;i++) {
								*((WORD *)(main_arr+i*bufwid))=*(wf+cho_style*19*16+cho*16+i);
							}
						if (jung != 0xff)
							for(i=0;i<16;i++) {
								*((WORD *)(main_arr+i*bufwid))|=*(wf+LOC_JUNG+jung_style*26*16+jung*16+i);
							}
						if (jong != 0xff)
							for(i=0;i<16;i++) {
								*((WORD *)(main_arr+i*bufwid))|=*(wf+LOC_JONG+jong_style*27*16+jong*16+i);
							}
						(*vPutFontImage)(x, y, effect);
					}
				}
			}
		}
		else { 
			fcp4=0;
			if(code2 & BIT_4) {  
				if((code2 & 0x0f)>=maxjp) return;
				fcp1=JPB;
				fcp2=code2;
				fcp3=*(b5+1)-1;
			}
			else {
				if((code2 & 0x0f)>=maxfull) return;
				fcp1=FSB;
				fcp2=code2;
				fcp3=(*(b5+1) & 0x7f)-32;
			}
			goto PUTCACHE;
		}
	}
	else { 
		if(b5[0]==0xff && b5[1]==0xff) {
			if(xs+ys>2 && full_vector<2) {
				bfc=vpage_no;
				goto PUTVFONT;
			}
			else {
				bfc=bpage_no;
				goto PUTBFONT;
			}
		}
		else {
			fcp1=HJB;
			fcp2=code2 & 0x3f;
			fcp3=b5[0];
			fcp4=b5[1];
		}
PUTCACHE:
		if(xs+ys+xunit>18 && full_vector<2) {
			fcp1+=BIT2VEC;
			bfc=pCheckFontCacheImage();
PUTVFONT:
			if(bfc) vRasterize(bfc+1, (WORD)(*bfc)<<1);
			(*vPutFontImage)(x, y, effect);
			return;
		}
		else {
			bfc=pCheckFontCacheImage()+1;
PUTBFONT:
			if(xs+ys<3) {
				(*vPutWordImage)(x, y, (WORD *)(bfc), effect);
			}
			else {
				for(i=0;i<16;i++) {
					*(main_arr+i*bufwid)=*(bfc+i*2);
					*(main_arr+i*bufwid+1)=*(bfc+i*2+1);
				}
				(*vPutFontImage)(x, y, effect);
			}
		}
	}
}

BYTE near old_scode2=0;
BYTE near old_svcode2=0;
static void near vPHF(int x, int y, BYTE *b4)
{
	BYTE code1, effect, code2;
	BYTE loc;
	WORD i;
	BYTE *image;

	code1=*b4;
	effect=*(b4+1) & 0x7f;
	code2=*(b4+3);
	xs=*(b4+2)>>4;
	ys=*(b4+2) & 0x0f;
	if(xs>6 || ys>6) return;
	bufwid=((yunit*ys+15)>>4)+1;
	bufhigh=xunit*xs;

	if(code1<32) {
		i=*(((WORD *)page_numbers)+code1);
		vNewRasterize(page_numbers+i,	*(((WORD *)page_numbers)+code1+1)-i);
		(*vPutFontImage)(x, y, effect);
	}
	else if(code2 & BIT_2) {
		code2 &= 0x0f;
		if(code2>=maxeng) {
			(*vPutByteImage)(x, y, null_image, effect);
			return;
    }
		if(xs+ys+xunit>18 && full_vector<2) {
			if(code2!=old_svcode2) {
				old_svcode2=code2;
				if(!gsvf[code2]) {
					gsvf[code2]=(BYTE *)pAllocUMB(svfsize[code2]);
					if(!gsvf[code2]) {
						gsvf[code2]=gsvf[svfrent];
						gsvftbl[code2]=gsvftbl[svfrent];
						gsvf[svfrent]=0;
						gsvftbl[svfrent]=0;
						svfrent=code2;
					}
					else {
						gsvftbl[code2]=(WORD *)pAllocUMB(MAXEFONT*2+2);
						if(!gsvftbl[code2]) {
							vFreeUMB(gsvf[code2]);
							gsvf[code2]=gsvf[svfrent];
							gsvftbl[code2]=gsvftbl[svfrent];
							gsvf[svfrent]=0;
							gsvftbl[svfrent]=0;
							svfrent=code2;
						}
					}
					if(!iReadAsciiVectorFont(gsvf[code2], gsvftbl[code2], efont_name[code2])) return;
				}
				svf=gsvf[code2];
				svftbl=gsvftbl[code2];
			}
			loc=code1-32;
			if(key_flag && wCheckKeyHit()) return;
			if(loc==0) {
        if(disp_space) {
					vRasterize(space, 4);
        }
				(*vPutFontImage)(x, y, effect);
			}
			else {
				vRasterize(svf+svftbl[loc], svftbl[loc+1]-svftbl[loc]);
				(*vPutFontImage)(x, y, effect);
			}
		}
		else {
			if(code2!=old_scode2) {
				old_scode2=code2;
				if(!gsf[code2]) {
					gsf[code2]=(BYTE *)pAllocUMB(EF_SIZE);
					if(!gsf[code2]) {
						gsf[code2]=gsf[sfrent];
						gsf[sfrent]=0;
						sfrent=code2;
					}
					if(!iReadBitmapFont(gsf[code2], efont_name[code2], EF_SIZE)) return;
				}
				if(disp_space) *(gsf[code2]+7)=0x10;
				sf=gsf[code2];
			}
			if(code1>31) {
				loc=code1-32;
				image=(sf+loc*16);
			}
			if(xs+ys<3) (*vPutByteImage)(x, y, image, effect);
			else {
				for(i=0;i<16;i++) main_arr[i*bufwid]=*(image+i);
				(*vPutFontImage)(x, y, effect);
			}
		}
	}
	else {
		if((code2 & 0x0f)>=maxhalf) return;
		fcp2=code2;
		fcp3=code1-32;
		if(code2 & 0x10) fcp3+=96;
		fcp4=0;
		if(xs+ys+xunit>18 && full_vector<2) {
			fcp1=HSV;
			bfc=pCheckFontCacheImage();
			vRasterize(bfc+1, (WORD)(*bfc)*2);
			(*vPutFontImage)(x, y, effect);
		}
		else {
			fcp1=HSB;
			bfc=pCheckFontCacheImage()+1;
			if(xs+ys<3) {
				(*vPutByteImage)(x, y, bfc, effect);
			}
			else {
				for(i=0;i<16;i++) {
					*(main_arr+i*bufwid)=*(bfc+i);
				}
				(*vPutFontImage)(x, y, effect);
			}
		}
	}
}

void vClipOff(void)
{
	bminx=bminy=-1;
	bmaxx=MaxX;
	bmaxy=MaxY;
}

extern BYTE show_rod;

void vClipOn(BYTE n)
{
  if(show_rod) bminx=tw[n].x+51;
	else bminx=tw[n].x+51-14;
  bminy=(tw[n].y<<3)+7;
	bmaxx=tw[n].x+(tw[n].lx<<3)-4;
  bmaxy=((tw[n].y+tw[n].ly)<<3)-16;
}

void vPFFfar(int x, int y, BYTE *b)
{
	vClipOff();
	vPFF(x, y, b);
	vClipOn(cur_w);
}

void vPHFfar(int x, int y, BYTE *b)
{
	vClipOff();
	vPHF(x, y, b);
	vClipOn(cur_w);
}

int iPutGraphicFont(int x, int y, BYTE b, BYTE mode)
{
	if((b & 0xf0)==0xe0) {
		b=b & 0x0f;
		vPutImage16x16(x, y, (WORD *)char_icon[b], mode);
		return 2;
  }
  else {
		b=b & 0x0f;
		vPutImage16x8(x, y, char_icons[b], mode);
		return 1;
	}
}

BYTE near r_size;

void vEraseRestArea(int x, int y, BYTE cur_size, BYTE width)
{
	int s=0, e;

	cur_size=r_size-cur_size;
	e=x+cur_size-1;
	s=x;
	if(bminx>s) s=bminx;
	else if(bmaxx<e) e=bmaxx;
	if(y<(bminy>>3)+1) {
		width=(bminy>>3)+1-y;
		y=(bminy>>3)+1;
	}
	else if(y+width>(bmaxy>>3)) width=(bmaxy>>3)-y;
	vBarByte(s, y, e, y+width, 0);
}

BYTE *psstr;

void vPT(int x, int y, BYTE flag)
{
	int h=0, v;
	register int tx;
	int sx, ex;

	if(bminx>x+flag || bmaxx<=x) return;
	ex=x+flag;
	sx=x;
	if(sx<bminx+1) sx=bminx;
	else if(ex>bmaxx) ex=bmaxx;
	if(y>(bminy>>3)+1) vBarByte(sx, (bminy>>3)+1, ex, y, 0);
	r_size=flag;
	x+=flag;
	while(1) {
		if(key_flag && wCheckKeyHit()) return;
		if(*psstr>127) {
			if(*(psstr+2)>127 && h) {
				x-=16;
				if(bminy>>3<y && bmaxy>>3>=y+1 && x<bmaxx) {
					if(flag>16) vEraseRestArea(x-flag+16, y, 16, 1);
					vPutImage16x8(x, y++, lf, 0);
				}
				break;
			}
			v=((*(psstr+3) & 0x0f)<<1);
			h=*(psstr+3) & 0xf0;
      tx=x-h;
      if(bminy>>3<y+v-1 && bmaxy>>3>y ) {
				if(flag>16) vEraseRestArea(x-flag, y, h, v);
				if(tx<bmaxx) vPFF(tx, y, psstr);
			}
      y+=v;
			psstr+=5;
    }
		else {
			if(*psstr==0) {
				if(bminy>>3<y && bmaxy>>3>=y+1 && x-flag<bmaxx)
					vEraseRestArea(x-flag, y, 0, 1);
				y++;
				psstr++;
				continue;
			}
			if(*psstr==13) {
				x-=16;
				if(bminy>>3<y && bmaxy>>3>=y+1 && x<bmaxx) {
          if(flag>16) vEraseRestArea(x-flag+16, y, 16, 1);
          vPutImage16x8(x, y++, cr, 0);
        }
				break;
			}
      if(*(psstr+1)>127 && h) {
				x-=16;
        if(bminy>>3<y && bmaxy>>3>=y+1 && x<bmaxx) {
					if(flag>16) vEraseRestArea(x-flag+16, y, 16, 1);
					vPutImage16x8(x, y++, lf, 0);
				}
        break;
      }
      v=(*(psstr+2) & 0x0f);
			h=*(psstr+2) & 0xf0;
      tx=x-h;
      if(bminy>>3<y+v-1 && bmaxy>>3>y  ) {
				if(flag>16) vEraseRestArea(x-flag, y, h, v);
				if(tx<bmaxx) vPHF(tx, y, psstr);
      }
      y+=v;
      psstr+=4;
		}
	}
	if(bmaxy>>3>y) {
		if(bminy>>3>=y) y=(bminy>>3)+1;
		vBarByte(sx, y, ex, bmaxy>>3, 0);
	}
}


BYTE bcc[5];

void vBPT(int x, int y, int sl, int el, BYTE flag)
{
  int h=0, v;
  register int tx;
	int sx, ex;

	if(bminx>x+flag || bmaxx<=x) return;
	ex=x+flag;
	sx=x;
	if(sx<bminx+1) sx=bminx;
	else if(ex>bmaxx) ex=bmaxx;
	if(y>(bminy>>3)+1) vBarByte(sx, (bminy>>3)+1, ex, y, 0);
	r_size=flag;
	x+=flag;
	while(1) {
		if(key_flag && wCheckKeyHit()) return;
		if(*psstr>127) {
			if(*(psstr+2)>127 && h) {
				x-=16;
        if(bminy>>3<y && bmaxy>>3>=y+1 && x<bmaxx) {
					if(flag>16) vEraseRestArea(x-flag+16, y, 16, 1);
          if(y>=sl && y<el) vPutImage16x8(x, y++, lf, 4);
          else vPutImage16x8(x, y++, lf, 0);
				}
        break;
      }
			v=((*(psstr+3) & 0x0f)<<1);
      h=*(psstr+3) & 0xf0;
			tx=x-h;
      if(bminy>>3<y+v-1 && bmaxy>>3>y ) {
        if(flag>16) vEraseRestArea(x-flag, y, h, v);
				if(y>=sl && y<el) {
          memcpy(bcc, psstr, 5);
					bcc[2]^= 0x04;
					if(tx<bmaxx) vPFF(tx, y, bcc);
        }
				else if(tx<bmaxx) vPFF(tx, y, psstr);
			}
			y+=v;
      psstr+=5;
    }
    else {
			if(*psstr==0) {
				if(bminy>>3<y && bmaxy>>3>=y+1 && x-flag<bmaxx) {
          if(y>=sl && y<el) {
						vEraseRestArea(x-flag, y, 16, 1);
						vPutImage16x8(x-16, y, null_image, 4);
          }
					else vEraseRestArea(x-flag, y, 0, 1);
        }
				y++;
        psstr++;
        continue;
			}
      if(*psstr==13) {
				x-=16;
				if(bminy>>3<y && bmaxy>>3>=y+1 && x<bmaxx) {
          if(flag>16) vEraseRestArea(x-flag+16, y, 16, 1);
          if(y>=sl && y<el) vPutImage16x8(x, y++, cr, 4);
          else vPutImage16x8(x, y++, cr, 0);
        }
				break;
      }
			if(*(psstr+1)>127 && h) {
				x-=16;
        if(bminy>>3<y && bmaxy>>3>=y+1 && x<bmaxx) {
					if(flag>16) vEraseRestArea(x-flag+16, y, 16, 1);
          if(y>=sl && y<el) vPutImage16x8(x, y++, lf, 4);
					else vPutImage16x8(x, y++, lf, 0);
				}
        break;
      }
			v=(*(psstr+2) & 0x0f);
			h=*(psstr+2) & 0xf0;
			tx=x-h;
			if(bminy>>3<y+v-1 && bmaxy>>3>y ) {
        if(flag>16) vEraseRestArea(x-flag, y, h, v);
        if(y>=sl && y<el) {
          memcpy(bcc, psstr, 4);
					bcc[1]^= 0x04;
					if(tx<bmaxx) vPHF(tx, y, bcc);
        }
        else if(tx<bmaxx) vPHF(tx, y, psstr);
			}
      y+=v;
      psstr+=4;
		}
  }
	if(bmaxy>>3>y) {
		if(bminy>>3>=y) y=(bminy>>3)+1;
		vBarByte(sx, y, ex, bmaxy>>3, 0);
	}
}

void vSPT(int x, int y, BYTE flag)
{
	int h=0, v;
  register int tx;
  int tg=0;

	if(jminx>x+flag || jmaxx<=x) return;
  x+=flag;
	while(1) {
    if(key_flag && wCheckKeyHit()) return;
    if(*psstr>127) {
      if(*(psstr+2)>127 && h) return;
      v=((*(psstr+3) & 0x0f)<<1);
      h=*(psstr+3) & 0xf0;
      tx=x-h;
			if(jminy>>3<y+tg/12+v-1 && jmaxy>>3>y+tg/12 && tx<jmaxx)
        vPFF(tx, y+tg/12, psstr);
			tg+=tw[cur_w].word_gap*v;
      y+=v;
      psstr+=5;
		}
		else {
			if(*psstr==0) {
        y++;
				psstr++;
				tg+=tw[cur_w].word_gap;
        continue;
      }
			if(*psstr==13) return;
      if(*(psstr+1)>127 && h) return;
      v=(*(psstr+2) & 0x0f);
      h=*(psstr+2) & 0xf0;
      tx=x-h;
			if(jminy>>3<y+tg/12+v-1 && jmaxy>>3>y+tg/12 && tx<jmaxx && *psstr!=32)
        vPHF(tx, y+tg/12, psstr);
      tg+=tw[cur_w].word_gap*v;
      y+=v;
			psstr+=4;
    }
  }
}

void vChangePutMode(BYTE flag)
{
  if(flag==0) {
		vPutWordImage=vPutSmallImage16x16;
    vPutByteImage=vPutSmallImage16x8;
		vPutFontImage=vPutSmallRastorImage;
		old_full=full_vector;
    full_vector=2;
    jopaning=ON;
  }
	else if(flag==1) {
		setmem(main_arr, main_arr_SIZE, 0);
    vPutWordImage=vPutImage16x16;
    vPutByteImage=vPutImage16x8;
		vPutFontImage=vPutRastorImage;
    full_vector=old_full;
		jopaning=OFF;
  }
}

BYTE clipping=OFF;
BYTE sb4[4]={0x00, 0x00, 0x11, 0x40};
BYTE sb5[5]={0x00, 0x00, 0x00, 0x11, 0xe0};

void vWriteString(int x, int y, BYTE *str, BYTE mode)
{
	WORD str_len;
	BYTE temp;
	BYTE tj;
	register WORD i;
	int txs, tys, tbl, tbw;
	int old_prn=0, oldxu, oldyu, oldsx, oldlen;

	if(printing) {
		old_prn=1;
		printing=0;
		oldxu=xunit;
		oldyu=yunit;
		oldsx=sx;
		oldlen=xlength;
		sx=0;
		xlength=96;
		xunit=yunit=16;
	}
	txs=xs;
	tys=ys;
	tbl=bufhigh;
	tbw=bufwid;
	if(!clipping) vClipOff();
	tj=jopaning;
	if(tj) vChangePutMode(1);
	temp=*(sf+7);
	*(sf+7)=0;
	sb4[1]=sb5[2]=mode;
	str_len=strlen((BYTE *)str);
	for(i=0 ; i<str_len ;) {
		sb4[0]=sb5[0]=str[i++];
		if(sb4[0]<127) {
			if(sb4[0]==32) vPutImage16x8(x, y+i-1, null_image, mode);
			else vPHF(x, y+i-1, sb4);
		}
		else {
			sb5[1]=str[i++];
			if(sb5[0]==0xff) {
				if(iPutGraphicFont(x, y+i-2, sb5[1], mode)==1) y--;
			}
			else vPFF(x, y+i-2, sb5);
		}
	}
	if(!clipping) vClipOn(cur_w);
	*(sf+7)=temp;
	if(tj) vChangePutMode(0);
	xs=txs;
	ys=tys;
	bufhigh=tbl;
	bufwid=tbw;
	if(old_prn) {
		printing=1;
		xunit=oldxu;
		yunit=oldyu;
		sx=oldsx;
		xlength=oldlen;
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
   vWriteString(x, y, buffer, mode);
   return(cnt);
}

void vSetDispOpt(BYTE sp_flag, BYTE lf_flag, BYTE cr_flag)
{
	int i;

  disp_space=sp_flag;
  disp_newline=lf_flag;
  disp_return=cr_flag;
  if(sp_flag==1) {
    *(sf+7)=0x10;
		for(i=0;i<6;i++)
      if(gsf[i]!=NULL) *(gsf[i]+7)=0x10;
  }
  else {
    *(sf+7)=0;
    for(i=0;i<6;i++)
      if(gsf[i]!=NULL) *(gsf[i]+7)=0;
  }
  if(lf_flag) lf=l_buf;
  else lf=null_image;
  if(cr_flag) cr=c_buf;
	else cr=null_image;
}

void vPutBitmapHanjaTable(int x, int y, WORD *buf, BYTE count)
{
  WORD i;

  vClipOff();
  for(i=0;i<count;i++) {
		vPutImage16x16(x, y+i*3, buf+(i<<4), 0);
  }
	vClipOn(cur_w);
}

int iSetFontTable(BYTE *fname, BYTE *hname[], BYTE *ename[], BYTE *kind)
{
	FILE *fp;
	BYTE buf[80];
	int i;

	if((fp=fopen(fname, "rt"))==NULL) return -1;
	for(i=0;i<14;i++) {
		fgets(buf, 80, fp);
		if(buf[0]==':') break;
		sscanf(buf, "%s %s %d", hname[i], ename[i], &(kind[i]));
		if(hname[i][0]=='3') kind[i]=3;
		*(hname[i]+2+strlen(hname[i]+2))=32;
	}
	fclose(fp);
	return i;
}

extern BYTE *H_font[];

int iSetHangulFontTable(void)
{
	sprintf(g_dir, "%s\\HGEFT.INF", exe_path);
	maxchan=iSetFontTable(g_dir, H_font+1, hfont_name, hgeft_kind);
	for(maxjhan=0;;maxjhan++) {
		if(hgeft_kind[maxjhan]) break;
	}
	if(maxjhan<1) {
		printf("Unusable HGEFT.INF or %S\\HG*.EFT!!\n", font_path);
		return 0;
	}
	return 1;
}

int iIsFont(BYTE *name)
{
	sprintf(g_dir, "%s\\%s.VFT", font_path, name);
	if(bCheckFile(g_dir)!=0) {
		printf("%s is not found!\n", g_dir);
		return 0;
	}
  sprintf(g_dir, "%s\\%s.FNT", font_path, name);
		if(bCheckFile(g_dir)!=0) {
		printf("%s is not found!\n", g_dir);
		return 0;
	}
	return 1;
}

WORD wGetFontSize(char *name)
{
	struct ffblk ffblk;

	sprintf(g_dir, "%s\\%s.VFT", font_path, name);
	findfirst(g_dir, &ffblk, FA_ARCH);
	return (WORD) ffblk.ff_fsize;
}

int iCheckFont(void)
{
	int i;
	int max=0;

	if(!iSetHangulFontTable()) return 0;
	for(i=0;i<6;i++) { // ascii
		if(!iIsFont(efont_name[i])) return 0;
		svfsize[i]=wGetFontSize(efont_name[i]);
		if(max<svfsize[i]) max=svfsize[i];
	}
	svfsize[0]=max;
	for(i=0;i<maxchan;i++) { // hangul
		if(hgeft_kind[i]==0) {
			if(!iIsFont(hfont_name[i])) return 0;
			wvfsize[i]=wGetFontSize(hfont_name[i]);
			if(max<wvfsize[i]) max=wvfsize[i];
		}
		else if(hgeft_kind[i]==1) {
			sprintf(g_dir, "%s\\%s.EFT", font_path, hfont_name[i]);
			if(bCheckFile(g_dir)!=0) {
				printf("Cannot found %s!!!\n", g_dir);
				return 0;
			}
		}
	}
	wvfsize[0]=max;
	if(!iIsFont("JAPAN")) return 0;
	if(!iIsFont("FULL")) return 0;
	if(!iIsFont("HALF")) return 0;
//	if(!iIsFont("HANJA")) return 0;
	sprintf(g_dir, "%s\\HAN2.KBD", font_path);
	if(bCheckFile(g_dir)!=0) {
		printf("%s is not found!\n", g_dir);
		return 0;
	}
  sprintf(g_dir, "%s\\HAN3.KBD", font_path);
  if(bCheckFile(g_dir)!=0) {
		printf("%s is not found!\n", g_dir);
		return 0;
	}
	if(bCheckFile(hanja_dic_name)!=0) {
		printf("%s is not found!\n", hanja_dic_name);
		printf("Trying to make it!!!\n");
		if(iMakeNullHanjaDic()==0) {
			printf("Cannot make %s!!!\n", hanja_dic_name);
			return 0;
		}
		else printf("%s is Made successful!!!\n", hanja_dic_name);
	}
	return 1;
}

int iInitDefaultFont(void)
{
	int num;

	if(gsf[0]==0){
		gsf[0]=(BYTE *)pAllocUMB(EF_SIZE);
		if(!gsf[0]) {
			printf("Not enough memory!!\n");
			return 0;
		}
		if(!iReadBitmapFont(gsf[0], efont_name[0], EF_SIZE)) {
			printf("Error reading ascii font!!\n");
			return 0;
		}
		sf=gsf[0];
	}

	if(gwf[0]==0) {
		gwf[0]=(WORD *)pAllocUMB(HF_SIZE);
		if(!gwf[0]) {
			printf("Not enough memory!!\n");
			return 0;
		}
		if(!iReadBitmapFont((BYTE *)gwf[0], hfont_name[0], HF_SIZE)) {
			printf("Error reading hangul font!!\n");
			return 0;
		}
		wf=gwf[0];
	}

	num=sb4[3] & 0x0f;
	if(num && gsf[num]==0) {
		gsf[num]=(BYTE *)pAllocUMB(EF_SIZE);
		if(!gsf[num]) {
			printf("Not enough memory!!\n");
			return 0;
		}
		if(!iReadBitmapFont(gsf[num], efont_name[num], EF_SIZE)) {
			printf("Error reading ascii font!!\n");
			return 0;
		}
		sf=gsf[num];
	}
	num=sb5[4] & 0x0f;
	if(num && gwf[num]==0) {
		gwf[num]=(WORD *)pAllocUMB(HF_SIZE);
		if(!gwf[num]) {
			printf("Not enough memory!!\n");
			return 0;
		}
		if(!iReadBitmapFont((BYTE *)gwf[num], hfont_name[num], HF_SIZE)) {
			printf("Error reading hangul font!!\n");
			return 0;
		}
		wf=gwf[num];
	}

	if(gsvf[0]==0) {
		gsvf[0]=(BYTE *)pAllocUMB(svfsize[0]);
		gsvftbl[0]=(WORD *)pAllocUMB(MAXEFONT*2+2);
		if(!gsvftbl[0] || !gsvf[0]) {
			printf("Not enough memory!!\n");
			return 0;
		}
		if(!iReadAsciiVectorFont(gsvf[0], gsvftbl[0], efont_name[0])) {
			printf("Error reading ascii Vector font!!\n");
			return 0;
		}
		svftbl=gsvftbl[0];
		svf=gsvf[0];
	}
	if(gwvf[0]==0) {
		gwvf[0]=(BYTE *)pAllocUMB(wvfsize[0]);
		gwvftbl[0]=(WORD *)pAllocUMB(MAXHFONT*2+2);
		if(!gwvftbl[0] || !gwvf[0]) {
			printf("Not enough memory!!\n");
			return 0;
		}
		if(!iReadHanJohapVectorFont(gwvf[0], gwvftbl[0], hfont_name[0])) {
			printf("Error reading hangul Vector font!!\n");
			return 0;
		}
		wvftbl=gwvftbl[0];
		wvf=gwvf[0];
	}
	return 1;
}

int iInitHangul(BYTE gmode)
{
	old_full=full_vector;
	vChangePutMode(1);
	setmem(main_arr, main_arr_SIZE, 0);

	if(!iCheckFont()) return 0;
	if(!iInitDefaultFont()) return 0;
	if(iInitGraphMode(gmode, 0)!=1) {
		printf("\nGraphics setup error!! \n");
		return 0;
	}
	return 1;
}

void vDeinitHangul(void)
{
	vDeinitGraphMode();
}

int iFreeFontBuffer(int s)
{
	int i, rtn=0;

	if(wfrent) {
		gwf[0]=gwf[wfrent];
		gwf[wfrent]=0;
		wfrent=0;
	}
	if(wvfrent) {
		gwvf[0]=gwvf[wvfrent];
		gwvftbl[0]=gwvftbl[wvfrent];
		gwvf[wvfrent]=0;
		gwvftbl[wvfrent]=0;
		wvfrent=0;
	}
	if(sfrent) {
		gsf[0]=gsf[sfrent];
		gsf[sfrent]=0;
		sfrent=0;
	}
	if(svfrent) {
		gsvf[0]=gsvf[svfrent];
		gsvftbl[0]=gsvftbl[svfrent];
		gsvf[svfrent]=0;
		gsvftbl[svfrent]=0;
		svfrent=0;
	}
	for(i=s;i<5;i++) {
		if(gwvf[i]) {
    	rtn=1;
			vFreeUMB(gwvftbl[i]);
			vFreeUMB(gwvf[i]);
			gwvftbl[i]=0;
			gwvf[i]=0;
		}
		if(gwf[i] && i!=(sb5[4] & 0x1f)) {
			rtn=1;
			vFreeUMB(gwf[i]);
			gwf[i]=0;
		}
	}
	for(i=s;i<6;i++) {
		if(gsvf[i]) {
			rtn=1;
			vFreeUMB(gsvf[i]);
			vFreeUMB(gsvftbl[i]);
			gsvf[i]=0;
			gsvftbl[i]=0;
		}
		if(gsf[i] && i!=(sb4[3] & 0x1f)) {
			rtn=1;
			vFreeUMB(gsf[i]);
			gsf[i]=0;
		}
	}
	old_scode2=old_svcode2=old_code2=old_vcode2=0;
	wf=gwf[0];
	wvf=gwvf[0];
	wvftbl=gwvftbl[0];
	sf=gsf[0];
	svf=gsvf[0];
	svftbl=gsvftbl[0];

	if(font_cache) {
		rtn=1;
		vFreeUMB(font_cache);
	}
	font_cache=0;
	fc_max=0;
	fc_loc=0;

	if(hfc) {
		rtn=1;
		vFreeUMB(hfc);
	}
	hfc=0;
	hfc_max=0;
	hfc_loc=0;

	if(hgjeft) {
		rtn=1;
		vFreeUMB(hgjeft);
	}
	hgjeft=0;
	if(hgjeft_tbl) {
		rtn=1;
		vFreeUMB(hgjeft_tbl);
	}
	hgjeft_tbl=0;
	hgjeft_flag=1;
	return rtn;
}

void vFreeAllUMB(void)
{
	int i;
	for(i=0;i<5;i++) {
		if(gwvf[i]) {
			vFreeUMB(gwvftbl[i]);
			vFreeUMB(gwvf[i]);
			gwvftbl[i]=0;
			gwvf[i]=0;
		}
		if(gwf[i]) {
			vFreeUMB(gwf[i]);
			gwf[i]=0;
		}
	}
	for(i=0;i<6;i++) {
		if(gsvf[i]) {
			vFreeUMB(gsvf[i]);
			vFreeUMB(gsvftbl[i]);
			gsvf[i]=0;
			gsvftbl[i]=0;
		}
		if(gsf[i]) {
			vFreeUMB(gsf[i]);
			gsf[i]=0;
		}
	}
	if(font_cache) vFreeUMB(font_cache);
	font_cache=0;
	fc_max=0;
	fc_loc=0;

	if(hfc) vFreeUMB(hfc);
	hfc=0;
	hfc_max=0;
	hfc_loc=0;

	if(hgjeft) vFreeUMB(hgjeft);
	hgjeft=0;
	if(hgjeft_tbl) vFreeUMB(hgjeft_tbl);
	hgjeft_tbl=0;
	hgjeft_flag=1;
}


void vSetImageStartLocation(int x)
{
	if(x!=0) sx=x-1;
	else sx=x;
}

void vSetUnit(int x, int y)
{
	xunit=x;
	yunit=y;
}

void vSetImageHeight(int len)
{
	xlength=len+2;
}

