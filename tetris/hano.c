/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Program Name : hano.c
    Copyright    : Electronic Dept. of Kyungpook Univ.
    Last Update  : 1994. 7. 11
    Compiler     : Borland C (\\tcc -ml hano.c c:\tcc\lib\graphics.lib)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _WIN32
#include <alloc.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <graphics.h>

typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char  byte;

int HANREV=0, ENGREV=0;

#define  HFONT10  0
#define  HFONT8   1
#define  HFONT2   2

static const byte INDEXHF[3][32] = {
	{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16,17,18,19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 6, 7, 8, 9,10,11,
	  0, 0,12,13,14,15,16,17, 0, 0,18,19,20,21, 0, 0 },
	{ 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
	 15,16, 0,17,18,19,20,21,22,23,24,25,26,27, 0, 0 }
};

static const byte REFSECOND10_8[2][20] = {
	{ 0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1 },
	{ 2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3 }
};

static const byte REFFIRST10[2][22] = {
	{ 0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0 },
	{ 0,5,5,5,5,5,5,5,5,6,8,8,8,6,7,9,9,9,7,6,8,5 }
};

static const byte REFFIRST8[2][22] = {
	{ 0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0 },
	{ 1,5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5 }
};

static const byte REFFIRST2[22] =
	{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0 };

static const byte REFLAST10_8[22] =
	{ 0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1 };

static const byte REFLAST2[22] =
	{ 0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,1,0,0 };

byte HF10_1[10][20][32];
byte HF10_2[4][22][32];
byte HF10_3[4][28][32];

byte HF8_1[8][20][32];
byte HF8_2[4][22][32];
byte HF8_3[4][28][32];

byte HF2_1[2][20][32];
byte HF2_2[22][32];
byte HF2_3[2][28][32];

byte EF[256][16];

byte (*HF1_ptr)[20][32];
byte (*HF2_ptr)[22][32];
byte (*HF3_ptr)[28][32];

byte (*HF101_ptr)[20][32];
byte (*HF102_ptr)[22][32];
byte (*HF103_ptr)[28][32];

byte (*HF21_ptr)[20][32];
byte (*HF22_ptr)[32];
byte (*HF23_ptr)[28][32];

byte (* EF_ptr)[16];

typedef struct {
	unsigned char sndchar;
	unsigned char fstchar;
} hanchar;

typedef struct {
	unsigned int jongsung : 5;
	unsigned int jungsung : 5;
	unsigned int chosung  : 5;
	unsigned int det_han_or_eng : 1;
} hancode;

typedef union {
	hanchar hchar;
	hancode hcode;
} hangul;

static char whichfont=HFONT10;
static char hbuffer[32];
static char ebuffer[16];

void ReadFontData(void)
{
    FILE *han, *eng;

//    han = fopen("HANMY10.FNT", "r+b");
    han = fopen("HUNMIN.KOR", "r+b");
    if(han == NULL) return;
    fread(HF10_1,10*20*32,1,han);
    fread(HF10_2, 4*22*32,1,han);
    fread(HF10_3, 4*28*32,1,han);
    fclose(han);
    HF101_ptr = HF10_1;
    HF102_ptr = HF10_2;
    HF103_ptr = HF10_3;

    han = fopen("HANGO8.FNT", "r+b");
    if(han == NULL) return;
    fread(HF8_1, 8*20*32,1,han);
    fread(HF8_2, 4*22*32,1,han);
    fread(HF8_3, 4*28*32,1,han);
    fclose(han);
    HF1_ptr = HF8_1;
    HF2_ptr = HF8_2;
    HF3_ptr = HF8_3;

    han = fopen("HANGO2.FNT", "r+b");
    if(han == NULL) return;
    fread(HF2_1, 2*20*32,1,han);
    fread(HF2_2, 22*32,1,han);
    fread(HF2_3, 2*28*32,1,han);
    fclose(han);
    HF21_ptr = HF2_1;
    HF22_ptr = HF2_2;
    HF23_ptr = HF2_3;

    eng = fopen("ENGLISH.FNT", "r+b");
    if(eng == NULL) return;
    fread(EF,256*16,1,eng);
    fclose(eng);
}

void HanCharDisplay(int x, int y, int CHARCOLOR, int BACKCOLOR, char *img)
{
    word xx, yy;
    byte data;

    for(yy=y; yy<y+16; yy++)
    {
	data = *img++;
	for(xx=0; xx<8; xx++)
	{
	    if(data&0x80)  putpixel(x+xx, yy, CHARCOLOR);
	    else           putpixel(x+xx, yy, BACKCOLOR);
	    data <<= 1;
	}
	data = *img++;
	for(xx=0; xx<8; xx++)
	{
	    if(data&0x80)  putpixel(x+8+xx, yy, CHARCOLOR);
	    else           putpixel(x+8+xx, yy, BACKCOLOR);
	    data <<= 1;
	}
    }
}

void EngCharDisplay(int x, int y, const int CHARCOLOR, const int BACKCOLOR, char *img)
{
    word xx, yy;
    byte data;

    for(yy=y; yy<y+16; yy++)
    {
	data = *img++;
	for(xx=0; xx<8; xx++)
	{
	    if(data&0x80)  putpixel(x+xx, yy, CHARCOLOR);
	    else           putpixel(x+xx, yy, BACKCOLOR);
	    data <<= 1;
	}
    }
}

static void or_img(byte *src, byte *dest, const int bytes)
{
    int i;

    for(i=0; i != bytes; i++) *(dest+i) |= *(src+i);
}

static void or_img32(byte *src)
{
    int i;

    for(i=0; i != 32; i++)
	*(hbuffer+i) |= *(src+i);
}


