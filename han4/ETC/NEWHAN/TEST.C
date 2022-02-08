#include <graphics.h>
#include <stdlib.h>
#include <conio.h>

#include "hanio.h"
#include "extendio.h"
#include "hangraph.h"

void save_image_han(int left, int top, int right, int bottom, char *file_name);
void load_image_han(int left, int top, char *file_name);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* (0, 0) µA Ìaœw ¤aÈwµA ‘¡œw ‹i³¡¡ ·qµw¢…¸aŸi Â‰bÐe”a. */
   set_attr_han(FONT_INVERSE);
   vbputs_han(0, 0, "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1", LIGHTGRAY, RED);
   /* (0, 1) µA ÑÂ”¢…¸aŸi Â‰bÐe”a. */
   set_attr_han(FONT_VERT_DBL | FONT_HORIZ_DBL);
   vputs_han(0, 1, "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1", BLUE);
   /* ¬å·i ‹u‰¡ ¶¥·i ‹aŸ¥”a. */
   draw_horiz_line(0, 50, 200, LIGHTMAGENTA);
   draw_vert_line(100, 20, 60, LIGHTGREEN);
   circle(100, 50, 30);

   set_attr_han(FONT_NORMAL);
   nputs_han(0, 10, "´a¢ Ç¡a ’Ÿa¡e ÑÁ¡e·i »¡¶““¡”a..", 80);
   if(!getch()) getch();
   save_image_han(0, 0, 40, 5, "image.dta");

   clrscr_han();
   nputs_han(0, 10, "´a¢ Ç¡a ’Ÿa¡e ¶¥œ ÑÁ¡e·i ¥¢ŠÐs“¡”a..", 80);
   if(!getch()) getch();
   load_image_han(0, 15, "image.dta");

   nputs_han(0, 10, "¶¥œ ÑÁ¡e·¡ ¥¢Š–A´ö¯s“¡Œa? ´a¢ Ç¡a ’Ÿa­A¶a..", 80);
   if(!getch()) getch();
   close_han();
}
