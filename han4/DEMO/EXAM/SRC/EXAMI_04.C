#include <stdlib.h>
#include "hanio.h"
#include "misc.h"

void main(void)
{
   char string[3+1];

   if(init_han() != OK_HAN) exit(1);

   /* �a���i ���b�h�a�a�� �A �a�� ���w ���b�h�i ϩ�a�a ���a. */
   set_max_strlen_han(3);
   while(1)
   {
      cputs_han("�w���� �a���e? --- ");
      gets_han(string);
      if(is_number(string)) break;
      puts_han("\7���a�e ���b�a�A�a..");
   }

   /* ���e ���a�i �a�a �a���e string�e ���a�a�e ���i �¯��i */
   /* �� ���� �����A �e���a�� atoi() �q���i ѡ�i ��  ���a. */
   printf_han("�w���� �a���e %d �A�����a.\n\n", atoi(string));
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
