#include <ctype.h>
#include "hanio.h"
#include "extendio.h"

#define MENU_NUMBER     6       /* ¡A“A· ˆ•® */

/* static_menu() Ðq®· ¶¥Ñw */
int static_menu(void);

void main(void)
{
   int menu;                                    /* ¬åÈ‚–E ¡A“A· ¤åÑ¡ */

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   aputs_han(33, 19, "¡A“AŸi  ¬åÈ‚Ða­A¶a", FONT_SHADE);
   menu = static_menu();                        /* ¡A“AŸi ¬åÈ‚¤h“e”a. */

   clrscr_han();
   printf_han("%d ¤å ¡A“AŸi ¬åÈ‚Ða­v¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.\n", menu);
   if(!getch()) getch();

   close_han();
}

int static_menu(void)
/* ¡A“AŸi ¬åÈ‚¤h“e Ðq® */
{
   char *menu[MENU_NUMBER] = { "ÑÁ·© ·ª‹¡    L",        /* ¡A“A ¢…¸aµi */
                               "ÑÁ·© ¸á¸w    S",
                               "¬ ÑÁ·©      N",
                               "—¡BÉ¡Ÿ¡     D",
                               "•¡¯a¡       O",
                               "{           Q"
                             };
   char hot_key[MENU_NUMBER] = { 'L', 'S', 'N', 'D', 'O', 'Q' }; /* Ðu Ç¡ */
   char key;
   int i;

   /* ¡A“A ¢…¸aµi·i Îa¯¡Ðe Ò ¤b¯a¡ ÉA–Ÿ¡Ÿi Ã¥”a. */
   for(i = 0; i < MENU_NUMBER; i++) xputs_han(35, 10 + i, menu[i]);
   draw_box_han(33, 9, 50, 16, BOX_H1V1);

   do
   {
      key = getch();
      if(key == 0) /* ÑÂ¸w Ç¡“e ¢¯¡Ðe”a. */
      {
         getch();
         continue;
      }

      /* ¤aŸe Ç¡ˆa ’‰v“e»¡ ˆñ¬aÐe”a. */
      /* ÀáŸ¡Ÿi ¶w·¡Ða‰A Ða‹¡ ¶áÐ keyŸi ”¢…¸a¡ ÀáŸ¡Ðe”a. */
      for(i = 0; i < MENU_NUMBER; i++) if(toupper(key) == hot_key[i]) break;
      if(i < MENU_NUMBER) break; /* ¤aŸe Ç¡ˆa ’‰v·a¡e žÏaŸi ¨a¹a aˆe”a. */
   }
   while(1);    /* ¢ÐeÓ¡ žÏaŸi ¤e¥¢Ðe”a. */

   return(i);   /* ¬åÈ‚–E ¡A“A· ¤åÑ¡Ÿi Ÿ¡ÈåÐe”a. */
}
