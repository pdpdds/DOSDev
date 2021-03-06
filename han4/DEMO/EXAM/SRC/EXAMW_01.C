#include "hanio.h"
#include "extendio.h"

void draw_logo(void);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* ���� ���e�i �a�� ҁ 5�� ���e �a�w�a�a ǡ�i ���a���i ���a���a. */
   draw_logo();
   wait_key_han(5);

   close_han();
}

void draw_logo(void)
{
   int x;

   x = (get_abs_maxx_han() + 1) / 2 - 17;
   set_attr_han(FONT_INVERSE);

   nputs_han(x, 10, "", 32);
   aputs_han(x, 11, "   �e�i ���b �a���a�១ <�e>  ",
             FONT_SHADOW | FONT_INVERSE);
   aputs_han(x, 12, "            Ver  4.1            ",
             FONT_SHADE | FONT_INVERSE);
   nputs_han(x, 13, "", 32);
   aputs_han(x, 14, "   �e�e�� : �� �eѡ(easyride)   ",
             FONT_OUTLINE | FONT_INVERSE);
   xputs_han(x, 15, "   �e�e�i : 1992 �e 6 �� 1 ��   ");
   nputs_han(x, 16, "", 32);
}
