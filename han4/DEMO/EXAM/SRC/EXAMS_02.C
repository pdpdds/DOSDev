#include "hanio.h"

void main(void)
{
   int year, month, day;

   if(init_han() != OK_HAN) exit(1);

   cputs_han("�i�a�i ���b�a�A�a(YY MM DD) -- ");
   scanf_han("%d %d %d", &year, &month, &day);
   printf_han("���i�e %d �e %d �� %d �������a.\n", year, month, day);

   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
