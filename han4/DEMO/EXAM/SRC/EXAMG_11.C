#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   draw_box_han(20, 9, 49, 11, BOX_H1V1);
   xputs_han(22, 10, "ÑÁ·©·i ¸á¸wÐa¯¡‰V¯s“¡Œa?");
   switch(get_yesno_han(47, 10, FONT_INVERSE))
   {
      case YES : xputs_han(22, 13, "¸á¸wÐ–¯s“¡”a.");
                 break;
      case NO  : xputs_han(22, 13, "¸á¸wÐa»¡ ´g´v¯s“¡”a.");
                 break;
      case ESC : xputs_han(22, 13, "¸á¸w·¡ Âá­¡–A´ö¯s“¡”a.");
                 break;
      default  : xputs_han(22, 13, "·¶·i ® ´ô“e ‰w¶·³“¡”a.");
                 break;
   }

   xputs_han(22, 14, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
