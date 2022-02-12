
#include "vga320.h"

unsigned int Adress[] = {
       0,   320,   640,   960,  1280,  1600,  1920,  2240,  2560,  2880,
    3200,  3520,  3840,  4160,  4480,  4800,  5120,  5440,  5760,  6080,
    6400,  6720,  7040,  7360,  7680,  8000,  8320,  8640,  8960,  9280,
    9600,  9920, 10240, 10560, 10880, 11200, 11520, 11840, 12160, 12480,
   12800, 13120, 13440, 13760, 14080, 14400, 14720, 15040, 15360, 15680,
   16000, 16320, 16640, 16960, 17280, 17600, 17920, 18240, 18560, 18880,
   19200, 19520, 19840, 20160, 20480, 20800, 21120, 21440, 21760, 22080,
   22400, 22720, 23040, 23360, 23680, 24000, 24320, 24640, 24960, 25280,
   25600, 25920, 26240, 26560, 26880, 27200, 27520, 27840, 28160, 28480,
   28800, 29120, 29440, 29760, 30080, 30400, 30720, 31040, 31360, 31680,
   32000, 32320, 32640, 32960, 33280, 33600, 33920, 34240, 34560, 34880,
   35200, 35520, 35840, 36160, 36480, 36800, 37120, 37440, 37760, 38080,
   38400, 38720, 39040, 39360, 39680, 40000, 40320, 40640, 40960, 41280,
   41600, 41920, 42240, 42560, 42880, 43200, 43520, 43840, 44160, 44480,
   44800, 45120, 45440, 45760, 46080, 46400, 46720, 47040, 47360, 47680,
   48000, 48320, 48640, 48960, 49280, 49600, 49920, 50240, 50560, 50880,
   51200, 51520, 51840, 52160, 52480, 52800, 53120, 53440, 53760, 54080,
   54400, 54720, 55040, 55360, 55680, 56000, 56320, 56640, 56960, 57280,
   57600, 57920, 58240, 58560, 58880, 59200, 59520, 59840, 60160, 60480,
   60800, 61120, 61440, 61760, 62080, 62400, 62720, 63040, 63360, 63680
};

int ScreenXStart = 0   ;
int ScreenYStart = 0   ;
int ScreenXEnd   = 319 ;
int ScreenYEnd   = 199 ;

#pragma inline

void SetVga( void )
{
   asm {
      mov ah, 0
      mov al, 13h
      int 10h
   }
}

void EndVga( void )
{
   asm {
      mov ah, 0
      mov al, 3
      int 10h
   }
}

void SetRgbPalette( byte *plt, int start, int len )
{
   char *p = plt + start * 3 ;
   len = len*3 ;

   asm mov  dx, 3dah            // Input Status Register
waitOn:
   asm in   al, dx
   asm test al, 8               // V-retrace check
   asm jz   waitOn

   asm push ds
   asm lds  si, p
   asm mov  dx, 3c8h            // DAC write index
   asm mov  al, byte ptr start
   asm out  dx, al
   asm mov  cx, word ptr len
   asm inc  dx                  // DAC Look-up table data register
   asm rep  outsb
   asm pop  ds
}

void SetRgbPaletteFast( byte *plt, int start, int len )
{
   char *p = plt + start * 3 ;
   len = len*3 ;

   asm push ds
   asm lds  si, p
   asm mov  dx, 3c8h            // DAC write index
   asm mov  al, byte ptr start
   asm out  dx, al
   asm mov  cx, word ptr len
   asm inc  dx                  // DAC Look-up table data register
   asm rep  outsb
   asm pop  ds
}

void SetRgbPaletteSnow( byte *pal )
{
   asm mov  dx, 3dah            // Input Status Register
waitOn0:
   asm in   al, dx
   asm test al, 8               // V-retrace check
   asm jz   waitOn0

   asm push ds
   asm lds  si, pal

   asm mov  dx, 3c8h
   asm mov  al, 0
   asm out  dx, al
   asm mov  dx, 3c9h
   asm mov  cx, 192
   asm rep  outsb

   asm mov  dx, 3dah            // Input Status Register
waitOn2:
   asm in   al, dx
   asm test al, 8               // V-retrace check
   asm jz   waitOn2

   asm mov  dx, 3c8h
   asm mov  al, 64
   asm out  dx, al
   asm mov  dx, 3c9h
   asm mov  cx, 192
   asm rep  outsb

   asm mov  dx, 3dah            // Input Status Register
waitOn3:
   asm in   al, dx
   asm test al, 8               // V-retrace check
   asm jz   waitOn3

   asm mov  dx, 3c8h
   asm mov  al, 128
   asm out  dx, al
   asm mov  dx, 3c9h
   asm mov  cx, 192
   asm rep  outsb

   asm mov  dx, 3dah            // Input Status Register
waitOn4:
   asm in   al, dx
   asm test al, 8               // V-retrace check
   asm jz   waitOn4

   asm mov  dx, 3c8h
   asm mov  al, 192
   asm out  dx, al
   asm mov  dx, 3c9h
   asm mov  cx, 192
   asm rep  outsb

   asm pop  ds
}

