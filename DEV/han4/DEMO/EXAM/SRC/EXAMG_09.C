#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   xputs_han(0, 0, "´e‘wÐa­A¶a?");

   xputs_han(14, 5, "(0, 0)  (1, 0)");
   xprintf_han(0, 6, "´a¯aÇ¡ Å¡—a = %6d  %6d",
               get_scr_buff_code(0, 0), scr_buff[0][1][0]);
   xprintf_han(0, 7, "¢…¸a· ¹·ŸA = %6d  %6d",
               get_scr_buff_id(0, 0), scr_buff[0][1][1]);
   xprintf_han(0, 8, "­¢       ¬÷ = %6d  %6d",
               get_scr_buff_attr(0, 0), scr_buff[0][1][2]);
   xprintf_han(0, 9, "¸å  ‰w   ¬‚ = %6d  %6d",
               get_scr_buff_color(0, 0), scr_buff[0][1][3] & 0x0f);
   xprintf_han(0, 10, "¤  ‰w   ¬‚ = %6d  %6d",
               get_scr_buff_bkcolor(0, 0), scr_buff[0][1][3] >> 4);

   aputs_han(0, 12, "´a¢ Ç¡a ’Ÿa­A¶a..", FONT_INVERSE);
   if(!getch()) getch();
   close_han();
}
