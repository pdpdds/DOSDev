/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          EGA graphics mode setting function                  */
/*   FILENAME           setega.c                                            */
/*   DATE & TIME        12/21/92 (MON) 13:12                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_gr_mode_ega(int mode)
{
   switch(mode)
   {
      case EGA_320x200x16 : mode = MODE_320x200x16; break;
      case EGA_640x200x16 : mode = MODE_640x200x16; break;
      case EGA_640x350x16 : mode = MODE_640x350x16; break;
   }
   set_video_mode(mode);
}
