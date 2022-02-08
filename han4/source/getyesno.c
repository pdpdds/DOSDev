/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_yesno_han()                                     */
/*   FILENAME           getyesno.c                                          */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include <time.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int get_yesno_han(int x, int y, int attr)
/* <Y>, <N> Ç¡Ÿi ·³b¤h´a ˆbˆb YES, NO Ÿi Ÿ¡ÈåÐe”a. */
{
   int key, attr2;
   clock_t time1, time2;

   /* ¹ÁÎaˆa ¸i¡µ »¡¸÷–E ‰w¶ error_exit_han() Ñ¡Â‰ */
   if(x < 0 || x > abs_maxx_han || y < 0 || y > abs_maxy_han)
   {
      error_exit_msg = "out of range error";
      error_exit_han("get_yesno_han", 1);
   }

   attr2    = attr_han;
   attr_han = attr;
   put_eng_font(x, y, ' ');

   cursor_han = eng_cursor;
   while(1)
   {
      /* Äá¬áŸi Œq¤b·¥”a. */
      draw_cursor_han(x, y);
      time1 = clock();
      while(!kbhit())
      if(((time2 = clock()) - time1) > cursor_twinkle_time)
      {
	 draw_cursor_han(x, y);
	 if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
         time1 = time2;
      }
      /*  a»¡ bµA“e Äá¬áˆa »¡¶¡»¡•¡¢ Ðe”a. */
      if(exist_cursor) draw_cursor_han(x, y);

      if((key = getch()) == 0)                  /* ÑÂ¸wÇ¡“e ¢¯¡Ðe”a. */
	 getch();
      if(key == 'y' || key == 'Y')              /* <Y> Ç¡a ’‰œá»¡¡e */
      {
	 put_eng_font(x, y, key);
	 key = YES;
	 break;
      }
      else if(key == 'n' || key == 'N')         /* <N> Ç¡ˆa ’‰œá»¡¡e */
      {
	 put_eng_font(x, y, key);
         key = NO;
         break;
      }
      else if(key == _ESC)                      /* <Esc> Ç¡ˆa ’‰œá»¡¡e */
      {
         key = ESC;
         break;
      }
   }

   attr_han = attr2;
   return(key);
}
