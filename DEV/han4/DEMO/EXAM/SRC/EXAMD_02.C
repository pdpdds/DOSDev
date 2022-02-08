#include "hanio.h"

void main(void)
{
   char string[51];

   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   puts_han("»¡‹q·e ®¸÷ ¡¡—a·³“¡”a. ´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");

   /* ¤b¯aŸi ‹aŸ¡‰¡ string·i é ¢…¸aµi¡  e—e Ò adv_gets_han() Ðq®Ÿi Ñ¡Â‰ */
   draw_box_han(0, 18, 51, 20, BOX_H1V1);
   string[0] = 0;
   adv_gets_han(1, 19, string, 50, FONT_NORMAL, "");

   puts_han("\n®¸÷ ¡¡—a·¥»¡ ÑÂ·¥Ð–¯s“¡Œa? ·¡¹A ¬s·³ ¡¡—a¡ ¸åÑÅÐ ¥¡‰V¯s“¡”a."
            "default_ins_change_func() Ðq®“e ´a»¢ Ñ¡Â‰Ða»¡ ´g¯s“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   /* ¬s·³ ¡¡—a¡ ¸åÑÅÐe”a. */
   ins_mode = 1;

   puts_han("\n»¡‹q·e ¬s·³ ¡¡—a·³“¡”a. ´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");
   adv_gets_han(1, 19, string, 50, FONT_NORMAL, "");

   puts_han("¬s·³ ¡¡—a·¥»¡ ÑÂ·¥Ð–¯s“¡Œa? ´a»¢ ¬wÈ œa·¥µA“e ®¸÷ ¡¡—aœa‰¡ "
            "Îa¯¡–A´á ·¶¯s“¡”a. ·¡¹A default_ins_change_func() Ðq®Ÿi Ñ¡Â‰"
            "Ð ¥¡‰V¯s“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   /* default_ins_change_func() Ðq®Ÿi Ñ¡Â‰Ðaµa ¬wÈ œa·¥· ¬s·³/®¸÷ */
   /* Îa¯¡¦Ÿi ˆ—¯¥Ðe”a. default_han_change_func() Ðq®µÁ ”iŸ¡ ¥I­¡Ÿ¡ */
   /* “e »¡ ´g“e”a.                                                 */
   default_ins_change_func();

   puts_han("\n¬wÈ œa·¥· ¬s·³/®¸÷ Îa¯¡¦ˆa ¤aŸa‰A ˆ—¯¥–A´ö¯s“¡Œa?");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
