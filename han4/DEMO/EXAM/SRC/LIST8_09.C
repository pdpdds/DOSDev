#include <stdio.h>
#include <alloc.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* Ðq® ¶¥Ñw */
void show_help(void);
void put_msg(char *string);

void main(void)
{
   unsigned char exit_key[] = { F1, 0 }, key;
   char string[41];
   if(init_han() != OK_HAN) exit(1);

   draw_box_han(19, 7, 60, 9, BOX_H1V1);
   xputs_han(19, 10, "•¡¶‘ i ÑÁ¡e·i ¥¡a¡e <F1>·i ’Ÿa­A¶a");
   strcpy(string, "");
   do
   {
      key = adv_gets_han(20, 8, string, 40, FONT_NORMAL, exit_key);
      if(key == F1)
      /* ¢…¸aµi·i ·³b¤h”aˆa <F1> Ç¡ˆa ’‰a»¡¡e •¡¶‘ i ÑÁ¡e·i ¥¡µa º…”a. */
      {
         show_help();
         /* Ñe¸ Äá¬á ¶áÃ¡µA¬á ‰­¢ ·³b·i ¤h•¡¢ Ðe”a. */
         adv_gets_han_index = adv_gets_han_index * (-1) - 1;
      }
   }
   /* <Esc>a <Enter> Ç¡Ÿi ’Ÿa¡e ¹·ža */
   while(key != ESC && key != ENTER);

   close_han();
}

/* •¡¶‘ i· ¦…œ·‰Á ÑÁ¡e Ça‹¡µA ‰Åe–E  Ça¡ ¬w® */
#define HELP_LINE       14                   /* ÑÁ¡eµA Îa¯¡–A“e º‰ ®    */
#define MAX_PAGE         2                   /* Îa¯¡Ði ® ·¶“e ÍA·¡»¡ ® */
#define MAX_HELP        HELP_LINE * MAX_PAGE /* Îa¯¡Ði ® ·¶“e º‰ ®     */

void show_help(void)
/* •¡¶‘ i ÑÁ·©·i ·ª´á¬á •¡¶‘ i·i ¥¡µa º…”a. */
{
   FILE *stream;
   char (*help)[68+2], *screen;
   int  cur_page, re_draw, line_no, key, i;

   /* •¡¶‘ iµA Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”wÐe”a. */
   if((help = malloc((68+2)*MAX_HELP)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return;
   }

   /* •¡¶‘ i ÑÁ·© list8_09.hlp· ¶w·i ·ª´á helpµA ¸á¸wÐe”a. */
   if((stream = fopen("list8_09.hlp", "rt")) == NULL)
   {
      put_msg("•¡¶‘ i ÑÁ·©·i ·ª·i ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return;
   }
   line_no = 0;
   while(!feof(stream))
      if(line_no >= MAX_HELP || fgets(help[line_no], 70, stream) == NULL)
         break;
      else
      {
         /* ¢…¸aµi· {µA ¦›·e '\n' ¢…¸a“e ¬b¹AÐe”a. */
         i = strlen(help[line_no]);
         if(help[line_no][i - 1] == '\n') help[line_no][i - 1] = 0;
         line_no++;
      }
   fclose(stream);

   /* ÑÁ¡e·i ¸á¸wÐe”a. */
   if((screen = save_text_han(4, 2, abs_maxx_han-4, 2+2+HELP_LINE+3)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return;
   }

   /* ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_box_han(4, 2, abs_maxx_han-4, 2+2+HELP_LINE+3, BOX_H1V1);
   aputs_han((abs_maxx_han-8)/2, 2+1, " •¡¶‘ i ", FONT_SHADE);
   draw_box_han(4, 2+2, abs_maxx_han-4, 2+2, BOX_H1V1);
   put_eng_font(4,              2+2, 195);
   put_eng_font(abs_maxx_han-4, 2+2, 180);
   draw_box_han(4, 2+2+HELP_LINE+1, abs_maxx_han-4, 2+2+HELP_LINE+1, BOX_H1V1);
   put_eng_font(4,              2+2+HELP_LINE+1, 195);
   put_eng_font(abs_maxx_han-4, 2+2+HELP_LINE+1, 180);
   xputs_han(8, 2+2+HELP_LINE+2, "Up/PgUp : ´| ÍA·¡»¡   Down/PgDn : "
                                 "”a·q ÍA·¡»¡   Esc : ‹a e ¥¡‹¡");

   /* helpµA ¸á¸w–E •¡¶‘ i·i ¥¡µa º…”a. */
   cur_page = 0;
   re_draw  = 1;
   while(1)
   {
      if(re_draw)
      /* ÍA·¡»¡Ÿi ¤aŽ´ö”a¡e •¡¶‘ i·i ¬¡ Îa¯¡Ðe”a. */
      {
         for(i = 0; i < HELP_LINE; i++)
            if(cur_page*HELP_LINE+i >= line_no)
               nputs_han(6, 2+3+i, "", 68);
            else
               nputs_han(6, 2+3+i, help[cur_page*HELP_LINE+i], 68);
      }

      /* ¬a¶w¸a· Ç¡ ·³b·i ÀáŸ¡Ðe”a. */
      key = getch_han();
      if(key == _ESC)
         break;
      else if(key == 0)
      {
         switch(getch_han())
         {
            case UP   :
            case PGUP : if(cur_page > 0) cur_page--;
                        re_draw = 1;
                        break;
            case DOWN :
            case PGDN : if(cur_page < MAX_PAGE - 1) cur_page++;
                        re_draw = 1;
                        break;
         }
      }
   }

   /* ÑÁ¡e·i ¥¢ŠÐa‰¡ helpµA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá¯¡Ç¥”a. */
   restore_text_han(4, 2, screen);
   free(help);
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
   /* 3. ¡A¯¡»¡Ÿi Â‰bÐe”a */
   xputs_han(box_x + 1, 11, string);
   /* 4. Ðe ˆ· Ç¡Ÿi ·³b¤h“e”a. */
   if(!getch()) getch();
   /* 5. ¶¥œ ÑÁ¡e·i ¥¢ŠÐe”a. */
   restore_text_han(box_x, 10, screen);
}
