#include <alloc.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

#define PAD_WIDTH  40
#define PAD_HEIGHT 10

/* Ðq® ¶¥Ñw */
int note_pad(int box_x, int box_y, char **text, int height, int width);

void main(void)
{
   /* ·³b¤h·e ¢…¸w—i·e char Í¡·¥Èá ¤µi·¥ textµA ¸á¸w–E”a. */
   char *(text[PAD_HEIGHT]);
   int  i, key;

   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   /* text µA Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”w¤h“e”a. */
   for(i = 0; i < PAD_HEIGHT; i++)
   {
      /* Ðe ˆ· Í¡·¥ÈáµA Ðe º‰ ¦…œ·· ¡A¡¡Ÿ¡Ÿi Ði”w¤h“e”a. */
      text[i] = (char *) malloc(PAD_WIDTH + 1);
      if(text[i] == NULL)
      {
         error_exit_msg = "¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a";
         error_exit_han("main()", 1);
      }
      strcpy(text[i], "");
   }

   xputs_han(15, 14, "-------------- ¬a¶w ˆa“wÐe Ç¡ ¡¢¢ --------------");
   xputs_han(15, 15, "1. <Left>, <Right>, <Ctrl>+<Left>, <Ctrl>+<Right>");
   xputs_han(15, 16, "   <Home>, <End>, <Up>, <Down>");
   xputs_han(15, 17, "   <Ctrl>+<PgUp>, <Ctrl>+<PgDn>");
   xputs_han(15, 18, "2. <Ins>, <BS>, <Del>, <Ctrl>+<T>");
   xputs_han(15, 19, "   <Ctrl>+<Y>, <Ctrl>+<Q>+<Y>");
   xputs_han(15, 20, "3. <Esc>, <Ctrl>+<End>");

   /* Ðe ÍA·¡»¡ ¦…œ·· ¢…¸w·i ·³b¤h“e”a. */
   key = note_pad(18, 0, text, PAD_HEIGHT, PAD_WIDTH);

   /* ·³b ‰i‰ÁŸi Â‰bÐe”a. */
   clrscr_han();
   cputs_han(" a»¡ bµA ’Ÿe Ç¡“e ");
   switch(key)
   {
      case ENTER   : cputs_han("<Enter> ·¡¡a ");     break;
      case ESC     : cputs_han("<Esc> ·¡¡a ");       break;
      case CTRLEND : cputs_han("<Ctrl>+<End>·¡¡a "); break;
   }
   puts_han("·³b¤h·e ¢…¸w—i·e ”a·q‰Á ˆ{¯s“¡”a.\n");
   for(i = 0; i < PAD_HEIGHT; i++)
   {
      puts_han(text[i]);
      free(text[i]);
   }
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}

int note_pad(int box_x, int box_y, char **text, int height, int width)
/* ‹©·¡ˆa width ·¥ ¢…¸aµi·i height ˆ ·³b¤h´a text µA ¸á¸wÐe”a. */
/* Ÿ¡Èå–A“e ˆt·e ¹·žaÐi ˜ ¬a¶w–E Ç¡· Å¡—a·¥•A ENTER, ESC, CTR  */
/* LEND º— Ðaa·¡”a.                                             */
{
   unsigned char exit_key[] = { UP, DOWN, CTRLPGUP, CTRLPGDN, CTRLEND, 0 };
   int cur_line, key, index;

   draw_box_han(box_x, box_y, box_x+width+1, box_y+height+1, BOX_H1V1);
   cur_line = 0;
   while(1)
   {
      key = adv_gets_han(box_x+1, box_y+1+cur_line, text[cur_line],
                         width, FONT_NORMAL, exit_key);
      /* adv_gets_han()·¡ Ñ¡Â‰–E »¢ÒµA“e adv_gets_han_index“e */
      /* Ñe¸· Äá¬á ¶áÃ¡µA ”Ðe ¸÷¥¡Ÿi ˆx“e”a.                */

      if(key == ESC || key == CTRLEND)
         break;
      else if(key == ENTER)
      {
         /* ¹A·© { º‰µA¬á <Enter> Ç¡Ÿi ’Ÿa¡e ·³b·i ¹·žaÐe”a. */
         if(++cur_line == height) break;
      }
      else if(key == UP && cur_line > 0)
      /* Ðe º‰ ¶á¡ ·¡•· */
         cur_line--;
      else if(key == DOWN && cur_line < height - 1)
      /* Ðe º‰ ´aœ¡ ·¡•· */
         cur_line++;
      else if(key == CTRLPGUP)
      /* ¹A·© Àõ º‰¡ ·¡•· */
         cur_line = 0;
      else if(key == CTRLPGDN)
      /* ¹A·© { º‰¡ ·¡•· */
         cur_line = height - 1;

      /* ”a¯¡ adv_gets_han()·¡ Ñ¡Â‰–A´ö·i ˜· ÂAÁ¡ Äá¬á ¶áÃ¡Ÿi ¹¡  */
      /* ¸÷Ðe”a.                                                    */
      /* <Enter>, <Ctrl>+<PgUp>, <Ctrl>+<PgDn> -> ¹A·© Àõ µi        */
      /* <Up>, <Down>                          -> Ñe¸ Äá¬á ¶áÃ¡    */
      /*  e´¢ ·¡•·Ðe º‰· ¢…¸aµi ‹©·¡ˆa ÂAÁ¡ Äá¬á ¶áÃ¡¥¡”a ¼l·i  ˜ */
      /* “e ¢…¸aµi· ¹A·© {µA ¶áÃ¡Ða•¡¢ ¹¡¸÷Ðe”a.                 */
      /* ‰·¯¢ : adv_gets_han_index = ¶¥Ða“e Äá¬á ¶áÃ¡ * (-1) - 1    */
      if(key == ENTER || key == CTRLPGUP || key == CTRLPGDN)
         adv_gets_han_index = 0 * (-1) - 1;
      else
      {
         if(strlen(text[cur_line]) < adv_gets_han_index)
         /* ¬¡ ·¡•·Ðe º‰· ¢…¸aµi ‹©·¡ˆa ”á ¼l”a¡e ¢…¸aµi· ¹A·© {µA */
            adv_gets_han_index = strlen(text[cur_line]) * (-1) - 1;
         else
         /* Â—¦…Ó¡ ‹©”a¡e Ñe¸ Äá¬á ¶áÃ¡µA */
            adv_gets_han_index = adv_gets_han_index * (-1) - 1;
      }
   }
   return(key);
}
