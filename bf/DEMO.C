
#include <sys\stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <alloc.h>
#include <stdio.h>
#include <dos.h>

#include "bf.h"

extern unsigned char far *mcga0 ; //= (unsigned char far *) 0xa0000000L ;
extern unsigned char far *mcga1 ;
extern unsigned char far *mcga2 ;

char *demo_mes[18] =
{
   "  ‹a—i· Ã±œ¢·e ·¡ˆõ·¡ Àá·q·e ´a“¡´ö”a",
   "¢®Ðe ‰Áˆá· µb¬aŸi ˆá¯iœá¥© Ï©¶a•¡´ô”a",
   "·¡£¡ ¸÷¯¥·e ‹a—iµA‰A »¡¤Ÿi ¤h»¡ ´g“eˆa?",
   "       ·¡¹A ¶Ÿ¡“e Œ´áa´¡ Ðe”a",
   "·¡ {´ô“e °a¶‘µA¬á ¸q—i‰¡ ¯¼»¡ ´g”a¡e...",
   " ",
   " ",
   " ",
   "       ˆq•¢,‹aœÏ¢,·q´b:¥i¤aœq",
   "    Ïa¡‹aœ£·,¬a¶…—a:¹¡¸Š(¤åˆ)",
   "   ¬a¶…—a¥¡¹¡,£A»¢¡¡—a:aÐaa(´e¬÷ÒA)",
   "        ­U´ó Ïa¡‹aœ£·:¸÷ º…",
   " ",
   " ",
   " ",
   " ",
   " ",
   " "
} ;

char h_char  = 0 ;
char h_count = 0 ;
char h_color = 0 ;
char h_delay = 0 ;

void HanWrite( void )
{
   int i, j ;

   if( h_delay == 2 )
   {
      if( h_count == 0 )
      {
	 if( h_color == 60 )
	 {
	    h_color = 0 ;
	    h_count++ ;
	 }
	 else
	 {
	    SetPalette( 254, h_color, h_color, h_color ) ;
	    h_color += 2 ;
	 }
      }

      if( h_count == 1 )
      {
	 if( h_color == 20 )
	 {
	    h_color = 60 ;
	    h_count++ ;
	 }
	 else h_color++ ;
      }

      if( h_count == 2 )
      {
	 if( h_color == -2 )
	 {
	    if( h_char < 7 ) BufBoxFill( 0, 170, 319, 190, 0, mcga1 ) ;
	    h_color = 0 ;
	    h_count++ ;
	 }
	 else
	 {
	    SetPalette( 254, h_color, h_color, h_color ) ;
	    h_color -= 2 ;
	 }
      }

      if( h_count == 3 )
      {
	 if( h_color == 10 )
	 {
	    h_color = 0 ;
	    h_count++ ;
	    h_char++ ;
	 }
	 else h_color++ ;
      }

      h_delay = 0 ;
   }
   else
      h_delay++ ;

   if( (h_count==0) || (h_count==1) || (h_count==2) || (h_color>20) )
      BufHprint( 0, 170, demo_mes[h_char], 254, mcga1 ) ;
   if( h_count == 4 ) h_count = 0 ;
   waitmSec( 10 ) ;
}

void demo_palette( int p_mode )
{
   unsigned char iPal[256][3] ;
   int i, j ;

   if( !p_mode )
   {
      for( j=0; j<256; j++ )
      {
	iPal[j][0] = Pal[j][0] ;
	iPal[j][1] = Pal[j][1] ;
	iPal[j][2] = Pal[j][2] ;
      }

      for( i=64; i>0; i-- )
      {
	 for( j=0; j<256; j++ )
	 {
	   if( 63 != iPal[j][0] ) iPal[j][0]++ ;
	   if( 63 != iPal[j][1] ) iPal[j][1]++ ;
	   if( 63 != iPal[j][2] ) iPal[j][2]++ ;
	 }
	 time_int_push = 1 ;
	 SetRgbPaletteSnow( (unsigned char *)iPal ) ;
	 time_int_push = 0 ;
      }
   }
   else
   {
      for( j=0; j<256; j++ )
      {
	iPal[j][0] = 63 ;
	iPal[j][1] = 63 ;
	iPal[j][2] = 63 ;
      }

      for( i=0; i<64; i++ )
      {
	 for( j=0; j<256; j++ )
	 {
	   if( Pal[j][0] != iPal[j][0] ) iPal[j][0]-- ;
	   if( Pal[j][1] != iPal[j][1] ) iPal[j][1]-- ;
	   if( Pal[j][2] != iPal[j][2] ) iPal[j][2]-- ;
	 }
	 time_int_push = 1 ;
	 SetRgbPaletteSnow( (unsigned char *)iPal ) ;
	 time_int_push = 0 ;
      }
   }
}

