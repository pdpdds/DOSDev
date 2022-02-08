/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          aclrscr_han()                                       */
/*   FILENAME           aclrscr.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/



#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void aclrscr_han(int attr)
/* ÑÁ¡e ¸åÁAŸi attr · ­¢¬÷·a¡ »¡¶…”a. */
{
   aclear_text_han(minx_han, miny_han, maxx_han, maxy_han, attr);
   curx_han = minx_han;
   cury_han = miny_han;
}
