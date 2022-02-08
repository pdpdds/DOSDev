#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void my_wait_func(void);

void main(void)
{
   char string[MAX_STRLEN+1];
   int ret_code;

   if(init_han() != OK_HAN) exit(1);

   /* ¡A¯¡»¡ˆa Â‰b–A“e œa·¥·i Ã±¤ñÐa‹¡ ´g‹¡ ¶áÐ ¶å•¡¶Ÿi ¬é¸÷ */
   set_window_han(0, 0, abs_maxx_han, abs_maxy_han - 1);
   /* ·³b ”‹¡ ¯¡ˆe •·´e my_wait_func·¡ Ñ¡Â‰–A•¡¢ Ðe”a. */
   set_han_wait_func(my_wait_func);

   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a.");
   puts_han("{·i a¡e <Esc> Ç¡Ÿi ’Ÿa­A¶a..\n");
   while(1)
   {
      ret_code = gets_han(string);
      /* <Esc> Ç¡ˆa ’‰a¹v·a¡e žÏaŸi ÈiÂ‰Ðe”a. */
      if(ret_code == ESC) break;
   }

   close_han();
}

void my_wait_func(void)
/* ·³b ”‹¡ ¯¡ˆe •·´e Ñ¡Â‰–A“e Ðq® */
{
   aputs_han(0, abs_maxy_han, "¨iŸ¡ ·³bÐa­A¶a", FONT_NORMAL);
   delay(100);
   aputs_han(0, abs_maxy_han, "¨iŸ¡ ·³bÐa­A¶a", FONT_INVERSE);
   delay(100);
}
