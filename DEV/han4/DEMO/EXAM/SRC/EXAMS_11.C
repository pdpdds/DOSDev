#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* ¶å•¡¶ µwµb·i Š¦…Ða“e ¤b¯aŸi ‹aŸ¥”a. */
   draw_box_han(9, 4, 61, 16, BOX_H1V1);
   /* ¶å•¡¶Ÿi ¬é¸÷Ðe”a. */
   set_window_han(10, 5, 60, 15);
   puts_han("{a¡e <Esc> Ç¡Ÿi ’Ÿa­A¶a..");
   do
   {
      puts_han("¢…¸w·i ·³bÐa­A¶a");
      if(gets_han(string) == ESC) break;
      /* <Esc> Ç¡Ÿi ’Ÿa¡e žÏaŸi ÈiÂ‰Ðe”a. */
      puts_han(string);
   }
   while(1);

   /* ¶å•¡¶Ÿi Ð¹AÐe Ò ÑÁ¡e·i »¡¶…”a. */
   init_window_han();
   clrscr_han();
   puts_han("¶å•¡¶ˆa Ð¹A–A´ö¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
