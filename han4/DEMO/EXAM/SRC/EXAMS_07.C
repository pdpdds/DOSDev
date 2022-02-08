#include "hanio.h"
#include "extendio.h"

void status_on(void);
void draw_status(void);
void my_han_change_func(void);
void my_ins_change_func(void);

void main(void)
{
   char string[41];

   if(init_han() != OK_HAN) exit(1);

   status_on();
   draw_box_han(19, 9, 60, 11, BOX_H1V1);
   strcpy(string, "¬wÈœa·¥·¡ ¸i aµ³“¡Œa?");
   adv_gets_han(20, 10, string, 40, FONT_NORMAL, "");

   close_han();
}

void status_on(void)
/* ¬wÈ œa·¥·¡ aÈaa•¡¢ Ða“e Ðq® */
{
   naputs_han(0, 0, " Ðe‹i/µw¢…   ¬s·³/®¸÷   –¤é¯¢/­A¤é¯¢",
              abs_maxx_han, FONT_INVERSE);
   draw_status();
   set_han_change_func(my_han_change_func);
   set_ins_change_func(my_ins_change_func);
   set_window_han(0, 1, abs_maxx_han, abs_maxy_han);
}

void draw_status(void)
/* ¬wÈ œa·¥·i Îa¯¡Ða“e Ðq® */
{
   if(is_han_mode())
   {
      modify_text_attr(1, 0, 4, 0, FONT_NORMAL);
      modify_text_attr(6, 0, 9, 0, FONT_INVERSE);
   }
   else
   {
      modify_text_attr(1, 0, 4, 0, FONT_INVERSE);
      modify_text_attr(6, 0, 9, 0, FONT_NORMAL);
   }

   if(ins_mode) /* ins_mode“e ¸åµb ¥e® */
   {
      modify_text_attr(13, 0, 16, 0, FONT_NORMAL);
      modify_text_attr(18, 0, 21, 0, FONT_INVERSE);
   }
   else
   {
      modify_text_attr(13, 0, 16, 0, FONT_INVERSE);
      modify_text_attr(18, 0, 21, 0, FONT_NORMAL);
   }

   if(get_keyboard_type() == BUL3_TYPE)
   {
      modify_text_attr(25, 0, 30, 0, FONT_INVERSE);
      modify_text_attr(32, 0, 37, 0, FONT_NORMAL);
   }
   else
   {
      modify_text_attr(25, 0, 30, 0, FONT_NORMAL);
      modify_text_attr(32, 0, 37, 0, FONT_INVERSE);
   }
}

void my_han_change_func(void)
/* Ðe/µw ¸åÑÅ Ç¡ˆa ’‰a»© ˜ a”a Ñ¡Â‰–A“e Ðq® */
{
   play("o2l64ec");     /* '—·•·' */
   draw_status();
}

void my_ins_change_func(void)
/* <INS> Ç¡ˆa ’‰a»© ˜ a”a Ñ¡Â‰–A“e Ðq® */
{
   play("o3l32e");      /* '—·' */
   draw_status();
}
