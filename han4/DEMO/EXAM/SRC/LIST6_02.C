#include "hanio.h"
#include "extendio.h"

void change_cursor_to_box(void);

void main(void)
{
   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   /* ¤b¯a ¡¡´·· Äá¬á¡ ¤aŽ…”a. */
   change_cursor_to_box();

   puts_han("µw¢… Äá¬áˆa ¸i ¥¡·³“¡Œa?\n\n");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a.");
   /* Ñe¸ ¹ÁÎaµA µw¢… Äá¬áŸi Â‰bÐe”a. */
   cursor_han = eng_cursor;             /* Ñe¸ Äá¬á“e µw¢… Äá¬á¡ ¬é¸÷ */
   draw_cursor_han(get_curx_han(), get_cury_han()); /* Äá¬áŸi ‹aŸ¥”a. */
   if(!getch()) getch();
   draw_cursor_han(get_curx_han(), get_cury_han()); /* Äá¬áŸi »¡¶…”a. */

   puts_han("Ðe‹i Äá¬áˆa ¸i ¥¡·³“¡Œa?\n\n");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a.");
   /* Ñe¸ ¹ÁÎaµA Ðe‹i Äá¬áŸi Â‰bÐe”a. */
   cursor_han = han_cursor;             /* Ñe¸ Äá¬á“e Ðe‹i Äá¬á¡ ¬é¸÷ */
   draw_cursor_han(get_curx_han(), get_cury_han()); /* Äá¬áŸi ‹aŸ¥”a. */
   if(!getch()) getch();
   draw_cursor_han(get_curx_han(), get_cury_han()); /* Äá¬áŸi »¡¶…”a. */

   close_han();
}

void change_cursor_to_box(void)
{
   /* Ðe‹i Äá¬á· ¡¡´·(¤b¯a ¡¡´·) */
   unsigned char shape32[HAN_FONT_SIZE] =
   {
      0xff,0xff,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0xff,0xff
   };
   /* µw¢… Äá¬á· ¡¡´·(¤b¯a ¡¡´·) */
   unsigned char shape16[ENG_FONT_SIZE] =
   {
      0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
      0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
   };

   /* ÑA”á· ˆt·i ¬é¸÷ */
   han_cursor[0] = 15;
   eng_cursor[0] =  7;
   /* §¡Ëa “ ¸ažaŸi ¬é¸÷ */
   memmove(han_cursor+1, shape32, HAN_FONT_SIZE);
   memmove(eng_cursor+1, shape16, ENG_FONT_SIZE);
}
