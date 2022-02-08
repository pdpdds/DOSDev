#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   int  ret_code;

   if(init_han() != OK_HAN) exit(1);

   puts_han("¢…¸aµi·i ·³bÐa­A¶a.(º—»¡ = <Esc>).");
   while(1)
   {
      ret_code = gets_han(string);
      if(ret_code == ESC) break;
      puts_han(string);
   }

   close_han();
}
