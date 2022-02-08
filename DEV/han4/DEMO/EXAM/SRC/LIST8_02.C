#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/* Ðq® ¶¥Ñw */
int save_to_file(int left, int top, int right, int bottom, char *filename);

void main(void)
{
   char string[MAX_STRLEN + 1];
   int msg_x, msg_y, msg_length; /* ¡A¯¡»¡ˆa Â‰b–I ¹ÁÎaµÁ ¡A¯¡»¡· ‹©·¡ */
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* ¡A¯¡»¡“e ¶å•¡¶µÁ ¢‰ÅÐa‰A ÑÁ¡e ¹A·© Ða”eµA aÈae”a. */
   msg_x = 0;
   msg_y = get_abs_maxy_han();
   /* ¡A¯¡»¡· ‹©·¡“e ÑÁ¡e· Í¢‰Á •··©Ða‰A Ðe”a. */
   msg_length = get_abs_maxx_han() + 1;

   /* ÑÁ¡e ¹A·© £»µA µb¬w· ¡A¯¡»¡ œa·¥·i  e—e”a. */
   naputs_han(msg_x, msg_y, "", msg_length, FONT_INVERSE);

   /* ÑÁ¡e·¡ ¹A”¡ ¸á¸w–A“e»¡ ˆñ¬aÐa‹¡ ¶áÐ £¡Ÿ¡ ÑÁ¡e·i Š¬÷Ð ‘½“e”a. */
   /* ¢…¸aµi·i Â‰bÐe ÒµA•¡ œa·¥Ï¡—aˆa –A»¡ ´g•¡¢ Ða‹¡ ¶áÐ puts_han  */
   /* ()·¡ ´a“¥ cputs_han() Ÿi ¬a¶wÐaµv”a.                              */
   for(i = 0; i < 50; i++) cputs_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1      ");
   modify_text_attr(10, 5, 70, 15, FONT_INVERSE);

   naputs_han(msg_x, msg_y, "´a¢ Ç¡a ’Ÿa¡e µb¬w·a¡ Îa¯¡–E ¦¦…·¡ ¸á¸w–S“¡”a",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   if(save_to_file(10, 5, 70, 15, "screen.dta") != OK_HAN)
      naputs_han(msg_x, msg_y, "ÑÁ¡e·i ¸á¸wÐi ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.",
                 msg_length, FONT_INVERSE);
   else
      naputs_han(msg_x, msg_y, "ÑÁ¡e·¡ ¸á¸w–A´ö¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.",
                 msg_length, FONT_INVERSE);
   if(!getch()) getch();
   close_han();
}

int save_to_file(int left, int top, int right, int bottom, char *filename)
/* (left, top) - (right, bottom) µwµb·i file_name µA ¸á¸wÐe”a.     */
/* ¹A”¡ ¸á¸w–A¡e OK_HAN ·i, µAœáˆa ¤i¬—Ða¡e µAœáÅ¡—aŸi Ÿ¡ÈåÐe”a. */
{
   FILE *stream;
   int  x, y;

   if((stream = fopen(filename, "wt")) == NULL) return(1);
   for(y = top; y <= bottom; y++)
   {
      for(x = left; x <= right; x++) fputc(scr_buff[y][x][0], stream);
      /* ‰w‰ ¦¦…µA¬á Ðe‹i·¡ ¸iŸ¡»¡ ´g•¡¢ Ðe ¤a·¡Ëa ”á ˆñ¬aÐe”a.   */
      /*  a»¡ b ‹i¸aˆa Ðe‹i Àõ¤å¼ ¤a·¡Ëa·¡¡e Ðe ¤a·¡Ëa ”á ¸á¸wÐe”a. */
      if(scr_buff[y][x - 1][1] == HAN1) fputc(scr_buff[y][x][0], stream);
      fputc('\n', stream);           /* ˆÐ— ¢…¸a Âˆa */
   }
   fclose(stream);
   return(OK_HAN);
}
