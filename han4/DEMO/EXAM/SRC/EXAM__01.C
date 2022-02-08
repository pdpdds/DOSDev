#include "hanio.h"
#include "misc.h"

void main(void)
{
   int gr_mode;

   /* ‹aœÏ¢ ¡¡—aŸi ¬åÈ‚¤h“e”a.                        */
   /* QŸi ¬åÈ‚Ð–ˆáa <Esc> Ÿi ’Ÿa¡e ¯©Ð—·i ¹·žaÐe”a.  */
   gr_mode = choose_gr_mode();
   if(gr_mode == -1) exit(1);

   /* ¬åÈ‚–E ‹aœÏ¢ ¡¡—a”¡ Á¡‹¡ÑÁÐe”a(BGI ÑÁ·©·e Ï©¶a ´ô”a). */
   if(_init_han(gr_mode) != OK_HAN) exit(2);

   puts_han("´e‘wÐa­A¶a?");

   if(!getch()) getch();
   /* ¤e—a¯¡ _init_hanµA ¬a¶wÐe ‹aœÏ¢ ¡¡—aŸi ‹a”¡ _close_hanµA ñ‰a   */
   /* º´á´¡ Ðe”a.                                                       */
   _close_han(gr_mode);
}
