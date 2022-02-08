/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          eputs_han(), enputs_han()                           */
/*   FILENAME           eputs.c                                             */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void eputs_han(int x, int y, char *string)
/* (x, y) µA ´a¯aÇ¡¢…¸a e ¬a¶wÐaµa ¢…¸aµi·i Â‰bÐe”a. */
{
   int index = 0, pos = 0;

   while(string[index])
   {
      put_eng_font(x + pos, y, string[index]);
      if(attr_han & FONT_HORIZ_DBL) pos++;
      pos++;
      index++;
   }
}

void enputs_han(int x, int y, char *string, int length)
/* (x, y) µA ´a¯aÇ¡¢…¸a e ¬a¶wÐaµa ¢…¸aµi·i length ¸a e Â‰bÐe”a. */
{
   int index = 0, pos = 0;

   while(string[index] && index < length)
   {
      put_eng_font(x + pos, y, string[index]);
      if(attr_han & FONT_HORIZ_DBL) pos++;
      pos++;
      index++;
   }

   for( ; index < length; index++)
   {
      put_eng_font(x + pos, y, ' ');
      if(attr_han & FONT_HORIZ_DBL) pos++;
      pos++;
   }
}
