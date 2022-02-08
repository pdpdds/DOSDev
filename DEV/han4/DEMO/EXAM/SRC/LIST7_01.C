#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

#define MENU_NUMBER     7       /* ¡A“A· ˆ•® */

void main(void)
{
   char *string[MENU_NUMBER] = { " ¬ ‹i       @N ",
                                 " ¦‰œáµ¡‹¡    @O ",
                                 " ¸á¸wÐa‹¡    @S ",
                                 " ¬·¡Ÿq·a¡  @V ",
                                 " ‰w¡¤aŽ‹¡     ",
                                 " •¡¯a¡ww´á @F0 ",
                                 " {          @X "
                               };
   int  current;                /* ¬åÈ‚–E ¡A“A· ¤åÑ¡ */
   char key;                    /*  a»¡ bµA ’‰a»¥ Ç¡ */

   if(init_han() != OK_HAN) exit(1);

   aputs_han(33, 19, "¡A“AŸi  ¬åÈ‚Ða­A¶a", FONT_SHADE);

   /* Àá·qµA“e 0¤å¼ ¡A“Aˆa ¬åÈ‚–A´á ·¶•¡¢ Ðe Ò ¡A“AŸi ¬åÈ‚¤h“e”a. */
   current = 0;
   key = popup_han(33, 8, string, MENU_NUMBER, &current);

   clrscr_han();
   switch(key)
   {
      case ESC   : puts_han("<Esc> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
                   break;
      case LEFT  : puts_han("<Left> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
                   break;
      case RIGHT : puts_han("<Right> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
                   break;
      case ENTER : printf_han("<%s> ¡A“AŸi ¬åÈ‚Ða­v¯s“¡”a.\n", string[current]);
                   break;
   }
   puts_han(" ´a¢ Ç¡a ’Ÿa­A¶a..");

   if(!getch()) getch();
   close_han();
}
