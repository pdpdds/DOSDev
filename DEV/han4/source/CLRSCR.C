/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          clrscr_han()                                        */
/*   FILENAME           clrscr.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void clrscr_han(void)
/* ÑÁ¡e ¸åÁAŸi »¡¶…”a. */
{
   clear_text_han(minx_han, miny_han, maxx_han, maxy_han);
   curx_han = minx_han;
   cury_han = miny_han;
}
