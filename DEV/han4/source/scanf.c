/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          scanf_han()                                         */
/*   FILENAME           scanf.c                                             */
/*   DATE & TIME        03/11/92(WED) 18:32                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int scanf_han(char *format,...)
/* ¬á¯¢”¡ ¸ažaŸi ·³b¤h“e”a. */
{
   char string[255+1];
   int  cnt;

   gets_han(string);
   cnt = vsscanf(string, format,...);

   return(cnt);
}
