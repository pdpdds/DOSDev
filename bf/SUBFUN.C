
#include "bf.h"

#define		LEFT		331
#define		RIGHT		333
#define		UP		328
#define		DOWN		336
#define		TAB		9
#define 	ESC		27
#define 	ENTER		13
#define		SPACE		32

/*
void MoveData386( byte *_data1, byte *_data2 )
{
   unsigned int _seg1, _seg2 ;

   _seg1 = FP_SEG( _data1 );
   _seg2 = FP_SEG( _data2 );

   asm     push ds
   asm     push es
   asm     mov  ds, _seg1
   asm     mov  si, OFFSET _data1
   asm     mov  es, _seg2
   asm     mov  di, OFFSET _data2
   asm     mov  cx, 16000
   asm     cld
   asm     rep  movsd
   asm     pop  es
   asm     pop  ds
}

void FillPage386( byte color, byte *tarbuff )
{
   unsigned int _seg2, _off2 ;

   _seg2 = FP_SEG( tarbuff ) ;
   _off2 = FP_OFF( tarbuff ) ;

   asm {
      push es
      mov  ax,  _seg2
      mov  es,  ax
      mov  di,  _off2
      mov  ah,  color
      mov  al,  color
      shl  eax, 16
      mov  ah,  color
      mov  al,  color
      mov  cx,  16000
      rep  stosd
      pop  es
   }
}

*/
void BufBackSpritePutImage( int x, int y, byte *buff, byte *tarbuff )
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
   asm     cmp ah, 255
   asm     jz  _exit
   asm     mov al, byte ptr es:[di]
   asm     cmp al, 34
   asm     je  j0
   asm     mov ah, 10
       j0:
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

/*
void JoyStickPos( int *x, int *y)
{
   static union REGS r ;

   r.h.ah = 0x84 ;
   r.x.dx = 1 ;
   int86( 0x15, &r, &r ) ;
   *x = r.x.ax ;
   *y = r.x.bx ;
}

void JoyStickButton( int *Button )
{
   int button = 0 ;
   static union REGS r ;

   r.h.ah = 0x84 ;
   r.x.dx = 0 ;
   int86( 0x15, &r, &r ) ;
   if( r.h.al & 0x20 && 1 ) button++ ;
   if( r.h.al & 0x10 && 1 ) button += 2 ;
   switch( button )
   {
      case 0 : *Button = 3 ; break ;
      case 1 : *Button = 1 ; break ;
      case 2 : *Button = 2 ; break ;
      case 3 : *Button = 0 ; break ;
   }
}
*/
int Crash( int sx1, int sy1, int sx2, int sy2,
	   int dx1, int dy1, int dx2, int dy2 )
{
   if( min( (sx2),(dx2) ) > max( (sx1),(dx1) ) )
      if( min( (sy2),(dy2) ) > max( (sy1),(dy1) ) ) return 1 ;
   return 0 ;
}

void Scale( word sx, word sy, word xs, word ys, byte *sou_buff, byte *tar_buff )
{
   unsigned int  xsize, ysize, x, y, xx, yy ;
   byte *_off_buff ;

   xsize = *sou_buff+(*(sou_buff+1)<<8) ;
   sou_buff += 2 ;
   ysize = *sou_buff+(*(sou_buff+1)<<8) ;
   sou_buff += 2 ;
   xsize++ ;
   ysize++ ;

   _off_buff = sou_buff ;
   tar_buff = tar_buff + Adress[sy] + sx ;

   for( y=0; y<ys; y++ )
   {
      yy = (ysize*y/ys) ;
      _off_buff = sou_buff + yy * xsize ;
      for( x=0; x<xs; x++ )
      {
	 xx = ((long)xsize*x/xs) ;
	 if( *(_off_buff+xx) ) *tar_buff = *(_off_buff+xx) ;
	 tar_buff++ ;
      }
      tar_buff += 320 - xs ;
   }
}