void count_demo( void )
{
   int y0=200, y1=400, i, j ;

   ViewIMG( "bf-td1", mcga1 ) ;

   MoveData( mcga1, mcga3, 64000 ) ;
   BufXLine( 0, 0, 64000, 0, mcga1 ) ;

   BufGetImage( 0, 0, 158, 98, mcga2+32000, mcga3 ) ;

   BufGetImage( 162, 102, 318, 198, mcga2, mcga3 ) ;
   for( i=0; i<400; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 80, y1, mcga2, mcga1 ) ;
      HanWrite() ;
      MoveData( mcga1, mcga0, 64000 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;                   
   }

   y0 = 200 ;
   for( i=0; i<200; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 80, y1, mcga2, mcga1 ) ;
      HanWrite() ;
      MoveData( mcga1, mcga0, 64000 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;
   }

   y1 = 200 ;
   BufGetImage( 0,   102, 158, 198, mcga2, mcga3 ) ;
   for( i=0; i<200; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 80, y1, mcga2, mcga1 ) ;
      HanWrite() ;
      MoveData( mcga1, mcga0, 64000 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;
   }

   y0 = 200 ;
   for( i=0; i<200; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 80, y1, mcga2, mcga1 ) ;
      HanWrite() ;
      MoveData( mcga1, mcga0, 64000 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;
   }

   y1 = 200 ;
   BufGetImage( 162, 0,   318, 98,  mcga2, mcga3 ) ;
   for( i=0; i<200; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 80, y1, mcga2, mcga1 ) ;
      HanWrite() ;
      MoveData( mcga1, mcga0, 64000 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;
   }

   y0 = 200 ;
   for( i=0; i<200; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 80, y1, mcga2, mcga1 ) ;
      HanWrite() ;
      MoveData( mcga1, mcga0, 64000 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;
   }

   y1 = 200 ;
   j  = 0 ;
   ViewIMG( "bf-td2", mcga1 ) ;
   BufGetImage( 0, 0, 319, 49, mcga2, mcga1 ) ;
   ViewIMG( "bf-td3", mcga1 ) ;
   MoveData( mcga1, mcga3, 64000 ) ;
   BufXLine( 0, 0, 64000, 0, mcga1 ) ;
   for( i=0; i<250; i++ )
   {
      BufPutImage( 80, y0, mcga2+32000, mcga1 ) ;
      BufPutImage( 0,  y1, mcga2, mcga1 ) ;
      if( ( y1+50 > 0 ) && ( y1+50 < 201 ) )
      {
	 BufMoveImage( 0, y1+50, 319, y1+50, mcga3, mcga1 ) ;
	 MoveData( mcga3+32000-(320*(j>>1)), mcga1+64000-(320*j), 320*j ) ;
	 j++ ;
      }
      MoveData( mcga1, mcga0, 64000 ) ;
      waitmSec( 10 ) ;
      y0-- ;
      y1-- ;
      if( kbhit() ) return ;
   }
   black_palette( 1 ) ;

   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
   BufHprint( 100, 80, "1995 º—‰AË±...", 254, mcga0 ) ;
   for( i=0; i<64; i++ ) SetPalette( 254, i, i, i ) ;
   waitmSec( 2000 ) ;
   for( i=63; i>0; i-- ) SetPalette( 254, i, i, i ) ;
   BufXLine( 0, 0, 64000, 0, mcga0 ) ;
}

