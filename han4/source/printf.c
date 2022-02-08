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
/* Ñe¸ ¹ÁÎaµA ¬á¯¢”¡ ¸ažaŸi Â‰bÐe”a. */
{
   char string[255+1];
   int cnt;

   cnt = vsprintf(string, format,...);
   cputs_han(string);

   return(cnt);
}
