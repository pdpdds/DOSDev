#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   int ret_code;

   if(init_han() != OK_HAN) exit(1);

   /* ¬wÈ œa·¥·i aÈaa‰A Ðe”a. */
   status_on_han();

   /* ­A¤é¯¢·a¡ ¤aŽ… Ò ¬wÈ œa·¥·i ”a¯¡ ‹aŸ¥”a. */
   set_keyboard_type(BUL3_TYPE);
   draw_status_han();

   if(get_keyboard_type() == BUL2_TYPE)
      puts_han("–¤é¯¢ ¸aÌe·i ¬a¶wÐa“e º—·³“¡”a.");
   else
      puts_han("­A¤é¯¢ ¸aÌe·i ¬a¶wÐa“e º—·³“¡”a.");

   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a.(º—»¡ = <Esc>)\n");

   while(1)
   {
      ret_code = gets_han(string);
      if(ret_code == ESC) break;
      puts_han(string);
   }

   close_han();
}
