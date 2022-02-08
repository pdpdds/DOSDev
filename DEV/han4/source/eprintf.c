/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          eprintf_han(), enprintf_han()                       */
/*   FILENAME           eprintf.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void eprintf_han(int x, int y, char *format,...)
/* (x, y) µA ¬á¯¢”¡ ¸ažaŸi ´a¯aÇ¡¢…¸a e ¬a¶wÐaµa Â‰bÐe”a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   eputs_han(x, y, string);
}

void enprintf_han(int x, int y, int length, char *format,...)
/* (x, y) µA ¬á¯¢”¡ ¸ažaŸi ´a¯aÇ¡¢…¸a e ¬a¶wÐaµa length ¸a e */
/* Â‰bÐe”a.                                                   */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   enputs_han(x, y, string, length);
}
