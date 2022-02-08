/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          aputch_han()                                        */
/*   FILENAME           aputch.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void aputch_han(int x, int y, char *string, int attr)
/* (x, y) �A attr �� �����a�� �e ���� ���a�i �b�e�a. */
{
   if(string[0] & 0x80) /* �b�i ���a�a �e�i���e */
      aput_han_font(x, y, string[0], string[1], attr);
   else                 /* �b�i ���a�a �w�����e */
      aput_eng_font(x, y, string[0], attr);
}
