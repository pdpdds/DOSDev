#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

void main(void) 
{
   FILE *stream;
   char bit_map[96];            /* 6x16 = 96 bytesˆa Ï©¶aÐa”a. */

   /* ÉA–Ÿ¡ ¢…¸a· Í¥ËaŸi ·ª´á —i·¥”a. */
   if((stream = fopen("examd_06.box", "rb")) == NULL)
   {
      puts("file not found --- examd_06.box");
      exit(2);
   }
   if(fread(bit_map, 16, 6, stream) != 6)
   {
      puts("READ error");
      fclose(stream);
      exit(3);
   }
   fclose(stream);

   if(init_han() != OK_HAN) exit(1);

   /* draw_bit_map_box_han() Ðq®¡ ¤b¯aŸi ‹aŸ¥”a. */
   draw_bit_map_box_han(0, 5, 30, 15, bit_map);

   /* µb¬w·a¡ ¤b¯aŸi ‹aŸ¥”a. */
   set_attr_han(FONT_INVERSE);
   draw_bit_map_box_han(35, 7, 65, 17, bit_map);
   /* FONT_SHADE¡ ÑÁ¡e·i »¡¶…”a */
   aclear_text_han(36, 8, 64, 16, FONT_SHADE);

   aputs_han(0, get_maxy_han(), "´a¢ Ç¡a ’Ÿa­A¶a..", FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
