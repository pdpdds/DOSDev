#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

#define MENU_NUMBER     7       /* ¡A“A· ˆ•® */

void main(void)
{
   menu_struct menu_data =
   {
      MENU_NUMBER, 0, 1, 0, BOX_H1V1, FONT_NORMAL,  33, 8, 50, 16,
   };
   char menu_data_x[MENU_NUMBER] = { 34, 34, 34, 34, 34, 34, 34, },
        menu_data_y[MENU_NUMBER] = {  9, 10, 11, 12, 13, 14, 15, };
   char *(menu_data_string[MENU_NUMBER]) = { " ¬ ‹i       @N ",
                                             " ¦‰œáµ¡‹¡    @O ",
                                             " ¸á¸wÐa‹¡    @S ",
                                             " ¬·¡Ÿq·a¡  @V ",
                                             " ‰w¡¤aŽ‹¡     ",
                                             " •¡¯a¡ww´á @F0 ",
                                             " {          @X "
                                           };
   char key;                                    /*  a»¡ bµA ’‰a»¥ Ç¡ */

   /* Á¡‹¡ÑÁ–A»¡ ´g·e menu_data ¶a­¡· ˆt·i »¡¸÷Ðe”a. */
   menu_data.x      = menu_data_x;
   menu_data.y      = menu_data_y;
   menu_data.string = menu_data_string;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   aputs_han(33, 19, "¡A“AŸi  ¬åÈ‚Ða­A¶a", FONT_SHADE);
   key = menu_han(&menu_data);

   clrscr_han();
   switch(key)
   {
      case ESC   : puts_han("<Esc> Ç¡Ÿi ’Ÿa­v¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
                   break;
      case LEFT  : puts_han("<Left> Ç¡Ÿi ’Ÿa­v¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
                   break;
      case RIGHT : puts_han("<Right> Ç¡Ÿi ’Ÿa­v¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
                   break;
      case DOWN  : puts_han("<Down> Ç¡Ÿi ’Ÿa­v¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
                   break;
      case ENTER : printf_han("<%s> ¡A“AŸi ¬åÈ‚Ða­v¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.\n",
                              menu_data.string[menu_data.current]);
                   break;
   }

   if(!getch()) getch();
   close_han();
}
