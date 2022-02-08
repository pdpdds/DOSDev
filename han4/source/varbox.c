/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.0                            */
/*   SUB-TITLE          draw_var_box_han()                                  */
/*   FILENAME           varbox.c                                            */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <dos.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_var_box_han(int left, int top, int right, int bottom,
		      int border_num, int attr, int delay_time)
/* Ça‹¡ˆa ¸ñ¸ñ Äá»¡“e ¤b¯aŸi ‹aŸ¥”a. */
{
   int attr2, xx1, xx2, yy1, yy2, xrepeat, yrepeat, i;
   char *screen;

   attr2    = attr_han;
   attr_han = attr;

   /* ¤b¯a· ÂAÁ¡ ¹ÁÎaµÁ ¡y¤å  eµA ¤b¯aŸi µÅ¬÷Ði »¡Ÿi ‰¬eÐe”a. */
   xx1     = (right - left + 1) / 2 + left - 1;
   xx2     = (right - left + 1) / 2 + 1 + (right - left + 1) % 2 + left - 1;
   yy1     = (bottom - top + 1) / 2 + top - 1;
   yy2     = (bottom - top + 1) / 2 + 1 + (bottom - top + 1) % 2 + top - 1;
   xrepeat = xx1 - left + 1;
   yrepeat = yy1 - top + 1;

   /* ˆa¡¡ ¤b¯aŸi ¸ñ¸ñ Äá»¡‰A Ðe”a. */
   for(i = 0; i < xrepeat; i++)
   {
      clear_text_han(xx1, yy1, xx2, yy2);
      draw_box_han(  xx1, yy1, xx2, yy2, border_num);
      xx1--;
      xx2++;
      delay(delay_time);
   }
   xx1++;
   xx2--;
   /* ­A¡¡ ¤b¯aŸi ¸ñ¸ñ Äá»¡‰A Ðe”a. */
   for(i = 0; i < yrepeat; i++)
   {
      clear_text_han(xx1, yy1, xx2, yy2);
      draw_box_han(  xx1, yy1, xx2, yy2, border_num);
      yy1--;
      yy2++;
      delay(delay_time);
   }

   attr_han = attr2;
}
