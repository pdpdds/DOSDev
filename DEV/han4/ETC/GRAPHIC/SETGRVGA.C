/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          VGA graphics mode setting function                  */
/*   FILENAME           setvga.c                                            */
/*   DATE & TIME        12/21/92 (MON) 13:12                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_gr_mode_vga(int mode)
{
   switch(mode)
   {
      case VGA_640x400x16 : mode = MODE_640x480x16; break;
      case VGA_640x480x2  : mode = MODE_640x480x2;  break;
      case VGA_640x480x16 : mode = MODE_640x480x16; break;
   }
   set_video_mode(mode);
}
