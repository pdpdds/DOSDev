/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          nvprintf_han()                                      */
/*   FILENAME           nvprintf.c                                          */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void nvprintf_han(int x, int y, int length, int color, char *format,...)
/* (x, y) µA color · ­¢¬÷·a¡ ¬á¯¢”¡ ¸ažaŸi length ¸a e Â‰bÐe”a. */
{

   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   nvputs_han(x, y, string, length, color);
}
