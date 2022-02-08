/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          rand_clear_text_han()                               */
/*   FILENAME           rndclear.c                                          */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <dos.h>
#include <stdlib.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void rand_clear_text_han(int left, int top, int right, int bottom,
			 int attr, unsigned count, unsigned delay_time)
/* ÑÁ¡e· ·©¦¦…·i œ…”ñÐa‰A »¡¶…”a. */
{
   int attr2, x, y, i;

   attr2    = attr_han;
   attr_han = attr;

   for(i = 0; i < count; i++)
   {
      x = random(right - left + 1) + left;
      y = random(bottom - top + 1) + top;
      put_eng_font(x, y, ' ');
      /* 10 ¸aŸi »¡¶‰¡ delay_time •·´e ”‹¡ */
      if(delay_time && !(i % 10)) delay(delay_time);
   }
   /*  a»¡ bµA“e ÑÁ¡e· ·©¦¦…·i ¡¡– »¡¶…”a. */
   clear_text_han(left, top, right, bottom);

   attr_han = attr2;
}
