#include <dos.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* Ðq® ¶¥Ñw */
int error_handler(int errval, int ax, int bp, int si);

void main(void)
{
   char filename[128], ret_key;

   if(init_han() != OK_HAN) exit(1);

   /* Ða—a¶Á´á µAœá Ð…—iœáŸi ¬é¸÷Ðe”a. */
   harderr(error_handler);
   /* ¸÷–E ÑÁ·©¡w·e *.c¡ Ða‰¡ Èá¥¡ CÑw —¡BÉ¡Ÿ¡ ¤b¯aŸi ¥¡µa º…”a. */
   ret_key = dir_box_short((get_maxx_han() - 58) / 2, 5, "*.c", filename);

   if(ret_key == ENTER) printf_han("\n\n<%s> ÑÁ·©·i ¬åÈ‚Ða­v¯s“¡”a.\n", filename);
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}

int error_handler(int errval, int ax, int bp, int si)
/* Ða—a¶Á´á¸â·¥ µAœáˆa ¤i¬—Ðaµv·i ˜ ¯©Ð—–A“e Ðq®. */
/* ÄñÌa·©¯¡µA ‰w‰¡ˆa 4ˆ aµ¡“e•A ¢¯¡Ð•¡ –E”a.   */
{
   char *screen;
   int  box_x;

   box_x = 40 - (42 + 2) / 2;
   screen = save_text_han(box_x, 10, box_x + 42 + 1, 12);
   draw_box_han(box_x, 10, box_x + 42 + 1, 12, BOX_H1V1);
   xputs_han(box_x + 1, 11, "µAœáˆa ¤i¬—Ðaµv¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   restore_text_han(box_x, 10, screen);
   hardresume(0);       /* µAœáŸi ignoreÐe”a. */

   return(0);
}