void step_demo( void )
{
   long d_t ;
   int door_count, door_loop, step_count, i, j ;

   h_char  = 7 ;
   h_count = 3 ;
   h_color = 0 ;
   h_delay = 0 ;

   ViewIMG( "bf-td4-0", mcga1 ) ;
   BufGetImage( 0,   0, 158, 160, mcga2, mcga1 ) ;
   memtoxms( mcga2, XMS_back, 0L,        32000L ) ;
   BufGetImage( 161, 0, 319, 160, mcga2, mcga1 ) ;
   memtoxms( mcga2, XMS_back, 1L*32000L, 32000L ) ;

   ViewIMG( "bf-td4-1", mcga1 ) ;
   BufGetImage( 0,   0, 158, 160, mcga2, mcga1 ) ;
   memtoxms( mcga2, XMS_back, 2L*32000L, 32000L ) ;
   BufGetImage( 161, 0, 319, 160, mcga2, mcga1 ) ;
   memtoxms( mcga2, XMS_back, 3L*32000L, 32000L ) ;

   ViewIMG( "bf-td4-2", mcga1 ) ;
   BufGetImage( 0,   0, 158, 160, mcga2, mcga1 ) ;
   memtoxms( mcga2, XMS_back, 4L*32000L, 32000L ) ;
   BufGetImage( 161, 0, 319, 160, mcga2, mcga1 ) ;
   memtoxms( mcga2, XMS_back, 5L*32000L, 32000L ) ;

   ViewIMG( "bf-td5-0", mcga1 ) ;
   memtoxms( mcga1, XMS_image, 0L, 64000L ) ;
   ViewIMG( "bf-td5-1", mcga1 ) ;
   memtoxms( mcga1, XMS_image, 64000L, 64000L ) ;

   ViewIMG( "bf-td6", mcga2 ) ;

   for( i=0; i<9; i++ ) BufGetImage( (i*31)+1, 1, (i*31)+1+29, 1+119, mcga3+(i*6000), mcga2 ) ;

   SetRgbPaletteSnow( (unsigned char *)&Pal ) ;

   j = 0 ;
   step_count = 0 ;
   door_count = 1 ;
   door_loop  = 0 ;

   while( !kbhit() )
   {
      d_t = GetTime() ;
      xmstomem( XMS_image, j*64000L, mcga1, 64000L ) ;
      xmstomem( XMS_back, step_count*32000L, mcga2, 32000L ) ;
      BufSpritePutImage( 0, 0, mcga2, mcga1 ) ;
      for( i=0; i<10; i++ ) HanWrite() ;
      if( (door_count>0) && (door_count<8) )
      {
	 BufPutImage( 210, 25, mcga3+((door_count-1)*6000), mcga1 ) ;
	 if( door_loop == 1 )
	 {
	    door_count++ ;
	    door_loop = 0 ;
	 } else door_loop++ ;
      }
      if( door_count > 7 )
      {
	 for( i=0; i<door_count; i++ )
	 {
	    BufPutImage( 210-i-10, 25, mcga3+(7*6000), mcga1 ) ;
	    BufPutImage( 210+i+10, 25, mcga3+(8*6000), mcga1 ) ;
	 }
	 if( door_loop == 1 )
	 {
	    door_count++ ;
	    door_loop = 0 ;
	 } else door_loop++ ;
      }
      if( door_count > 60 ) break ;
      MoveData( mcga1, mcga0, 64000 ) ;
      while( d_t+20L > GetTime() ) ;
      if( step_count == 5 ) step_count = 0 ; else step_count++ ;
      if( j == 0 ) j = 1 ; else j = 0 ;
//      if( (h_char == 12) && !door_count ) door_count = 1 ;
   }
}

int speed ;

void main( int argc, char *argv[] )
{
   if( argc == 2 )
   {
      init_xms() ;                           // Initialize XMS Memory
      SetVga() ;                             // VGA 13H Mode Seting
      main_data_init() ;                     // Main Datas Initialze
      BfConfig( 0, 0 ) ;                     // Config Data Read And Set
      key_clear() ;                          // Key Buffer Clear
      if( !strcmpi( argv[1], "START" ) )
      {
         AdLib_Start( "bf-td" ) ;
         SetRgbPaletteSnow( (unsigned char *)&Pal ) ;
         SetPalette( 254, 0, 0, 0 ) ;
         if( !kbhit() ) count_demo() ;
         if( !kbhit() ) step_demo() ;
         demo_palette( 0 ) ;
         ViewIMG( "bf-td7", mcga0 ) ;
         demo_palette( 1 ) ;
         getch() ;
         AdLib_End() ;
      }
      main_data_close() ;
   }
   execl( "bf.exe", "bf.exe", "START", "START", NULL) ;
}