void reverseimage(byte *img, const int bytes)
{
    int i;

    for(i=0; i<bytes; i++) img[i] = ~img[i];
}

static void MakeHanChar(int x, int y, const int CHARCOLOR, const int BACKCOLOR, const unsigned char fstbyte, const unsigned char sndbyte)
{
    hangul h;
    int  b1, b2, b3, first, mid, last, last_D=0;

    memset(hbuffer, 0, 32);

    h.hchar.fstchar = fstbyte;
    h.hchar.sndchar = sndbyte;

    first = INDEXHF[0][h.hcode.chosung];
    mid   = INDEXHF[1][h.hcode.jungsung];
    last  = INDEXHF[2][h.hcode.jongsung];

    if(last) last_D =1;

    switch(whichfont)
    {

    case HFONT10:
		b3 =REFLAST10_8[mid];
		b2 =REFSECOND10_8[last_D][first];
		b1 =REFFIRST10   [last_D][mid];
		if(first) or_img32(HF101_ptr[b1][first]);
		if(mid  ) or_img32(HF102_ptr[b2][mid]  );
		if(last ) or_img32(HF103_ptr[b3][last] );
		break;

    case HFONT8:
		b3 =REFLAST10_8[mid];
		b2 =REFSECOND10_8[last_D][first];
		b1 =REFFIRST8    [last_D][mid];
		if(first) or_img32(HF1_ptr[b1][first]);
		if(mid  ) or_img32(HF2_ptr[b2][mid]  );
		if(last ) or_img32(HF3_ptr[b3][last] );
		break;

    case HFONT2 :
		b3 = REFLAST2[mid];
		b1 = REFFIRST2[mid];
		if(first) or_img32(HF21_ptr[b1][first]);
		if(mid  ) or_img32(HF22_ptr[mid]      );
		if(last ) or_img32(HF23_ptr[b3][last] );
		break;
    }

    if(HANREV==1) reverseimage(hbuffer, 32);
    HanCharDisplay(x,y,CHARCOLOR, BACKCOLOR, hbuffer);
}

static void MakeEngChar(int x, int y, const int CHARCOLOR, const int BACKCOLOR, const char font)
{
    int i;
    for(i=0; i<16; i++)
	ebuffer[i]=EF[font][i];
    if(ENGREV==1) reverseimage(ebuffer, 16);
    EngCharDisplay(x,y,CHARCOLOR,BACKCOLOR,ebuffer);
}

void hprintxy(int x, int y, const int CHARCOLOR, const int BACKCOLOR, char *str)
{
    unsigned char fstbyte, sndbyte;

    while(*str != '\0') {
	fstbyte = *str++;
	if(fstbyte & 0x80) {
	    sndbyte = *str++;
	    MakeHanChar(x,y,CHARCOLOR,BACKCOLOR,fstbyte,sndbyte);
	    x += 16;
	} else
	{
	    MakeEngChar(x,y,CHARCOLOR,BACKCOLOR,fstbyte);
	    x += 8;
	}
    }
}

/*									*/
/*	GPRINTF_XY: Used like PRINTF except the output is sent to the	*/
/*	screen in graphics mode at the specified co-ordinate.		*/
/*									*/

void gprintf_XY( int xloc, int yloc, const int fc, const int bc, char *fmt, ... )
{
  va_list  argptr;			/* Argument list pointer	*/
  char str[140];			/* Buffer to build sting into	*/
  va_start( argptr, fmt );		/* Initialize va_ functions	*/
  (void) vsprintf( str, fmt, argptr );	/* prints string to buffer	*/
//  outtextxy( *xloc, *yloc, str );	/* Send string in graphics mode */
  hprintxy(xloc, yloc, fc, bc, str);
//  *yloc += textheight( "H" ) + 2;       /* Advance to next line         */
  va_end( argptr );			/* Close va_ functions		*/

}

#define S256_320_200    0
#define S256_640_400    1
#define S256_640_480    2
#define S256_800_600    3
#define S256_1024_768   4

int huge DETECTVGA256(void)
{
    // return S256_320_200;
    // return S256_640_400;
     return S256_640_480;       // ¬å?¡¡—a 640*480
    // return S256_800_600;
    // return S256_1024_768;
}
void SVGA256_driver(void);

void InitGR(void) {
    int GraphDriver, GraphMode, ErrorCode;

    ReadFontData();
    GraphDriver = DETECT;

    //installuserdriver("SVGA256", DETECTVGA256);
//    (void) registerbgidriver(SVGA256_driver);
    initgraph( &GraphDriver, &GraphMode, "");
    ErrorCode = graphresult();
    if(ErrorCode != grOk) {
	printf("Graphics System Error: %s\n", grapherrormsg(ErrorCode));
	exit(1);
    }
    setcolor(WHITE);
}

#ifdef TEST

void main(void)
{
    int i,j,k,l;
    int GraphDriver, GraphMode, ErrorCode;

    ReadFontData();
    InitGR();

    whichfont=2;

    for(i=0;i<30;i++) {
	  for(j=0; j<30; j++)
	      hprintxy( j*24, i*16, i, j, "ˆap");
    }
    (void)getchar();

    whichfont=0;

    for(i=0;i<30;i++) {
	  for(j=0; j<30; j++)
	      hprintxy( i*24, j*16, i, j, "ˆap");
    }
    (void)getchar();

    whichfont=1;

    for(i=0;i<30;i++) {
	  for(j=0; j<30; j++)
	      hprintxy( j*24, i*16, i, j, "ˆap");
    }
    (void) getchar();

    gprintf_XY(getmaxx()/2, getmaxy()/2, WHITE, BLACK, "Left Mem : %ld KB", coreleft()/1024);
    (void) getchar();

    closegraph();
}

#endif

