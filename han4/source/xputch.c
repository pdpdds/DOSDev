/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          xputch_han()                                        */
/*   FILENAME           xputch.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void xputch_han(int x, int y, char *string)
/* (x, y) µA Ðe ˆ· ¢…¸aŸi Â‰bÐe”a. */
{
   if(string[0] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      put_han_font(x, y, string[0], string[1]);
   else                 /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
      put_eng_font(x, y, string[0]);
}
