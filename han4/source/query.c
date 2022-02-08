/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.1                       */
/*   SUB-TITLE          state querying functions                            */
/*   FILENAME           query.c                                             */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

int get_max_strlen_han(void)
{
   return(max_strlen_han);
}

int get_keyboard_type(void)
{
   return(bul3_mode);
}

int get_scr_buff_code(int x, int y)
{
   return(scr_buff[y][x][0]);
}

int get_scr_buff_id(int x, int y)
{
   return(scr_buff[y][x][1]);
}

int get_scr_buff_attr(int x, int y)
{
   return(scr_buff[y][x][2]);
}

int get_scr_buff_color(int x, int y)
{
   return(scr_buff[y][x][3] & 0x0f);
}

int get_scr_buff_bkcolor(int x, int y)
{
   return((scr_buff[y][x][3] >> 4) & 0x0f);
}

unsigned get_video_seg(void)
{
   return(video_seg);
}

int is_vga_mode(void)
{
   return(in_vga_system);
}

int get_abs_maxx_han(void)
{
   return(abs_maxx_han);
}

int get_abs_maxy_han(void)
{
   return(abs_maxy_han);
}

int get_maxx_han(void)
{
   return(maxx_han);
}

int get_maxy_han(void)
{
   return(maxy_han);
}

int get_minx_han(void)
{
   return(minx_han);
}

int get_miny_han(void)
{
   return(miny_han);
}

int get_curx_han(void)
{
   return(curx_han);
}

int get_cury_han(void)
{
   return(cury_han);
}

int get_attr_han(void)
{
   return(attr_han);
}

int get_color_han(void)
{
   return(color_han);
}

int get_bkcolor_han(void)
{
   return(bkcolor_han);
}

int is_han_mode(void)
{
   return(han_mode);
}

int get_output_offset_han(void)
{
   return(output_offset_han);
}
