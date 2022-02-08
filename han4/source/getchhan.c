/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          getch_han()                                         */
/*   FILENAME           getchhan.c                                          */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int getch_han(void)
/* Ðe ˆ· Ç¡Ÿi ·³b¤h“e”a. */
{
   /* ·³b ”‹¡ ¯¡ˆe•·´e han_wait_func() Ÿi ‰­¢ Ñ¡Â‰ */
   while(!kbhit())
      if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
   /* ·³b¤h·e Ç¡· ´a¯aÇ¡Å¡—aŸi Ÿ¡Èå */
   return(getch());
}
