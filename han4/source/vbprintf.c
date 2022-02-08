/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vbprintf_han()                                      */
/*   FILENAME           vbprintf.c                                          */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vbprintf_han(int x, int y, int color, int bkcolor, char *format,...)
/* (x, y) µA color · ¸å‰w¬‚‰Á bkcolor · ¤‰w¬‚·a¡ ¬á¯¢”¡ */
/* ¸ažaŸi Â‰bÐe”a.                                           */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   vbputs_han(x, y, string, color, bkcolor);
}
