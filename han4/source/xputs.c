/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          xputs_han()                                         */
/*   FILENAME           xputs.c                                             */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void xputs_han(int x, int y, char *string)
/* (x, y) �A ���a�i�i �b�e�a. */
{
   int index = 0, pos = 0;

   while(string[index])
   {
      if(string[index] & 0x80) /* �b�i ���a�a �e�i���e */
      {
	 put_han_font(x + pos, y, string[index], string[index+1]);
         if(attr_han & FONT_HORIZ_DBL) pos += 4; else { pos++; pos++; }
         index++; index++;
      }
      else                     /* �b�i ���a�a �w�����e */
      {
	 put_eng_font(x + pos, y, string[index]);
	 if(attr_han & FONT_HORIZ_DBL) pos++;
	 pos++;
	 index++;
      }
   }
}
