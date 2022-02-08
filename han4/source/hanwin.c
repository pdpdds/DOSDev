/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.1                       */
/*   SUB-TITLE          window functions                                    */
/*   FILENAME           hanwin.c                                            */
/*   DATE & TIME        03/11/92(WED) 19:01                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

void set_window_han(int left, int top, int right, int bottom)
/* sets window(not draws the border) */
{
   if((left < 0)   || (left > abs_maxx_han)  ||
      (top < 0)    || (top > abs_maxy_han)   ||
      (right < 0)  || (right > abs_maxx_han) ||
      (bottom < 0) || (bottom > abs_maxy_han)) return;

   minx_han = left;
   miny_han = top;
   maxx_han = right;
   maxy_han = bottom;
   curx_han = minx_han;
   cury_han = miny_han;
}

void init_window_han(void)
/* recovers window */
{
   maxx_han = abs_maxx_han;
   maxy_han = abs_maxy_han;
   minx_han = miny_han = curx_han = cury_han = 0;
}
