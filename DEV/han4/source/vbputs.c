/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vbputs_han()                                        */
/*   FILENAME           vbputs.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vbputs_han(int x, int y, char *string, int color, int bkcolor)
/* (x, y) �A color �� ��w���� bkcolor �� ���w���a�� ���a�i�i �b�e�a. */
{
   int index = 0, pos = 0;
   int bkcolor2;

   bkcolor2    = bkcolor_han;
   bkcolor_han = bkcolor;
   while(string[index])
   {
      if(string[index] & 0x80) /* �b�i ���a�a �e�i���e */
      {
	 vput_han_font(x + pos, y, string[index], string[index+1], color);
         if(attr_han & FONT_HORIZ_DBL) pos += 4; else { pos++; pos++; }
         index++; index++;
      }
      else                     /* �b�i ���a�a �w�����e */
      {
         vput_eng_font(x + pos, y, string[index], color);
         if(attr_han & FONT_HORIZ_DBL) pos++;
	 pos++;
         index++;
      }
   }
   bkcolor_han = bkcolor2;
}
