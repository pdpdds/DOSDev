#include <stdio.h>
#include <string.h>

#pragma inline
#define HANJA24FNT "hanja24.fnt"

typedef unsigned char BYTE;

static const int  MaxColumn = 80;
static const int  MaxX      = 640;
static const int  MaxY      = 480;
static const BYTE WriteOP   = 0x00;
static const BYTE mdata[8]  = {0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};

void vga_bitmap( int x, int y, int w, int h, int color, const char* img)
{
    int bytenum,imginc,taillen,incr,x_width;
    char sh1,sh2,tailmask,prev,wfunc;

    x_width = MaxColumn;
    wfunc   = WriteOP;

    if( x+w > MaxX ){
    bytenum = (MaxX-x+7)/8;
    taillen = MaxX&7;
    }
    else{
    bytenum = ((x&7)+w+7)/8;
    taillen = (x+w)&7;
    }
    if( y+h > MaxY )    h = MaxY-y;

    asm     push    ds
    asm     mov     dx , w
    asm     mov     ax , h

    asm     mov     bx , x
    asm     mov     cx , 3

    asm     mov     ax , bx         //sh1 = x & 7
    asm     and     ax , 7
    asm     mov     sh1 , al

    asm     mov     ax , bytenum
    asm     sub     ax , x_width         //incr = 80 - bytenum
    asm     neg     ax
    asm     mov     incr , ax

    asm     mov     al , 8          //sh2 = 8 - sh1
    asm     sub     al , sh1
    asm     mov     sh2 , al

    asm     mov     ax , dx         //imginc = (w+7)>>3
    asm     add     ax , 7
    asm     shr     ax , cl
    asm     mov     imginc , ax
    asm     mov     ax , taillen
    asm     mov     si , ax         //tailmask = mdata[taillen]
    asm     mov     al , mdata[si]
    asm     mov     tailmask , al

    asm     cmp     byte ptr sh1 , 0         //if( sh1 )  bytenum--
    asm     jz      next_vbm1
    asm     dec     word ptr bytenum
next_vbm1:
    asm     cmp     word ptr taillen , 0     //if( taillen )  bytenum--
    asm     jz      next_vbm2
    asm     dec     word ptr bytenum
next_vbm2:
    asm     shr     bx , cl         // x = x>>3
    asm     mov     ax , 0A000h
    asm     mov     es , ax
    asm     mov     ax , y          // di = x + y*80
    asm     mov     dx , x_width
    asm     mul     dx
    asm     add     ax , bx
    asm     mov     di , ax

    asm     lds     si , dword ptr img

    asm     mov     bx , color
    asm     mov     dx , 03ceh
    asm     mov     ax , 0305h
    asm     out     dx , ax
    asm     mov     ah , wfunc
    asm     mov     al , 3
    asm     out     dx , ax
    asm     mov     ah , bl
    asm     mov     al , 0
    asm     out     dx , ax
    asm     mov     ax , 0F01h
    asm     out     dx , ax
    asm     mov     ax , 0FF08h
    asm     out     dx , ax

    asm     mov     ah , tailmask
    asm     mov     cx , h
    asm     cmp     byte ptr sh1 , 0
    asm     jne     bit_put
byte_put:
    asm     mov     dx , cx
    asm     mov     cx , bytenum
    asm     jcxz    no_full_byte
    asm     push    si
full_byte:
    asm     mov     al , es:[di]
    asm     movsb
    asm     loop    full_byte
no_full_byte:
    asm     cmp     word ptr taillen , 0
    asm     je      next_byte_put
    asm     mov     al , es:[di]
    asm     lodsb
    asm     and     al , ah
    asm     stosb
next_byte_put:
    asm     pop     si
    asm     add     si , imginc
    asm     add     di , incr
    asm     mov     cx , dx
    asm     loop    byte_put
    asm     jmp     end_pbm
bit_put:
    asm     mov     dx , cx
    asm     push    si
    asm     lodsb
    asm     mov     prev , al
    asm     mov     bh , es:[di]
    asm     mov     cl , sh1
    asm     shr     al , cl
    asm     stosb
    asm     mov     cx , bytenum
    asm     mov     al , prev
    asm     jcxz    no_byte
full_byte_bit:
    asm     mov     bh , es:[di]
    asm     push    cx
    asm     mov     cl , sh1
    asm     mov     ch , sh2
    asm     mov     bh , [si]
    asm     shr     bh , cl
    asm     xchg    cl , ch
    asm     shl     al , cl
    asm     or      al , bh
    asm     stosb
    asm     lodsb
    asm     pop     cx
    asm     loop    full_byte_bit
no_byte:
    asm     cmp     word ptr taillen , 0
    asm     je      next_bit_put
    asm     mov     bh , es:[di]
    asm     mov     cl , sh1
    asm     mov     ch , sh2
    asm     mov     bh , [si]
    asm     shr     bh , cl
    asm     xchg    cl , ch
    asm     shl     al , cl
    asm     or      al , bh
    asm     and     al , ah
    asm     stosb
next_bit_put:
    asm     pop     si
    asm     add     si , imginc
    asm     add     di , incr
    asm     mov     cx , dx
    asm     loop    bit_put
end_pbm:
    asm     mov     dx , 03CEh
    asm     mov     ax , 0005h
    asm     out     dx , ax
    asm     mov     ax , 0001h
    asm     out     dx , ax
    asm     mov     ax , 0003h
    asm     out     dx , ax
    asm     mov     ax , 0000h
    asm     out     dx , ax
    asm     mov     ax , 0FF08h
    asm     out     dx , ax
    asm     pop     ds
}
#if 0
static void scale_bitmap
(int xsize1,int ysize1,int xsize2,int ysize2,unsigned char *src,unsigned char *dest)
{
    unsigned int i,j,k,m,n;
    unsigned int nbyte1,nbyte2;
    static unsigned char on[8]={ 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01    };
    static unsigned char off[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe    };
    nbyte1 = (xsize1+7)/8;
    nbyte2 = (ysize2+7)/8;
    for(i=0;i<ysize2;i++)
    {
    for(j=0;j<xsize2;j++)
    {
        m = (j*xsize1)/xsize2;
        n = (j*ysize1)/ysize2;
        if( src[n*nbyte1+(m/8)] & on[m%8] )
            dest[i*nbyte2+(j/8)] |= on[j%8];
        else    dest[i*nbyte2+(j/8)] &= off[j%8];
    }
    for(j=xsize2;j<nbyte2*8;j++)
	dest[i*nbyte2+(j/8)] &= off[j%8];
    }
}


