#pragma inline
/*
    GRAPHIC routines drect to screen for AGE Ga pan.
                          written by LeeSH
*/
#include <graphics.h>
#include <stdio.h>
#include <mem.h>
#include <alloc.h>
#include <string.h>
#include <dos.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <conio.h>
#include "vga320.h"

ENGFONT *engfont;
HANFONT *hanfont;
int IsGrp;

/*
   Unused code is 0. using codes vary from 1 to 0x1F.
*/
Byte chotbl[]= { 0, 0, 1, 2, 3, 4, 5, 6,
                 7, 8, 9,10,11,12,13,14,
                15,16,17,18,19, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0
               };
Byte jungtbl[]={ 0, 0, 0, 1, 2, 3, 4, 5,
                 0, 0, 6, 7, 8, 9,10,11,
                 0, 0,12,13,14,15,16,17,
                 0, 0,18,19,20,21, 0, 0
                };
Byte jongtbl[]={ 0, 0, 1, 2, 3, 4, 5, 6,
                 7, 8, 9,10,11,12,13,14,
                15,16, 0,17,18,19,20,21,
                22,23,24,25,26,27, 0, 0
                };

void far *malloc2( my_size_t sz)
{
    void far *pt;

    pt = farmalloc(sz);
    if(pt==NULL)
        error( "Out of Memory.");
    return pt;
}

void far *free2( void far *pt )
{
    if( pt )
        farfree( pt );
    return NULL;
}

void readhanfont(void)
{
    int i;
    FILE *fp;

    fp=fopen("HAN.SFT","rb");
    if(fp==NULL)
        error( "Hangul font File open." );
    for(i=0;i<HANCHARNO;i++) {
        if( 0==fread( &hanfont[i], HANFONTSIZE, 1, fp) )
            error( "Hangul font file read." );
    }
    fclose(fp);
}

void readengfont(void)
{
    int i;
    FILE *fp;

    fp=fopen("ENG.SFT","rb");
    if(fp==NULL)
        error( "English font file open." );
    for(i=0;i<ENGCHARNO;i++) {
        if(0==fread( &engfont[i], ENGFONTSIZE, 1, fp) )
            error( "English Font file read." );
    }
    fclose(fp);
}

void initeh(void)
{
  engfont=(ENGFONT *)malloc2(sizeof(ENGFONT)*ENGCHARNO);
  hanfont=(HANFONT *)malloc2(sizeof(HANFONT)*HANCHARNO);
  readengfont();
  readhanfont();
}

void clreh(void)
{
  hanfont=free2(hanfont);
  engfont=free2(engfont);
}

void mode(int no)
{
  union REGS r;
  r.h.ah=0;
  r.h.al=no;
  int86(16,&r,&r);
}

int getmode( void )
{
    union REGS r;
    int vmode;

    r.h.ah  = 15;
    vmode = int86( 0x10, &r, &r) & 255;
    return vmode;
}

int mydetectgraph( void )
{
    int vmode = getmode();

    if( vmode == 3 || vmode == 0x13 )
        return VGA;

    if( vmode == 7 )
        return HERCMONO;

    return DETECT;
}

void init320()
{
  int gd;

  gd=mydetectgraph();

  if(gd==VGA) {
    mode(0x13);
    IsGrp=VGALOW;
  }
  else
    error( "Your Video card is not for Me." );
  gclrscr();
  initeh();
}

void close320()
{
  clreh();
  if (IsGrp==VGALOW)
    mode(3);
  IsGrp=NOGRP;
}

void error( char *s )
{
    close320();
    puts( s );
    exit( -1 );
}

void VSMtoVSMp(Byte *VSM1, Byte *VSM2, int x1, int y1, int x2, int y2)
{
    unsigned i, j, k;
    Byte *p1, *p2;

    j=x2-x1+1;
    k=x1+y1*320U;
    p2=VSM1+k;
    p1=VSM2+k;

    for(i=y1;i<=y2;i++) {
        memcpy( p2, p1, j);
        p1+=320;
        p2+=320;
    }
}

