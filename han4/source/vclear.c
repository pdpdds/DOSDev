/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vclear_text_han()                                   */
/*   FILENAME           vclear.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vclear_text_han(int left, int top, int right, int bottom, int color)
/* ÑÁ¡e· ·©¦¦…·i color µA »¡¸÷–E ¬‚·a¡ »¡¶…”a. */
{
   int bkcolor2, x, y;

   bkcolor2    = bkcolor_han;
   bkcolor_han = color;
   for(y = top; y <= bottom; y++)
      for(x = left; x <= right; x++) put_eng_font(x, y, ' ');
   bkcolor_han = bkcolor2;
}
