/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vprintf_han() function                              */
/*   FILENAME           vprintf.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vprintf_han(int x, int y, int color, char *format,...)
/* (x, y) µA color · ­¢¬÷·a¡ ¬á¯¢”¡ ¸ažaŸi Â‰bÐe”a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   vputs_han(x, y, string, color);
}