void VSMvlinep(Byte *ptr, int y1, int y2,int x, int width, int co)
{
    int i, j;

    for( i=0; i< width ; x++, i++) {
        for(j=y1 ; j<=y2; j++)
            PutPixelp( ptr, x, j, co);
    }
}

void VSMhlinep(Byte *ptr, int x1, int x2, int y, int width, int co)
{
  int i, j;

  for(i=0 ; i<width; y++, i++) {
    for( j=x1; j<=x2; j++)
      PutPixelp( ptr, j, y, co);
  }
}

void VSMPutImage2cop(Byte *p1, int x, int y,void *data, int color, int mode)
{
    int i, j, t;
    Byte *b;
    unsigned char mask;
    FONT *buf;

    buf=(FONT *)data;
    b=&(buf->ch);

    for(j=y;j<=y+buf->y;j++)
    for(i=x;i<=x+buf->x;i+=8){
        mask = 0x80;
        for ( t = 0; t < 8; t++ ){
            if (  (*b) & mask )
                PutPixelp( p1, i + t, j, color );
            else if (!mode)
                PutPixelp( p1, i+t,j,0);
            mask >>= 1;
        }
        b++;
    }
}

void VSMPutImagep(Byte *ptr, int x, int y, void *data, int mode)
{
    int i, j;
    Byte *b, *p;
    FONT *buf;
    unsigned seg1, seg2, off1, off2;

    buf=(FONT *)data;
    b=&(buf->ch);
    p=ptr+x+y*320U;

    if(mode==COPY_PUT) {
      i=buf->x+1;
      for(j=0;j<=buf->y;j++,b+=i, p+=320)
        memcpy( p, b, i );
    }
    else if(mode==OR_PUT) {  /* If bitmap has color( != 0 ), putpixel */
/*    PutImageCode for Assembler.  */
      i=buf->x+1;
      j=buf->y+1;
      seg1= FP_SEG( b );
      off1= FP_OFF( b );
      seg2= FP_SEG( p );
      off2= FP_OFF( p );

      asm   push  ds
      asm   push  si
      asm   mov   ds, seg1
      asm   mov   es, seg2
      asm   mov   si, off1
      asm   mov   di, off2

      asm   mov   cx, j
L1:
      asm   mov   bx, cx

      asm   mov   cx, i
L2:
      asm   mov   ah, ds:[si]
      asm   cmp   ah, 0
      asm   jz    L3

      asm   mov   es:[di], ah
L3:
      asm   inc   si
      asm   inc   di
      asm   loop  L2

      asm   mov   ax,   off2
      asm   add   ax,   320
      asm   mov   off2, ax
      asm   mov   di,   ax

      asm   mov   cx,   bx
      asm   loop  L1

      asm   pop   si
      asm   pop   ds

    }
    else if(mode==AND_PUT) { /* If bitmap pixel color==0, clear pixel.*/
/*   AND_PUT assembler code */
      i=buf->x+1;
      j=buf->y+1;
      seg1= FP_SEG( b );
      off1= FP_OFF( b );
      seg2= FP_SEG( p );
      off2= FP_OFF( p );

      asm   push  ds
      asm   push  si
      asm   mov   ds, seg1
      asm   mov   es, seg2
      asm   mov   si, off1
      asm   mov   di, off2

      asm   mov   cx, j
LL1:
      asm   mov   bx, cx

      asm   mov   cx, i
      asm   mov   ah, 0
LL2:
      asm   cmp   ah, ds:[si]
      asm   jnz   LL3

      asm   mov   es:[di], ah
LL3:
      asm   inc   si
      asm   inc   di
      asm   loop  LL2

      asm   mov   ax,   off2
      asm   add   ax,   320
      asm   mov   off2, ax
      asm   mov   di,   ax

      asm   mov   cx,   bx
      asm   loop  LL1

      asm   pop   si
      asm   pop   ds
    }
}

void *VSMGetImagep(Byte *PTR, int x1, int y1, int x2, int y2, void *ptr)
{ /* getImage from VSMem2 */
  FONT *buf;
  Byte *p, *b;
  int i, j;

  buf=(FONT *)ptr;
  buf->x=x2-x1;
  buf->y=y2-y1;
  p=&(buf->ch);
  i=buf->x+1;
  b=PTR+x1+y1*320U;

  for(j=y1;j<=y2;j++, b+=320, p+=i)
    memcpy( p, b, i);

  return ptr;
}

