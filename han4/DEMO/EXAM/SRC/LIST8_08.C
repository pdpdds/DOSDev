#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/* Ðq® ¶¥Ñw */
void put_msg(char *string);

void main(void)
{
   char string[4], quit;

   if(init_han() != OK_HAN) exit(1);

   set_max_strlen_han(3);
   quit = 0;
   do
   {
      cputs_han("”w¯¥· a·¡“e? ");
      gets_han(string);
      if(!is_number(string))
      /* ®•¸a ·¡¶A· ¢…¸aŸi ·³bÐ–·a¡e ¡A¯¡»¡Ÿi Â‰bÐe”a. */
         put_msg("®•¸a e ·³bÐa­A¶a");
      else
         quit = 1;
   }
   while(!quit);
   put_msg("´a¢ Ç¡a ’Ÿa­A¶a");

   close_han();
}

void put_msg(char *string)
/* ¡A¯¡»¡ Â‰b Ðq® */
{
   char *screen;
   int  box_x;

   box_x = 40 - (strlen(string) + 2) / 2;
   /* 1. ¡A¯¡»¡ˆa Â‰b–I µwµb·i ¸á¸wÐe”a. */
   screen = save_text_han(box_x, 10, box_x + strlen(string) + 1, 12);
   /* 2. ¡A¯¡»¡ ¤b¯aŸi ‹aŸ¥”a. */
   draw_box_han(box_x, 10, box_x + strlen(string) + 1, 12, BOX_H1V1);
   /* 3. ¡A¯¡»¡Ÿi Â‰bÐe”a. */
   xputs_han(box_x + 1, 11, string);
   /* 4. Ðe ˆ· Ç¡Ÿi ·³b¤h“e”a. */
   if(!getch()) getch();
   /* 5. ¶¥œ ÑÁ¡e·i ¥¢ŠÐe”a. */
   restore_text_han(box_x, 10, screen);
}
