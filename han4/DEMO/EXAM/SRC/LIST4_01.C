#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* ¶å•¡¶ µwµb·i Š¦…Ða“e ¤b¯aŸi ‹aŸ¥”a. */
   draw_box_han(9, 4, 70, 21, BOX_H1V1);
   /* ¶å•¡¶Ÿi ¬é¸÷Ðe”a. */
   set_window_han(10, 5, 69, 20);

   do
   {
      puts_han("¢…¸w·i ·³bÐa­A¶a");
      gets_han(string);
      puts_han(string);
   }
   while(strlen(string) != 0);     /* ‹a· <Enter> Ç¡Ÿi Ã© ˜Œa»¡ ¤e¥¢Ðe”a. */

   puts_han("´a¢ Ç¡a ’Ÿa­A¶a");
   getch();
   close_han();
}
