/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          CGA graphics mode setting function                  */
/*   FILENAME           setcga.c                                            */
/*   DATE & TIME        12/21/92 (MON) 13:12                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_gr_mode_cga(int mode)
{
   switch(mode)
   {
      case CGA_320x200x4 : mode = MODE_320x200x4; break;
      case CGA_640x200x2 : mode = MODE_640x200x2; break;
   }
   set_video_mode(mode);
}

