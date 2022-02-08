/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          draw_bit_map_box_han()                              */
/*   FILENAME           bitbox.c                                            */
/*   DATE & TIME        03/11/92(WED) 19:02                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_bit_map_box_han(int left, int top, int right, int bottom, char *bit_map)
/* ���a�� �b�a�i �a���a. */
{
   char *(border[6]);   /* �A���� ���a�� �����i �a��ǡ�e ͡���� */
   int column, row;

   /* border �a �A���� ���a�� ͥ�a�i �a��ǡ���� �e�a. */
   for(column = 0; column < 6; column++)
      border[column] = bit_map + column * ENG_FONT_SIZE;

   /* ����i �a�ᴡ �i ���A ���e �១�i �e�a. */
   if(left == right)
      border[0] = border[1] = border[2] = border[4] = border[5] = border[3];
   if(top == bottom)
      border[0] = border[2] = border[3] = border[4] = border[5] = border[1];

   /* �A���� ���a�� �����i �e���e�a. */
   if(attr_han != FONT_NORMAL)
      for(column = 0; column < 6; column++)
	 modify_image(border[column], border[column], ENG_FONT_SIZE, attr_han);

   /* �A���� ���a�i �b�a�a �b�a�i �a���a. */
   putimage_8( left,    top, border[0]);
   putimage_8(right,    top, border[2]);
   putimage_8( left, bottom, border[4]);
   putimage_8(right, bottom, border[5]);
   for(column = left + 1; column < right; column++)
   {
      putimage_8(column,    top, border[1]);
      putimage_8(column, bottom, border[1]);
   }
   for(row = top + 1; row < bottom; row++)
   {
      putimage_8( left, row, border[3]);
      putimage_8(right, row, border[3]);
   }
}
