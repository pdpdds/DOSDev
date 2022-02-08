/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          nvputs_han()                                        */
/*   FILENAME           nvputs.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void nvputs_han(int x, int y, char *string, int length, int color)
/* (x, y) µA color · ­¢¬÷·a¡ ¢…¸aµi·i length ¸a e Â‰bÐe”a. */
{
   int index = 0, pos = 0;

   while(string[index] && index < length)
   {
      if(string[index] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      {
	 if(index + 2 > length) break;
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

   for( ; index < length; index++)
   {
      vput_eng_font(x + pos, y, ' ', color);
      if(attr_han & FONT_HORIZ_DBL) pos++;
      pos++;
   }
}
