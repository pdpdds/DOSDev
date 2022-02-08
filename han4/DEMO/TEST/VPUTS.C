/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vputs_han()                                         */
/*   FILENAME           vputs.c                                             */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vputs_han(int x, int y, char *string, int color)
/* (x, y) µA color · ­¢¬÷·a¡ ¢…¸aµi·i Â‰bÐe”a. */
{
   int index = 0, pos = 0;

   while(string[index])
   {
      if(string[index] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      {
	 vput_han_font(x + pos, y, string[index], string[index+1], color);
         if(attr_han & FONT_HORIZ_DBL) pos += 4; else { pos++; pos++; }
         index++; index++;
      }
      else                     /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
      {
         vput_eng_font(x + pos, y, string[index], color);
         if(attr_han & FONT_HORIZ_DBL) pos++;
         pos++;
         index++;
      }
   }
}