void SetPalette( int color_no, char red, char green, char blue )
{
   asm {
      mov ah,10h
      mov al,10h
      mov bx,color_no
      mov dh,red
      mov ch,green
      mov cl,blue
      int 10h
   }
}

int  BufGetPixel( int x, int y, byte *tarbuff )
{
   return *(tarbuff+x+Adress[y]);
}

void BufPutPixel( int x, int y, int color, byte *tarbuff )
{
   *(tarbuff+x+Adress[y]) = color;
}

void BufXLine( int sx, int sy, word value, byte color, byte *tarbuff )
{
   unsigned int _seg2, _off2 ;

   _seg2 = FP_SEG( tarbuff ) ;
   _off2 = FP_OFF( tarbuff ) + sx + Adress[ sy ] ;
   value++;

   asm {
      push es

      mov ax, _seg2
      mov es, ax
      mov di, _off2
      mov ah, color
      mov al, color
      mov cx, value
      shr cx, 1
      rep stosw
   }
   asm jnb _EXIT
   asm stosb
   _EXIT :

   asm pop es
}

void BufYLine( int sx, int sy, int value, int color, byte *tarbuff )
{
   unsigned int _seg2, _off2 ;

   _seg2 = FP_SEG( tarbuff ) ;
   _off2 = FP_OFF( tarbuff ) ;
   _off2 += sx + Adress[ sy ] ;
   value++;

   asm push es
   asm mov ax, _seg2
   asm mov es, ax
   asm mov di, _off2
   asm mov ah, color
   asm mov cx, value
   _Y_LOOP :
   asm mov byte ptr es:[di], ah
   asm add di, 320
   asm loop _Y_LOOP
   asm pop es
}

void BufBoxLine( int sx, int sy, int ex, int ey, int color, byte *tarbuff )
{
   BufXLine( sx, sy, ex-sx, color, tarbuff );
   BufXLine( sx, ey, ex-sx, color, tarbuff );
   BufYLine( sx, sy, ey-sy, color, tarbuff );
   BufYLine( ex, sy, ey-sy, color, tarbuff );
}

void BufBoxFill( int sx, int sy, int ex, int ey, int color, byte *tarbuff )
{
   int x, y;

   for( y = sy; ey >= y; y++ )
      BufXLine( sx, y, ex-sx, color, tarbuff );
}

static int distance, xerr=0, yerr=0, delta_x, delta_y, incx, incy ;
void BufLine( int startx, int starty, int endx, int endy, int color, byte *tarbuff )
{
   int j ;

   xerr = 0 ;
   yerr = 0 ;
   incx = 0 ;
   incy = 0 ;

   delta_x = endx - startx ;
   delta_y = endy - starty ;
   if( delta_x > 0 ) incx=1 ; else if( delta_x == 0 ) incx=0 ; else incx=-1 ;
   if( delta_y > 0 ) incy=1 ; else if( delta_y == 0 ) incy=0 ; else incy=-1 ;

   delta_x = abs( delta_x ) ;
   delta_y = abs( delta_y ) ;
   if( delta_x > delta_y ) distance = delta_x ; else distance = delta_y ;

   *(tarbuff+startx+Adress[starty]) = color ;
   for( j=0; j<=distance; j++ )
   {
      *(tarbuff+startx+Adress[starty]) = color ;
      xerr += delta_x ;
      yerr += delta_y ;
      if( xerr > distance ) { xerr -= distance ; startx += incx ; }
      if( yerr > distance ) { yerr -= distance ; starty += incy ; }
   }
   *(tarbuff+startx+Adress[starty]) = color ;
}

void BufGetImage( word x, word y, word x1, word y1, byte *buff, byte *tarbuff )
{
  unsigned int width=0, height=0 ;

  width   = x1 - x ;
  height  = y1 - y ;

  *buff++ = width &  0x00ff  ;
  *buff++ = width >> 8       ;
  *buff++ = height &  0x00ff ;
  *buff++ = height >> 8      ;
  x       += Adress[ y ]     ;
  width++                    ;
  height++                   ;

   asm     push ds
   asm     push es
   asm     les di, buff
   asm     lds si, tarbuff
   asm     add si, x
   asm     mov cx, height
       puty:
   asm     push cx
   asm     mov cx, width
   asm     shr cx, 1
   asm     rep movsw
   asm     jnb JUMP
   asm     movsb
       JUMP :
   asm     add si, 320
   asm     sub si, width
   asm     pop cx
   asm loop puty
   asm     pop es
   asm     pop ds
}

