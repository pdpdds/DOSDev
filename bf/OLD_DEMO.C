
#include <sys\stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <alloc.h>
#include <stdio.h>
#include <dos.h>

#include "bf.h"
int setpage = 1 ;

#define PCM_BURSTER  "buster0"
#define PCM_MULT_GUN "mgun"

extern unsigned char far *mcga0 ; //= (unsigned char far *) 0xa0000000L ;
extern unsigned char far *mcga1 ;
extern unsigned char far *mcga2 ;

void HanWrite( char *sou, char *tar, int mode )
{
   int i, j ;

   BufXLine( 0, 0, 320*20, 15, mcga1+4 ) ;
   *(mcga1+0) = 319 &  0x00ff ;
   *(mcga1+1) = 319 >> 8      ;
   *(mcga1+2) = 19  &  0x00ff ;
   *(mcga1+3) = 19  >> 8      ;

   for( i=1; i<=15; i++ )
   {
      if( i <= 8 )
      {
	 BufHprint( 5, 2, tar, 16-i, mcga1+4 ) ;
	 BufHprint( 5, 2, sou, i,    mcga1+4 ) ;
      }
      else
      {
	 BufHprint( 5, 2, sou, i,    mcga1+4 ) ;
	 BufHprint( 5, 2, tar, 16-i, mcga1+4 ) ;
      }
      BufSpritePutImage( 0, 180, mcga1, mcga0 ) ;
      DelayTime( 1 ) ;
   }

   if( mode )
   {
      for( i=0; i<35; i++ ) DelayTime( 1 ) ;
   }
}

void HanWrite2( char *sou, char *tar )
{
   int i, j ;

   BufXLine( 0, 0, 320*20, 15, mcga1+4 ) ;
   *(mcga1+0) = 319 &  0x00ff ;
   *(mcga1+1) = 319 >> 8      ;
   *(mcga1+2) = 19  &  0x00ff ;
   *(mcga1+3) = 19  >> 8      ;

   for( i=1; i<=15; i++ )
   {
      if( i <= 8 )
      {
	 BufHprint( 5, 2, tar, 16-i, mcga1+4 ) ;
	 BufHprint( 5, 2, sou, i,    mcga1+4 ) ;
      }
      else
      {
	 BufHprint( 5, 2, sou, i,    mcga1+4 ) ;
	 BufHprint( 5, 2, tar, 16-i, mcga1+4 ) ;
      }
      BufSpritePutImage( 0, 90, mcga1, mcga0 ) ;
      DelayTime( 1 ) ;
   }

   for( i=0; i<25; i++ ) DelayTime( 1 ) ;
}

void HanWrite3( char *sou1, char *sou2, char *sou3 )
{
   int i, j ;

   BufXLine( 0, 0, 320*60, 0, mcga1+4 ) ;
   *(mcga1+0) = 319 &  0x00ff ;
   *(mcga1+1) = 319 >> 8      ;
   *(mcga1+2) = 59  &  0x00ff ;
   *(mcga1+3) = 59  >> 8      ;

   for( i=15; i>=1; i-- )
   {
      BufHprint( 5, 2,  sou1, i, mcga1+4 ) ;
      BufHprint( 5, 22, sou2, i, mcga1+4 ) ;
      BufHprint( 5, 42, sou3, i, mcga1+4 ) ;
      BufSpritePutImage( 0, 60, mcga1, mcga0 ) ;
      DelayTime( 1 ) ;
   }

   DelayTime( 200 ) ;

   for( i=1; i<=15; i++ )
   {
      BufHprint( 5, 2,  sou1, i, mcga1+4 ) ;
      BufHprint( 5, 22, sou2, i, mcga1+4 ) ;
      BufHprint( 5, 42, sou3, i, mcga1+4 ) ;
      BufSpritePutImage( 0, 60, mcga1, mcga0 ) ;
      DelayTime( 1 ) ;
   }
}

void black_palette( int mode )
{
   unsigned char iPal[256][3] ;
   int i, j ;

   if( !mode )
   {
      for( i=0; i<256; i++ )
      {
	iPal[i][0] = 0 ;
	iPal[i][1] = 0 ;
	iPal[i][2] = 0 ;
      }
      time_int_push = 1 ;
      SetRgbPaletteSnow( (unsigned char *)iPal ) ;
      time_int_push = 0 ;
   }
   else
   {
      for( i=16; i>0; i-- )
      {
	 for( j=0; j<256; j++ )
	 {
	   iPal[j][0]=(Pal[j][0]*i)>>4;
	   iPal[j][1]=(Pal[j][1]*i)>>4;
	   iPal[j][2]=(Pal[j][2]*i)>>4;
	 }
	 time_int_push = 1 ;
	 SetRgbPaletteSnow( (unsigned char *)iPal ) ;
	 time_int_push = 0 ;
      }
   }
}

void white_palette( void )
{
   unsigned char iPal[256][3] ;
   int i, j ;

   for( i=0; i<16; i++ )
   {
      for( j=0; j<256; j++ )
      {
	 iPal[j][0]=(Pal[j][0]*i)>>4;
	 iPal[j][1]=(Pal[j][1]*i)>>4;
	 iPal[j][2]=(Pal[j][2]*i)>>4;
      }
      time_int_push = 1 ;
      SetRgbPaletteSnow( (unsigned char *)iPal ) ;
      time_int_push = 0 ;
   }
}

