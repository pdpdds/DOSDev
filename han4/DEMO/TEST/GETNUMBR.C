/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_number_han()                                    */
/*   FILENAME           getnumbr.c                                          */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include <ctype.h>
#include <time.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int get_number_han(int x, int y, char *number, int limit, int attr, char *user_exit_key)
/* (x, y) µA¬á ®Ÿi ·³b¤h“e”a. */
{
   int index, key, is_first, attr2, i;
   clock_t time1, time2;

   /* ¹ÁÎaˆa ¸i¡µ »¡¸÷–E ‰w¶ error_exit_han() Ñ¡Â‰ */
   if(x < 0 || x > abs_maxx_han || y < 0 || y > abs_maxy_han)
   {
      error_exit_msg = "out of range error";
      error_exit_han("get_number_han", 1);
   }

   /* limit · ˆt·¡ ¸i¡µ »¡¸÷–E ‰w¶ error_exit_han() Ñ¡Â‰ */
   if(limit <= 0)
   {
      error_exit_msg = "Illegal <limit>";
      error_exit_han("get_number_han", 1);
   }

   attr2    = attr_han;
   attr_han = attr;
   number[limit] = 0;

   is_first   = 1;
   index      = strlen(number);
   cursor_han = eng_cursor;
   while(1)
   {
      nputs_han(x, y, number, limit);

      /* Äá¬áŸi Œq¤b·¥”a. */
      draw_cursor_han(x + index, y);
      time1 = clock();
      while(!kbhit())
      if(((time2 = clock()) - time1) > cursor_twinkle_time)
      {
	 draw_cursor_han(x + index,y);
	 if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
         time1 = time2;
      }
      /*  a»¡ bµA“e Äá¬áˆa »¡¶¡»¡•¡¢ Ðe”a. */
      if(exist_cursor) draw_cursor_han(x + index,y);

      key = getch();
      if(isdigit(key) || key == '+' || key == '-' || key == '.')
      /* ®•¸a, +, -, .  e ·³b¤h“e”a. */
      {
         if((key == '+' || key == '-') && index != 0)
         /* +, - “e ¢…¸aµi· ¬å–µA e µ© ® ·¶”a. */
            continue;
         else if(key == '.')
         /* . “e ¢…¸aµiµA ”e Ðaa e ·³b–I ® ·¶”a. */
         {
	    for(i = 0; i < index; i++) if(number[i] == '.') break;
	    if(i < index) continue;
         }

         /* Àá·q¦Èá ®•¸a, +, -, . Ÿi ·³bÐa¡e number µA £¡Ÿ¡ */
         /* ·³b–E ¢…¸a—i·e »¡¶‰¡ ¬¡ ·³b¤h“e”a.           */
	 if(is_first)
         {
            index     = 0;
            is_first  = 0;
            number[0] = 0;
	 }

         number[index++] = key;
         number[index]   = 0;
         if(index >= limit)
         /* limit ˆ· ®•¸aŸi ·³b¤h·a¡e žÏaŸi ÈiÂ‰(key “e Ÿ¡Èå */
         /* ˆt·a¡ ¬a¶w–E”a)                                     */
         {
            key = 0;
            break;
         }
      }
      else if(key == _ESC)              /* <Esc> Ç¡ˆa ’‰œá»¡¡e */
      {
         key = ESC;
	 break;
      }
      else if(key == _ENTER)            /* <Enter> Ç¡ˆa ’‰œá»¡¡e */
      {
         key = ENTER;
	 break;
      }
      else if(key == _BS && index > 0)  /* <BS> Ç¡ˆa ’‰œá»¡¡e */
      {
	 number[index--]= 0;
      }
      else if(key == 0)                 /* ÑÂ¸wÇ¡ˆa ’‰œá»¡¡e */
      {
         /* ’‰œá»¥ Ç¡ˆa ¬a¶w¸a ¸÷· ÈiÂ‰Ç¡·¡¡e žÏaŸi ÈiÂ‰ */
	 key = getch();
         for(i = 0; i < strlen(user_exit_key); i++)
            if(key == user_exit_key[i]) break;
         if(i < strlen(user_exit_key)) break;
      }
   }

   nputs_han(x, y, number, limit);
   attr_han = attr2;
   return(key);
}
