/*-------------------------------------------------*/
/*-------------  e —e ·¡ : ¯¥ ¢… ¬ó ---------------*/
/*------------- HITEL ID : sms999   ---------------*/
/*-------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <dos.h>
#include <io.h>	//For Borland Compiler //
#include "KorHdr.h"

typedef enum {FALSE,TRUE} bool;

static int _hanjapoint =0;
static const char HNJFont[] ="HANJA.FNT";

/* •¡A§¡ Í¥Ëa ÉA·¡§iŠ¬÷ */
static const byte table[3][32]=
{
 {
   0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
  15, 16,17,18,19,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
 },
 {
  0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 6, 7, 8, 9,10,11,
  0, 0,12,13,14,15,16,17, 0, 0,18,19,20,21, 0, 0
 },
 {
   0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
  15,16, 0,17,18,19,20,21,22,23,24,25,26,27, 0, 0
 }
};

static const byte FontType[3][22]=
{
//static const char type1_no[22]=
	{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
	3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0
	},
//static const char type1_yes[22]=
	{
	5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7,
	7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5,
	},
//static const char _type3[22]=
	{
	0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0,
	2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1
	}
};

extern const byte dflt_KorFont   [360*32];
extern const byte dflt_SPCFont   [128*32];
extern const byte dflt_GRPFont [1503][32];
//extern const byte dflt_HNJFont [4888][32];
extern const byte dflt_EngFont  [256][16];

static byte first[8][20][32];     /* Ðe‹i· Á¡¬÷ */
static byte   mid[4][22][32];     /* Ðe‹i· º—¬÷ */
static byte   end[4][28][32];     /* Ðe‹i· ¹·¬÷ */

static byte (*firstPtr)[20][32];  /* Ðe‹i· Á¡¬÷ */
static byte (*  midPtr)[22][32];  /* Ðe‹i· º—¬÷ */
static byte (*  endPtr)[28][32];  /* Ðe‹i· ¹·¬÷ */

static byte english[256][16];     /* µw¢… */
static byte (*asciiPtr) [16];

//static const byte SPCFont[360][32];
//static const byte GRPFont[360][32];
//static const byte HNJFont[4888][32];
static byte (*SPCFontPtr)[32];
static byte (*GRPFontPtr)[32];
static byte (*HNJFontPtr)[32];
static byte han_mem[32];          /* hangul  font memory  */
static byte eng_mem[16];          /* english font memory  */

static int cur_x =0, cur_y =0, b =0, font_type =1;
static int graphcolor, graphback, oldback;

void font_init(void)
{
  SPCFontPtr = (byte(*)[32]) dflt_SPCFont;
  GRPFontPtr = (byte(*)[32]) dflt_GRPFont;
  asciiPtr   = (byte(*)[16]) dflt_EngFont;
//  HNJFontPtr = (byte(*)[32]) dflt_HNJFont;
  firstPtr   = (byte(*)[20][32]) dflt_KorFont;
    midPtr   = (byte(*)[22][32]) (dflt_KorFont+8*20*32);
   endPtr    = (byte(*)[28][32]) (dflt_KorFont+8*20*32+4*22*32);
}

int inithanja(const char *name)
{
  _hanjapoint=open(name,O_RDONLY|O_BINARY);
  if (_hanjapoint==-1) return FALSE;
  //_hooknocode=hanja_process;
  return TRUE;
}

void closehanja(void)
{
  if (_hanjapoint!=-1) close(_hanjapoint);
}

/* -------------- ·¡£¡»¡Ÿi ÐsÃ¥”a. ---------------*/
static void or_IMG(byte *src, byte *dest, const int bytes);