void scroll_title( void )
{
   long sc_loop ;
   int i ;

   *(mcga1+0) = 319 &  0x00ff ;
   *(mcga1+1) = 319 >> 8      ;
   *(mcga1+2) = 199 &  0x00ff ;
   *(mcga1+3) = 199 >> 8      ;
   *(mcga2+0) = 319 &  0x00ff ;
   *(mcga2+1) = 319 >> 8      ;
   *(mcga2+2) = 199 &  0x00ff ;
   *(mcga2+3) = 199 >> 8      ;

   ViewIMG( "demo01", mcga1+4 ) ;
   memtoxms( mcga1+4, XMS_hand, 0L, 64000 ) ;
   BufXLine( 0, 0, 320*200, 0, mcga1+4 ) ;
   BufXLine( 0, 0, 320*200, 0, mcga2+4 ) ;
   BufMHprint( 0,   0, "1998.9.23 ·¡¯aœaµI‰Á ´aœsŠ‚· ¸å¸— ¤i¤i", 4<<4, mcga1+4 ) ;
   BufMHprint( 0,  30, "1998.9.28 ”aŠ‚¸âŠ…·¡ ·¡¯aœaµI¡ Ìa¥w",    4<<4, mcga1+4 ) ;
   BufMHprint( 0,  60, "1998.10.2 ·©¥¥ ¸a¶á” Ìa¥w",              4<<4, mcga1+4 ) ;
   BufMHprint( 0,  90, "1998.10.3 ¸a¶á”· Å¡¯a¥e‰w ÐeŠ‚‰·‰b",    4<<4, mcga1+4 ) ;
   BufMHprint( 0, 120, "1998.10.5 ‰w¬wq•¡ ¸ñw",                 4<<4, mcga1+4 ) ;
   BufMHprint( 0, 150, "1998.10.6 q¦‚Ðe Ñs¬wÐaµA ‰·•·”·w",      4<<4, mcga1+4 ) ;
   BufMHprint( 0, 180, "1998.10.8 ‰w¬w•¡µÁ ¸åœaq•¡ »¡µb ¸ñw",   4<<4, mcga1+4 ) ;
   BufMHprint( 0,  10, "1998.10.10 ·¡¯aœaµI· ¯wŸ¡",              4<<4, mcga2+4 ) ;
   BufMHprint( 0,  40, "”aŠ‚¸âŠ…· ÐeŠ‚Ìa¥w ‰i¸÷. £¡Š‚·e ¦‰Àq",   4<<4, mcga2+4 ) ;
   BufMHprint( 0,  70, "1998.10.13 ”¸å·i ‰·¯sÐa”å £¡Š…‹¡ ¤i‰e",  4<<4, mcga2+4 ) ;
   BufMHprint( 0, 100, "1998.10.15 Â—À÷•¡·©”Œa»¡ ¸ñw",          4<<4, mcga2+4 ) ;

   memtoxms( mcga1+4, XMS_hand, 64000L, 64000 ) ;
   memtoxms( mcga2+4, XMS_hand, 128000L, 64000 ) ;

   key_clear() ;

   black_palette( 0 ) ;
   xmstomem( XMS_hand, 0L, mcga0, 64000 ) ;
   white_palette() ;

   sc_loop = GetTime() ;
   for( i=220; i>(-220-120); i-- )
   {
      xmstomem( XMS_hand, 0L, mcga1, 64000 ) ;
      xmstomem( XMS_hand, 64000L, mcga2+4, 64000 ) ;
      BufSpritePutImage( 0, i,  mcga2, mcga1 ) ;
      xmstomem( XMS_hand, 128000L, mcga2+4, 64000 ) ;
      BufSpritePutImage( 0, i+200, mcga2, mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      while( GetTime() < sc_loop+6L ) ;              // Delay Speed
      sc_loop = GetTime() ;
      if( kbhit() )
      {
	 Getkey( 1 ) ;
	 break ;
      }
   }

   black_palette( 1 ) ;
}

void move_mouth( int count, int mode )
{
   static unsigned char far *mouth_p[3] ;
   static unsigned char far *mouth_k[3] ;
   int i, j ;

   switch( mode )
   {
      case 0 :
	 mouth_k[0] = (unsigned char far *)farmalloc( 110L*97L+4L ) ;
	 mouth_k[1] = (unsigned char far *)farmalloc( 110L*97L+4L ) ;
	 mouth_k[2] = (unsigned char far *)farmalloc( 110L*97L+4L ) ;
	 ViewIMG( "demo02-1", mcga1 ) ;
	 BufGetImage( 102, 31, 102+109, 31+96, mouth_k[2], mcga1 ) ;
	 ViewIMG( "demo02-2", mcga1 ) ;
	 BufGetImage( 1,   1,  1+109,   1+96,  mouth_k[0], mcga1 ) ;
	 BufGetImage( 112, 1,  112+109, 1+96,  mouth_k[1], mcga1 ) ;
	 break ;
      case 1 :
	 for( j=0; j<count; j++ )
	 {
	    if( random(5)!=2 )
	    {
	       BufPutImage( 102, 31, mouth_k[0], mcga0 ) ;
	       DelayTime( 10 ) ;
	       BufPutImage( 102, 31, mouth_k[1], mcga0 ) ;
	       DelayTime( 10 ) ;
	       BufPutImage( 102, 31, mouth_k[0], mcga0 ) ;
	       DelayTime( 10 ) ;
	       BufPutImage( 102, 31, mouth_k[2], mcga0 ) ;
	       DelayTime( 10 ) ;
	    }
	    else DelayTime( 30 ) ;
	 }
	 DelayTime( 20 ) ;
	 break ;
      case 2 :
	 farfree( mouth_k[0] ) ;
	 farfree( mouth_k[1] ) ;
	 farfree( mouth_k[2] ) ;
	 break ;
      case 10:
	 mouth_p[0] = (unsigned char far *)farmalloc( 115L*66L+4L ) ;
	 mouth_p[1] = (unsigned char far *)farmalloc( 115L*66L+4L ) ;
	 mouth_p[2] = (unsigned char far *)farmalloc( 115L*66L+4L ) ;
	 ViewIMG( "demo02-3", mcga1 ) ;
	 BufGetImage( 151, 114, 151+114, 114+65, mouth_p[2], mcga1 ) ;
	 ViewIMG( "demo02-4", mcga1 ) ;
	 BufGetImage( 1,   1,  1+114,   1+65,  mouth_p[0], mcga1 ) ;
	 BufGetImage( 117, 1,  117+114, 1+65,  mouth_p[1], mcga1 ) ;
	 break ;
      case 11:
	 for( j=0; j<count; j++ )
	 {
	    if( random(5)!=2 )
	    {
	       BufPutImage( 151, 114, mouth_p[0], mcga0 ) ;
	       DelayTime( 15 ) ;
	       BufPutImage( 151, 114, mouth_p[1], mcga0 ) ;
	       DelayTime( 15 ) ;
	       BufPutImage( 151, 114, mouth_p[0], mcga0 ) ;
	       DelayTime( 15 ) ;
	       BufPutImage( 151, 114, mouth_p[2], mcga0 ) ;
	       DelayTime( 15 ) ;
	    }
	    else DelayTime( 40 ) ;
	 }
	 DelayTime( 20 ) ;
	 break ;
      case 12:
	 farfree( mouth_p[0] ) ;
	 farfree( mouth_p[1] ) ;
	 farfree( mouth_p[2] ) ;
	 break ;
   }
}

void talk( void )
{
   ViewIMG( "demo02",   mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 0L, 64000 ) ;
   ViewIMG( "demo02-1", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 64000L, 64000 ) ;
   ViewIMG( "demo02-3", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 128000L, 64000 ) ;

   move_mouth( 0, 0  ) ;
   move_mouth( 0, 10 ) ;

   black_palette( 0 ) ;
   xmstomem( XMS_hand, 0L, mcga0, 320*200 ) ;
   white_palette() ;

   key_clear() ;
//   PlayVocFile( "bf_demo.voc", 0 ) ;          // Phone Bell
   HanWrite( " ", "S- ‘A. ”É·w §¡¬á¯© ·³“¡”a.", 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 64000L, mcga0, 320*180 ) ;
   HanWrite( "S- ‘A. ”É·w §¡¬á¯© ·³“¡”a.",
	     "K- ”É·wA ¸åÐi i·¡ ·¶¯s“¡”a.", 0 ) ;
   move_mouth( 9, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 0L, mcga0, 320*180 ) ;
   HanWrite( "K- ”É·wA ¸åÐi i·¡ ·¶¯s“¡”a.",
	     "S- ¸áµA‰A  i³qÐa¯³¯¡¶a.", 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 64000L, mcga0, 320*180 ) ;
   HanWrite( "S- ¸áµA‰A  i³qÐa¯³¯¡¶a.",
	     "K- ‹b§¡¬aÐw·a¡ »¢¸ó ¸å”iÐa‰¡ ¯¼¯s“¡”a.", 0 ) ;
   move_mouth( 16, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 128000L, mcga0, 320*180 ) ;
   HanWrite( "K- ‹b§¡¬aÐw·a¡ »¢¸ó ¸å”iÐa‰¡ ¯¼¯s“¡”a.",
	     "P- §¡¬á. ˆa ¤h‰V‘A. ·¡½¢·a¡ µe‰iÐa‰A", 0 ) ;
   move_mouth( 15, 11 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 64000L, mcga0, 320*180 ) ;
   HanWrite( "P- §¡¬á. ˆa ¤h‰V‘A. ·¡½¢·a¡ µe‰iÐa‰A",
	     "K- ÐeŠ‚·i ¢¬w »¡¶¥Ða‰¡ ¯¼¯s“¡”a.", 0 ) ;
   move_mouth( 13, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;
   HanWrite( "K- ÐeŠ‚·i ¢¬w »¡¶¥Ða‰¡ ¯¼¯s“¡”a.",
	     "K- ÐáœbÐ º¯¡‰V¯s“¡Œa ?", 0 ) ;
   move_mouth( 9, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 128000L, mcga0, 320*180 ) ;
   HanWrite( "K- ÐáœbÐ º¯¡‰V¯s“¡Œa ?",
	     "P- ’Š¯³“¡Œa ?", 0 ) ;
   move_mouth( 5, 11 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 64000L, mcga0, 320*180 ) ;
   HanWrite( "P- ’Š¯³“¡Œa ?",
	     "K- £¡¯aÈá K œa‰¡ e ´i´aº¯³¯¡µ¡", 0 ) ;
   move_mouth( 13, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;
   HanWrite( "K- £¡¯aÈá K œa‰¡ e ´i´aº¯³¯¡µ¡",
	     "K- ¸å¸—µA¬á ¯wŸ¡Ða•¡¢ •³‰¡ ¯¼¯s“¡”a.", 0 ) ;
   move_mouth( 15, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 128000L, mcga0, 320*180 ) ;
   HanWrite( "K- ¸å¸—µA¬á ¯wŸ¡Ða•¡¢ •³‰¡ ¯¼¯s“¡”a.",
	     "P- ‹aœý”a¡e ”w¯¥·¡ ¶¥Ða“e ˆõ·e ?", 0 ) ;
   move_mouth( 12, 11 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 64000L, mcga0, 320*180 ) ;
   HanWrite( "P- ‹aœý”a¡e ”w¯¥·¡ ¶¥Ða“e ˆõ·e ?",
	     "K- ¸å¸—·¡ {eÒ ·¡´¡‹¡ Ða‰V¯s“¡”a.", 0 ) ;
   move_mouth( 14, 1 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;

   xmstomem( XMS_hand, 128000L, mcga0, 320*180 ) ;
   HanWrite( "K- ¸å¸—·¡ {eÒ ·¡´¡‹¡ Ða‰V¯s“¡”a.",
	     "P- ·q... ¬—ˆbÐ ¥¡‰V­¡.", 0 ) ;
   move_mouth( 7, 11 ) ;
//   PlayVocFile( "bf_demo.voc", 1 ) ;
   HanWrite( "P- ·q... ¬—ˆbÐ ¥¡‰V­¡.",
	     " ", 0 ) ;

   move_mouth( 0, 2  ) ;
   move_mouth( 0, 12 ) ;

   black_palette( 1 ) ;
}

void change_title( void )
{
   long sc_loop ;
   int  i, j, x[4], y[4] ;
   unsigned char far *img[4] ;

   for( i=0; i<4; i++ )
      img[i] = (unsigned char far *)farmalloc( (124L*114L)+4L ) ;

   ViewIMG( "bfdemob1", mcga1 ) ;
   BufHprint( 20, 170, "µA·¡¯a—i· ÂAˆw· Íe” 'ÎŸe '‰i¬÷!", 147, mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 0L, 64000 ) ;

   ViewIMG( "bfdemob2", mcga1 ) ;
   BufHprint( 20, 170, "µA·¡¯a—i· ÂAˆw· Íe” 'ÎŸe '‰i¬÷!", 147, mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 64000L, 64000 ) ;

   ViewIMG( "demo03", mcga1 ) ;
   BufGetImage( 27,  34, 27+123,  34+113, img[0], mcga1 ) ;
   BufGetImage( 163, 34, 163+123, 34+113, img[1], mcga1 ) ;
   ViewIMG( "demo04", mcga1 ) ;
   BufGetImage( 27,  34, 27+123,  34+113, img[2], mcga1 ) ;
   BufGetImage( 163, 34, 163+123, 34+113, img[3], mcga1 ) ;

   j = 0 ;

   if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
   else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
   black_palette( 0 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;

   for( i=0; i<10; i++ )
   {
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   x[0] = 20 ;
   y[0] = 205 ;
   x[1] = 180 ;
   y[1] = -114 ;
   while( 1 )
   {
      y[0] -= 10 ;
      y[1] += 10 ;
      if( y[0] < 50 ) break ;
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      BufPutImage( x[0], y[0], img[0], mcga1 ) ;
      BufPutImage( x[1], y[1], img[1], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   for( i=0; i<10; i++ )
   {
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      BufPutImage( x[0], y[0], img[0], mcga1 ) ;
      BufPutImage( x[1], y[1], img[1], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   while( 1 )
   {
      y[0] -= 10 ;
      y[1] += 10 ;
      if( y[0] <= -114 ) break ;
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      BufPutImage( x[0], y[0], img[0], mcga1 ) ;
      BufPutImage( x[1], y[1], img[1], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   x[2] = -124 ;
   y[2] = -10 ;
   x[3] = 320 ;
   y[3] = 100 ;
   while( 1 )
   {
      x[2] += 10 ;
      x[3] -= 10 ;
      if( x[2] > 30 ) break ;
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      BufPutImage( x[2], y[2], img[2], mcga1 ) ;
      BufPutImage( x[3], y[3], img[3], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   for( i=0; i<10; i++ )
   {
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      BufPutImage( x[2], y[2], img[2], mcga1 ) ;
      BufPutImage( x[3], y[3], img[3], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   while( 1 )
   {
      x[2] += 10 ;
      x[3] -= 10 ;
      if( x[2] >= 320 ) break ;
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      BufPutImage( x[2], y[2], img[2], mcga1 ) ;
      BufPutImage( x[3], y[3], img[3], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   j = 0 ;
   for( i=0; i<10; i++ )
   {
      sc_loop = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      MoveData( mcga1, mcga0, 64000 ) ;
      if( kbhit() ) { getch(); break ; }
      while( (GetTime()-sc_loop) < 6L ) ;              // Delay Speed
   }

   for( i=0; i<4; i++ ) farfree( img[i] ) ;
}

void fly_bf( void )
{
   long tim ;
   int  x, y, file, i, j  ;

   j = 0 ;

   file = open( "bfdemo.idb", O_RDONLY | O_BINARY, S_IWRITE | S_IREAD ) ;
   if( file == -1 ) return ;

   for( i=0; i<19+40; i++ )       // Sprite Count Value 18
   {
      tim = GetTime() ;
      if( j == 0 ) { xmstomem( XMS_hand, 0L, mcga1, 64000 ) ; j = 1 ; }
      else         { xmstomem( XMS_hand, 64000L, mcga1, 64000 ) ; j = 0 ; }
      if( (i >= 10) && (i <= 28) )
      {
	 if( i == 13 ) sayPCM( PCM_BURSTER, 1 ) ;
	 read( file, &x, 2L ) ;
	 read( file, &y, 2L ) ;
	 read( file, mcga2+4L, (x+1)*(y+1) ) ;
	 *(mcga2+0) = x &  0x00ff ;
	 *(mcga2+1) = x >> 8      ;
	 *(mcga2+2) = y &  0x00ff ;
	 *(mcga2+3) = y >> 8      ;
	 BufSpritePutImage( 160-(x>>1), 135-(y>>1)-((i-10)<<2), mcga2, mcga1 ) ;
	 MoveData( mcga1, mcga0, 64000 ) ;
	 while( (GetTime()-tim) < (20L-(i-10L)) ) ;
      }
      else
      {
	 MoveData( mcga1, mcga0, 64000 ) ;
	 while( (GetTime()-tim) < 20L ) ;
      }
   }

   black_palette( 1 ) ;
}

void f16_fire( void )
{
   unsigned char far *f16 ;
   unsigned char far *fire[4] ;
   char frame_count  ;
   char fire_attr    ;
   char fire_count   ;
   char bf_end       ;
   int  bf_x, bf_y   ;
   int  f16_x, f16_y ;
   long tim          ;
   int  bk_x, i, j   ;

   frame_count = 0 ;
   fire_attr = 0 ;

   bf_x   = -50 ;
   bf_y   = 20 ;
   bf_end = 0  ;
   bk_x   = 0 ;

   f16_x  = 250 ;
   f16_y  = 130 ;

   f16     = (unsigned char far *)farmalloc( 80L*25L+4L ) ;
   fire[0] = (unsigned char far *)farmalloc( 15L*15L+4L ) ;
   fire[1] = (unsigned char far *)farmalloc( 40L*30L+4L ) ;
   fire[2] = (unsigned char far *)farmalloc( 80L*60L+4L ) ;
   fire[3] = (unsigned char far *)farmalloc( 100L*110L+4L ) ;

   ViewIMG( "demo05", mcga1 ) ;
   BufGetImage( 0,   155, 79,     155+24, f16,     mcga1 ) ;
   BufGetImage( 0,   85,  14,     85+14,  fire[0], mcga1 ) ;
   BufGetImage( 15,  85,  15+39,  85+29,  fire[1], mcga1 ) ;
   BufGetImage( 55,  85,  55+79,  85+59,  fire[2], mcga1 ) ;
   BufGetImage( 135, 85,  135+99, 85+109, fire[3], mcga1 ) ;

   ViewIMG( "demo05-1", mcga1 ) ;
   BufGetImage( 0,   0,   309,    114,    mcga2,     mcga1 ) ;
   memtoxms( mcga2, XMS_hand, 0L, 310*115+4 ) ;

   ViewIMG( "demo05-2", mcga1 ) ;
   BufGetImage( 0,   0,   259,    144,    mcga2,     mcga1 ) ;
   memtoxms( mcga2, XMS_hand, 64000L, 260*145+4 ) ;

   *(mcga1+0) = 319 &  0x00ff ;
   *(mcga1+1) = 319 >> 8      ;
   *(mcga1+2) = 199 &  0x00ff ;
   *(mcga1+3) = 199 >> 8      ;

   *(mcga2+0) = 319 &  0x00ff ;
   *(mcga2+1) = 319 >> 8      ;
   *(mcga2+2) = 199 &  0x00ff ;
   *(mcga2+3) = 199 >> 8      ;

   ViewIMG( "bfdemob3", mcga2+4 ) ;
   memtoxms( mcga2, XMS_hand, 128000L, 64000 ) ;

   BufPutImage( bk_x, 0, mcga2, mcga1 ) ;
   xmstomem( XMS_hand, 0L, mcga2, 310*115+4 ) ;
   BufSpritePutImage( bf_x+random(10), bf_y+random(10), mcga2, mcga1 ) ;
   BufSpritePutImage( f16_x+random(5), f16_y+random(5), f16, mcga1 ) ;

   black_palette( 0 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;

   do
   {
      tim = GetTime() ;

      xmstomem( XMS_hand, 128000L, mcga2, 64000 ) ;
      BufPutImage( bk_x,     0, mcga2, mcga1 ) ;
      BufPutImage( bk_x-320, 0, mcga2, mcga1 ) ;
      BufSpritePutImage( f16_x+random(3), f16_y+random(3), f16, mcga1 ) ;
      if( !bf_end )
      {
	 xmstomem( XMS_hand, 0L, mcga2, 310*115+4 ) ;
	 BufSpritePutImage( bf_x, bf_y, mcga2, mcga1 ) ;
      }
      else
      {
	 xmstomem( XMS_hand, 64000L, mcga2, 260*145+4 ) ;
	 BufSpritePutImage( bf_x, bf_y, mcga2, mcga1 ) ;
      }
      if( fire_attr )
      {
	 for( i=0; i<3; i++ )
	 {
	    if( fire_count != 6 )
	    {
	       j = fire_count - i ;
	       if( fire_count == 0 ) sayPCM( PCM_MULT_GUN, 1 ) ;
	       if( j == 0 ) BufSpritePutImage( f16_x,     130, fire[0], mcga1 ) ;
	       if( j == 1 ) BufSpritePutImage( f16_x-30,  120, fire[1], mcga1 ) ;
	       if( j == 2 ) BufSpritePutImage( f16_x-90,  80,  fire[2], mcga1 ) ;
	       if( j == 3 ) BufSpritePutImage( f16_x-150, 40,  fire[3], mcga1 ) ;
	    }
	    else
	       fire_attr = 0 ;
	 }
	 fire_count++ ;
      }
      else
      {
	 if( random( 5 ) == 2 )
	 {
	    fire_attr  = 1 ;
	    fire_count = 0 ;
	 }
      }
      MoveData( mcga1, mcga0, 64000 ) ;
      while( (GetTime()-tim) < 15L ) ;
      frame_count++ ;
      if( kbhit() )
      {
	 Getkey( 1 ) ;
	 break ;
      }
      if( !bf_end ) bf_x += 4 ;
      else          bf_x += 15 ;
      if( (frame_count > 15) && !fire_attr )
      {
	 bf_end = 1 ;
	 bf_x += 20 ;
      }
      if( bf_x > 340 ) bf_end = 2 ;

      f16_x -= 2 ;
      bk_x  += 10 ;
      if( bk_x == 320 ) bk_x = 0 ;
   } while( (bf_end < 2) || fire_attr ) ;

   farfree( f16 ) ;
   farfree( fire[0] ) ;
   farfree( fire[1] ) ;
   farfree( fire[2] ) ;
   farfree( fire[3] ) ;

   black_palette( 1 ) ;
}

void f16_face( void )
{
   unsigned char far *face[3] ;
   char face_attr  ;
   char face_count ;
   int  i, j       ;

   face_attr = 1 ;
   face_count = 0 ;

   for( i=0; i<3; i++ ) face[i] = (unsigned char far *)farmalloc( 105L*93L+4L ) ;

   ViewIMG( "demo06", mcga1 ) ;
   BufGetImage( 1,   1,  1+104,   1+92, face[0], mcga1 ) ;
   BufGetImage( 108, 1,  108+104, 1+92, face[2], mcga1 ) ;

   ViewIMG( "demo07", mcga1 ) ;
   BufGetImage( 133, 29, 133+104, 29+92, face[1], mcga1 ) ;

   black_palette( 0 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;

   do
   {
      switch( random( 2 ) )
      {
	 case 0 : if( face_attr != 0 ) face_attr-- ; break ;
	 case 1 : if( face_attr != 2 ) face_attr++ ; break ;
      }
      if( face_attr == 0 ) BufPutImage( 133, 29, face[0], mcga0 ) ;
      if( face_attr == 1 ) BufPutImage( 133, 29, face[1], mcga0 ) ;
      if( face_attr == 2 ) BufPutImage( 133, 29, face[2], mcga0 ) ;

      DelayTime( 50 ) ;

      if( kbhit() )
      {
	 Getkey( 1 ) ;
	 break ;
      }
      face_count++ ;
   } while( face_count < 12 ) ;

   for( i=0; i<3; i++ ) farfree( face[i] ) ;

   black_palette( 1 ) ;
}

void f16_target( void )
{
   char far *bf ;
   char bf_attr ;
   long tim ;
   int bf_y ;
   int bk_y ;

   bf = (unsigned char far *)farmalloc( 250L*64L+4L ) ;

   ViewIMG( "bfdemob3", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 0L, 64000 ) ;

   ViewIMG( "demo08", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 64000L, 64000 ) ;

   ViewIMG( "demo09", mcga1 ) ;
   BufGetImage( 0, 0, 249, 63, bf, mcga1 ) ;

   *(mcga2+0) = 319 &  0x00ff ;
   *(mcga2+1) = 319 >> 8      ;
   *(mcga2+2) = 199 &  0x00ff ;
   *(mcga2+3) = 199 >> 8      ;

   xmstomem( XMS_hand, 0L, mcga2+4, 64000 ) ;
   BufSpritePutImage( 0, 0, mcga2, mcga1 ) ;
   xmstomem( XMS_hand, 64000L, mcga2+4, 64000 ) ;
   BufSpritePutImage( 0, 0, mcga2, mcga1 ) ;
   black_palette( 0 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;

   bf_attr = 0 ;
   bf_y = 160 ;
   bk_y = 0 ;
   do
   {
      tim = GetTime() ;
      if( !bf_attr ) bf_y -= 3 ;
      else           bf_attr++ ;
      xmstomem( XMS_hand, 0L, mcga2+4, 64000 ) ;
      BufPutImage( 0, bk_y,     mcga2, mcga1 ) ;
      BufPutImage( 0, bk_y-200, mcga2, mcga1 ) ;
      BufSpritePutImage( 40+random(3), bf_y+random(3), bf, mcga1 ) ;
      xmstomem( XMS_hand, 64000L, mcga2+4, 64000 ) ;
      BufSpritePutImage( 0, 0, mcga2, mcga1 ) ;

      if( kbhit() )
      {
	 Getkey( 1 ) ;
	 break ;
      }
      MoveData( mcga1, mcga0, 64000 ) ;
      while( (GetTime()-tim) < 20L ) ;
      if( !bf_attr)
      {
	 if( bf_y < 40 ) bf_attr = 1 ;
      }
      bk_y += 5 ;
      if( bk_y == 200 ) bk_y = 0 ;
   } while( bf_attr < 15 ) ;

   farfree( bf ) ;

   black_palette( 1 ) ;
}

void f16_strike( void )
{
   unsigned char far *strike[2] ;
   unsigned char far *hand0 ;
   unsigned char far *hand1 ;
   char strike_count ;
   char strike_attr ;
   long tim ;

   hand0     = (unsigned char far *)farmalloc( 78L*39L+4L ) ;
   hand1     = (unsigned char far *)farmalloc( 78L*39L+4L ) ;
   strike[0] = (unsigned char far *)farmalloc( 160L*70L+4L ) ;
   strike[1] = (unsigned char far *)farmalloc( 160L*85L+4L ) ;

   ViewIMG( "demo10", mcga1 ) ;
   BufGetImage( 1, 1,   1+77,  1+38,   hand0,     mcga1 ) ;
   BufGetImage( 1, 45,  1+159, 45+69,  strike[0], mcga1 ) ;
   BufGetImage( 1, 115, 1+159, 115+84, strike[1], mcga1 ) ;

   ViewIMG( "demo11", mcga1 ) ;
   memtoxms( mcga1, XMS_hand, 0L, 64000 ) ;
   BufGetImage( 67, 50, 67+77, 50+38, hand1, mcga1 ) ;
   BufPutImage( 67, 50, hand0, mcga1 ) ;
   black_palette( 0 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;
   DelayTime( 100 ) ;

   strike_count = 0 ;
   strike_attr  = 0 ;
   sayPCM( PCM_MULT_GUN, 1 ) ;
   do
   {
      tim = GetTime() ;
      xmstomem( XMS_hand, 0L, mcga1, 64000 ) ;
      BufPutImage( 67, 50, hand1, mcga1 ) ;
      BufSpritePutImage( 70, 120, strike[strike_attr], mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      while( (GetTime()-tim) < 10L ) ;
      if( strike_attr ) strike_attr = 0 ;
      else              strike_attr = 1 ;
      if( kbhit() )
      {
	 Getkey( 1 ) ;
	 break ;
      }
      if( VoiceFlag==VOICE_IDLE ) sayPCM( PCM_MULT_GUN, 1 ) ;
   } while( ++strike_count < 40 ) ;

   farfree( hand0 ) ;
   farfree( hand1 ) ;
   farfree( strike[0] ) ;
   farfree( strike[1] ) ;

   black_palette( 1 ) ;
}

void f16_kill( void )
{
   unsigned char far *bf ;
   int bf_x ;
   int bk_x ;
   long tim ;

   *(mcga1+0) = 319 &  0x00ff ;
   *(mcga1+1) = 319 >> 8      ;
   *(mcga1+2) = 199 &  0x00ff ;
   *(mcga1+3) = 199 >> 8      ;
   *(mcga2+0) = 319 &  0x00ff ;
   *(mcga2+1) = 319 >> 8      ;
   *(mcga2+2) = 199 &  0x00ff ;
   *(mcga2+3) = 199 >> 8      ;

   bf = (unsigned char far *)farmalloc( 52L*44L+4L ) ;
   bf_x = 160 ;

   bk_x = 0 ;

   ViewIMG( "bfdemob3", mcga1+4 ) ;
   memtoxms( mcga1+4, XMS_hand, 0L, 64000 ) ;

   ViewIMG( "demo12", mcga1+4 ) ;
   BufGetImage( 1, 1, 1+51,  1+43, bf, mcga1+4 ) ;

   ViewIMG( "demo13", mcga1+4 ) ;
   memtoxms( mcga1+4, XMS_hand, 64000L, 64000 ) ;

   xmstomem( XMS_hand, 0L, mcga2+4, 64000 ) ;
   BufPutImage( bk_x, 0,  mcga2, mcga1 ) ;
   BufSpritePutImage( bf_x, 50, bf,    mcga1 ) ;
   xmstomem( XMS_hand, 64000L, mcga2+4, 64000 ) ;
   BufSpritePutImage( 0,    0,  mcga2, mcga1 ) ;
   black_palette( 0 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;

   sayPCM( PCM_BURSTER, 1 ) ;
   do
   {
      tim = GetTime() ;
      xmstomem( XMS_hand, 0L, mcga2+4, 64000 ) ;
      BufPutImage( bk_x,     0,  mcga2, mcga1 ) ;
      BufPutImage( bk_x+320, 0,  mcga2, mcga1 ) ;
      BufSpritePutImage( bf_x, 50, bf,    mcga1 ) ;
      xmstomem( XMS_hand, 64000L, mcga2+4, 64000 ) ;
      BufSpritePutImage( 0,    0,  mcga2, mcga1 ) ;
      MoveData( mcga1, mcga0, 64000 ) ;
      while( (GetTime()-tim) < 15L ) ;
      if( kbhit() )
      {
	 Getkey( 1 ) ;
	 break ;
      }
      bf_x += 4 ;
      bk_x -= 5 ;
      if( bk_x == -320 ) bk_x = 0 ;
   } while( bf_x < 300 ) ;

   farfree( bf ) ;

   black_palette( 1 ) ;
}

void demo_main( void )
{
//   VoiceDirectPlay( "C:\\VEDIT\\JAEGOO.VOC" ) ;

   SetRgbPaletteSnow( (unsigned char *)&Pal ) ;

   ViewIMG( "demo02", mcga1 ) ;
   HanWrite2( " ", "‹a—i· Ã±œ¢·e ·¡ˆõ·¡ Àá·q·e ´a“¡´ö”a." ) ;
   HanWrite2( "‹a—i· Ã±œ¢·e ·¡ˆõ·¡ Àá·q·e ´a“¡´ö”a.",
	      "‹a ¢®Ðe µb¬aŸi ¶Ÿ¡“e ‹¡´âÐe”a." ) ;
   HanWrite2( "‹a ¢®Ðe µb¬aŸi ¶Ÿ¡“e ‹¡´âÐe”a.",
	      "‹a—i·¡ ‰iÅ¡ ¶Ÿ¡· Ã¥Š e·e ´a“¡”a." ) ;
   HanWrite2( "‹a—i·¡ ‰iÅ¡ ¶Ÿ¡· Ã¥Š e·e ´a“¡”a.",
	      "¶Ÿ¡“e Ðw¬w ·¸´á¬á“e ´e–Iˆõ·¡”a." ) ;
   HanWrite2( "¶Ÿ¡“e Ðw¬w ·¸´á¬á“e ´e–Iˆõ·¡”a.",
	      "‹a—i· Ã±œ¢·¡ Š…¬a¸â·¥ˆõ ª… ´a“¡œa," ) ;
   HanWrite2( "‹a—i· Ã±œ¢·¡ Š…¬a¸â·¥ˆõ ª… ´a“¡œa,",
	      "¢…ÑÁ· Ã±œ¢•¡ ·¶”a“e ˆõ·i...." ) ;
   HanWrite2( "¢…ÑÁ· Ã±œ¢•¡ ·¶”a“e ˆõ·i....", " " ) ;

   AdLib_Start( "bfdemo1" ) ;               // Set Music
   scroll_title() ;
   AdLib_End() ;                            // Music End

   AdLib_Start( "bfdemo2" ) ;               // Music Start
   talk() ;
   key_clear() ;
   AdLib_End() ;		            // Music End

   AdLib_Start( "bfdemo3" ) ;               // Music Start
   change_title() ;
   fly_bf() ;
   key_clear() ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   HanWrite3( " ", "             º— ‰A Ë± 1993", " " ) ;
   key_clear() ;
   AdLib_End() ;			      // Music End

   AdLib_Start( "bfdemo4" ) ;                 // Music Start
   f16_fire() ;
   key_clear() ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   HanWrite3( " ", "           ˆq •¢ : ¥i ¤a œq", " " ) ;
   key_clear() ;

   f16_face() ;
   key_clear() ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   HanWrite3( " ", "         Ïa ¡ ‹a œ‘ : ¹¡ ¸ Š", " " ) ;
   key_clear() ;

   f16_target() ;
   key_clear() ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   HanWrite3( "      ¬a ¶… —a : ¹¡ ¸ Š, a Ða a",
	      "       ·q  ´b  : ¥i ¤a œq, KRISS",
	      "      ‹a œ Ï¢ : ¥i ¤a œq" ) ;
   key_clear() ;

   f16_strike() ;
   key_clear() ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   HanWrite3( "       ¯¡ a Ÿ¡ µ¡ : ¥i ¤a œq",
	      "       ¥¡¹¡ ‹aœÏ¢ : ¹¡ ¸ Š",
	      "       ˆq¬a —aŸ©¦… : ¥e •· Ñ¡, COEYES" ) ;
   key_clear() ;

   f16_kill() ;
   key_clear() ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   HanWrite3( " ", "       Àå Ÿ¡ ´e   Q •·   º— ‰A Ë±", " " ) ;
   key_clear() ;

}

/*
   asm {
      XOR  AH, AH       // 360x400 mode
      MOV  AL, 13H
      INT  10H
      MOV  DX, 03C4H
      MOV  AX, 0604H
      OUT  DX, AX
      MOV  AX, 0100H
      OUT  DX, AX
      MOV  DX, 03C2H
      MOV  AL, 0E7H
      OUT  DX, AL
      MOV  DX, 03C4H
      MOV  AX, 0300H
      OUT  DX, AX
      MOV  DX, 0D4H
      MOV  AL, 11H
      OUT  DX, AL
      INC  DX
      IN   AL, DX
      AND  AL, 07FH
      OUT  DX, AL
   }
*/
