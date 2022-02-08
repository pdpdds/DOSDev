#include "hanio.h"
#include "misc.h"

void main(void)
{
   int gr_mode;

   /* ‹aœÏ¢ ¡¡—aŸi ¬åÈ‚¤h“e”a.                        */
   /* QŸi ¬åÈ‚Ð–ˆáa <Esc>Ÿi ’Ÿa¡e ¯©Ð—·i ¹·žaÐe”a.   */
   gr_mode = choose_gr_mode();
   if(gr_mode == -1) exit(1);

   /* ÑÁ¡e ¬wÈŸi ¬åÈ‚–E ‹aœÏ¢ ¡¡—a¡ ¤aŽ…”a.           */
   /* ¶¥œ ÑÁ¡e·e »¡¶¡»¡»¡ ´g“e”a(BGI ÑÁ·©·e Ï©¶a ´ô”a). */
   set_to_gr_mode(gr_mode);
   if(!getch()) getch();

   /* ÑÁ¡e ¬wÈŸi ”a¯¡ ÉB¯aËa ¡¡—a¡ ¤aŽ…”a.               */
   /* ¤e—a¯¡ set_to_gr_modeµA ¬a¶wÐe ‹aœÏ¢ ¡¡—aŸi ‹a”¡  */
   /* set_to_text_modeµA ñ‰a º´á´¡ Ðe”a.                 */
   set_to_text_mode(gr_mode);
   puts("\n\nagain in text mode..");
}
