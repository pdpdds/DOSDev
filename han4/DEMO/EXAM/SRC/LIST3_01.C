#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);
   puts_han("¢…¸w·i ·³bÐa­A¶a");
   gets_han(string);
   puts_han(string);
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a");
   getch();
   close_han();
}
