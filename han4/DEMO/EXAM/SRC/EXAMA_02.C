#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *string = "®aœa Š…”“e Àá·q ¶a•··i ˜áa ¸åŠ… ¬q¯³ e µ¡Àå ¡w·¡ "
                  "Áa—i´áµÖ“e•A, ÌÐaµa ¶a•·¬÷·a¡ •©´aˆe ˆõ·e ‰a¶ Ã©¤‚  "
                  "¡wµA »¡a»¡ ´g´v”a.";

   if(init_han() != OK_HAN) exit(1);

   set_window_han(20, 5, 60, 20);
   draw_box_han(19, 4, 61, 21, BOX_H1V1);

   /* ‹a· puts_han() Ðq®¡ Â‰bÐe”a. */
   set_attr_han(FONT_INVERSE);
   puts_han("puts_han() Ðq®¡ Â‰bÐa¡e ”a·q‰Á ˆ{·e ¡¡´··¡ –S“¡”a.");
   set_attr_han(FONT_NORMAL);
   puts_han(string);
   set_attr_han(FONT_INVERSE);

   /* adv_puts_han() Ðq®¡ Â‰bÐe”a(2Äe —iµa ³e”a). */
   puts_han("\nadv_puts_han() Ðq®¡ Â‰bÐa¡e ”a·q‰Á ˆ{·e ¡¡´··¡ –S“¡”a.");
   set_attr_han(FONT_NORMAL);
   adv_puts_han(string, 2);
   set_attr_han(FONT_INVERSE);
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();

   close_han();
}
