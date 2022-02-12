
#include <dos.h>

unsigned char mcolor[16]    ;
unsigned char scolor[16][3] ;
char time_int_push = 0 ;
extern unsigned char Pal[256][3] ;

static void interrupt (*old_isr08)() ;

static void interrupt new_isr08(void)
{
   if( !time_int_push )
   {
//      unsigned char i ;

/*      for( i=1; i<7; i++ )
      {
	 mcolor[i]++     ;
	 if( mcolor[i]   == 7 ) mcolor[i] = 1 ;
	 memcpy( Pal[i+239], scolor[mcolor[i]], 3 ) ;
      }
      for( i=1; i<9; i++ )
      {
	 mcolor[i+6]++  ;
	 if( mcolor[i+6] == 9 ) mcolor[i+6] = 1 ;
	 memcpy( Pal[i+245], scolor[mcolor[i+6]+6], 3 ) ;

      } */
      if( !mcolor[15] )
      {
	 mcolor[15] = 1 ;
	 SetPalette( 254, 63, 0, 0 ) ;
/*	 Pal[254][0] = 63 ;
	 Pal[254][1] = 0 ;
	 Pal[254][2] = 0 ; */
      }
      else
      {
	 mcolor[15] = 0 ;
	 SetPalette( 254, 63, 63, 63 ) ;
/*	 Pal[254][0] = 63 ;
	 Pal[254][1] = 63 ;
	 Pal[254][2] = 63 ; */
      }
//      SetRgbPalette( (unsigned char *)&Pal, 254, 1 ) ;
   }
   (*old_isr08)() ;
}

void SetInt08(void)
{
//   old_isr08 = getvect( 0x8 ) ;
//   setvect( 0x8, new_isr08 ) ;
}

void EndInt08()
{
//   setvect( 0x08, old_isr08 ) ;
}

void AniColor( void )
{
//   int i ;

//   for( i=1; i<7 ; i++ ) mcolor[i]   = i ;  /* Move Color Rgb Init */
/*   for( i=1; i<9 ; i++ ) mcolor[i+6] = i ;
   for( i=1; i<15; i++ )
   {
      scolor[i][0] = Pal[i+239][0] ;
      scolor[i][1] = Pal[i+239][1] ;
      scolor[i][2] = Pal[i+239][2] ;
   }   */
}