#include "hanio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* ¬a¶w¸a ¸÷· ¤b¯a· ¡¡´··i ¸÷·Ðe”a. user_defined_box_shape“e ¸åµb ¥e® */
   user_defined_box_shape = "012345";

   /*  Ça¡ ¬w®¡w·i Îa¯¡Ða¡e¬á ¤b¯aŸi ‹aŸ¥”a. */
   for(i = 0; i < 5; i++)
   {
      draw_box_han(i*15, 1, 10+i*15, 6, i);
      switch(i)
      {
         case 0 : xputs_han( 1, 0, "BOX_H1V1");
                  break;
         case 1 : xputs_han(16, 0, "BOX_H1V2");
                  break;
         case 2 : xputs_han(31, 0, "BOX_H2V1");
                  break;
         case 3 : xputs_han(46, 0, "BOX_H2V2");
                  break;
         case 4 : xputs_han(58, 0, "USER_DEFINED_BOX");
                  break;
      }
   }

   /* µb¬w·a¡ ¤b¯aŸi ‹aŸ¥”a. ¦“e aclear_text_han() Ðq®¡ »¡¶¡ º´á´¡ */
   /* Ðe”a. Ñe¸ ­¢¬÷·¡ FONT_INVERSE¡ ¬é¸÷–A´ö”áœa•¡ clear_text_han() Ðq */
   /* ®Ÿi ¬a¶wÐa¡e FONT_NORMAL¡  »¡¶‹¡ ˜¢…µA  clear_text_han() Ðq®¡ */
   /* »¡¶¡e ´e –E”a.(aclear_text_han Àq¹¡)                               */
   set_attr_han(FONT_INVERSE);
   for(i = 0; i < 5; i++)
   {
      draw_box_han(i*15, 7, 10+i*15, 12, i);
      aclear_text_han(i*15+1, 7+1, 10+i*15-1, 12-1, FONT_INVERSE);
   }

   /* ¦…Ñ··a¡ ¤b¯aŸi ‹aŸ¥”a.  ¦“e vclear_text_han() Ðq®¡  »¡¶¡ º´á´¡ */
   /* Ðe”a. Ñe¸ ¬‚·¡ LIGHTMAGENTA ¡ ¬é¸÷–A´ö”áœa•¡ clear_text_han()  Ðq® */
   /* Ÿi ¬a¶wÐa¡e BLACK ·a¡ »¡¶‹¡ ˜¢…µA clear_text_han()  Ðq®¡  »¡¶¡e */
   /* ´e –E”a.(vclear_text_han Àq¹¡)                                        */
   set_color_han(LIGHTMAGENTA);
   for(i = 0; i < 5; i++)
   {
      draw_box_han(i*15, 13, 10+i*15, 18, i);
      vclear_text_han(i*15+1, 13+1, 10+i*15-1, 18-1, LIGHTMAGENTA);
   }

   aputs_han(0, 20, "´a¢ Ç¡a ’Ÿa­A¶a..", FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
