#include <stdio.h>
#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   int number = 0;

   if(init_han() != OK_HAN) exit(1);

   gets_han(string);
   while(string[number] != '\0') number++;
   printf_han("���� %d���� �i�a�i ���b�h�v�s���a.\n", number);
   puts_han("\n�a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();

   close_han();
}