int bf_title( void )
{
   char *menu_mes[5] = {
		    "* Àá·q¦Èá *",
		    "* ·¡ ´á ¬á *",
		    "* Ì“¢¡¡—a *",
                    "* £A»¢¡¡—a *",
		    "* a ˆa ‹¡ *"
   } ;
   int select_v=0, i ;

   ViewIMG( "bf-td7", mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   key_clear() ;

   for( i=0; i<5; i++ )
   {
      Hprint( 110+1, 110+(i<<4)+1, menu_mes[i], 79 ) ;
      Hprint( 110,   110+(i<<4),   menu_mes[i], 64 ) ;
   }

   mouse_cursor_on() ;

   i = 1 ;
   while( 1 )
   {
      MHprint( 110, 110+(select_v<<4), menu_mes[select_v], 64 ) ;
      if( kbhit() )
      {
	 Hprint( 110, 110+(select_v<<4), menu_mes[select_v], 64 ) ;
	 i = Getkey( 1 ) ;
	 if( i == UP )
	 {
	    if( select_v == 0 ) select_v=4 ;
	    else                select_v-- ;
	 }
	 if( i == DOWN )
	 {
	    if( select_v == 4 ) select_v=0 ;
	    else                select_v++ ;
	 }
	 if( (i == ENTER) || (i == SPACE) )
	 {
            panicmode = 0 ;                     // Orignal Story Mode
	    if( select_v == 0 ) i = 1 ;         // Start Game,       Menu 1
            if( select_v == 1 ) i = 2 ;         // Continue Game,    Menu 2
            if( select_v == 2 ) panicmode = 1 ; // Panic Mode Game,  Menu 3
	    if( select_v == 3 ) i = 3 ;         // Music Mode Game,  Menu 4
	    if( select_v == 4 ) i = 0 ;         // Exit Game,        Menu 5
	    break ;
	 }
      }
      if( read_mouse() == 2 ) break ;
   }

   mouse_cursor_off() ;

   black_palette( 1 ) ;
   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;

   AdLib_End() ;                                 // Music End

   return i ;
}


void Buster_XY_Data_Load( void )
{
/*   int buster_file, i, j, k ;

   buster_file = open( "buster.dat", O_RDONLY | O_BINARY, S_IWRITE | S_IREAD ) ;
   if( buster_file == -1 )
   {
      printf( "\a File Not Found !\n\a" ) ;
      for( k=0; k<2; k++ )
      {
	 for( j=0; j<5; j++ )
	 {
	    for( i=0; i<16; i++ )
	    {
	       buster.x[k][j][0][i] = 0 ;
	       buster.x[k][j][1][i] = 0 ;
	       buster.y[k][j][0][i] = 0 ;
	       buster.y[k][j][1][i] = 0 ;
	    }
	 }
      }
   }
   else
   {
      read( buster_file, &buster.x, 704 ) ;
      read( buster_file, &buster.y, 704 ) ;
   }
   close( buster_file ) ;
   */

   unsigned char b_x[] =
   {
      251,   2,  10,  19,  23,  33,  41,  48,  54,  49,  41,  31,  23,  17,   7, 255,
      240, 247, 255,  11,  18,  27,  40,  48,  54,  50,  42,  26,  17,  10, 254, 243,
      253,   6,  10,  16,  24,  35,  41,  44,  53,  48,  43,  33,  25,  18,   8,   4,
      242, 251,   1,  12,  22,  32,  41,  46,  53,  49,  43,  25,  16,  13, 253, 247,
      249,   0,   6,  21,  24,  31,  40,  49,  57,  52,  43,  33,  24,  19,   7,   0,
      238, 246, 251,  13,  19,  27,  39,  50,  56,  53,  43,  28,  18,  12, 254, 244,
      247,   1,   8,  21,  23,  30,  42,  48,  58,  53,  41,  31,  24,  20,   6, 255,
      236, 245, 253,  13,  18,  24,  40,  49,  58,  54,  41,  26,  18,  13, 253, 243,
      253,   6,   7,  13,  24,  34,  47,  46,  58,  53,  45,  40,  25,  12,   5,   3,
      242, 250, 253,   5,  19,  29,  45,  46,  58,  52,  45,  34,  19,   5, 252, 248,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       29,  25,  23,  22,  24,  21,  18,  17,  10,  18,  20,  21,  24,  25,  20,  22,
       29,  22,  18,  22,  22,  21,  17,  17,  11,  21,  25,  15,  15,  19,  19,  14,
      253,   5,  10,  17,  24,  36,  41,  44,  53,  49,  43,  33,  25,  17,   8,   3,
      242, 250,   1,  11,  22,  32,  41,  46,  53,  49,  43,  25,  16,  12, 252, 246,
      248,   4,  12,  22,  25,  29,  38,  47,  58,  48,  38,  28,  25,  21,  10,   2,
      237, 248,   2,  15,  20,  23,  36,  48,  58,  49,  38,  24,  19,  14,   1, 246,
        0,   6,   9,  17,  24,  34,  41,  44,  50,  48,  43,  34,  25,  18,  10,   5,
      246, 250,   1,  11,  22,  30,  41,  47,  50,  49,  43,  26,  16,  13, 254, 248,
        2,  13,   8,  16,  24,  35,  42,  46,  48,  51,  45,  35,  25,  17,   6,   0,
      248,   0,   0,  11,  22,  32,  41,  48,  47,  52,  45,  27,  16,  11, 251, 243,
      252,   4,   7,  14,  23,  33,  45,  51,  53,  52,  46,  32,  23,  17,   6, 254,
      239, 248, 253,   6,  18,  28,  43,  52,  55,  52,  45,  27,  17,  10, 250, 242,
      253,   2,   7,  13,  24,  37,  45,  48,  53,  50,  46,  38,  25,  14,   5,   2,
      242, 247, 254,   7,  22,  34,  44,  50,  53,  51,  46,  30,  16,   9, 249, 245,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        1,   7,  19,  18,  23,  35,  41,  44,  48,  48,  43,  35,  23,  15,   8,   2,
      246, 252,  10,  12,  21,  32,  41,  47,  48,  49,  43,  27,  14,  10, 253, 246,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      253,   1,   8,  14,  24,  38,  43,  48,  53,  51,  44,  36,  25,  16,   7,   1,
      242, 246, 255,   8,  22,  35,  43,  51,  53,  52,  44,  28,  16,  11, 251, 245,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      253,   4,  12,  17,  24,  34,  39,  46,  54,  52,  42,  35,  25,  17,  10,   0,
      242, 249,   3,  11,  22,  31,  39,  48,  54,  52,  42,  27,  16,  12, 254, 244,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
   } ;

   unsigned char b_y[] =
   {
       21,  33,  38,  41,  43,  43,  41,  33,  21,  12,   4, 255, 254, 253,   4,  13,
       15,  31,  37,  42,  44,  43,  39,  30,  15,   4, 251, 245, 242, 243, 251,   6,
       22,  33,  39,  38,  44,  43,  39,  32,  25,  10,   4,   0, 253, 253,   2,   5,
       16,  30,  40,  38,  45,  42,  36,  28,  19,   4, 253, 245, 241, 243, 251, 255,
       23,  29,  40,  45,  47,  46,  39,  32,  22,  14,   1, 253, 251, 254,   2,   8,
       16,  27,  39,  46,  48,  46,  37,  28,  16,   6, 247, 242, 239, 244, 249,   0,
       22,  30,  39,  41,  46,  45,  38,  30,  25,  11,   6,   0, 253, 254,   4,  12,
       16,  27,  38,  42,  47,  45,  37,  27,  19,   3, 253, 246, 241, 244, 250,   5,
       22,  30,  38,  42,  44,  40,  37,  32,  23,  11,   2, 255, 251, 253,   2,   5,
       16,  28,  37,  43,  45,  40,  35,  28,  17,   4, 249, 245, 239, 243, 249, 254,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       24,  23,  24,  25,  31,  15,  18,  17,  22,  21,  15,  18,  19,  19,  18,  18,
       17,  15,  20,  15,  19,  14,  12,  12,  17,  17,  13,  16,  10,  14,  17,  16,
       22,  32,  39,  38,  44,  43,  40,  33,  24,  10,   4,   0, 253, 253,   2,   5,
       16,  29,  40,  39,  45,  42,  36,  29,  18,   4, 253, 245, 241, 243, 251,   0,
       23,  32,  35,  40,  45,  41,  36,  30,  23,  15,   7,   0, 254,   1,   7,  15,
       17,  30,  35,  41,  46,  42,  35,  28,  17,   7, 253, 247, 242, 247, 253,   7,
       21,  32,  37,  36,  43,  41,  38,  31,  24,   9,   4,   1, 255, 254,   2,   4,
       15,  28,  38,  37,  44,  40,  35,  27,  17,   4, 253, 246, 243, 244, 252, 254,
       22,  31,  38,  40,  46,  45,  39,  30,  22,   8,   3, 255, 253, 254,   3,   7,
       16,  28,  39,  40,  46,  43,  36,  26,  16,   2, 252, 244, 242, 244, 252,   1,
       20,  26,  34,  41,  41,  40,  35,  25,  20,  13,   7, 254, 252, 254,   7,  14,
       16,  26,  35,  43,  44,  42,  36,  23,  16,   6, 255, 245, 242, 244, 254,   8,
       21,  29,  36,  36,  44,  40,  36,  30,  23,  11,   5,   2, 255, 255,   3,   5,
       15,  26,  37,  37,  45,  39,  32,  25,  17,   5, 254, 247, 243, 245, 252,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       21,  29,  31,  39,  41,  41,  36,  31,  20,  10,   7,   2,   1, 255,   7,  12,
       15,  26,  32,  40,  42,  40,  33,  26,  14,   4,   0, 248, 245, 245,   0,   7,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       21,  29,  35,  41,  44,  43,  37,  30,  23,  11,   5,   1, 253, 254,   4,   6,
       15,  26,  37,  42,  45,  42,  35,  26,  17,   6, 254, 246, 241, 244, 253,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
       23,  29,  37,  41,  44,  43,  39,  31,  25,  12,   6,   2, 254,   0,   5,  11,
       17,  26,  38,  42,  45,  42,  36,  27,  19,   7, 255, 247, 242, 246, 254,   6,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
   } ;

   memcpy( &buster.x, &b_x, 704L ) ;
   memcpy( &buster.y, &b_y, 704L ) ;
}

void save_continue_data( void )                  //**** Save Continue Data
{
   int wing_file ;                               //**** Wing Man File Handle

   wing_file = open( "bf.con", O_WRONLY | O_CREAT | O_BINARY | O_TRUNC, S_IREAD | S_IWRITE ) ;
   write( wing_file, &stage.number, 1 ) ;        //**** Stage Number Write
   write( wing_file, &wing_man, 50 ) ;           //**** Wing Man Data Write
   close( wing_file ) ;                          //**** Close Wing Man File
}

void load_continue_data( void )                  //**** Load Continue Data
{
   int wing_file ;                               //**** Wing Man File Handle

   wing_file = open( "bf.con", O_RDONLY | O_BINARY, S_IWRITE | S_IREAD ) ;
   if( wing_file == -1 ) return ;                //**** File Not Found !!!
   read( wing_file, &stage.number, 1 ) ;         //**** Stage Number Read
   read( wing_file, &wing_man, 50 ) ;            //**** Wing Man Data Read
   close( wing_file ) ;                          //**** Close Wing Man File
   stage.number-- ;                              //**** Stage Number One --
}

/*
void Buster_XY_Edit_Mode( void )
{
   int air_value, fly_mode, key, buster_file, se_air[3], i, j ;

   AdLib_End() ;
   AdLib_Start( "bfdemo3" ) ;

   Buster_XY_Data_Load() ;

   BufXLine( 0, 0, 64000, 0, mcga2 ) ;
   BufMHprint( 50, 10,  "BLUE FALCON Buster Edit Mode", 3<<4, mcga2 ) ;
   BufMHprint( 50, 140, "     *** KEY-HELP ***",  5<<4, mcga2 ) ;
   BufMHprint( 50, 160, "UP, DOWN, LEFT, RIGHT, TAB",  5<<4, mcga2 ) ;
   BufMHprint( 50, 180, "Air change, Fly mode, Buster attr", 5<<4, mcga2 ) ;
   MoveData( mcga2, mcga1, 64000 ) ;
   MoveData( mcga2, mcga0, 64000 ) ;

   se_air[0] = my_air[0].air ;
   j = 1 ;
   for( i=0; i<11; i++ )                      //************* Search Play Air
   {
      if( stage.air_new[i] )
      {
	 se_air[j] = i ;
	 j++ ;
      }
   }

   air_value = 0 ;
   fly_mode = 0 ;
   while( 1 )
   {
      BufMoveImage( 0, 40, 319, 160, mcga2, mcga1 ) ;
      sprintf( st, "X=%2d, Y=%2d", buster.x[buster.attr][se_air[air_value]][buster.size][fly_mode], buster.y[buster.attr][se_air[air_value]][buster.size][fly_mode] ) ;
      BufMHprint( 60, 40, st, 5<<4, mcga1 ) ;
      sprintf( st, "Buster attr=%d, Fly mode=%d", buster.size, fly_mode ) ;
      BufMHprint( 60, 60, st, 5<<4, mcga1 ) ;

      BufSpritePutImage( 130+buster.x[buster.attr][se_air[air_value]][buster.size][fly_mode], 75+buster.y[buster.attr][se_air[air_value]][buster.size][fly_mode], buster_image[buster.img_num[se_air[air_value]]][buster.size][fly_mode], mcga1 ) ;
      Air_Output( 130, 75, fly_mode, se_air[air_value], mcga1 ) ;
      BufMoveImage( 0, 40, 319, 160, mcga1, mcga0 ) ;

      while( !kbhit() )
      {
	 if( !CheckPlay() )
	 {
	    AdLib_End() ;
	    AdLib_Start( "bfdemo3" ) ;
	 }
      }
      key = Getkey( 1 ) ;
      switch( key )
      {
	 case UP    : if( buster.y[buster.attr][se_air[air_value]][buster.size][fly_mode] != -20 ) buster.y[buster.attr][se_air[air_value]][buster.size][fly_mode]-- ;
		      break ;
	 case DOWN  : if( buster.y[buster.attr][se_air[air_value]][buster.size][fly_mode] !=  70 ) buster.y[buster.attr][se_air[air_value]][buster.size][fly_mode]++ ;
		      break ;
	 case LEFT  : if( buster.x[buster.attr][se_air[air_value]][buster.size][fly_mode] != -20 ) buster.x[buster.attr][se_air[air_value]][buster.size][fly_mode]-- ;
		      break ;
	 case RIGHT : if( buster.x[buster.attr][se_air[air_value]][buster.size][fly_mode] !=  80 ) buster.x[buster.attr][se_air[air_value]][buster.size][fly_mode]++ ;
		      break ;
	 case 'A'   :
	 case 'a'   :
		      if( air_value == 2 ) air_value = 0 ;
		      else                 air_value++ ;
		      break ;
	 case 'F'   :
	 case 'f'   :
		      if( fly_mode == 15 ) fly_mode = 0 ;
		      else                 fly_mode++ ;
		      break ;
	 case 'B'   :
	 case 'b'   :
		      if( !buster.size ) buster.size = 1 ;
		      else               buster.size = 0 ;
		      break ;
      }
      if( key == TAB ) break ;
   }

   buster_file = open( "buster.dat", O_WRONLY | O_CREAT | O_BINARY | O_TRUNC, S_IREAD | S_IWRITE ) ;
   write( buster_file, &buster.x, 704 ) ;
   write( buster_file, &buster.y, 704 ) ;
   close( buster_file ) ;
}

void Rader_XY_Edit_Mode( void )
{
   int key, x, y, xr, yr ;

   BufMHprint( 50, 10,  "BLUE FALCON Rader Edit Mode", 3<<4, mcga1 ) ;

   x = 0 ;
   y = 0 ;
   xr = 46 ;
   yr = 74 ;
   while( 1 )
   {
      BufMoveImage( 0, 0, 319, 60, mcga1, mcga0 ) ;

      BufPutPixel( 245+2+21+(x/xr), 20+2+13+(y/yr), 255, mcga0 ) ;

      sprintf( st, "X=%2d, Y=%2d", xr, yr ) ;
      BufMHprint( 60, 40, st, 5<<4, mcga0 ) ;
      key = Getkey( 1 ) ;
      switch( key )
      {
	 case 'X'   : xr++ ; break ;
	 case 'x'   : xr-- ; break ;
	 case 'Y'   : yr++ ; break ;
	 case 'y'   : yr-- ; break ;
      }
      if( key == TAB ) break ;
   }
}
*/
void SetBFPalette( int mode )
{
   static char count=0 ;
   unsigned char p[256][3] ;
   int i ;

   if( (mode == 0) && count )
   {
      if( count==1 )
      {
	 time_int_push = 1 ;
	 SetRgbPaletteSnow( (unsigned char *)Pal ) ;
	 time_int_push = 0 ;
      }
      count-- ;
   }
   if( (mode == 1) && !count )
   {
      for( i=0; i<(256-16); i++ )
      {
	 p[i][0] = (Pal[i][0] + Pal[i][1] + Pal[i][2] + 60)/3 ;
	 if( p[i][0] > 63 ) p[i][0] = 63 ;
	 p[i][1] = 0 ;
	 p[i][2] = 0 ;
      }
      p[255][0] = (Pal[255][0] + Pal[255][1] + Pal[255][2] + 60)/3 ;
      if( p[255][0] > 63 ) p[255][0] = 63 ;
      p[255][1] = 0 ;
      p[255][2] = 0 ;
      time_int_push = 1 ;
      SetRgbPalette( (unsigned char *)p, 0, 256-16 ) ;
      SetRgbPaletteFast( (unsigned char *)p, 255, 1 ) ;
      time_int_push = 0 ;
      count = 2 ;
   }
   if( (mode == 2) && !count )
   {
      for( i=0; i<(256-16); i++ )
      {
	 p[i][0] = p[i][1] = p[i][2] = ((Pal[i][0] + Pal[i][1] + Pal[i][2]+60) / 3) ;
	 if( p[i][0] > 63 )
	 {
	    p[i][0] = p[i][1] = p[i][2] = 63 ;
	 }
      }
      p[255][0] = p[255][1] = p[255][2] = ((Pal[255][0] + Pal[255][1] + Pal[255][2]+60) / 3) ;
      if( p[255][0] > 63 )
      {
	 p[255][0] = p[255][1] = p[255][2] = 63 ;
      }
      time_int_push = 1 ;
      SetRgbPalette( (unsigned char *)p, 0, 256-16 ) ;
      SetRgbPaletteFast( (unsigned char *)p, 255, 1 ) ;
      time_int_push = 0 ;
      count = 2 ;
   }
}

void SetFlyPalette( int fly_count )
{
   unsigned char p[256][3] ;
   int i ;

   if( fly_count <= 5 ) return ;

   fly_count = ((fly_count-5)<<1) ;
   for( i=0; i<(256-16); i++ )
   {
      p[i][0] = Pal[i][0] + fly_count ;
      p[i][1] = Pal[i][1] + fly_count ;
      p[i][2] = Pal[i][2] + fly_count ;
      if( p[i][0] > 63 ) p[i][0] = 63 ;
      if( p[i][1] > 63 ) p[i][1] = 63 ;
      if( p[i][2] > 63 ) p[i][2] = 63 ;
   }
   p[255][0] = Pal[255][0] ;
   p[255][1] = Pal[255][1] ;
   p[255][2] = Pal[255][2] ;
   time_int_push = 1 ;
   SetRgbPaletteFast( (unsigned char *)p, 0, (256-16) ) ;
   SetRgbPaletteFast( (unsigned char *)p, 255, 1 ) ;
   time_int_push = 0 ;
}

void palette_view( void )
{
   int x, y, gkey, i ;
   char str[80] ;

   i = 0 ;
   for( y=0; y<8; y++ )
   {
      for( x=0; x<32; x++ )
      {
	 BufBoxFill( x*10, y*10, (x*10)+9, (y*10)+9, i++, mcga0 ) ;
      }
   }
   x = 0 ;
   y = 0 ;
   while( 1 )
   {
      BufBoxLine( x, y, x+9, y+9, 0, mcga0 ) ;
      gkey = Getkey(1) ;
      BufBoxLine( x, y, x+9, y+9, BufGetPixel( x+5, y+5, mcga0 ), mcga0 ) ;
      if( gkey == UP    ) y -= 10 ;
      if( gkey == DOWN  ) y += 10 ;
      if( gkey == LEFT  ) x -= 10 ;
      if( gkey == RIGHT ) x += 10 ;
      if( gkey == ESC   ) break ;
      gotoxy( 1, 11 ) ;
      printf( "%3d", BufGetPixel( x+5, y+5, mcga0 ) ) ;
      sprintf( str, "%3d", BufGetPixel( x+5, y+5, mcga0 ) ) ;
      MoveData( mcga1+(320*100), mcga0+(320*100), 320*16 ) ;
      Hprint( 0, 100, str, BufGetPixel( x+5, y+5, mcga0 ) ) ;
   }

   MoveData( mcga1, mcga0, 64000 ) ;
}

char weapon_number[15] = { 0, 2, 3, 4, 5, 6, 7, 8, 0, 0, 8, 8, 8, 8, 8 } ;
unsigned char *mis_mes[8] = {
 "ˆáŸ¡“e ¼l»¡ e ¡wº—ŸI·e ³i eÐs“¡”a.",
 "·¡ Š¯¢·e ˆáŸ¡“e á ‹©´á¶a.",
 "£¡Š‚µA¬á ³a·¡“e ¯¥Ñw·¡ºa. ˆáŸ¡•¡ ‹©‰¡¶a.",
 "Â‰Àá“e ´i®ˆa ´ô“e•A ”eˆáŸ¡¶w·¡œa”áŠ…¶a.",
 "·¡œåˆå ´á—¡¬á µ¥ˆå»¡... º—¸wˆáŸ¡·³“¡”a.",
 "º—”eˆáŸ¡¶w·¡œa”å•A, µb¯¡ ¯¥¢‹¡·³“¡”a.",
 "º—”eˆáŸ¡œa”å•A, ¸÷ÁA“e ¸á•¡ ¡¡Ÿa‰V´á¶a.",
 "¸wˆáŸ¡¶w £¡¬a·©·¡œa”áŠ…¶a."
} ;
unsigned char s_mis_image[8][147] ;
extern int mouse_x, mouse_y ;

void Select_Weapon( void )
{
   int i, select = 0 ;

   if( !weapon_number[stage.number-1] )
   {
      if( stage.number == 1  ) my_mis.weapon = 1 ;
      if( stage.number == 10 ) my_mis.weapon = 0 ;
      return ;
   }

   ViewIMG( "weapon", mcga1 ) ;
   for( i=0; i<8; i++ )
   {
      BufGetImage( 1+(i*14), 1, 1+12+(i*14), 1+10, s_mis_image[i], mcga1 ) ;
   }

   ViewIMG( "w-sel", mcga2 ) ;
   for( i=weapon_number[stage.number-1]; i<8; i++ )
   {
      BufBoxFill( 109, 86+(i*11), 109+67, 86+7+(i*11), 27, mcga2 ) ;
      BufBoxFill( 183, 86+(i*11), 183+26, 86+7+(i*11), 27, mcga2 ) ;
   }

// BufMHprint( 232, 164, "¢‹¡ ”q”w", 5<<4, mcga2 ) ;
   MoveData( mcga2, mcga1, 64000 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;

   mouse_cursor_on() ;

   while( 1 )
   {
      if( select < weapon_number[stage.number-1] )
      {
	 my_mis.weapon = select ;

         MoveData( mcga2, mcga1, 64000 ) ;
	 BufSpritePutImage( 93,  49, s_mis_image[select], mcga1 ) ;
	 BufSpritePutImage( 100, 55, s_mis_image[select], mcga1 ) ;
	 BufSpritePutImage( 219, 49, s_mis_image[select], mcga1 ) ;
	 BufSpritePutImage( 212, 55, s_mis_image[select], mcga1 ) ;

	 BufHprint( 160-((strlen(mis_mes[select])>>1)<<3), 10, mis_mes[select], 204, mcga1 ) ;
         BufSpritePutImage( 205, 90+(select*11), mouse_shape, mcga1 ) ;
         MoveData( mcga1, mcga0, 64000 ) ;
      }
      i = 0 ;
      while( !kbhit() )
      {
	 i = read_mouse() ;
	 if( i == 2 ) break ;
      }
      if( i == 2 )
      {
	 for( i=0; i<8; i++ )
	 {
	    if( Crash( 109,     86+(i*11), 109+67,    86+7+(i*11),
		       mouse_x, mouse_y,   mouse_x+1, mouse_y+1 )) select = i ;
	    if( Crash( 183,     86+(i*11), 183+26,    86+7+(i*11),
		       mouse_x, mouse_y,   mouse_x+1, mouse_y+1 )) select = i ;
	 }
	 if( Crash( 150,     30,      150+25,    30+20,
		    mouse_x, mouse_y, mouse_x+1, mouse_y+1 )) break ;
      }
      else
      {
	 i = Getkey( 1 ) ;
	 if( (i>='1') && (i<='8') )
	 {
	    i -= '0' ;
	    select = i - 1 ;
	 }
         if( i == SPACE )
         {
            if( select == weapon_number[stage.number-1]-1 ) select = 0 ;
            else select++ ;
         }
	 if( i == ENTER ) break ;
      }
   }

   mouse_cursor_off() ;

   black_palette( 1 ) ;
   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
}

char *wing_man_mess1[10] = { "F16·e ˆa  {‰V”a.",
			     "F14“e ˆå—i»¡  a¯¡œa.",
			     "F15ˆa ¸ó‹eÐ µ¥”a.",
			     "F18·e ˆa  {‰V”a.",
			     "A10·¡ ·¡½¢µA ·¶”a.",
			     "F117·i ¤i‰eÐ–”a.",
			     "FSX”a. µ¡¤á",
			     "F22Ÿi ‰·‰bÐa‰V”a.",
			     "MF1Ÿi ‰·‰bÐa‰V”a.",
			     "F4Ÿi ¬w”Ða‰V”a." } ;

char *wing_man_mess21[10] = { "Ðe”¿q·¡´¡.. ˆa¥s‰A..",
			      "¹½´a! ·©”e Ðe ” ¸s´v”a!",
			      "¸s´v”a...Ðaa...",
			      "´a¸a! Àõ¤å¼!",
			      "‹a ¬¡“e ˆa ÀáŸ¡Ð–”a.",
			      "£¡´eÐa”a...¸å¸—·¡œa ´á¼é® ´ô´ö”a.",
			      "OK! Ðe” ‰bÂÐ–”a.",
			      "ÐaÐa...Ðe” ‰bÂÐ–¯s“¡”a!",
			      "¬aœq·i º‚µv´á...ˆa...",
			      "Ðe” ‰bÂ." } ;
char *wing_man_mess22[10] = { "¡±Î‰Ÿ¥”a.. –”¼..",
			      "¨a¬¡! –”¼!!",
			      "–‰...",
			      "¶Ò! –”¼..¹½Š…!",
			      "¬Œ¡! a ÐeÉA ˆ‹¡“¡Œa º‚¸§´a!",
			      "a•¡ ˆi®¢ ¸å¸—µA ¢‰—i´áˆa“eˆåˆa.",
			      "–”¼”a..ˆq‰b..",
			      "´¡Ñ¡! 2 ”¼·³“¡”a!!",
			      "¸á —i·¡ ¢¯e ºAˆa ·¶“eˆá»¡..",
			      "–”¼ ‰bÂÐ–·q." } ;
char *wing_man_mess23[10] = { "´é a—e»¡ ¬w”Ðº a !",
			      "¹½´a! ¸s´v”a! ”a ”ñ¥a!",
			      "ÒÒ....",
			      "¸÷¹¡º…! ¹½´a¶a!",
			      "·¡ •¡´¡»¡ ¬¡—i...”a •Q¥a!",
			      "¸÷ i ·¡œá‰¡ ¯¼»¡ ´g´v”a...",
			      "ˆa ·¡œý‰A ¸iÐa”a“¡...",
			      "µ¡~µ!!!",
			      "´a´a...e ´b a´¡...",
			      "Ò..­A”¼..»¥¸÷Ða¸a..¸÷¯¥»³º—!" } ;

char *wing_man_mess3[10] = { "–áŸi ¦ÈbÐe”a..",
			     "´a´a´b! ·¡¹E {¸w·¡”a..",
			     "...´a—¡µ¡¯a ´a£¡‰¡!",
			     "´a..´e•á!!!!!",
			     "Ça´a´b...·¡œå ³¡¤i....",
			     "....´a¡E.",
			     "´a¡!! º‚‰¡¯¼»¡ ´g´a!!!",
			     "”¸w...´e‘wÓ¡...‹a •·´e..",
			     "‹aœ... ºAˆt·i ¤h“eˆá´¡...·a·b..",
			     "·a´a´a´a´a´a´b...." } ;

char *wing_man_mess4[10] = { " å¸á ÈiÂ‰Ðe”a..",
			     "·a·b..‰bÂ”wÐ–”a..a“e ¢¬aÐa”a.",
			     "ÈiÂ‰Ðaµv”a. Š¹¡¤aœe”a.",
			     "¹E¸w! ÈiÂ‰Ða‰V”a!",
			     "·¡œå °·!...ÈiÂ‰Ða‰V”a.",
			     "Ò...‹¡ÁAÌa­¥...ÈiÂ‰Ðe”a.",
			     "µb¯¡ e ´e•á...ÈiÂ‰Ðe”a..",
			     "´Š´Š~ ÈiÂ‰ÐiA¶a~",
			     "a“e ¢¬aÓ¡ ÈiÂ‰Ða»¡ e..‹a—i·e..",
			     "”¸w“± ÈiÂ‰Ða‰V¯s“¡”a." } ;

char select_man[9]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
char select_attr[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
void Select_WingMan( void )
{
   int select_man_x[9] = { 4, 4+66, 4+66+66, 4, 4+66, 4+66+66, 4, 4+66, 4+66+66 } ;
   int select_man_y[9] = { 3, 3, 3, 3+64, 3+64, 3+64, 3+64+64, 3+64+64, 3+64+64 } ;
   int max_select, select, i ;
   char far *sunglass, *sunback ;

   if( (stage.number == 1) || (stage.number == 10) ) return ;

   for( i=0; i<9; i++ )
   {
      if( select_attr[i] )
      {
	 wing_man[i+1].total_exp++ ;
      }
      select_man[i]  = 0 ;              // Wing Man Number Clear ( 0~9 )
      select_attr[i] = 0 ;              // Wing Man ATTR Clear ( 0,1 )
   }

   for( i=0; i<10; i++ )
   {
      wing_man[i].total_exp += wing_man[i].stage_exp ;
      wing_man[i].stage_exp = 0 ;                  // Wing Man Stage Exp Zero
      if( wing_man[i].sungjang >= random(100) ) wing_man[i].total_exp++ ;
      if((wing_man[i].total_exp>=3 )&&(wing_man[i].level<1)) wing_man[i].level=1 ;
      if((wing_man[i].total_exp>=6 )&&(wing_man[i].level<2)) wing_man[i].level=2 ;
      if((wing_man[i].total_exp>=9 )&&(wing_man[i].level<3)) wing_man[i].level=3 ;
      if((wing_man[i].total_exp>=12)&&(wing_man[i].level<4)) wing_man[i].level=4 ;
   }

   sunglass = (char far *)farmalloc( 31L*11L+4L ) ;
   sunback  = (char far *)farmalloc( 34L*13L+4L ) ;
   ViewIMG( "bf3pic", mcga2 ) ;
   BufGetImage( 224, 1,  224+30, 1+10,  sunglass, mcga2 ) ;
   BufGetImage( 223, 15, 223+31, 15+11, sunback,  mcga2 ) ;

   ViewIMG( "p-sel", mcga2 ) ;
   for( i=1; i<10; i++ )
   {
      if( wing_man[i].kill ) BufBoxFill( select_man_x[i-1], select_man_y[i-1],
					 select_man_x[i-1]+64, select_man_y[i-1]+62,
					 20, mcga2 ) ;
   }
   MoveData( mcga2, mcga1, 64000 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;

   mouse_cursor_on() ;

   max_select = 2 ;
   select = 0 ;

   SetPalette( 254, 63, 63, 63 ) ;
   while( 1 )
   {
      BufBoxLine( select_man_x[select],    select_man_y[select],
		  select_man_x[select]+62, select_man_y[select],
		  254, mcga0 ) ;
      BufMoveImage( 215, 120, 305, 165+20, mcga2, mcga1 ) ;
      if( !wing_man[select+1].kill )
      {
	 if( wing_man[select+1].level == 0 ) sprintf( st, "¤a¥¡", wing_man[select+1].level ) ;
	 if( wing_man[select+1].level == 1 ) sprintf( st, "žÇ¡", wing_man[select+1].level ) ;
	 if( wing_man[select+1].level == 2 ) sprintf( st, "¥AÉAœw", wing_man[select+1].level ) ;
	 if( wing_man[select+1].level == 3 ) sprintf( st, "µA·¡¯a", wing_man[select+1].level ) ;
	 if( wing_man[select+1].level == 4 ) sprintf( st, "ÈsµA·¡¯a", wing_man[select+1].level ) ;
	 BufHprint( 230, 122, st, 102, mcga1 ) ;
         sprintf( st, "%s", wing_man[select+1].name ) ;
         BufHprint( 230, 145, st, 205, mcga1 ) ;
	 sprintf( st, "‰wÐñÃ¡ %d", wing_man[select+1].total_exp ) ;
	 BufHprint( 215, 165, st, 210, mcga1 ) ;
      }
      else
      {
	 BufHprint( 220, 130, "* ¸å ¬a *", 210, mcga1 ) ;
      }
      BufMoveImage( 215, 120, 305, 165+20, mcga1, mcga0 ) ;

      if( select_attr[select] )
      {
	 Hprint( select_man_x[select]+10, select_man_y[select]+40,
		 "Â‰ ‰b", 254 ) ;
      }
      else
      {
	 BufMoveImage( select_man_x[select]+10, select_man_y[select]+40,
		       select_man_x[select]+10+40, select_man_y[select]+40+16,
		       mcga1, mcga0 ) ;
      }

      i = 0 ;
      while( !kbhit() )
      {
	 i = read_mouse() ;
	 if( i == 2 ) break ;
      }

      BufMoveImage( select_man_x[select],    select_man_y[select],
		    select_man_x[select]+62, select_man_y[select],
		    mcga1, mcga0 ) ;

      if( !select_attr[select] )
      {
	 BufMoveImage( select_man_x[select]+10, select_man_y[select]+40,
		       select_man_x[select]+10+40, select_man_y[select]+40+16,
		       mcga1, mcga0 ) ;
      }

      if( i == 2 )
      {
	 for( i=0; i<8; i++ )
	 {
	    if( Crash( 109,     86+(i*11), 109+67,    86+7+(i*11),
		       mouse_x, mouse_y,   mouse_x+1, mouse_y+1 )) select = i ;
	    if( Crash( 183,     86+(i*11), 183+26,    86+7+(i*11),
		       mouse_x, mouse_y,   mouse_x+1, mouse_y+1 )) select = i ;
	 }
	 if( Crash( 150,     30,      150+25,    30+20,
		    mouse_x, mouse_y, mouse_x+1, mouse_y+1 )) break ;
      }
      else
      {
	 i = Getkey( 1 ) ;
	 if( i == UP    )
	 {
	    if( (select!=0) && (select!=1) && (select!=2) ) select -=3 ;
	 }
	 if( i == DOWN  )
	 {
	    if( (select!=6) && (select!=7) && (select!=8) ) select +=3 ;
	 }
	 if( i == LEFT  )
	 {
	    if( (select!=0) && (select!=3) && (select!=6) ) select-- ;
	 }
	 if( i == RIGHT )
	 {
	    if( (select!=2) && (select!=5) && (select!=8) ) select++ ;
	 }
	 if( i == SPACE )
	 {
	    if( !wing_man[select+1].kill )
	    {
	       if( select_attr[select] )
	       {
		  select_attr[select] = 0 ;
		  max_select++ ;
                  for( i=11; i>=0; i-- )
                  {
                     BufMoveImage( select_man_x[select]+10, select_man_y[select]+25,
		                   select_man_x[select]+62, select_man_y[select]+40,
	                           mcga2, mcga1 ) ;
                     BufSpritePutImage( select_man_x[select]+23, select_man_y[select]+14+i,
                                        sunglass, mcga1 ) ;
                     BufSpritePutImage( select_man_x[select]+22, select_man_y[select]+24,
                                        sunback, mcga1 ) ;
                     BufMoveImage( select_man_x[select]+10, select_man_y[select]+25,
		                   select_man_x[select]+62, select_man_y[select]+40,
	                           mcga1, mcga0 ) ;
                     DelayTime( 10 ) ;
                  }
	       }
	       else
	       {
		  if( max_select )
		  {
		     select_attr[select] = 1 ;
		     select_man[select] = select + 1 ;  // Skip Kim Gun il
		     max_select-- ;
                     for( i=0; i<12; i++ )
                     {
                        BufMoveImage( select_man_x[select]+10, select_man_y[select]+25,
		                      select_man_x[select]+62, select_man_y[select]+40,
	                              mcga2, mcga1 ) ;
                        BufSpritePutImage( select_man_x[select]+23, select_man_y[select]+14+i,
                                           sunglass, mcga1 ) ;
                        BufSpritePutImage( select_man_x[select]+22, select_man_y[select]+24,
                                           sunback, mcga1 ) ;
                        BufMoveImage( select_man_x[select]+10, select_man_y[select]+25,
		                      select_man_x[select]+62, select_man_y[select]+40,
	                              mcga1, mcga0 ) ;
                        DelayTime( 10 ) ;
                     }
		  }
	       }
	    }
	 }
	 if( i == ENTER ) break ;
      }
   }

   farfree( sunglass ) ;
   farfree( sunback  ) ;

   mouse_cursor_off() ;

   black_palette( 1 ) ;
   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
}

void panic_air_select( void )
{
   char air_name[9][10] = { "MF1", "F4", "F16", "F14", "F15", "F18", "F22", "FSX", "BF" } ;
   int a_select_x[9] = { 4, 4+66, 4+66+66, 4, 4+66, 4+66+66, 4, 4+66, 4+66+66 } ;
   int a_select_y[9] = { 3, 3, 3, 3+64, 3+64, 3+64, 3+64+64, 3+64+64, 3+64+64 } ;
   int a_select, i ;

   ViewIMG( "a-sel", mcga1 ) ;
   BufHprint( 215, 130, "Ì “¢ ¡¡ —a", 210, mcga1 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;

   mouse_cursor_on() ;

   a_select = 0 ;

   AdLib_Start( "bf-sel" ) ;
   SetPalette( 254, 63, 63, 63 ) ;
   while( 1 )
   {
      BufXLine( a_select_x[a_select],    a_select_y[a_select],    62, 19, mcga0 ) ;
      BufYLine( a_select_x[a_select]+62, a_select_y[a_select],    51, 19, mcga0 ) ;
      BufYLine( a_select_x[a_select],    a_select_y[a_select],    51, 24, mcga0 ) ;
      BufXLine( a_select_x[a_select],    a_select_y[a_select]+51, 62, 24, mcga0 ) ;

      BufYLine( a_select_x[a_select],    a_select_y[a_select]+53, 9,  19, mcga0 ) ;
      BufXLine( a_select_x[a_select],    a_select_y[a_select]+62, 62, 19, mcga0 ) ;
      BufXLine( a_select_x[a_select],    a_select_y[a_select]+53, 62, 24, mcga0 ) ;
      BufYLine( a_select_x[a_select]+62, a_select_y[a_select]+53, 9,  24, mcga0 ) ;
      sprintf( st, "‹¡¹· - %s", air_name[a_select] ) ;
      BufHprint( 215, 160, st, 210, mcga0 ) ;
      BufSpritePutImage( a_select_x[a_select]+45, a_select_y[a_select]+58, mouse_shape, mcga0 ) ;

      i = 0 ;
      while( !kbhit() )
      {
	 i = read_mouse() ;
	 if( i == 2 ) break ;
      }

      BufMoveImage( a_select_x[a_select], a_select_y[a_select],
                    a_select_x[a_select]+62, a_select_y[a_select]+62, mcga1, mcga0 ) ;
      BufMoveImage( 215, 125, 305, 165+20, mcga1, mcga0 ) ;
      BufMoveImage( a_select_x[a_select]+45, a_select_y[a_select]+58, a_select_x[a_select]+45+29, a_select_y[a_select]+58+24, mcga1, mcga0 ) ;

      if( i == 2 )
      {
	 for( i=0; i<8; i++ )
	 {
	    if( Crash( 109,     86+(i*11), 109+67,    86+7+(i*11),
		       mouse_x, mouse_y,   mouse_x+1, mouse_y+1 )) a_select = i ;
	    if( Crash( 183,     86+(i*11), 183+26,    86+7+(i*11),
		       mouse_x, mouse_y,   mouse_x+1, mouse_y+1 )) a_select = i ;
	 }
	 if( Crash( 150,     30,      150+25,    30+20,
		    mouse_x, mouse_y, mouse_x+1, mouse_y+1 )) break ;
      }
      else
      {
	 i = Getkey( 1 ) ;
	 if( i == UP    )
	 {
	    if( (a_select!=0) && (a_select!=1) && (a_select!=2) ) a_select -=3 ;
	 }
	 if( i == DOWN  )
	 {
	    if( (a_select!=6) && (a_select!=7) && (a_select!=8) ) a_select +=3 ;
	 }
	 if( i == LEFT  )
	 {
	    if( (a_select!=0) && (a_select!=3) && (a_select!=6) ) a_select-- ;
	 }
	 if( i == RIGHT )
	 {
	    if( (a_select!=2) && (a_select!=5) && (a_select!=8) ) a_select++ ;
	 }
	 if( i == SPACE )
	 {
            if( a_select == 0 ) my_air[0].air = _MF1 ;
            if( a_select == 1 ) my_air[0].air = _F4  ;
            if( a_select == 2 ) my_air[0].air = _F16 ;
            if( a_select == 3 ) my_air[0].air = _F14 ;
            if( a_select == 4 ) my_air[0].air = _F15 ;
            if( a_select == 5 ) my_air[0].air = _F18 ;
            if( a_select == 6 ) my_air[0].air = _F22 ;
            if( a_select == 7 ) my_air[0].air = _FSX ;
            if( a_select == 8 ) my_air[0].air = _BF  ;
            break ;
	 }
	 if( i == ESC   ) break ;
      }
   }

   mouse_cursor_off() ;

   AdLib_End() ;
   black_palette( 1 ) ;
   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
}

void panic_score_write( unsigned int score )
{
   int score_file, i, j ;

   ViewIMG( "score", mcga1 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;                      // Set Slow White Palette

   score_file = open( "bf.sco", O_RDONLY | O_BINARY, S_IWRITE | S_IREAD ) ;
   if( score_file == -1 )
   {
//      printf( "\a File Not Found !\n\a" ) ;
      for( i=0; i<7; i++ ) panic_score_board[i] = 0 ;
      for( j=0; j<7; j++ )
      {
	 for( i=0; i<35; i++ ) panic_score_name[j][i] = 0 ;
      }
   }
   else
   {
      read( score_file, &panic_score_board, 14 ) ;
      read( score_file, &panic_score_name, 7*35 ) ;
   }
   close( score_file ) ;

   for( j=0; j<7; j++ )
   {
      if( panic_score_board[j] < score ) break ;
   }
   hanputsxy( 90, 10, 6<<4, "ÂA‰¡· Ìa·©œõ 7 ·¥" ) ;
   hanputsxy( 10,  30, 3<<4, "®…¶á ¸ñ®" ) ;
   if( (j != 7) && score )
   {
      for( i=5; i>=j; i-- )
      {
         panic_score_board[i+1] = panic_score_board[i] ;
         strcpy( panic_score_name[i+1], panic_score_name[i] ) ;
      }
      panic_score_board[i+1] = score ;

      for( i=0; i<7; i++ )
      {
        sprintf( st, "%d   %5d", i+1, panic_score_board[i] ) ;
        hanputsxy( 10, 50+(i*20), 210, st ) ;
        if( j != i ) BufHprint( 10+(10*8), 50+(i*20), panic_score_name[i], 205, mcga0 ) ;
      }

      for( i=0; i<35; i++ ) panic_score_name[j][i] = 0 ;
      haninxy( 10+(10*8), 50+(j*20), 5<<4, panic_score_name[j], 27 ) ;

      score_file = open( "bf.sco", O_WRONLY | O_CREAT | O_BINARY | O_TRUNC, S_IREAD | S_IWRITE ) ;
      write( score_file, &panic_score_board, 14 ) ;
      write( score_file, &panic_score_name, 7*35 ) ;
      close( score_file ) ;
   }
   else
   {
      for( i=0; i<7; i++ )
      {
        sprintf( st, "%d   %5d", i+1, panic_score_board[i] ) ;
        hanputsxy( 10, 50+(i*20), 210, st ) ;
        BufHprint( 10+(10*8), 50+(i*20), panic_score_name[i], 205, mcga0 ) ;
      }
   }
   key_clear() ;                          // Key Buffer clear
   Getkey(1) ;                            // Push Any Key
   black_palette( 1 ) ;                   // Set Slow Black Palette
   black_palette( 0 ) ;                   // All Black Palette
}

/*
char *mes_data[15] = {
	 "11Íe” Íe”¸w ˆw»¥¬÷·¡”a.",                 // 1 Stage Start Demo
	 "·©¥¥· F16 Íe”Ÿi Í¡ÀbÐ–”a.",
	 "¬á½¢·a¡ Ð·Ða‰¡ ·¶“e—u Ða”a.",
	 "¶a‰bÐa‰V”a. Over",
	 " ",
	 "¹½”a ˆw»¥¬÷. ¶a‰bÐaœa.",
	 "ÐeŠ‚µA•¡ ·¥¸ˆa ·¶”a“eˆé ¥¡µaº¡œa.",
	 " ",

	 "¬a­¡Ðe ¯wŸ¡“e ·¶‰V»¡ e..",                  // 1 Stage End Demo
	 "£¡Š…‹¡—i‰Á °a¶‹¡µE",
	 "¶Ÿ¡· ¸w§¡“e á¢ ˜é´á»¥”a..",
	 "·¡ ¸å¸—·e »©®¤cµA ´ô“eˆåˆa?..",
	 " ",
	 "11Íe” ®‰¡Ð–”a.",
	 "Šá”Ðaœa.",
	 " ",

	 " ",                                         // 2 Stage Start Demo
	 "ÒÒ...­©ˆ...e áŸi £¨“e”a.",
	 " ",
	 " ",
	 " ",
	 "·¡¤å·¡ Àõ Â‰‰b·¡”a.",
	 "¡¡–—i Ò‰ŸWÐe ¯©b·i ¥¡·¡‹© ¤aœe”a.",
	 " ",

	 " ",                                         // 2 Stage End Demo
	 "¸÷ i ””eÐe ‹¡ÁAŠ…...",
	 "·¡ ¸÷•¡¡e ‹a—i‰Á °a¶‰® ·¶‰V´á!",
	 " ",
	 " ",
	 "ÎŸe · Àõ §¡Ð—­¡ˆq·¡ ´á˜åˆa ?",
	 " ",
	 " ",

	 " ",                                         // 3 Stage Start Demo
	 "F16 2”µÁ A10 3”Ÿi ¤i‰e !",
	 "¶a‰bÐa‰V”a. Over",
	 " ",
	 " ",
	 "1 Íe” ¶a‰bÐaœa !",
	 " ",
	 " ",

	 " ",                                         // 3 Stage End Demo
	 "¸â‹¡ ¡¡– ‰bÂ...",
	 "Šá”Ða‰V¯s“¡”a...",
	 " ",
	 " ",
	 "ˆw»¥¬÷­¡w ®‰¡Ð–”a !",
	 " ",
	 " ",

	 "¸â· Í¢‰b‹¡Ÿi ¤i‰eÐ–”a..",                  // 4 Stage Start Demo
	 " g·e ¸åÊ‹¡—i·¡ Ñ¡¶áÐa‰¡ ·¶”a..",
	 "Í¢‰b‹¡Ÿi ´á˜ý‰A Ði»¡",
	 "»¡¯¡Ÿi ¤aœe”a. Over ! ",
	 " ",
	 "ˆw»¥¬÷­¡w ¶a‰bÐaœa !",
	 " ",
	 " ",

	 " ",                                         // 4 Stage End Demo
	 "·±¢ µÅ® Ð–¯s“¡”a.",
	 "¸â· Í¢‰b‹¡Ÿi ¸s´v¯s“¡”a.",
	 " ",
	 " ",
	 "µb¯¡ µA·¡¯a ”sŠ…...",
	 "®‰¡Ð–”a...",
	 " ",

	 " ",                                         // 5 Stage Start Demo
	 "¸â· F16 2”µÁ F15 2”Ÿi ¤i‰eÐ–”a.",
	 "»¡¯¡Ÿi ¤aœe”a. Over",
	 " ",
	 " ",
	 "ˆw»¥¬÷­¡w ¶a‰bÐaœa !",
	 " ",
	 " ",

	 " ",                                         // 5 Stage End Demo
	 "·±¢ µÅ® Ð–¯s“¡”a.",
	 " ",
	 " ",
	 " ",
	 "µb¯¡ µA·¡¯a—i ”sŠ…...",
	 "®‰¡Ð–”a...",
	 " ",

	 " ",                                         // 6 Stage Start Demo
	 "¸÷ÁA ¦‰¡w· ¸â‹¡Ÿi ¤i‰eÐ–”a.",
	 "»¡¯¡Ÿi ¤aœe”a. Over",
	 " ",
	 " ",
	 "¸â·i ¸i ¬iÍaœa...",
	 "¸â·¡ ‰·‰bÐa¡e ¶a‰bÐaœa",
	 " ",

	 " ",                                         // 6 Stage End Demo
	 "·±¢ µÅ® Ð–¯s“¡”a.",
	 " ",
	 " ",
	 " ",
	 "µb¯¡ µA·¡¯a—i ”sŠ…...",
	 "®‰¡Ð–”a...",
	 " ",

	 " ",                                         // 7 Stage Start Demo
	 "F14 2”µÁ F18 2”Ÿi ¤i‰eÐ–”a.",
	 "»¡¯¡Ÿi ¤aœe”a. Over",
	 " ",
	 " ",
	 "¸â·i ¸i ¬iÍaœa...",
	 "¸â·¡ ‰·‰bÐa¡e ¶a‰bÐaœa",
	 " ",

	 " ",                                         // 7 Stage End Demo
	 "·¡¹A ‰·º—·e ¶Ÿ¡ˆa ¶­AÐ¹v”a...",
	 "q·eˆå »¡¬w· ¸å¶—i· ÑÉ´¢·¥•A...",
	 "áÓŸi £¨“e”a...",
	 " ",
	 "µb¯¡ µA·¡¯a—i ”sŠ…...",
	 "®‰¡Ð–”a...",
	 " ",

	 " ",                                         // 8 Stage Start Demo
	 "·¡¤åµA ÂAÁ¡¡ ¸åÊ‹¡¶w A·¡¹a¥w‹¡ˆa",
	 "ˆ¤i–A´á —i´áµÖ”a“e•A",
	 "¹Aˆa Ðe¤å ¬a¶wÐ ¥¡‰V¯s“¡”a.",
	 " ",
	 "¯¥¥w‹¡“¡ ¹¡¯±Ðœa.",
	 "¸â·¡ ‰·‰bÐa¡e ¶a‰bÐaœa",
	 " ",

	 "µa‹¡“e ˆw »¥¬÷.",                           // 8 Stage End Demo
	 "‹¡ÁA· ¬wÈˆa ·¡¬wÐa”a.",
	 "§¡Ð—¬wÈˆa ¦‰´e¸÷ Ða”a...·¡œá”aˆa....",
	 "´”wÁ¡ ­©ˆœa“e ‹¡¹·¸aÁAˆa ¦‰´eÐa”á“¡",
	 " ",
	 "ˆw»¥¬÷ ­¡w !!!",
	 "ˆw»¥¬÷ ­¡w !!!",
	 " ",

	 " ",                                         // 9 Stage Start Demo
	 "»¡‹q F16 ·i ·¡¶w Šá”º—·¡”a.",
	 "Over !",
	 " ",
	 " ",
	 "¸â‹¡ˆa ·¶·a“¡ ¹¡¯±Ðaœa...",
	 "·¡¬w·¡”a...",
	 " ",

	 " ",                                         // 9 Stage End Demo
	 "µa‹¡“e ˆw »¥¬÷.",
	 "¸á‹¡Ÿi ¡¡– ¶a‰bÐ–”a.",
	 "Šá”Ðe”a.",
	 " ",
	 "‰¡¬—Ð–”a.. ˆw»¥¬÷ ­¡w !",
	 " ",
	 " ",

	 " ",                                         // 10 Stage Start Demo
	 "FSX 2”µÁ F23 2”Ÿi ¤i‰eÐ–”a.",
	 "»¡¯¡Ÿi ¤aœe”a. Over",
	 " ",
	 " ",
	 "¸â·i ¸i ¬iÍaœa...",
	 "¸â·¡ ‰·‰bÐa¡e ¶a‰bÐaœa",
	 " ",

	 " ",                                         // 10 Stage End Demo
	 "¥i”aŸe ·¡¥e·¡ ´ô“eÐe ·¡¹E ¶Ÿ¡· ¯wŸ¡”a",
	 "·¡¥e·¡ ´ô“e Ðe...",
	 " ",
	 " ",
	 "µb¯¡ µA·¡¯a—i ”sŠ…...",
	 "®‰¡Ð–”a...",
	 " ",

	 " ",                                         // 11 Stage Start Demo
	 "F23 2”µÁ F29 2”Ÿi ¤i‰eÐ–”a.",
	 "»¡¯¡Ÿi ¤aœe”a. Over",
	 " ",
	 " ",
	 "¸â·i ¸i ¬iÍaœa...",
	 "¸â·¡ ‰·‰bÐa¡e ¶a‰bÐaœa",
	 " ",

	 "B-1B Í¢‰b‹¡ Ÿi ¤i‰eÐaµv”a.",                // 11 Stage End Demo
	 "µb¯¡ ¸â·e Ð‚· ¬a¶w·i",
	 "¡¢¸â·a¡ Ð–”å ˆõ ˆ{”a.",
	 "´á˜ý‰A Ðiˆõ·¥»¡ »¡¯¡Ÿi ¤aœe”a.",
	 "B-1B Ÿi ˆa“wÐa¡e ·¡ ‹¡»¡¡",
	 "·A•¡¯¡Ç¡‹¡ ¤aœe”a.",
	 "ˆá¦Ði ‰w¶ ‰bÂ¯¡Åa•¡ ¹½”a.",
	 " ",

	 "µa‹¡“e ˆw »¥¬÷.",                           // 12 Stage Start Demo
	 "B-1B Í¢‰b‹¡ Ÿi ‹¡»¡¡ ·A•¡Ða‰¡ ·¶”a.",
	 "B-1B “e ®…®…Ó¡ Ðw¥¢Ðaµv”a.",
	 " ",
	 "µa‹¡“e ÎŸe  –—»¡.",
	 "‹a ½¢·a¡ ¸â· Íe”ˆa ¸ó‹eº—·¡”a.",
	 "´a a Í¢‰b‹¡Ÿi ‘¡Ÿ¡“e ˆõ ˆ{”a.",
	 "¤wÐ··e q•·½¢. ·w¸å·i ¤aœe”a.",

	 " ",                                         // 12 Stage End Demo
	 "µa‹¡“e ˆw»¥¬÷",
	 "¡¡– À÷­¡Ð–”a.",
	 " ",
	 "µa‹¡“e ÎŸe  –—»¡.",
	 "´a»¢ {a»¡ ´g´v”a.",
	 "¬á½¢¤wÐ··a¡ ¸â Íe” ¸ó‹eº—·¡”a.",
	 "FSX 2 ”µÁ F14 2 ””a.",

	 " ",                                         // 13 Stage Start Demo
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",

	 " ",                                         // 13 Stage End Demo
	 "¹½´a. ·¡¹A •©´aˆe”a.",
	 "º…§¡ ”e”eÓ¡ Ð–œa‰¡ µeœb¤aœe”a.",
	 " ",
	 "ÂA‰¡”a!! ˆw»¥¬÷!!",
	 "´aŠ… Íe”“e ¸åÑa Ï¡Ð´ô·¡ ŠáÑÅÐ–”a.",
	 "•©´aµ¡¡e ¡¡–—i ˆa e ´e–‰V”a‰¡",
	 "‹¡”aŸ¡‰¡ ·¶”a."

	 " ",                                         // 14 Stage Start Demo
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",

	 " ",                                         // 14 Stage End Demo
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",
	 " ",
	 " "
} ;

extern char back_scroll_mode ;
unsigned char *talk_image[7] ;

void one_talk( char *talk_code )
{
   static char talk_eye_ani_count = 0 ;

   if( talk_eye_ani_count )
   {
      talk_eye_ani_count-- ;
      if( !talk_eye_ani_count )
      {
	 if( ( *talk_code == 3 ) || ( *talk_code == 4 ) || ( *talk_code == 5 ) ) *talk_code -= 3 ;
      }
   }

   if( (random(50)==25) && !talk_eye_ani_count )
   {
      if( ( *talk_code == 0 ) || ( *talk_code == 1 ) || ( *talk_code == 2 ) ) *talk_code += 3 ;
      talk_eye_ani_count = 4 + random(4) ;
   }
   BufPutImage( 30, 38, talk_image[*talk_code], mcga0 ) ;
   waitmSec( 50 ) ;
   if( (random(50)==25) && !talk_eye_ani_count )
   {
      if( ( *talk_code == 0 ) || ( *talk_code == 1 ) || ( *talk_code == 2 ) ) *talk_code += 3 ;
      talk_eye_ani_count = 4 + random(4) ;
   }
   BufPutImage( 30, 38, talk_image[(*talk_code)+1], mcga0 ) ;
   waitmSec( 50 ) ;
   if( (random(50)==25) && !talk_eye_ani_count )
   {
      if( ( *talk_code == 0 ) || ( *talk_code == 1 ) || ( *talk_code == 2 ) ) *talk_code += 3 ;
      talk_eye_ani_count = 4 + random(4) ;
   }
   BufPutImage( 30, 38, talk_image[(*talk_code)+2], mcga0 ) ;
   waitmSec( 50 ) ;
   if( (random(50)==25) && !talk_eye_ani_count )
   {
      if( ( *talk_code == 0 ) || ( *talk_code == 1 ) || ( *talk_code == 2 ) ) *talk_code += 3 ;
      talk_eye_ani_count = 4 + random(4) ;
   }
   BufPutImage( 30, 38, talk_image[(*talk_code)+1], mcga0 ) ;
   waitmSec( 50 ) ;
   if( (random(50)==25) && !talk_eye_ani_count )
   {
      if( ( *talk_code == 0 ) || ( *talk_code == 1 ) || ( *talk_code == 2 ) ) *talk_code += 3 ;
      talk_eye_ani_count = 4 + random(4) ;
   }
   BufPutImage( 30, 38, talk_image[*talk_code], mcga0 ) ;
   waitmSec( 50 ) ;
   if( (random(8)==4) && !talk_eye_ani_count )
   {
      BufPutImage( 30, 38, talk_image[6], mcga0 ) ;
      waitmSec( 100 ) ;
      BufPutImage( 30, 38, talk_image[*talk_code], mcga0 ) ;
      waitmSec( 100 ) ;
   }
}

void stage_demo( int demo_mode )
{
   char mes_buff[3] ;
   int mes_count = ( (stage.number-1) << 4 ) + ( demo_mode << 3 ) ;
   long mes_view_time ;
   int air_move_value, air_move_attr, air_move_loop ;
   int i, j, x, sc_x, mes_x, mes_y, end_code ;

   silentVoice( 1 ) ;

   time_int_push = 1 ;

   air_move_value = 0 ;
   air_move_attr  = 0 ;
   air_move_loop  = 2 ;

   key_clear() ;

   if( !demo_mode )
   {
      unsigned char far *sour_buff ;
      unsigned char far *air_image ;
      int back_page ;

      sour_buff = (unsigned char far *)farmalloc( 320L*200L ) ;
      air_image = (unsigned char far *)farmalloc( 274L*66L+4L ) ;

      BufXLine( 0, 0, 64000, 0, mcga0 ) ;
      BufXLine( 0, 0, 64000, 0, sour_buff ) ;
      ViewIMG( "s-demo-0", mcga1 ) ;
      BufGetImage( 0, 0, 273, 65, air_image, mcga1 ) ;
      if( !back_scroll_mode )
      {
	 xmstomem( XMS_back, 0L, mcga2+4, 64000 ) ;
	 xmstomem( XMS_back, 0L, mcga3+4, 64000 ) ;
      }
      else
      {
	 xmstomem( XMS_back, 0L,     mcga2+4, 64000 ) ;
	 xmstomem( XMS_back, 64000L, mcga3+4, 64000 ) ;
      }

      *(mcga2+0) = 319 &  0x00ff ;
      *(mcga2+1) = 319 >> 8      ;
      *(mcga2+2) = 199 &  0x00ff ;
      *(mcga2+3) = 199 >> 8      ;

      *(mcga3+0) = 319 &  0x00ff ;
      *(mcga3+1) = 319 >> 8      ;
      *(mcga3+2) = 199 &  0x00ff ;
      *(mcga3+3) = 199 >> 8      ;

      back_page = 0 ;
      sc_x = 0 ;
      for( x = -300; x < 22; x+=2 )
      {
	 if( kbhit() ) break ;

	 sc_x -= 2 ;
	 if( sc_x == -320 )
	 {
	    sc_x = 0 ;
	    if( !back_page ) back_page = 1 ;
	    else             back_page = 0 ;
	 }
	 if( !back_page )
	 {
	    BufPutImage( sc_x,     0, mcga2, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga3, mcga1 ) ;
	 }
	 else
	 {
	    BufPutImage( sc_x,     0, mcga3, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga2, mcga1 ) ;
	 }

	 if( !air_move_loop )
	 {
	    if( !air_move_attr )
	    {
	       air_move_value-- ;
	       if( air_move_value == -5 ) air_move_attr = 1 ;
	    }
	    else
	    {
	       air_move_value++ ;
	       if( air_move_value == 5 ) air_move_attr = 0 ;
	    }
	    air_move_loop = 4 ;
	 }
	 else
	    air_move_loop-- ;
	 BufSpritePutImage( x, 70+air_move_value, air_image, mcga1 ) ;
	 MoveData( mcga1, mcga0, 64000 ) ;
      }

      if( !kbhit() )
      {
	 for( i=0; i<4; i++ )
	 {
	    BufHprint( 15+1, (i<<4)+1+8, mes_data[i+mes_count], 15, sour_buff ) ;
	    BufHprint( 15, (i<<4)+8, mes_data[i+mes_count], (6<<4)+2, sour_buff ) ;
	 }
	 mes_count += 4 ;
      }

      end_code = 1 ;
      mes_view_time = GetTime() ;
      while( end_code )
      {
	 if( kbhit() ) break ;

	 sc_x -= 2 ;
	 if( sc_x == -320 )
	 {
	    sc_x = 0 ;
	    if( !back_page ) back_page = 1 ;
	    else             back_page = 0 ;
	 }
	 if( !back_page )
	 {
	    BufPutImage( sc_x,     0, mcga2, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga3, mcga1 ) ;
	 }
	 else
	 {
	    BufPutImage( sc_x,     0, mcga3, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga2, mcga1 ) ;
	 }

	 if( !air_move_loop )
	 {
	    if( !air_move_attr )
	    {
	       air_move_value-- ;
	       if( air_move_value == -5 ) air_move_attr = 1 ;
	    }
	    else
	    {
	       air_move_value++ ;
	       if( air_move_value == 5 ) air_move_attr = 0 ;
	    }
	    air_move_loop = 2 ;
	 }
	 else
	    air_move_loop-- ;
	 BufSpritePutImage( x, 70+air_move_value, air_image, mcga1 ) ;
	 SpriteMoveData( sour_buff, mcga1, 64000 ) ;
	 MoveData( mcga1, mcga0, 64000 ) ;
	 if( (GetTime()-mes_view_time) > 500L ) end_code = 0 ;
      }

      if( !kbhit() )
      {
	 for( i=0; i<4; i++ )
	 {
	    BufHprint( 15+1, (i<<4)+128+1, mes_data[i+mes_count], 15, sour_buff ) ;
	    BufHprint( 15, (i<<4)+128, mes_data[i+mes_count], (5<<4)+2, sour_buff ) ;
	 }
      }

      end_code = 1 ;
      mes_view_time = GetTime() ;
      while( end_code )
      {
	 if( kbhit() ) break ;

	 sc_x -= 2 ;
	 if( sc_x == -320 )
	 {
	    sc_x = 0 ;
	    if( !back_page ) back_page = 1 ;
	    else             back_page = 0 ;
	 }
	 if( !back_page )
	 {
	    BufPutImage( sc_x,     0, mcga2, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga3, mcga1 ) ;
	 }
	 else
	 {
	    BufPutImage( sc_x,     0, mcga3, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga2, mcga1 ) ;
	 }

	 if( !air_move_loop )
	 {
	    if( !air_move_attr )
	    {
	       air_move_value-- ;
	       if( air_move_value == -5 ) air_move_attr = 1 ;
	    }
	    else
	    {
	       air_move_value++ ;
	       if( air_move_value == 5 ) air_move_attr = 0 ;
	    }
	    air_move_loop = 2 ;
	 }
	 else
	    air_move_loop-- ;
	 BufSpritePutImage( x, 70+air_move_value, air_image, mcga1 ) ;
	 SpriteMoveData( sour_buff, mcga1, 64000 ) ;
	 MoveData( mcga1, mcga0, 64000 ) ;
	 if( (GetTime()-mes_view_time) > 500L ) end_code = 0 ;
      }

      i = 2 ;
      j = 0 ;
      for( x = 20; x < 170; x += 3 )
      {
	 if( kbhit() ) break ;

	 sc_x -= 2 ;
	 if( sc_x == -320 )
	 {
	    sc_x = 0 ;
	    if( !back_page ) back_page = 1 ;
	    else             back_page = 0 ;
	 }
	 if( !back_page )
	 {
	    BufPutImage( sc_x,     0, mcga2, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga3, mcga1 ) ;
	 }
	 else
	 {
	    BufPutImage( sc_x,     0, mcga3, mcga1 ) ;
	    BufPutImage( sc_x+320, 0, mcga2, mcga1 ) ;
	 }

	 Scale( x+((280-(280/i))>>1), 70+((65-(65/i))>>1), 280/i, 65/i, air_image, mcga1 ) ;
	 if( j==2 ) { j = 0 ; i++ ; }
	 else j++ ;
	 MoveData( mcga1, mcga0, 64000 ) ;
      }
      farfree( sour_buff ) ;
      farfree( air_image ) ;
   }
   else
   {
      unsigned char far *demo_air_image ;
      unsigned char far *mask_image ;
      char talk_code = 0 ;
      unsigned char one_data ;

      demo_air_image = (unsigned char far *)farmalloc( 194L*108L+4L ) ;
      mask_image = (unsigned char far *)farmalloc( 113L*106L+4L ) ;
      for( i=0; i<7; i++ ) talk_image[i] = (unsigned char far *)farmalloc( 49L*48L+4L ) ;
      ViewIMG( "bf-talk", mcga1 ) ;
      BufGetImage( 0, 0, 112, 105, mask_image, mcga1 ) ;
      for( i=0; i<3; i++ ) BufGetImage( 114+(51*i), 1,  114+(51*i)+48, 1+47,  talk_image[i], mcga1 ) ;
      for( i=0; i<3; i++ ) BufGetImage( 114+(51*i), 50, 114+(51*i)+48, 50+47, talk_image[i+3], mcga1 ) ;
      BufGetImage( 114+(51*3), 1, 114+(51*3)+48, 1+47, talk_image[6], mcga1 ) ;

      ViewIMG( "s-demo-1", mcga1 ) ;
      BufGetImage( 0, 0, 193, 107, demo_air_image, mcga1 ) ;

      ViewIMG( "s-demo-2", mcga2 ) ;
      MoveData( mcga2, mcga0, 64000 ) ;
      MoveData( mcga2, mcga1, 64000 ) ;

      for( x = 50; x > 0; x-- )
      {
	 if( kbhit() ) break ;

	 BufMoveImage( 0, 0, 319, 40+100, mcga2, mcga1 ) ;
	 Scale( 160-((193/x)>>1), 40-((107/x)>>1)+50-x, 193/x, 107/x, demo_air_image, mcga1 ) ;
	 BufMoveImage( 0, 0, 319, 40+100, mcga1, mcga0 ) ;
      }

      for( i=1; i<54; i+=2 )
      {
	 if( kbhit() ) break ;

	 Scale( 0, 53-i, 113, i<<1, mask_image, mcga1 ) ;
	 BufMoveImage( 0, 0, 113, 106, mcga1, mcga0 ) ;
      }

      mes_x = 0 ;
      mes_y = 0 ;
      while( 1 )
      {
	 if( kbhit() ) break ;

	 one_data = mes_data[mes_count][mes_x] ;
	 if( one_data == 0 )
	 {
	    mes_count++ ;
	    if( mes_y == 3 ) break ;
	    mes_x = 0 ;
	    mes_y++ ;
	    continue ;
	 }
	 if( one_data >= 0x80 )
	 {
	    mes_buff[0] = mes_data[mes_count][mes_x] ;
	    mes_buff[1] = mes_data[mes_count][mes_x+1] ;
	    mes_buff[2] = 0 ;
	    BufHprint( 15+(mes_x<<3)+1, 136+(mes_y<<4)+1, mes_buff, 15, mcga0 ) ;
	    BufHprint( 15+(mes_x<<3), 136+(mes_y<<4), mes_buff, (6<<4)+2, mcga0 ) ;
	    mes_x += 2 ;
	    one_talk( &talk_code ) ;
	 }
	 else
	 {
	    mes_buff[0] = mes_data[mes_count][mes_x] ;
	    mes_buff[1] = 0 ;
	    BufHprint( 15+(mes_x<<3)+1, 136+(mes_y<<4)+1, mes_buff, 15, mcga0 ) ;
	    BufHprint( 15+(mes_x<<3), 136+(mes_y<<4), mes_buff, (6<<4)+2, mcga0 ) ;
	    mes_x++ ;
	    DelayTime( 1 ) ;
	 }
      }

      if( !kbhit() )
      {
	 DelayTime( 200 ) ;

	 BufPutImage( 0, 0, mask_image, mcga1 ) ;
	 MoveData( mcga1, mcga0, 64000 ) ;

	 for( i=0; i<4; i++ )
	 {
	    BufHprint( 15+1, (i<<4)+136+1, mes_data[i+mes_count], 15, mcga0 ) ;
	    BufHprint( 15, (i<<4)+136, mes_data[i+mes_count], (5<<4)+2, mcga0 ) ;
	 }
	 mes_count += 4 ;
      }

      end_code = 1 ;
      mes_view_time = GetTime() ;
      while( end_code )
      {
	 if( kbhit() ) break ;

	 if( (GetTime()-mes_view_time) > 500L ) end_code = 0 ;
      }

      for( i=53; i>0; i-=3 )
      {
	 if( kbhit() ) break ;

	 BufMoveImage( 0, 0, 113, 106, mcga2, mcga1 ) ;
	 Scale( 0, 53-i, 113, i<<1, mask_image, mcga1 ) ;
	 BufMoveImage( 0, 0, 113, 106, mcga1, mcga0 ) ;
      }
      if( !kbhit() )
      {
	 BufMoveImage( 0, 0, 113, 106, mcga2, mcga0 ) ;
	 x++ ;
	 Scale( 160-((193/x)>>1), 40-((106/x)>>1)+50-x, 193/x, 106/x, demo_air_image, mcga0 ) ;
      }

      farfree( demo_air_image ) ;
      farfree( mask_image ) ;
      for( i=0; i<7; i++ ) farfree( talk_image[i] ) ;
   }

   black_palette( 1 ) ;
   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   time_int_push = 1 ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   time_int_push = 0 ;
   key_clear() ;

   time_int_push = 0 ;
}
*/

void demo0( void )
{
   int bfs[20] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 8, 8, 16, 16, 32, 32, 64, 64, 64 } ;
   int bfx, bfmode, bust_onoff, i ;

   AdLib_Start( "bf-start" ) ;

   ViewIMG( "bf-td3", mcga1 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;

   ViewIMG( "takeoff1", mcga1 ) ;
   waitmSec( 1300 ) ; if( kbhit() ) return ;

   BufMoveImage(   0,   0, 160, 100, mcga1, mcga0 ) ;
   waitmSec( 1800 ) ; if( kbhit() ) return ;
   BufMoveImage( 160, 100, 319, 199, mcga1, mcga0 ) ;
   waitmSec( 1800 ) ; if( kbhit() ) return ;
   BufMoveImage( 160,   0, 319, 160, mcga1, mcga0 ) ;
   waitmSec( 1800 ) ; if( kbhit() ) return ;
   BufMoveImage(   0, 100, 319, 199, mcga1, mcga0 ) ;
   waitmSec( 1800 ) ; if( kbhit() ) return ;

   MoveData( mcga0, mcga1, 64000 ) ;
   ViewIMG( "takeoff2", mcga2 ) ;
   BufGetImage( 0, 85, 319, 177, mcga3, mcga2 ) ;
   BufPutImage( 0, 49, mcga3, mcga1 ) ;
   BufGetImage( 42,  18,  273, 78,  mcga3, mcga2 ) ;
   BufGetImage( 103, 186, 148, 193, mcga3+16000, mcga2 ) ;
   BufGetImage( 158, 186, 193, 193, mcga3+17000, mcga2 ) ;
   BufGetImage( 204, 187, 223, 194, mcga3+18000, mcga2 ) ;
   BufGetImage( 236, 187, 241, 194, mcga3+19000, mcga2 ) ;
   MoveData( mcga1, mcga2, 64000 ) ;
   BufSpritePutImage( 62, 65, mcga3, mcga1 ) ;
   BufMoveImage( 0, 49, 319, 141, mcga1, mcga0 ) ;
   waitmSec( 1800 ) ; if( kbhit() ) return ;

   for( i=0; i<10; i++ ) BufXLine( 0, 140+i, 319, (16*10)-1, mcga0 ) ;
   for( i=0; i<50; i++ )
   {
      BufXLine( 0, 49-i,  319, (16*10)-1, mcga0 ) ;
      BufXLine( 0, 150+i, 319, (16*10)-1, mcga0 ) ;
      waitmSec( 10 ) ;
   }

   BufSpritePutImage( 274, 99, mcga3+19000, mcga0 ) ;
   waitmSec( 150 ) ;
   BufSpritePutImage( 274, 99, mcga3+18000, mcga0 ) ;
   waitmSec( 125 ) ;

   bfx = 0 ;
   bfmode = 0 ;
   bust_onoff = 0 ;
   i = 0 ;
   while( (bfmode != 2) && !kbhit() )
   {
      if( bfmode==0 )
      {
	 if( i == 30 )
	 {
	    bfmode = 1 ;
	    i = 0 ;
	 }
	 else i++ ;
      }

      if( bfmode == 1 )
      {
	 if( i == 25 ) bfmode = 2 ;
	 else          bfx += bfs[i++] ;
      }
      BufMoveImage( 0, 49, 319, 141, mcga2, mcga1 ) ;
      BufSpritePutImage( 62-bfx, 65, mcga3, mcga1 ) ;

      if( !bust_onoff )
      {
	 BufSpritePutImage( 274-bfx, 99, mcga3+17000, mcga1 ) ;
	 bust_onoff = 1 ;
      }
      else
      {
	 BufSpritePutImage( 274-bfx, 99, mcga3+16000, mcga1 ) ;
	 bust_onoff = 0 ;
      }
      BufMoveImage( 0, 49, 319, 141, mcga1, mcga0 ) ;
      waitmSec( 50 ) ;
   }
   if( !kbhit() ) waitmSec( 800 ) ;
}

void demo1( void )
{
   int dx[15]    = { 100, 60, 45, 30, 22, 17, 14, 11, 9, 7, 5, 4, 3, 2, 1 } ;
   int dy[15]    = { 100, 60, 45, 30, 22, 17, 14, 11, 9, 7, 5, 4, 3, 2, 1 } ;
   int sloop[10] = { 50,50,40,40,30, 30, 20, 10,  10,   0 } ; // Speed Loop
   int door_mode, door_onoff, i ;

   ViewIMG( "takeoff3", mcga1 ) ;
   BufGetImage( 88, 100, 283, 185, mcga2, mcga1 ) ;
   ViewIMG( "takeoff4", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 0L, 64000L ) ;
   ViewIMG( "takeoff5", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 64000L, 64000L ) ;
   door_mode = 0 ;
   door_onoff = 0 ;
   while( (door_mode != 2) && !kbhit() )
   {
      if( !door_onoff )
      {
         xmstomem( XMS_hand, 0L, mcga1, 64000L ) ;
	 door_onoff = 1 ;
      }
      else
      {
         xmstomem( XMS_hand, 64000L, mcga1, 64000L ) ;
	 door_onoff = 0 ;
      }
      if( door_mode == 0 )
      {
	 if( i == 40 ) { door_mode = 1 ; i = 0 ; }
	 else i++ ;
      }
      if( door_mode == 1 )
      {
	 if( i == 15 ) door_mode = 2 ;
	 else Scale( 160-((190/dx[i])>>1), 130-((85/dy[i])>>1), 190/dx[i], 85/dy[i], mcga2, mcga1 ) ;
	 i++ ;
	 waitmSec( sloop[i] ) ;
      }
      else waitmSec( 50 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      waitmSec( 80 ) ;
   }
   BufXLine( 0, 0, 64000, 255, mcga0 ) ;
   waitmSec( 1500 ) ;
}

void stage_demo( int demo_mode )
{
   if( demo_mode == 0 )
   {
     demo0() ;
     if( !kbhit() ) demo1() ;
   }
   key_clear() ;
}
