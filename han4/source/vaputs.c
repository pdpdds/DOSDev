/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vaputs_han()                                        */
/*   FILENAME           vaputs.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vaputs_han(int x, int y, char *string, int color, int attr)
/* (x, y) µA color · ¸å‰w¬‚‰Á attr · ­¢¬÷·a¡ ¢…¸aµi·i Â‰bÐe”a. */
{
   int index = 0, pos = 0;
   int attr2;

   attr2    = attr_han;
   attr_han = attr;
   while(string[index])
   {
      if(string[index] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      {
         vput_han_font(x + pos, y, string[index], string[index+1], color);
         if(attr & FONT_HORIZ_DBL) pos += 4; else { pos++; pos++; }
	 index++; index++;
      }
      else                     /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
      {
         vput_eng_font(x + pos, y, string[index], color);
         if(attr & FONT_HORIZ_DBL) pos++;
	 pos++;
         index++;
      }
   }
   attr_han = attr2;
}
