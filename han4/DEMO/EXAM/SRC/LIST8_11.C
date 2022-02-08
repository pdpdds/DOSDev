#include <stdio.h>
#include <dos.h>
#include "hanio.h"
#include "extendio.h"

/* Ðq® ¶¥Ñw */
void my_redirect_func(char *string);
void put_msg(char *string);
int  error_handler(int errval, int ax, int bp, int si);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* Ða—a¶Á´á µAœá Ð…—iœáŸi ¬é¸÷Ðe”a. */
   harderr(error_handler);
   puts_han("”a·q 5º‰ e Â‰b·¡ ¸»¡Ð·–S“¡”a.");
   puts_han("ÏaŸ¥ÈáŸi Åa‰¡ ´a¢ Ç¡a ’Ÿa­A¶a..\n");
   /* Â‰b–A“e •A·¡Èáˆa my_redirect_func·a¡ ¸å”i–A•¡¢ Ðe”a.     */
   /* puts_han(), cputs_han(), printf_han() ¡¡– ¸»¡Ð··¡ ˆa“wÐa”a.*/
   redirect_func = my_redirect_func;
   printf_han("*****\n");
   puts_han("Â‰b ¸»¡Ð··i ·¡¶wÐa¡e Ïa¡‹aœ‘·");
   puts_han("¯©Ð— ‰i‰ÁŸi ¤a¡ ÑÁ·©·¡a ÏaŸ¥Èá¡");
   puts_han("Â‰bÐi ® ·¶¯s“¡”a.");
   cputs_han("*****\n");
   /* Â‰b–A“e •A·¡Èáˆa ´a¢ Ðq®µA•¡ ¸å”i–A»¡ ´g•¡¢ Ðe”a. */
   redirect_func = NULL;
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}

void my_redirect_func(char *string)
/* ¸»¡Ð·–E •A·¡ÈáŸi ÀáŸ¡Ða“e Ðq® */
{
   /* ¸»¡Ð·–E •A·¡Èá string·i ÏaŸ¥Èá¡ Â‰bÐe”a. */
   fputs(string, stdprn);
}

void put_msg(char *string)
/* ¡A¯¡»¡ Â‰b Ðq®(list9_10.c Àq‰¡) */
{
   char *screen;
   int  box_x;

   box_x = 40 - (strlen(string) + 2) / 2;
   screen = save_text_han(box_x, 10, box_x + strlen(string) + 1, 12);
   draw_box_han(box_x, 10, box_x + strlen(string) + 1, 12, BOX_H1V1);
   xputs_han(box_x + 1, 11, string);
   if(!getch()) getch();
   restore_text_han(box_x, 10, screen);
}

int error_handler(int errval, int ax, int bp, int si)
/* Ða—a¶Á´á¸â·¥ µAœáˆa ¤i¬—Ðaµv·i ˜ ¯©Ð—–A“e Ðq® */
/* ÄñÌa·©¯¡µA 4ˆ· ‰w‰¡ˆa aµ¡“e•A ¢¯¡Ð•¡ –E”a. */
{
   put_msg("µAœáˆa ¤i¬—Ðaµv¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   hardresume(0);       /* µAœáŸi ¢¯¡Ðe”a. */
   return(0);
}
