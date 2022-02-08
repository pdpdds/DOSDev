/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          graphics mode setting function                      */
/*   FILENAME           setgr.c                                             */
/*   DATE & TIME        12/21/92 (MON) 13:12                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_gr_mode(int mode)
{
   static int info[][3] =
   {
      { 640, 400 }, {  640, 400 }, {  720, 348 }, { 720, 348 },
      { 640, 200 }, {  320, 200 }, {  640, 350 }, { 320, 200 },
      { 640, 200 }, {  640, 480 }, {  640, 400 }, { 640, 480 },
      { 640, 480 }, {  640, 400 }, {  800, 600 },
      { 800, 600 }, { 1024, 768 }, { 1024, 768 }
   };

   width_gr      = info[mode][0];
   height_gr     = info[mode][1];

   byte_per_line = width_text = width_gr >> 3;
   height_text	 = height_gr >> 4;

   switch(mode)
   {
      case HGC_640x400x2_0   :
      case HGC_640x400x2_1   :
      case HGC_720x348x2_0   :
      case HGC_720x348x2_1   : set_gr_mode_hgc(mode); break;
      case CGA_320x200x4     :
      case CGA_640x200x2     : set_gr_mode_cga(mode); break;
      case EGA_320x200x16    :
      case EGA_640x200x16    :
      case EGA_640x350x16    : set_gr_mode_ega(mode); break;
      case VGA_640x400x16    :
      case VGA_640x480x2     :
      case VGA_640x480x16    : set_gr_mode_vga(mode); break;
      case SVGA_640x400x256  :
      case SVGA_640x480x256  :
      case SVGA_800x600x16   :
      case SVGA_800x600x256  :
      case SVGA_1024x768x16  :
      case SVGA_1024x768x256 : set_gr_mode_svga(mode); break;
   }
   gr_mode = mode;
}
