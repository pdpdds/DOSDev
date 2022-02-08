/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          draw_box_han()                                      */
/*   FILENAME           drawbox.c                                           */
/*   DATE & TIME        03/11/92(WED) 19:02                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

/* �a�w�a ���� �b�a�� �����i �a��ǡ�e ͡���� */
char *user_defined_box_shape;

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_box_han(int left, int top, int right, int bottom, int border_num)
/* �b�a�i �a���a. */
{
   static const unsigned char border_char[4][6] =       /* �A���� ���a */
   { { 218, 196, 191, 179, 192, 217 },
     { 214, 196, 183, 186, 211, 189 },
     { 213, 205, 184, 179, 212, 190 },
     { 201, 205, 187, 186, 200, 188 }
   };
   char border[6];
   int  column, row;

   /* border_num �A �a�a �A���� ���a�� �a�aǡ š�a�i border �A ���a */
   if(border_num == BOX_USER_DEFINED)
      for(column = 0; column <= 5; column++) border[column] = user_defined_box_shape[column];
   else
      for(column = 0; column <= 5; column++) border[column] = border_char[border_num][column];

   /* ����i �a�ᴡ �i ���A ���e �១�i �e�a. */
   if(left == right)
      border[0] = border[1] = border[2] = border[4] = border[5] = border[3];
   if(top == bottom)
      border[0] = border[2] = border[3] = border[4] = border[5] = border[1];

   /* �A���� ���a�i �b�a�a �b�a�i �a���a. */
   put_eng_font( left,    top, border[0]);
   put_eng_font(right,    top, border[2]);
   put_eng_font( left, bottom, border[4]);
   put_eng_font(right, bottom, border[5]);
   for(column = left + 1; column < right; column++)
   {
      put_eng_font(column,    top, border[1]);
      put_eng_font(column, bottom, border[1]);
   }
   for(row = top + 1; row < bottom; row++)
   {
      put_eng_font( left, row, border[3]);
      put_eng_font(right, row, border[3]);
   }
}
