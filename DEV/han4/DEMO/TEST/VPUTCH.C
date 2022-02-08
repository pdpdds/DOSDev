/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vputch_han(), vaputch_han(), vbputch_han()          */
/*   FILENAME           vputch.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vputch_han(int x, int y, char *string, int color)
/* (x, y) �A color �� ��w���a�� �e ���� ���a�i �b�e�a. */
{
   if(string[0] & 0x80) /* �b�i ���a�a �e�i���e */
      vput_han_font(x, y, string[0], string[1], color);
   else                 /* �b�i ���a�a �w�����e */
      vput_eng_font(x, y, string[0], color);
}

void vaputch_han(int x, int y, char *string, int color, int attr)
/* (x, y) �A color �� ��w���� attr �� �����a�� �e ���� ���a�i �b�e�a. */
{
   int attr2;

   attr2    = attr_han;
   attr_han = attr;
   if(string[0] & 0x80) /* �b�i ���a�a �e�i���e */
      vput_han_font(x, y, string[0], string[1], color);
   else                 /* �b�i ���a�a �w�����e */
      vput_eng_font(x, y, string[0], color);
   attr_han = attr2;
}

void vbputch_han(int x, int y, char *string, int color, int bkcolor)
/* (x, y) �A color �� ��w���� bkcolor �� ���w���a�� �e ���� ���a�i �b�e�a. */
{
   int bkcolor2;

   bkcolor2    = bkcolor_han;
   bkcolor_han = bkcolor;
   if(string[0] & 0x80) /* �b�i ���a�a �e�i���e */
      vput_han_font(x, y, string[0], string[1], color);
   else                 /* �b�i ���a�a �w�����e */
      vput_eng_font(x, y, string[0], color);
   bkcolor_han = bkcolor2;
}
