/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          aputs_han()                                         */
/*   FILENAME           aputs.c                                             */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void aputs_han(int x, int y, char *string, int attr)
/* (x, y) �A attr �� �����a�� ���a�i�i �b�e�a. */
{
   int index = 0, pos = 0;

   while(string[index])
   {
      if(string[index] & 0x80) /* �b�i ���a�a �e�i���e */
      {
	 aput_han_font(x + pos, y, string[index], string[index+1], attr);
         if(attr & FONT_HORIZ_DBL) pos += 4; else { pos++; pos++; }
         index++; index++;
      }
      else                     /* �b�i ���a�a �w�����e */
      {
         aput_eng_font(x + pos, y, string[index], attr);
         if(attr & FONT_HORIZ_DBL) pos++;
         pos++;
         index++;
      }
   }
}
