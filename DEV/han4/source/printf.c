/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          printf_han()                                        */
/*   FILENAME           printf.c                                            */
/*   DATE & TIME        03/11/92(WED) 18:30                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int printf_han(char *format,...)
/* �e�� ���a�A �ᯢ���� �a�a�i �b�e�a. */
{
   char string[255+1];
   int cnt;

   cnt = vsprintf(string, format,...);
   cputs_han(string);

   return(cnt);
}
