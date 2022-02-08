#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   status_on_han();
   set_han_change_key(F2, 1, 0);
   puts_han("<F2> Ç¡¡ Ðe/µw ¸åÑÅ·i Ð ¥¡­A¶a..");
   gets_han(string);

   close_han();
}
