#include "hanio.h"
#include "extendio.h"

/* Ðq® ¶¥Ñw */
void draw_border(int left, int top, int right, int bottom, int is_relief);
void draw_3dbox(int left, int top, int right, int bottom, int color);

void main(void)
{
   int left, top, i;
   if(init_han() != OK_HAN) exit(1);

   vclrscr_han(LIGHTBLUE);
   /* 16¬‚·i ¬a¶wÐaµa 16ˆ· ·³ÁA ¤b¯aŸi ‹aŸ¥”a. */
   for(i = 0; i < 16; i++)
   {
      left = (i % 4) * 20;
      top  = (i / 4) * 6;
      draw_3dbox(left, top, left + 15, top + 4, i);
      set_vbcolor_han((i + 8) % 16, i);
      xprintf_han(left + 7, top + 2, "%2d", i);
   }
   xputs_han(0, get_abs_maxy_han(), "´a¢ Ç¡a ’Ÿa­A¶a..");

   if(!getch()) getch();
   close_han();
}

void draw_border(int left, int top, int right, int bottom, int is_relief)
/* EGA/VGAµA¬á ·³ÁA ¤b¯aŸi  e—a“e •A Ï©¶aÐe ÉA–Ÿ¡Ÿi ‹aŸ¡“e Ðq®  */
/* left, top, right, bottom·e ¡¡– ‹aœÏ¢ ¹ÁÎa”a.                 */
/* is_reliefˆa 1·¡¡e ´·ˆb, 0·¡¡e ·qˆb·a¡ ÉA–Ÿ¡Ÿi ‹aŸ¥”a.        */
/* draw_3dbox()µA ·Ð Ñ¡Â‰–A“e Ðq®”a.                           */
{
   int width, height;

   width  = right - left + 1;
   height = bottom - top + 1;
   if(is_relief)
   {
      draw_horiz_line(left,     top,        width,     WHITE);
      draw_horiz_line(left,     bottom,     width,     DARKGRAY);
      draw_horiz_line(left + 1, bottom - 1, width - 2, DARKGRAY);

      draw_vert_line(left,       top,     height,     WHITE);
      draw_vert_line(left + 1,   top + 1, height - 2, WHITE);
      draw_vert_line(right,      top,     height,     DARKGRAY);
      draw_vert_line(right - 1,  top + 1, height - 2, DARKGRAY);
   }
   else
   {
      draw_horiz_line(left,     top,        width,     DARKGRAY);
      draw_horiz_line(left,     bottom,     width,     WHITE);
      draw_horiz_line(left + 1, bottom - 1, width - 2, WHITE);

      draw_vert_line(left,      top,     height,     DARKGRAY);
      draw_vert_line(left + 1,  top + 1, height - 2, DARKGRAY);
      draw_vert_line(right,     top,     height,     WHITE);
      draw_vert_line(right - 1, top + 1, height - 2, WHITE);
   }
}

void draw_3dbox(int left, int top, int right, int bottom, int color)
/* ¸é” ¹ÁÎa¡ (left, top) - (right, bottom) µwµbµA color¡ »¡¸÷Ðe ¬‚·i  */
/* ·¡¶wÐ ·³ÁA ¤b¯aŸi ‹aŸ¡“e Ðq®                                        */
{
   int x1, y1, x2, y2, width, height;

   /* left, top, right, bottomµA Ð”wÐa“e ‹aœÏ¢ ¹ÁÎaŸi ‰¬e  */
   x1     = left << 3;
   y1     = top  << 4;
   x2     = (right  << 3) + 7;
   y2     = (bottom << 4) + 15;
   width  = x2 - x1 + 1 - 8;
   height = y2 - y1 + 1 - 16;

   if(in_vga_system)
   /* EGA/VGA ·¥ ‰w¶(in_vga_system·e ¸åµb ¥e®) */
   {
      vclear_text_han(left, top, right, bottom, color);
      draw_border(x1,     y1,     x2,     y2,     1);
      draw_border(x1 + 4, y1 + 8, x2 - 4, y2 - 8, 0);
   }
   else
   {
      draw_vert_line(x1+4,   y1+8,   height, WHITE);
      draw_vert_line(x2-4,   y1+8,   height, WHITE);
      draw_vert_line(x2+1-4, y1+1+8, height, WHITE);
      draw_vert_line(x2+2-4, y1+2+8, height, WHITE);

      draw_horiz_line(x1+4,   y1+8,   width,  WHITE);
      draw_horiz_line(x1+4,   y2-8,   width,  WHITE);
      draw_horiz_line(x1+1+4, y2+1-8, width,  WHITE);
      draw_horiz_line(x1+2+4, y2+2-8, width,  WHITE);
   }
}
