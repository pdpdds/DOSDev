#include "hanio.h"
#include "extendio.h"

/* Ðq® ¶¥Ñw */
void left_scrl(int left, int top, int right, int bottom, int width);
void right_scrl(int left, int top, int right, int bottom, int width);

void main(void)
{
   char string[MAX_STRLEN + 1];
   int msg_x, msg_y, msg_length;     /* ¡A¯¡»¡ˆa Â‰b–I ¹ÁÎaµÁ ¡A¯¡»¡· ‹©·¡ */
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* ¡A¯¡»¡“e ¶å•¡¶µÁ ¢‰ÅÐa‰A ÑÁ¡e ¹A·© Ða”eµA aÈae”a. */
   msg_x = 0;
   msg_y = get_abs_maxy_han();
   /* ¡A¯¡»¡· ‹©·¡“e ÑÁ¡e· Í¢‰Á •··©Ða‰A Ðe”a. */
   msg_length = get_abs_maxx_han() + 1;

   /* ÑÁ¡e ¹A·© £»µA µb¬w· ¡A¯¡»¡ œa·¥·i  e—e”a. */
   naputs_han(msg_x, msg_y, "", msg_length, FONT_INVERSE);

   /* ÑÁ¡e·¡ ¹A”¡ ¯aÇa©–A“e»¡ ˆñ¬aÐa‹¡ ¶áÐ £¡Ÿ¡ ÑÁ¡e·i Š¬÷Ð ‘½“e”a. */
   /* ¢…¸aµi·i Â‰bÐe ÒµA•¡ œa·¥Ï¡—aˆa –A»¡ ´g•¡¢ Ða‹¡ ¶áÐ  puts_han() */
   /* ·¡ ´a“¥ cputs_han() Ÿi ¬a¶wÐaµv”a.                                  */
   for(i = 0; i < 50; i++) cputs_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1      ");

   naputs_han(msg_x, msg_y, "´a¢ Ç¡a ’Ÿa¡e ¸åÁA ÑÁ¡e·¡ µ¡Ÿe½¢·a¡ ¯aÇa©–S“¡”a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();
   /* ¡A¯¡»¡ œa·¥·i ¹A¶AÐe ¸å ÑÁ¡e·i µ¡Ÿe½¢·a¡ 5 Äéœñ e ¯aÇa©¯¡Ç¥”a. */
   right_scrl(0, 0, get_maxx_han(), get_maxy_han() - 1, 5);

   naputs_han(msg_x, msg_y, "´a¢ Ç¡a ’Ÿa¡e ¸åÁA ÑÁ¡e·¡ ¶E½¢·a¡ ¯aÇa©–S“¡”a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();
   /* ¡A¯¡»¡ œa·¥·i ¹A¶AÐe ¸å ÑÁ¡e·i ¶E½¢·a¡ 5 Äéœñ e ¯aÇa©¯¡Ç¥”a. */
   left_scrl(0, 0, get_maxx_han(), get_maxy_han() - 1, 5);

   naputs_han(msg_x, msg_y, "¯aÇa©·¡ –A´ö¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();
   close_han();
}

void right_scrl(int left, int top, int right, int bottom, int width)
/* (left, top) - (right, bottom) µwµb·i µ¡Ÿe½¢·a¡ width Äéœñ eÇq ¯aÇa©Ðe”a.*/
{
   char *screen;

   /* ·¥¸aˆa ¸i¡µ »¡¸÷–A´ö·a¡e error_exit_han()·i Ñ¡Â‰Ðaµa ¯©Ð—·i */
   /* º—”eÐe”a.                                                   */
   /* ¯©¹A¡“e left, top, right, bottom —w· ¹ÁÎa•¡ ”a  ˆñ¬aÐ´¡  */
   /* Ða»¡ e µa‹¡¬á“e µ¡¶w· µa»¡ˆa  g·e ˆõ e ˆñ¬aÐa•¡¢ Ð–”a.    */
   if(left > right + width || width < 1) error_exit_han("scrl_left", 1);

   /* 1. ÑÁ¡e·i ¸á¸wÐe”a. */
   if((screen = save_text_han(left, top, right - width, bottom)) == NULL)
   {
      /* ¡A¡¡Ÿ¡ˆa ¡¡¸aœa¡e µAœá ÀáŸ¡ Ðq® error_exit_han()·i Ñ¡Â‰Ðaµa */
      /* ¯©Ð—·i º—”eÐe”a.                                             */
      error_exit_han("scrl_left", 1);
   }

   /* 2. ¹ÁÎaŸi ·¡•·¯¡Åa ÑÁ¡e·i ¥¢ŠÐe”a. */
   restore_text_han(left + width, top, screen);

   /* 3. a á»¡ ¦¦…·i »¡¶…”a. */
   clear_text_han(left, top, left + width - 1, bottom);
}

void left_scrl(int left, int top, int right, int bottom, int width)
/* (left, top) - (right, bottom) µwµb·i ¶E½¢·a¡ width Äéœñ eÇq ¯aÇa©Ðe”a. */
{
   char *screen;

   /* ·¥¸aˆa ¸i¡µ »¡¸÷–A´ö·a¡e error_exit_han()·i Ñ¡Â‰Ðaµa ¯©Ð—·i  */
   /* º—”eÐe”a.                                                    */
   /* ¯©¹A¡“e left, top, right, bottom —w· ¹ÁÎa•¡ ”a  ˆñ¬aÐ´¡   */
   /* Ða»¡ e µa‹¡¬á“e µ¡¶w· µa»¡ˆa  g·e ˆõ e ˆñ¬aÐa•¡¢ Ð–”a.     */
   if(left + width > right || width < 1) error_exit_han("scrl_left", 1);

   /* 1. ÑÁ¡e·i ¸á¸wÐe”a */
   if((screen = save_text_han(left + width, top, right, bottom)) == NULL)
   {
      /* ¡A¡¡Ÿ¡ˆa ¡¡¸aœa¡e µAœá ÀáŸ¡ Ðq® error_exit_han()·i Ñ¡Â‰Ðaµa  */
      /* ¯©Ð—·i º—”eÐe”a.                                              */
      error_exit_han("scrl_right", 1);
   }

   /* 2. ¹ÁÎaŸi ·¡•·¯¡Åa ÑÁ¡e·i ¥¢ŠÐe”a. */
   restore_text_han(left, top, screen);

   /* 3. a á»¡ ¦¦…·i »¡¶…”a. */
   clear_text_han(right - width + 1, top, right, bottom);
}
