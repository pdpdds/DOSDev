
#include "bf.h"

char ending_hmesg[15][128] = {
  "2005 ‘e 8¶© 15·© ‰×¥¢¸é...",
  "ÎŸe  ¦”“e 50 µa‹¡· ¸â—i‰Á ÐqA",
  "¸å¡iÐaµv”a. ",
  "‹a—i· ‰·¡µÁ Ó¬—·a¡ ÐeŠ‚·e",
  "ˆe¯¥Ó¡ Š‚É¡Ÿi »¡Ç¡“e•A ¬÷‰·Ðaµv”a.",
  "‹aœáa...",
  "ÐeŠ‚·e ÎŸe Ÿi ‹¡´âÐa»¡ ´g´v”a.",
  "q´a·¶“e ¸aža“e, ‹a—i· ·¡Ÿq‰Á...",
  "2005 ‘e 8¶© 15·© ¸å¡i...ª…...",
  " ",
  "­A‰“e ”a¯¡ Ñ¥œe· ¯¡”µA ¨a¹a—i‰¡",
  "´|·a¡· £¡œ“e ’Š•¡ µ‰eÐi ® ",
  "´ô·aŸ¡œa.. ",
  "e ´a»¢ ‹¡´âÐa‰¡ ·¶”a..",
  "‹aˆa º‚»¡ ´g´v”a“e ˆõ·i...",
} ;

char create_hmesg[13][128] = {
  "       - STAFF -",
  " ",
  "    - team º—/‰A/Ë±",
  "ˆq•¢, ¯¡aŸ¡µ¡ : ¥i¤aœq",
  "  Ïa¡‹aœ£· : ¹¡¸Š",
  "    ‹aœÏ¢ : ¥i¤aœq",
  "    ¬a¶…—a : ¹¡¸Š",
  "    ·q ´b : ¥i¤aœq",
  "  £A»¢ ¡¡—a : ´e¬÷ÒA",
  "   Ë± ¥¡¹¡ : ´e¬÷ÒA",
  "  1995 ‘e  º—/‰A/Ë±/",
  " ",
  "     - THE  END -",
} ;

void ending1( void )
{
   unsigned char iPal[256][3] ;
   long speed_loop ;
   int y_line, i ;

   AdLib_Start( "ending" ) ;              //*** AdLib Music Start
   ViewIMG( "ending3", mcga1 ) ;
   MoveData( mcga1, mcga2, 64000 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;                      //*** Set Slow Orignal Palette
   y_line = 210 ;
   while( y_line-- > -440 )
   {
      speed_loop = GetTime() ;
      MoveData( mcga2, mcga1, 64000 ) ;
      for( i=0; i<15; i++ )
      {
         BufHprint( 20, y_line+(i*30), ending_hmesg[i], 203, mcga1 ) ;
      }
      MoveData( mcga1, mcga0, 64000 ) ;
      while( GetTime() < speed_loop+15L ) ;        // Delay Speed
   }
   black_palette( 1 ) ;                   //*** Set Slow Black Palette

   ViewIMG( "ending4", mcga1 ) ;
   MoveData( mcga1, mcga2, 64000 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;
   white_palette() ;                      //*** Set Slow Orignal Palette
   y_line = 210 ;
   while( y_line-- > -390 )
   {
      speed_loop = GetTime() ;
      MoveData( mcga2, mcga1, 64000 ) ;
      for( i=0; i<13; i++ )
      {
         BufHprint( 70, y_line+(i*30), create_hmesg[i], 203, mcga1 ) ;
      }
      MoveData( mcga1, mcga0, 64000 ) ;
      while( GetTime() < speed_loop+15L ) ;        // Delay Speed
   }
   black_palette( 1 ) ;                   //*** Set Slow Black Palette
   AdLib_End() ;                          //*** AdLib Music Stop And End
}