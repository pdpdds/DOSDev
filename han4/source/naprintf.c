/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          naprintf_han()                                      */
/*   FILENAME           naprintf.c                                          */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void naprintf_han(int x, int y, int length, int attr, char *format,...)
/* (x, y) µA attr · ­¢¬÷·a¡ ¬á¯¢”¡ ¸ažaŸi length ¸a e Â‰bÐe”a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   naputs_han(x, y, string, length, attr);
}
