/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vclrscr_han()                                       */
/*   FILENAME           vclrscr.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vclrscr_han(int color)
/* ÑÁ¡e ¸åÁAŸi color µA »¡¸÷–E ¬‚·a¡ »¡¶…”a. */
{
   vclear_text_han(minx_han, miny_han, maxx_han, maxy_han, color);
   curx_han = minx_han;
   cury_han = miny_han;
}
