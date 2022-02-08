/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vaprintf_han()                                      */
/*   FILENAME           vaprintf.c                                          */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vaprintf_han(int x, int y, int color, int attr, char *format,...)
/* (x, y) µA color · ¸å‰w¬‚‰Á attr · ­¢¬÷·a¡ ¬á¯¢”¡ */
/* ¸ažaŸi Â‰bÐe”a. 	                                 */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   vaputs_han(x, y, string, color, attr);
}
