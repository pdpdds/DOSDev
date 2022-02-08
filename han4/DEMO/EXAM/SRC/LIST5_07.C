#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *screen;                 /* ÑÁ¡e •A·¡ÈáŸi ¸á¸wÐi ¥e® */
   char string[MAX_STRLEN + 1];
   int msg_x, msg_y, msg_length; /* ¡A¯¡»¡ˆa Â‰b–I ¹ÁÎaµÁ ¡A¯¡»¡· ‹©·¡ */
   int i;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   /* ¡A¯¡»¡“e ¶å•¡¶µÁ ¢‰ÅÐa‰A ÑÁ¡e ¹A·© Ða”eµA aÈae”a. */
   msg_x = 0;
   msg_y = get_abs_maxy_han();
   /* ¡A¯¡»¡· ‹©·¡“e ÑÁ¡e· Í¢‰Á •··©Ða‰A Ðe”a. */
   msg_length = get_abs_maxx_han() + 1;

   /* ÑÁ¡e ¹A·© £»µA µb¬w· ¡A¯¡»¡ œa·¥·i  e—e”a. */
   naputs_han(msg_x, msg_y, "", msg_length, FONT_INVERSE);

   /* ÑÁ¡e·¡ ¹A”¡ ¥¢Š–A“e»¡ ˆñ¬aÐa‹¡ ¶áÐ £¡Ÿ¡ ÑÁ¡e·i Š¬÷Ð ‘½“e”a. */
   /* ¢…¸aµi·i Â‰bÐe ÒµA•¡ œa·¥Ï¡—aˆa –A»¡ ´g•¡¢ Ða‹¡ ¶áÐ puts_han  */
   /* () Ðq®ˆa ´a“¥ cputs_han() Ðq®Ÿi ¬a¶wÐaµv”a.                     */
   for(i = 0; i < 50; i++) cputs_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1      ");
   naputs_han(msg_x, msg_y, "´a¢ Ç¡a ’Ÿa¡e Ìs´ó ¶å•¡¶ˆa aÈas“¡”a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   /* 1. ÑÁ¡e·i ¸á¸wÐe”a. */
   if((screen = save_text_han(20, 5, 60, 15)) == NULL)
   {
      /* ¡A¡¡Ÿ¡ˆa ¡¡¸aœa¡e ¡A¯¡»¡Ÿi Â‰bÐe Ò ¯©Ð—·i º—”eÐe”a. */
      naputs_han(msg_x, msg_y, "¡A¡¡Ÿ¡ˆa ¡¡¸aœs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.",
                 msg_length, FONT_INVERSE);
      if(!getch()) getch();
      close_han();
      exit(1);
   }

   /* 2. ¤b¯aŸi ‹aŸ¡‰¡ Èa·¡Ëi·i Â‰bÐe”a. */
   draw_box_han(20, 5, 60, 15, BOX_H1V1);
   aputs_han(34, 5, " Ìs´ó ¶å•¡¶ ", FONT_INVERSE);

   /* 3. Ï©¶aÐe ÑÁ¡e µwµb·i »¡¶…”a. */
   clear_text_han(21, 6, 59, 14);

   /* 4. ¸â”wÐe ÀáŸ¡Ÿi Ðe”a.                                     */
   /* µa‹¡¬á“e ¶å•¡¶Ÿi ¬é¸÷Ðe Ò ¶å•¡¶µA¬á ¢…¸aµi·i ·³b¤h‰¡ */
   /* Â‰bÐ ¥¥”a. ‹a· <Enter>Ÿi Ã¡¡e {…”a.                  */
   set_window_han(21, 6, 59, 14);
   naputs_han(msg_x, msg_y, "¢…¸aµi·i ·³bÐ ¥¡­A¶a.",
              msg_length, FONT_INVERSE);

   do
   {
      gets_han(string);
      puts_han(string);
   }
   while(strlen(string) != 0);          /* while(!string[0]); œa‰¡ Ð•¡ –E”a. */
   set_attr_han(FONT_SHADE);
   naputs_han(msg_x, msg_y, "´a¢ Ç¡a ’Ÿa¡e Ìs´ó ¶å•¡¶ˆa ¬aœa»³“¡”a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   /* 5. ÑÁ¡e·i ¥¢ŠÐe”a. */
   restore_text_han(20, 5, screen);

   naputs_han(msg_x, msg_y, "ÑÁ¡e·¡ ¹A”¡ ¥¢Š–A´ö¯s“¡Œa? ´a¢ Ç¡a ’Ÿa­A¶a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