void BufPutImage( int x, int y, byte *buff, byte *tarbuff )
{
   int  sizex=0, sizey=0, SeekXY=0 ;
   unsigned int Sseekx=0, Eseekx=0 ;
   unsigned int Sseeky=0, Eseeky=0 ;
   unsigned int Sseekybyte=0 ;

   sizex = *buff+(*(buff+1)<<8) ;
   buff += 2                    ;
   sizey = *buff+(*(buff+1)<<8) ;
   buff += 2                    ;

   if( x+sizex < ScreenXStart ) return ;
   if( y+sizey < ScreenYStart ) return ;
   if( x       > ScreenXEnd   ) return ;
   if( y       > ScreenYEnd   ) return ;

   if( x       < ScreenXStart ) Sseekx = ScreenXStart - x ;
   if( x+sizex > ScreenXEnd   ) Eseekx = x+sizex-ScreenXEnd ;

   if( y       < ScreenYStart ) {
      Sseeky = ScreenYStart - y ;
      Sseekybyte = Sseeky * (sizex+1) ;
      x += Adress[0] ;
   } else {
      x += Adress[y] ;
   }
   if( y+sizey > ScreenYEnd ) Eseeky = y+sizey-ScreenYEnd ;

   SeekXY = ScreenXEnd - sizex + Sseekx + Eseekx ;
   sizex -= Sseekx + Eseekx ;
   sizey -= Sseeky + Eseeky ;
   sizex++                  ;
   sizey++                  ;
   Eseekx += Sseekx         ;

	asm  push ds
	asm  push es
	asm  les  di,  tarbuff
	asm  lds  si,  buff
	asm  mov  ax,  sizey
	asm  mov  bx,  sizex
	asm  add  si,  Sseekybyte
	asm  add  si,  Sseekx
	asm  add  di,  x
	asm  add  di,  Sseekx
	asm  cld
	LOOP: asm
		asm  mov   cx, bx
		asm  shr   cx,  1
		asm  rep   movsw
		asm  jnb   JUMP
		asm  movsb
	JUMP: asm
		asm  add di, SeekXY
		asm  add si, Eseekx
		asm  dec   ax
		asm  jne   LOOP
	asm  pop  es
	asm  pop  ds
}

void BufSpritePutImage( int x, int y, byte *buff, byte *tarbuff )
{
   int x1=0, y1=0, SeekXY=0 ;
   unsigned int Sseekx=0, Eseekx=0 ;
   unsigned int Sseeky=0, Eseeky=0 ;
   unsigned int Sseekybyte=0 ;

   x1    = *buff+(*(buff+1)<<8) ;
   buff += 2                    ;
   y1    = *buff+(*(buff+1)<<8) ;
   buff += 2                    ;

   if( x+x1 < ScreenXStart ) return ;
   if( y+y1 < ScreenYStart ) return ;
   if( x    > ScreenXEnd   ) return ;
   if( y    > ScreenYEnd   ) return ;

   if( x    < ScreenXStart ) Sseekx = ScreenXStart - x ;
   if( x+x1 > ScreenXEnd   ) Eseekx = x+x1-ScreenXEnd ;

   if( y    < ScreenYStart ) {
      Sseeky = ScreenYStart - y ;
      Sseekybyte = Sseeky * (x1+1) ;
      x += Adress[0] ;
   } else {
      x += Adress[y] ;
   }
   if( y+y1 > ScreenYEnd ) Eseeky = y+y1-ScreenYEnd ;

   SeekXY = ScreenXEnd - x1 + Sseekx + Eseekx ;
   x1 -= Sseekx + Eseekx ;
   y1 -= Sseeky + Eseeky ;
   x1++                  ;
   y1++                  ;
   Eseekx += Sseekx      ;

   asm     push ds
   asm     push es
   asm     les di, tarbuff
   asm     lds si, buff
   asm     mov cx, y1
   asm     add si, Sseekybyte
   asm     add si, Sseekx
   asm     add di, x
   asm     add di, Sseekx
       puty:
   asm     push cx
   asm     mov cx, x1
       putx:
   asm     mov ah, [si]
   asm     test ah, ah
   asm     jz  _exit
   asm     mov byte ptr es:[di], ah
       _exit:
   asm     inc di
   asm     inc si
   asm loop putx
   asm     add di, SeekXY
   asm     add si, Eseekx
   asm     pop cx
   asm loop puty
   asm     pop es
   asm     pop ds
}

void MoveData( byte *_data1, byte *_data2, word data_size )
{
   asm     push ds
   asm     push es
   asm     lds  si, _data1
   asm     les  di, _data2
   asm     mov  cx, data_size
   asm     shr  cx, 1
   asm     cld
   asm     rep  movsw
   asm     pop  es
   asm     pop  ds
}

