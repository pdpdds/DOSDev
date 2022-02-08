#include <stdio.h>
#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   int number = 0;

   if(init_han() != OK_HAN) exit(1);

   gets_han(string);
   while(string[number] != '\0') number++;
   printf_han("¡¡– %dˆ· ‹i¸aŸi ·³b¤h´v¯s“¡”a.\n", number);
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();

   close_han();
}


