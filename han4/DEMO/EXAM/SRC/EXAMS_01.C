#include <stdlib.h>
#include "hanio.h"

void draw_dummy(void);

void main(void)
{
   char *screen;

   if(init_han() != OK_HAN) exit(1);

   draw_dummy();
   puts_han("ÑÁ¡e·i »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   /* ÑÁ¡e ¸åÁAŸi ¸á¸wÐe Ò »¡¶…”a. */
   screen = save_text_han(0, 0, abs_maxx_han, abs_maxy_han);
   if(screen == NULL)
   {
      close_han();
      puts("Memory is not enough..");
      exit(2);
   }
   clrscr_han();

   puts_han("ÑÁ¡e·i ¥¢ŠÐa‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
   puts_han("ÑÁ¡e·¡ ¥¢Š–E ÒµA ”a¯¡ ´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   restore_text_han(0, 0, screen);
   if(!getch()) getch();
   close_han();
}

void draw_dummy(void)
/* ·±·¡ ÑÁ¡e·i Š¬÷Ða“e Ðq® */
{
   int i, color, bkcolor, attr, rand_attr;

   /* Ñe¸ ¬‚‰Á ­¢¬÷·i ¸á¸wÐe”a. */
   color   = get_color_han();
   bkcolor = get_bkcolor_han();
   attr    = get_attr_han();

   for(i = 0; i < 50; i++)
   {
      /* ¬‚‰Á ­¢¬÷·i ”iŸ¡ Ðaµa ¢…¸aµi·i Â‰bÐe”a. */
      set_color_han(random(16));
      set_bkcolor_han(random(16));
      /* ÑÂ” ¢…¸a“e Ï¡Ðe”a. */
      rand_attr = 1 << random(8);
      if((rand_attr & FONT_VERT_DBL) == 0 && (rand_attr & FONT_HORIZ_DBL) == 0)
         set_attr_han(rand_attr);
      cputs_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1");
      set_color_han(WHITE);
      set_bkcolor_han(BLACK);
      set_attr_han(FONT_NORMAL);
      cputs_han("     ");
   }
   putch_han("\n");

   /* ¶¥œ ¬‚‰Á ­¢¬÷·i ¥¢ŠÐe”a. */
   set_color_han(color);
   set_bkcolor_han(bkcolor);
   set_attr_han(attr);
}
