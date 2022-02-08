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
/* (x, y) �A �a�aǡ���a�e �a�w�a�a ���a�i�i �b�e�a. */
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
/* (x, y) �A �a�aǡ���a�e �a�w�a�a ���a�i�i length �a�e �b�e�a. */
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
