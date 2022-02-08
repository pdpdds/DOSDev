/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          nprintf_han()                                       */
/*   FILENAME           nprintf.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void nprintf_han(int x, int y, int length, char *format,...)
/* (x, y) �A �ᯢ���� �a�a�i length �a�e �b�e�a. */
{
   char string[LIMITX_HAN+1];

   vsprintf(string, format,...);
   nputs_han(x, y, string, length);
}
