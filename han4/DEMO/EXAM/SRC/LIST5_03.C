#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char code1, code2;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   puts_han("�{�i ���� ��Ё��e <ESC> ǡ�i ���a�A�a.\n\n\n");

   do
   {
      puts_han("�a�� ǡ�a ���a�A�a.");

      code1 = getch();
      if(code1 == _ESC) break;  /* _ESC�e <ESC> ǡ�� �a�aǡ š�a�t       */
                                /* <ESC> ǡ�i ���a�e ���a�i �a�a �a�e�a. */

      if(code1 == 0)    /* �¸w ǡ�i �����a�e */
         printf_han("���q ���e ǡ�e �¸w ǡ���� ");
      else
         printf_han("���q ���e ǡ�e ���e ǡ���� ");

      printf_han("š�a�e '0x%02X'", code1);
      if(code1 == 0)    /* �¸w ǡ�i �����a�e �e �� �� ǡ�i ���e�a. */
      {
         code2 = getch();
         printf_han(" + '0x%02X'", code2);
      }
      printf_han("�����a.\n\n");
   }
   while(1);

   close_han();
}
