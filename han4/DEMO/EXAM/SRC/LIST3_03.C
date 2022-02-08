#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);
   set_han_change_key(F2, 1, 0);

   circle(200, 200, 100);
   rectangle(200, 200, 400, 300);

   puts_han("¢…¸w·i ·³bÐa­A¶a");
   gets_han(string);
   puts_han(string);
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a");
   getch();
   close_han();
}