void VSMClrBoxp(Byte *ptr, int x1, int y1, int x2, int y2, int co)
{
  int i, j;
  Byte *p;
  p=ptr+y1*320U+x1;
  j=x2-x1+1;
  for(i=y1;i<=y2;i++, p+=320)
    memset( p, co, j);
}

/*********************************************************************
  Hangul output routines for Foreground color + background color
**********************************************************************/
/*  algorithm.
      1st, clear character area with backgorund color.
      2nd, draw character with foreground color with OR_PUT.
      That's all !!!
    All goes to VSMem2, Background.
*/
void VSMengput2p(Byte *ptr, int x, int y, Byte ch, int fco, int bco)
{
  char buf[ENGFONTSIZE+4]={ 7, 0, 15, 0 };

  VSMClrBoxp( ptr, x, y, x+7, y+15, bco );
  if(ch>=32) {
    memcpy( buf+4, &engfont[ch-32], ENGFONTSIZE );
    VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
  }
}

void VSMhanput2p(Byte *ptr, int x,int y, Byte ch1, Byte ch2, int fco, int bco)
{
  char buf[HANFONTSIZE+4]={ 15, 0, 15, 0 };
  int cho, jung, jong;

  ch1&=0x7F;
  cho=chotbl[ ch1>>2 ];
  jung=jungtbl[ ((ch1&3)<<3) | (ch2>>5) ];
  jong=jongtbl[ ch2 & 0x1F ];

  VSMClrBoxp( ptr, x, y, x+15, y+15, bco );

  if(cho) {
    memcpy( buf+4, &hanfont[--cho], HANFONTSIZE );
    VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
    if(jung) {
      memcpy( buf+4, &hanfont[jung+JUNG_START], HANFONTSIZE );
      VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
    }
    if(jong) {
      memcpy( buf+4, &hanfont[jong+JONG_START], HANFONTSIZE );
      VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
    }
  }
  else if(jung) {
    memcpy( buf+4, &hanfont[jung-1+JUNG_START], HANFONTSIZE );
    VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
    if(jong) {
      memcpy( buf+4, &hanfont[jong-1+JONG_START], HANFONTSIZE );
      VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
    }
  }
  else if(jong) {
    memcpy( buf+4, &hanfont[jong-1+JONG_START], HANFONTSIZE );
    VSMPutImage2cop( ptr, x, y, buf, fco, OR_PUT );
  }
}

void VSMprintxy2p(Byte *ptr, int x, int y, char *s, int fco, int bco)
{
   while( *s ) {
     if( *s & 0x80 ) {
       VSMhanput2p( ptr, x, y, *s, *(s+1), fco, bco);
       x+=16;
       s++; s++;
     }
     else {
       VSMengput2p(ptr, x, y, *s++, fco, bco);
       x+=8;
     }
   }
}

#define MFEID    "Multi_Size_Font\x1A"     /* 16 characters */

/************************************************************************
    Return value is pointer to MFE font data
        Always read A font. if Error, It exits.
*************************************************************************/

char *loadMFEfont(char *fname, int no1)
{
  int handle;
  Byte buf[32];
  int fx, fy;
  unsigned off, allfontno;
  FONT *ibuf;

  handle=open(fname, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD );
  if(handle==-1)
    error( "Error : File Open" );

  if(-1==read(handle, buf, 32)) {
    close(handle);
    error( "Error : File Read." );
  }

  if(strcmp(MFEID, (char *)buf)) {
    close(handle);
    error( "Error : Not MFE file." );
  }
  if( !buf[27] )
    error( "Error : Not MFE file." );

  allfontno=buf[30]+buf[31]*256U;

  if( -1==lseek( handle, no1*( sizeof(unsigned) ), SEEK_CUR) ) {
    close(handle);
    error( "Error : File Reading Error." );
  }

  if( -1==read( handle, &off, sizeof(unsigned) ) ) {
    close(handle);
    error( "Error : File Read." );
  }

  if(-1==lseek( handle, 32+allfontno*( sizeof(unsigned) )+off, SEEK_SET) ) {
    close(handle);
    error( "Error : File Read." );
  }

  ibuf=(FONT *)malloc2( sizeof(FONT ) );
  if(-1==read(handle, ibuf, 4 ) ) {
    close(handle);
    error( "Error : File Read." );
  }

  fx=ibuf->x;
  fy=ibuf->y;
  off=ImageSize( 0, 0, fx+1, fy+1);
  free2(ibuf);
  ibuf=(FONT *)malloc2( off );
  ibuf->x=fx;
  ibuf->y=fy;

  off-=4;
  if(-1==read(handle, &(ibuf->ch), off ) ) {
    close(handle);
    error( "Error : File Read." );
  }
  close(handle);
  return (char *)ibuf;
}

