#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   puts_han("¢…¸aµi·i ·³bÐa­A¶a. Äá¬á“e aÈaa»¡ ´g¯s“¡”a.");
   hide_cursor_han();
   gets_han(string);

   puts_han("¢…¸aµi·i ·³bÐa­A¶a. Äá¬áˆa aÈas“¡”a.");
   show_cursor_han();
   gets_han(string);

   close_han();
}
