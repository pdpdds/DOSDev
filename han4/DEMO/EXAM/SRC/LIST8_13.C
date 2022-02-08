#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   unsigned char exit_key[] = { F9, F10, 0 }, key;
   char string[41], hanja_code[3], hangul_code[3];
   int ret_value;

   if(init_han() != OK_HAN) exit(1);
   status_on_han();
   puts_han("Ðe¸aŸi ·³b¤h“e ¤w¤ó·i ¥¡µa º“e Ïa¡‹aœ‘·³“¡”a.");
   puts_han("¬a¶wÐi ® ·¶“e Ç¡“e ”a·q‰Á ˆ{¯s“¡”a.");
   puts_han("------------------------------------");
   puts_han("1. F9  : Ðe‹i·i Ðe¸a¡ ¥eÑÅ");
   puts_han("2. F10 : Ðe¸aŸi Ðe‹i¡ ¥eÑÅ");
   puts_han("3. adv_gets_han()· Ç¡ ‹a”¡ ¬a¶w");

   draw_box_han(19, 9, 60, 11, BOX_H1V1);
   strcpy(string, "Ðe‹i‰Á Ðe¸a");
   while(1)
   {
      key = adv_gets_han(20, 10, string, 40, FONT_NORMAL, exit_key);
      /* adv_gets_han()·¡ Ñ¡Â‰–E »¢ÒµA adv_gets_han_index“e */
      /* Ñe¸· Äá¬á ¶áÃ¡µA ”Ðe ¸÷¥¡Ÿi ˆx“e”a.              */

      if(key == ENTER || key == ESC) break;
      if(key == F9 && (string[adv_gets_han_index] & 0x80))
      /* Ñe¸ Äá¬á ¶áÃ¡· ‹i¸aˆa Ðe‹i·© ˜ Ð”wÐa“e Ðe¸aŸi ¬åÈ‚¤h“e”a.*/
      {
         ret_value = choose_hanja(string + adv_gets_han_index, hanja_code);
         if(ret_value == ENTER)
         /* ¬åÈ‚–E Ðe¸a· Å¡—a¡ ¤aŽ…”a. */
         {
            string[adv_gets_han_index]     = hanja_code[0];
            string[adv_gets_han_index + 1] = hanja_code[1];
         }
      }
      else if(key == F10 && (unsigned char) string[adv_gets_han_index] >= 0xe0)
      /* Ñe¸ Äá¬á ¶áÃ¡· ‹i¸aˆa Ðe¸a·© ˜“e Ðe‹i¡ ¤aŽ…”a. */
      {
         hanja_to_hangul(string + adv_gets_han_index, hangul_code);
         string[adv_gets_han_index]     = hangul_code[0];
         string[adv_gets_han_index + 1] = hangul_code[1];
      }
      else
      /* <F9>, <F10> Ç¡ˆa ÀáŸ¡–A»¡ ´g·e ‰w¶µA“e ·¥•B¯aŸi ‹a”¡ –…”a. */
      /* »b, Äá¬á· ¶áÃ¡Ÿi ¤aŽ»¡ ´g“e”a.                              */
      {
         adv_gets_han_index = adv_gets_han_index * (-1) - 1;
         continue;
      }

      /* <F9>, <F10> Ç¡ˆa ÀáŸ¡–E ‰w¶ ”a¯¡ adv_gets_han()·¡ Ñ¡Â‰–A */
      /* ¡e ÂAÁ¡· Äá¬á ¶áÃ¡ˆa Ñe¸· Äá¬á ¶áÃ¡ ¤a¡ ”a·q·¡ –A•¡¢ */
      /* adv_gets_han_index· ˆt·i ¹¡¸÷Ðe”a.                       */
      /* ‰·¯¢ : adv_gets_han_index = ¶¥Ða“e Äá¬á ¶áÃ¡ * (-1) - 1   */
      adv_gets_han_index = (adv_gets_han_index + 2) * (-1) - 1;
      if(adv_gets_han_index > strlen(string)) adv_gets_han_index = strlen(string);
   }
   clrscr_han();
   cputs_han("·³b¤h·e ‹i¸a“e <");
   cputs_han(string);
   cputs_han("> ·³“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