void SpriteMoveData( byte *_data1, byte *_data2, word data_size )
{
   asm     push ds
   asm     push es
   asm     lds si, _data1
   asm     les di, _data2
   asm     mov cx, data_size
   asm     shr cx, 1
       Start:
   asm     mov ax, [si]
   asm     test ax, ax
   asm     jz  _Seek2                  /* AX Color 0 */
   asm     test al, al
   asm     jz  _Seek                   /* AL Color 0 */
   asm     test ah, ah
   asm     jz  _Seek1                  /* AH Color 0 */
   asm     mov word ptr es:[di], ax
   asm     jmp _Seek2
       _Seek:
   asm     mov byte ptr es:[di+1], ah
   asm     jmp _Seek2
       _Seek1:
   asm     mov byte ptr es:[di], al
       _Seek2:
   asm     add si, 2
   asm     add di, 2
   asm loop Start
   asm     pop es
   asm     pop ds
}

void BufMoveImage( int sx, int sy, int ex, int ey, byte *sour, byte *dest )
{
   int sizex=0, sizey=0 ;
   unsigned int SeekXY=0, Sseekx=0, Sseeky=0, Eseekx=0, Eseeky=0 ;

   sizex = ex - sx ;
   sizey = ey - sy ;

   if( sx+sizex < ScreenXStart ) return ;
   if( sy+sizey < ScreenYStart ) return ;
   if( sx       > ScreenXEnd   ) return ;
   if( sy       > ScreenYEnd   ) return ;

   if( sx       < ScreenXStart ) Sseekx = ScreenXStart - sx ;
   if( sx+sizex > ScreenXEnd   ) Eseekx = sx+sizex-ScreenXEnd ;

   if( sy       < ScreenYStart ) {
      Sseeky = ScreenYStart - sy ;
      sx += Adress[0] ;
   } else {
      sx += Adress[sy] ;
   }
   if( sy+sizey > ScreenYEnd ) Eseeky = sy+sizey-ScreenYEnd ;

   SeekXY = ScreenXEnd - sizex + Sseekx + Eseekx ;
   sizex -= Sseekx + Eseekx ;
   sizey -= Sseeky + Eseeky ;
   sizex++                  ;
   sizey++                  ;

	asm  push ds
	asm  push es
	asm  lds  si,  sour
	asm  les  di,  dest
	asm  mov  ax,  sizey
	asm  mov  bx,  sizex
	asm  add  si,  sx
	asm  add  si,  Sseekx
	asm  add  di,  sx
	asm  add  di,  Sseekx
	asm  cld
	LOOP: asm
		asm  mov   cx, bx
		asm  shr   cx, 1
		asm  rep   movsw
		asm  jnb   JUMP
		asm  movsb
	JUMP: asm
		asm  add di, SeekXY
		asm  add si, SeekXY
		asm  dec   ax
		asm  jne   LOOP
	asm  pop  es
	asm  pop  ds
}

void MoveImage( int sx, int sy, int ex, int ey, int newx, int newy, byte *tarbuff )
{
   char far *backimage ;

   backimage = (char far *)farmalloc( (ex-sx+1L)*(ey-sy+1L)+4L ) ;
   BufGetImage( sx, sy, ex, ey, backimage, tarbuff ) ;
   BufPutImage( newx, newy, backimage, tarbuff ) ;
   farfree( backimage ) ;
}

int Getkey( int mode )
{
   int ch;

   if( mode==1 )
   {
      while(!bioskey(1));
      if ((ch=getch())==0) return (256+getch());
      return(ch);
   }
   if( !bioskey(1) ) return 0;
   if( (ch=getch())==0 ) return (256+getch());
   return(ch);
}

long GetTime( void )
{
   long a, m, s, h;

   union REGS regs;
   regs.h.ah = 0x2C;
   intdos( &regs, &regs );
   a = regs.h.ch;
   m = regs.h.cl;
   s = regs.h.dh;
   h = regs.h.dl;

   return( a*360000L+m*6000L+s*100L+h );
}

void DelayTime( word data )
{
   long a, m, s, h, stim=0 ;
   union REGS regs ;

   regs.h.ah = 0x2C ;
   intdos( &regs, &regs ) ;
   a = regs.h.ch ;
   m = regs.h.cl ;
   s = regs.h.dh ;
   h = regs.h.dl ;
   stim = (a*360000L+m*6000L+s*100L+h) ;

   while( ((a*360000L+m*6000L+s*100L+h) - stim) < data )
   {
      regs.h.ah = 0x2C ;
      intdos( &regs, &regs ) ;
      a = regs.h.ch ;
      m = regs.h.cl ;
      s = regs.h.dh ;
      h = regs.h.dl ;
   }
}
