#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("�e�w�a�A�a? center_puts_han() �q���� ���񷳓��a..");

   /* ���e�� �A�� �a�e�A �e�� �b���i �a���a.          */
   /* get_abs_maxx_han + 1 = ���e�� ͢(�啡���� ����) */
   naputs_han(0, get_abs_maxy_han(), "", get_abs_maxx_han() + 1, FONT_INVERSE);

   /* �A�����i �e�� ���a�� �b�e�a. */
   center_puts_han(get_abs_maxy_han(), "�a�� ǡ�a ���a�A�a",
                   get_color_han(), FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
