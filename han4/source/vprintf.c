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
/* (x, y) �A color �� �����a�� �ᯢ���� �a�a�i �b�e�a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   vputs_han(x, y, string, color);
}