/*--------------------- Ðe‹iÍ¥Ëa ·ª‹¡ -----------------------*/
void han_font(const char *han)
{
 const int fnt =open(han, O_RDONLY | O_BINARY);
 if (fnt == -1)
 {
    graph_close();
    printf(" '%s' Font FILE not found !\n", han);
    exit(1);
 }
 (void)read(fnt, first, 5120); firstPtr =first;
 (void)read(fnt, mid,   2816);   midPtr =mid;
 (void)read(fnt, end,   3584);   endPtr =end;
 (void)close(fnt);
}
/*-------------------- µw¢…Í¥Ëa ·ª‹¡ ---------------------*/
void eng_font(const char *eng)
{
 const int fnt = open(eng,O_RDONLY | O_BINARY);
 if (fnt == -1) {
    graph_close();
    printf("'%s' Font FILE not found !\n", eng);
    exit(1);
 }
 (void)read(fnt, english, 4096); asciiPtr =english;
 (void)close(fnt);
}
/*----------------- ·¡£¡»¡Ðe‹i Â‰b -------------------*/
void hputch(const int x, const int y, byte *f)
{
  register unsigned i = 0, offset, oldoffset;

  register char far *video = (char far *)(0xA0000000L);
	   offset = (x-1) + ((y-1)*640*2) ;
	oldoffset = offset;
  //if(oldback != graphback) {
  outp(0x03ce, 0x00);  outp(0x03cf, graphback);
  outp(0x03ce, 0x01);  outp(0x03cf, 0x0f);
  outp(0x03ce, 0x05);  outp(0x03cf, 0x00);
  outp(0x03ce, 0x08);  outp(0x03cf, 0xff);
  //}
  if (b==16)
  {
     for(i=0; i<16; i++){ video[offset] =video[offset];	offset += 80;}

     outp(0x03ce,0x0);  outp(0x03cf,graphcolor);
     outp(0x03ce,0x08); offset =oldoffset;

     for(i=0 ; i<16; i++)
     { outp(0x03cf, f[i]); video[offset] =video[offset]; offset += 80;}

  } else
  if (b==32)
  {
     for(i=0; i<16; i++)
     {
	video[offset] =video[offset++];	video[offset] =video[offset];
	offset += 79;
     }
     outp(0x03ce,0x00); outp(0x03cf,graphcolor); outp(0x03ce,0x08);
     offset =oldoffset;

     for(i=0 ; i<32; )
     {
	outp(0x03cf, f[i++]); video[offset] =video[offset++];
	outp(0x03cf, f[i++]); video[offset] =video[offset];
	offset += 79;
     }
  }
  outp(0x03cf,0xff);
}
/* ----------- Ðe‹i·i Á¡¬÷,º—¬÷,¹·¬÷Ìe”e -------------*/
void han_chp(byte cho, byte jung, byte jong, const int font)
{
  register int typ1 =0, typ2 =0, typ3 =0;

  cho  = table[0][cho]; jung =table[1][jung]; jong =table[2][jong];

  typ1 = (jong ? 1 : 0);
  typ1 = FontType[typ1][jung];
  typ3 = FontType   [2][jung];
  typ2 = ((cho == 0 || cho == 1 || cho == 16) ? 0 : 1) + (jong ? 2 : 0);

  set_font(font);
  memset(han_mem, 0, 32);

  if (cho)  or_IMG( firstPtr[typ1][cho],  han_mem, 32);
  if (jung) or_IMG(   midPtr[typ2][jung], han_mem, 32);
  if (jong) or_IMG(   endPtr[typ3][jong], han_mem, 32);

//  DoFontImage[font_type](han_mem, 32);
  DoFontImage(han_mem, 32, font_type);
  b=32; hputch(cur_x, cur_y, han_mem);
  cur_x+=2;
}

void spc_chp(const byte c1, const byte c2, const int font)
{
  set_font(font);
  memset (han_mem, 0, 32);
  memmove(han_mem, SPCFontPtr+(unsigned char)((c2-128)), 32);
//  DoFontImage[font_type](eng_mem, 32);
  DoFontImage (han_mem, 32, font_type);
  b=32;  hputch(cur_x, cur_y, han_mem);
  cur_x+=2;
}

void grp_chp(const byte c1, const byte c2, const int font)
{

  const int fontID[7]    = {0xd480, 0xd931, 0xda31, 0xdb31, 0xdc31, 0xdd31, 0xde31};
  const int addoff[7]    = {125,    162,    188,    162,    188,    177,    175};
  const int sblank[7]    = {1,      1,      0,      0,      0,      0,      0};
/*
    int add_table[6] = {124,    269,    475,    637,    807,    984};
*/
  int   off, i,   j;
  unsigned char hc1, hc2, hc3;

  for (i = 0; i < 7; i++) {
	hc1 = (fontID[i] >> 8) & 0xff;
	hc2 = (fontID[i] & 0xff);
	if ((c1 == hc1) && (c2 >= hc2)) break;
  }
  hc3 = c2;
  off = hc3 - hc2;

  if (  (off < sblank[i]) || (hc3 >= 0x78 && hc3 <= 0x89)  || (hc3 >= (addoff[i] + 0x31 + 18 + sblank[i]))  )
	off = 486;
  if (hc1 == 0xd4) {
	off = hc3 - hc2;
	if (off > addoff[i] || off == 0)    off  = 486;
  }
  if (off != 486) {
	if ((hc1 != 0xd4) && (hc3 >= 0x8a)) off -= 18;
	off -= sblank[i];
	for (j = 0; j < i; j++)             off += addoff[j];
  }
  set_font(font);
  memset (han_mem, 0, 32);
  memmove(han_mem, GRPFontPtr+(unsigned char)((off-124)), 32);
//  DoFontImage[font_type](eng_mem, 32);
  DoFontImage (han_mem, 32, font_type);
  b=32;  hputch(cur_x, cur_y, han_mem);
  cur_x+=2;
}

void hnj_chp(const byte c1, const byte c2, const int font)
{
  long offset;

  offset=(c1-0xE0)*188 + ((c2 < 0x91) ? (c2-0x31) : (c2-0x43));
  offset*=32;
  lseek(_hanjapoint, offset, SEEK_SET);
  set_font(font);
  memset (han_mem, 0, 32);
  read(_hanjapoint, han_mem, 32);
//  memmove(han_mem, HNJFontPtr[offset], 32);
//  DoFontImage[font_type](han_mem, 32);
  DoFontImage (han_mem, 32, font_type);
  b=32;  hputch(cur_x, cur_y, han_mem);
  cur_x+=2;
}