int getkey( void )
{
    int ch = getch();
    return ch ? ch: 256+getch();
}


#define PIC_STARTX          0
#define PIC_STARTY          0
#define PIC_MAXY          199

typedef struct {
    char man;          /* PCX ID */
    char ver;          /* Version */
    char encode;       /* Compress or not */
    char bpp;          /* Bit Per Pixel */
    int xmin, ymin;
    int xmax, ymax;
    int hres, vres;
    unsigned char pal[48];  /* Pallette for 16 color IMAGE */
    char reserved;
    char nplane;            /* No. of Plane */
    int bpl;                /* Byte Per Line */
    int palinfo;            /* Pallette info. */
    int scanhres, scanvres;
    char extra[54];
} PCXHEADER;
/*******************************************************************
  This returns YES if file is PCX, and success to read.
  If this returns NO, it's Not PCX256 file.
     You can test other format file reader with this value,
                      !! The Picture Chain !!
     NOTE: buffer should be 64K.
********************************************************************/
int loadPCX256( char *filename, unsigned char far *buf)
{
  FILE *fp;
  PCXHEADER header;
  unsigned char pal[256*3];
  unsigned xx=0,  yy=0;
  int ch, Repeat_Count, data, flag=0, i;

  fp = fopen( filename, "rb" );
  if( fp == NULL )
    error( filename ); //"Picture File Not Exist." );

  fseek( fp, -768, SEEK_END );
  fread( pal, 768, 1, fp );
  fseek( fp, 0L, SEEK_SET );
  fread( &header, 128, 1, fp );

  if( header.man!=10 ) {  /* Not PCX */
    fclose( fp );
    return NO;
  }
  if(header.bpp==8) {  /* PCX 256 */
    xx = PIC_STARTX;
    yy = PIC_STARTY;

    for( i=0; i < 768; i++ )
        pal[i] >>= 2;
    update_pallette( pal );

    while( (ch = getc(fp)) != EOF && flag == 0 ) {
      if ((ch & 0xC0) == 0xC0) {
        Repeat_Count = ch & 0x3F;
        data = getc(fp);
      }
      else {
        Repeat_Count=1;
        data = ch;
      }
      while (Repeat_Count--) {
        if (xx >= header.xmax+PIC_STARTX+1) {
          xx=PIC_STARTX;
          if (++yy > PIC_MAXY) {
	    flag=1;
            break;
          }
        }
        if (xx < 320 && yy < 200 )
          buf[ xx++ + yy * 320U ] = data; /* PutPixel ?? */
        else
          xx++;
      }
    }
    fclose( fp );
  }
  else {  /* Not PCX 256 */
    fclose ( fp );
    return NO;
  }
  return YES;
}

void update_pallette( unsigned char *DAC )
{
    unsigned int DacSeg = FP_SEG(DAC),
        DacOff = FP_OFF(DAC);
    asm {
        mov ax,1012h
        mov bx,00h
        mov cx,100h
        mov dx, DacSeg
        mov es,dx
        mov dx, DacOff
        int 10h
    }
/*/
     int i;

    for ( i = 0; i < 256; i++ )
    {
        outportb(0x03c8, (unsigned char) i );
        outportb(0x03c9, DAC[i*3]  >>2  );
        outportb(0x03c9, DAC[i*3+1] >>2 );
        outportb(0x03c9, DAC[i*3+2]  >>2);
    }
*/
}
