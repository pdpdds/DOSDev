/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          aclear_text_han()                                   */
/*   FILENAME           aclear.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void aclear_text_han(int left, int top, int right, int bottom, int attr)
/* ÑÁ¡e· ·©¦¦…·i attr · ­¢¬÷·a¡ »¡¶…”a. */
{
   int attr2, x, y;

   attr2    = attr_han;
   attr_han = attr;
   for(y = top; y <= bottom; y++)
      for(x = left; x <= right; x++) put_eng_font(x, y, ' ');
   attr_han = attr2;
}