/* ------------------- µw¢… Ìe”e ------------------*/
void eng_chp(const byte keycode, const int font)
{
  set_font(font);
  memset (eng_mem, 0, 16);
  memmove(eng_mem, asciiPtr[keycode], 16);
//  DoFontImage[font_type](eng_mem, 32);
  DoFontImage (eng_mem, 16, font_type);
  b=16;  hputch(cur_x, cur_y, eng_mem);
  cur_x++;
}

/* --------------- ÑÁ¡e·i À÷­¡ -------------- */
void graph_cls(const int color)
{
  register unsigned i;
  register char far * video = (char far *)0xA0000000L;

  outp(0x03ce,0x00);  outp(0x03cf,color);  /* ¬‚»¡¸÷ */
  outp(0x03ce,0x01);  outp(0x03cf,0x0f);
  outp(0x03ce,0x03);  outp(0x03cf,0x00);
  outp(0x03ce,0x08);  outp(0x03cf,0xff);

  /* œa·¥”w 80 , 640*480 ·¡¡e (480-1) */
  for(i=0; i<80*479 ; ) video[i++] =0xFF;
  outp(0x03ce,0x01);  outp(0x03cf,0x00);
}

#define	    isHNJ(c)		((c  >= 224) && (c  <= 249 ))
#define	    isgraph(c)		((c  >= 217) && (c  <= 222 ))
#define     isspc(c)		((c  == 212) && (c  >= 0x80))
#define     isKSC(c1, c2)	((c1 > 0xA1) && (c2 >  0xA1))

/* -------------- ¡ww´á 1 : Ðe‹i‰Áµw¢…Â‰b --------------*/
void gputs(const int x, const int y, const int font, char *st)
{
  unsigned char data1, data2;
  unsigned char cho, jung, jong;
  register unsigned int i =0;

  cur_x=x; cur_y=y;

  for(i=0; i<strlen(st); i++)
  {
       data1 = *(st+i);

       if(data1 &0x80)
      {
	 if(cur_x >= 78) { cur_x=1; cur_y++; }
	 if(isspc(data1)){
		i++; 	data2 =*(st+i);
		spc_chp(data1, data2, font);
	 } else
	 if(isgraph(data1)) {
		i++; 	data2 =*(st+i);
		grp_chp(data1, data2, font);
	 } else
	 if(isHNJ(data1)){
		i++; 	data2 =*(st+i);
		if(_hanjapoint == -1) cur_x +=2;
		else hnj_chp(data1, data2, font);

	 } else
	 {
		i++; 	data2 =*(st+i);
		cho  =(data1 & 124) >> 2;
		jung =(data1 & 3) * 8 + (data2 >> 5);
		jong =(data2 & 31);
		han_chp(cho, jung, jong, font);
	 }

      } else
      {
	switch(data1)
	{
//	  case ' ' :  if (cur_x < 78) cur_x++;
//			else { cur_x =1, cur_y++; } 	    continue;
	  case '\t':  cur_x = 8 - ((cur_x + 7) % 8);
		      if(cur_x < 78) { cur_x =1; cur_y++;}  continue;
	  case '\n':  if(cur_x < 78) cur_x++;
			else { cur_x =1, cur_y++; }   	    continue;
	  case '\r':  cur_x=1; cur_y++;  		    continue;
	}
	 eng_chp(data1, font);
      }
  }
}

/* -------------- ¡ww´á 2 : Ðe‹i‰Áµw¢…Â‰b --------------*/
void gprintf(const int x, const int y, const int font, char *format,...)
{
  char str[256];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(str,format,argptr);  gputs(x,y,font,str);
  va_end(argptr);
}
/*-------------------- ‹aœÏ¢¡¡—a ¬é¸÷ ---------------------*/
void graph_mode(const int mode)
{ union REGS r; r.h.ah = 0; r.h.al = mode; int86(0x10,&r,&r); }

/*-------------------- ‹aœÏ¢¡¡—a ”h‹¡  ---------------------*/
void graph_close(void)
{ union REGS r;
  r.h.ah = 0; r.h.al = 0x03;  int86(0x10,&r,&r);
  closehanja();
}

void gr_goto(const int _xx, const int _yy) { cur_x =_xx; cur_y =_yy; }

int graph_init(const int grcolor)
{
  if(!inithanja(HNJFont)) {
	puts("No HANJA Font\n");
	(void) getchar();
  }
  graph_mode(0x12); gr_goto(1, 1); font_init();
  if (grcolor != -1) graph_cls(grcolor);
  return(0);
}

void gColor(const int fcolor, const int bcolor)
{ graphcolor =fcolor; graphback= oldback = bcolor;}

/* -------------- ·¡£¡»¡Ÿi ÐsÃ¥”a. ---------------*/
static void or_IMG(byte *src, byte *dest, const int bytes)
{
  register unsigned int i;
  for(i=0; i < bytes; i++) dest[i] |= src[i];
}
/* -------------- ´á˜å Í¥Ëa·¥»¡ Ìe”e -------------*/
void set_font(const int new_font)
{  font_type = new_font;  }
