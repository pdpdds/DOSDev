#include "hanio.h"
#include "keydef.h"


void main(void)
{
   short data[11], i = 0, k = 14, count = 0, temp, x = 11, j;

   if(init_han() != OK_HAN) exit(1);
   draw_box_han( 1, 1, 78, 28, 0);     /* ÑÁ¡e Š¬÷ */
   vputs_han( 34, 2, "Bubble Sort", RED);
   draw_box_han( 2, 3, 77, 3, BOX_H1V1);

   set_window_han( 3, 4, 76, 22 );           /* ¶å•¡¶ ¬é¸÷ */

   set_color_han(BLUE); /* <------ ·¡¦¦…¬á¦Èá ¯aÇa©·i ¶¥Ð¶a..{Œa»¡ */
   gotoxy_han( 0, 0 );
   puts_han( "   ¸a, ¯¡¸bÐ ¥©Œa¶a..  " );

   getch_han();

   set_color_han(YELLOW);       /* ÑÁ¡e· ·³b¦¦… Š¬÷ */
   gotoxy_han( 0, 2 );
   puts_han( "   ·³b ˆ•® : 10ˆ       ¤ñ¶á : -999 <= data >= 999 " );
   gotoxy_han( 15, 3 );
   puts_han( "1    2    3    4    5    6    7    8    9   10" );
   gotoxy_han( 0, 4 );
   puts_han( "Sort Data :" );


   for( ; i < 10 ; i++ )
   {
      gotoxy_han( k, 4 );   /* ·³b ‰Á¸÷ */
      scanf_han("%d", &data[i] );
      if(( data[i] >999 ) || ( data[i] < -999 ))
         {
	    if( strlen(data[i]) == 0 ) /* <----·¡ ¦¦…•¡ ¸i´e•á¶a.. */
	      {                        /*     ESCa ENTERŸi ’Ÿi¯¡.. */
		xputs_han(1, 1, "  ·³b·¡ ´ô´ö·s“¡”a.."
			" ”a¯¡ ·³b Ða­A¶a..  ");
              }
              else
              {
		xputs_han(1,1, "  ·³bÇa‹¡ˆa 999¥¡”a Çaˆáa"
			" -999¥¡”a ¸b·i® ´ô·s“¡”a  ");
              }
            i--;
            clear_text_han( k, 8, 75, 8 );
         }
         else k += 5;
   }

   gotoxy_han( 0, 6 );         /* ·³b •A·¡Èa ÑÂ·¥‰Á¸÷ */
   puts_han( "Input Data :" );
   k = 14;

   for( i = 0 ; i < 10 ; i++ )
   {
      gotoxy_han( k, 6 );
      printf_han( "%d", data[i] );
      k += 5;
   }

   set_color_han(BLUE);
   gotoxy_han( 0, 8 );
   puts_han( "   x“eˆa¶a.. ´a¢Ç¡a ’Ÿa­A¶a..  " );

   getch_han();

   k = 10;
   set_color_han(BLACK);

      set_color_han(BLACK);
      gotoxy_han( 0, k );
      printf_han( "pass %2d :", 1 );

   for( i = 0 ; i < 100 ; i++ )    /* ­¡Ëa ¯¡Ð— ‰Á¸÷ */
   {
      for( j = 0 ; j < 9 ; j++ )
      {
         if( data[j] > data[j+1] )
         {
            temp = data[j];
            data[j] = data[j+1];
            data[j+1] = temp;
            count = 1;
         }
      }

      if( count == 0 ) break;

      count = 0;

      for( j = 0 ; j < 10 ; j++ )
      {
         gotoxy_han( x, k );
         printf_han( "%d", data[j] );
         x += 5;
      }

      x = 11;

      k += 2;
      set_color_han(BLUE);
      gotoxy_han( 0, k );
      printf_han( "  pass %d ‰Á¸÷·¡ {v·s“¡”a.. ´a¢Ç¡a ’Ÿa­A¶a..", i+1 );
      k += 2;

      getch_han();

      set_color_han(BLACK);
      gotoxy_han( 0, k );
      printf_han( "pass %2d :", i + 2 );
   }

   set_color_han(RED);
   gotoxy_han( 0, k );
   puts_han( "  ¹± ·¡Ðˆa ˆaa¶a.. Ù1¡¡Ÿa‰V·a¡e ”a¯¡Ðe¤å Ð ¥¡­A¶a " );

   init_window_han();
   getch();
}
