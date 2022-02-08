/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          xprintf_han()                                       */
/*   FILENAME           xprintf.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void xprintf_han(int x, int y, char *format,...)
/* (x, y) µA ¬á¯¢”¡ ¸ažaŸi Â‰bÐe”a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   xputs_han(x, y, string);
}
