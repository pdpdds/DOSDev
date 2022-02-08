#include "hanio.h"
#include <dos.h>

void main(void)
{
   union REGS inr, outr;
   int state;

      inr.h.ah = 0x0f;
      int86(0x10, &inr, &outr);
      inr.h.ah = 0x00;
      inr.h.al = 0x12;
      int86(0x10, &inr, &outr);
   stay_in_gr_mode = 1;
   if((state = _init_han(INIT_VGA)) != OK_HAN) exit(state);
   in_vga_system = 1;
   video_seg = 0xa000;
   abs_maxx_han = maxx_han = 79;
   abs_maxy_han = maxy_han = 29;
   curx_han = 0; cury_han = 5;
   puts_han("This is sub program.");
   puts_han("Type any key..");
   _close_han(INIT_VGA);
}
