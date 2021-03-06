
#include "bf.h"

char ending_hmesg[15][128] = {
  "2005 �e 8�� 15�� �ץ���...",
  "΁�e�� �����e 50 �a���� ��i�� �q�A",
  "��i�a�v�a. ",
  "�a�i�� ������ Ӂ���a�� �e���e",
  "�e��ӡ ��ɡ�i ��ǡ�e�A �����a�v�a.",
  "�a��a...",
  "�e���e ΁�e���i �����a�� �g�v�a.",
  "�q�a���e �a�a�e, �a�i�� ���q��...",
  "2005 �e 8�� 15�� ��i...��...",
  " ",
  "�A���e �a�� ѥ�e�� �����A �a�a�i��",
  "�|�a���� �����e ������ ���e�i �� ",
  "���a���a.. ",
  "�e �a�� �����a�� ���a..",
  "�a�a ���� �g�v�a�e ���i...",
} ;

char create_hmesg[13][128] = {
  "       - STAFF -",
  " ",
  "    - team ��/�A/˱",
  "�q��, ���a���� : �i�a�q",
  "  �a���a���� : ������",
  "    �a��Ϣ : �i�a�q",
  "    �a���a : ������",
  "    �q �b : �i�a�q",
  "  �A�� ���a : �e���A",
  "   ˱ ���� : �e���A",
  "  1995 �e  ��/�A/˱/",
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