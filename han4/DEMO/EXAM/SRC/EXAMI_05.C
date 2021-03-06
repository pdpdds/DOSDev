#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int color;

   if(init_han() != OK_HAN) exit(1);

   if(!is_vga_mode())
   /* HGC�i �a�w�a�� ���a�e ��З�i ���e�e�a. */
   {
      puts_han("EGA/VGA �A��e ��З �a�w�e �a���a�������a.");
      puts_han("�a�� ǡ�a ���a�A�a..");
      if(!getch()) getch();
      close_han();
      exit(1);
   }

   for(color = 0; color < 16; color++)
      vputs_han(0, color, "�a���e ���� �e�i�����a.", color);

   xputs_han(0, 17, "�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
