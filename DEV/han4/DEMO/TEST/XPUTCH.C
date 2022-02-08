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
/* (x, y) �A �e ���� ���a�i �b�e�a. */
{
   if(string[0] & 0x80) /* �b�i ���a�a �e�i���e */
      put_han_font(x, y, string[0], string[1]);
   else                 /* �b�i ���a�a �w�����e */
      put_eng_font(x, y, string[0]);
}
