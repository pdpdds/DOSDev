#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   /* set_max_strlen_han(40)·i Ñ¡Â‰Ða»¡ ´g‰¡ string· Ça‹¡Ÿi 41¡    */
   /* ¸s·a¡e ¯¡¯aÉQ·¡ ¸÷»¡Ði ®•¡ ·¶”a. µá ‹aœå»¡ ·¡·AŸi ¡¡Ÿa¡e gets */
   /* _han·i ·ª´á ¥¡œa.                                              */
   char string[40 + 1];
   int ret_code;

   if(init_han() != OK_HAN) exit(1);

   /* ·³b ˆa“wÐe ¢…¸a· ®Ÿi 40 ¸a¡ º‰·¥”a. */
   set_max_strlen_han(40);
   printf_han("·³b ˆa“wÐe ¢…¸a· ®“e %dˆ·³“¡”a.\n", get_max_strlen_han());
   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a.(º—»¡ = <Esc>)\n");
   puts_han("+---------+---------+---------+---------+");
   puts_han("12345678901234567890123456789012345678901");

   while(1)
   {
      ret_code = gets_han(string);
      if(ret_code == ESC) break;
      puts_han(string);
   }

   close_han();
}
