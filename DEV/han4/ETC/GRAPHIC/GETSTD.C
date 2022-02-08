#include "graph.h"

int get_standard_mode(int adapter)
{
   static unsigned char standard_mode[] =
   {
      -1, -1, HGC_640x400x2_0, CGA_640x200x2, EGA_640x350x16,
      VGA_640x480x16, -1
   };
   if(adapter >= ET3000)
      return(SVGA_640x480x256);
   else
      return(standard_mode[adapter]);
}

int is_avail_mode(int adapter, int mode)
{
   /* number of avaliable modes */
   static unsigned char usable_mode[] =
   {
      0, 0, 4, 2, 3, 3, 0,
   };
   int start, end;

   start = get_standard_mode(adapter);
   end = start + ((adapter >= ET3000) ? 6 : usable_mode[adapter]);
   if(start >= EGA_640x350x16) start = CGA_640x200x2;

   return(mode >= start && mode < end);
}
