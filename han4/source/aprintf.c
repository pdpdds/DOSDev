/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          aprintf_han()                                       */
/*   FILENAME           aprintf.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void aprintf_han(int x, int y, int attr, char *format,...)
/* (x, y) µA attr · ­¢¬÷·a¡ ¬á¯¢”¡ ¸ažaŸi Â‰bÐe”a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   aputs_han(x, y, string, attr);
}