int    VERTICAL, HORIZONTAL;

static void scale_img( int xbyte,int flag,char *src,char *dest )
{
    const static char horiz[]={0x00,0x03,0x0C,0x0f,0x30,0x33,0x3C,0x3f,0xc0,0xc3,0xcc,0xcf,0xf0,0xf3,0xfc,0xff};
    register int i,j;
    int bytes = xbyte*16;

#if 0
    if( (flag & VERTICAL) && (flag & HORIZONTAL) )
    {
    for(i=0;i<16;i++)
    {
        for( j=0;j<xbyte;j++ )
        {
        *dest++ = horiz[ (src[j]>>4)&0xf ];
        *dest++ = horiz[ src[j]&0xf ];
        }
	for( j=0;j<xbyte;j++ )
        {
        *dest++ = horiz[ (*src>>4)&0xf ];
        *dest++ = horiz[ *src++&0xf ];
        }
    }
    }
    else if(flag & VERTICAL )
    {
    for(i=0;i<16;i++ )
    {
        for( j=0;j<xbyte;j++ )      *dest++ = src[j];
        for( j=0;j<xbyte;j++ )      *dest++ = *src++;
    }
    }
    else if( flag & HORIZONTAL )
    {
#endif
    for( i=0;i<bytes;i++ )
    {
	*dest++ = horiz[ (src[i]>>4)&0xf ];
	*dest++ = horiz[ src[i]&0xf ];
    }
//    }
}
#endif

int hj24( int x, int y, int Tcolor, const char *s )
{
    BYTE byte1,byte2;
    FILE *fp;
    long index;
    char fntimg[72];

    byte1 = *s;
    byte2 = *(s+1);

    if( byte1 < 0xe0 ) return 1;

    byte1 -= 0xe0;
    index  = byte1*188+( (byte2 <0x91)? (byte2-0x31):(byte2-67) );
    index *= 72;

    if( !(fp = fopen(HANJA24FNT,"rb" )))
    {
      puts("Can Not Find HanJa24 Font !!");
      return 0;
    }
    fseek( fp,index,SEEK_SET );
    fread( fntimg,72,1,fp );
    fclose(fp);
    vga_bitmap( x, y,   24,   24, Tcolor, fntimg);
    return 1;
}

int mk_hj24( int x, int y, int Tcolor, BYTE byte1, BYTE byte2 )
{
    FILE *fp;
    long index;
    char buf[512];
    char fntimg[72];

    if( byte1 < 0xe0 ) return 1;

    byte1 -= 0xe0;
    index  = byte1*188+( (byte2 <0x91)? (byte2-0x31):(byte2-67) );
    index *= 72;

    if( !(fp = fopen(HANJA24FNT,"rb" )))
    {
      puts("Can Not Find HanJa24 Font !!");
      return 0;
    }
//    memset(fntimg,0, 72);
//    memset(buf, 0, 256);

    fseek( fp,index,SEEK_SET );
    fread( fntimg,72,1,fp );
    fclose(fp);
//    scale_img(8, 1, fntimg, buf);
//    scale_bitmap(16, 16, 24*2, 24, fntimg, buf);
    vga_bitmap( x, y,   24,   24, Tcolor, fntimg);
    return 1;
}

void put_hj24( int x, int y, int Tcolor, const char *s )
{
    int scale = 2;

    while( *s )
    {
      if( (unsigned char)*s >= 0xe0 )
      if( !hj24( x, y, Tcolor, s ) )
    x+=8;

      if( *s & 0x80 )
      {
    s+=2;    x+=24;
      } else
      {
    s++;     x+=12;
      }
    }
}

#ifdef TEST

#include <dos.h>
#include <conio.h>

int main(void)
{
  static const char *hj_txt1 = " õF çþ çþ  ";
  static const char *hj_txt2 = " äÞ ÷Û èU âÐ ";

  static const BYTE YP =10;
  static int i=0;

  _AX=0x12;
  geninterrupt(0x10);

  for(i =0; i < 16 ; i++)
  {
    put_hj24(10, YP + (27*i), i, hj_txt2);
    i++;
    put_hj24(10, YP + (27*i), i, hj_txt1);
  }
#if 0

  put_hj24(10, YP+27,    3, hj_txt2);
  put_hj24(10, YP+27*2, 12, hj_txt1);
  put_hj24(10, YP+27*3, 14, hj_txt2);

  put_hj24(10, YP+27*4,  1, hj_txt1);
  put_hj24(10, YP+27*5,  2, hj_txt2);
  put_hj24(10, YP+27*6,  5, hj_txt1);
  put_hj24(10, YP+27*7, 24, hj_txt2);
#endif

  getch();
 /*
 * ÑÁ¡e·i ÉB¯aËa ¡¡—a¡ •©a ‘½“e”a
 */
  _AX = 0x03;
  geninterrupt(0x10);

  return(0);
}
#endif