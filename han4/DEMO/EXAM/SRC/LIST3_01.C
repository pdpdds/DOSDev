#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);
   puts_han("���w�i ���b�a�A�a");
   gets_han(string);
   puts_han(string);
   puts_han("�a�� ǡ�a ���a�A�a");
   getch();
   close_han();
}
