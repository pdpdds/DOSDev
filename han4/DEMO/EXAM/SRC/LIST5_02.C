#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char name[MAX_STRLEN+1];

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }
   set_han_change_key(F2, 1, 0);

   puts_han("�e�w�a�A�a? �w���� ���q�e ���������a?");
   gets_han(name);
   printf_han("\n�q�a�s���a, %s��. �a�� ǡ�a ���a�A�a.\n", name);
   getch();

   close_han();
}
