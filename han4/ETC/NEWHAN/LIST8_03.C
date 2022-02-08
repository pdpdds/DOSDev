#include <graphics.h>
#include <alloc.h>
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   unsigned size;
   void *screen;

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
   /* (0, 0) - (400, 80) µwµb·i ¸á¸wÐa“e •A Ï©¶aÐe ¡A¡¡Ÿ¡· ´··i sizeµA ¸á¸w */
   size = imagesize(0, 0, 400, 80);
   /* Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”w¤h“e”a. */
   screen = malloc(size);
   if(screen == NULL)
   {
      error_exit_msg = "¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a";
      error_exit_han("main()", 1);
   }
   /* (0, 0) - (400, 80) µwµb·i screen µA ¸á¸wÐe”a. */
   getimage(0, 0, 400, 80, screen);
   clrscr_han();

   nputs_han(0, 10, "´a¢ Ç¡a ’Ÿa¡e ¶¥œ ÑÁ¡e·i ¥¢ŠÐs“¡”a..", 80);
   if(!getch()) getch();
   putimage(0, 0, screen, COPY_PUT);

   nputs_han(0, 10, "¶¥œ ÑÁ¡e·¡ ¥¢Š–A´ö¯s“¡Œa? ´a¢ Ç¡a ’Ÿa­A¶a..", 80);
   if(!getch()) getch();
   free(screen);
   close_han();
}
