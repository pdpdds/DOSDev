/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.1                       */
/*   SUB-TITLE          environment setting functions                       */
/*   FILENAME           setenv.c                                            */
/*   DATE & TIME        03/11/92(WED) 18:52                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

void set_han_change_key(int code, int is_extend, int flag)
/* defines hangul/english changing key */
{
   han_change_code      = code;
   han_change_flag      = flag;
   is_han_change_extend = is_extend;
}

void set_han_change_func(void (*func) (void))
/* sets auto-executing function when hangul/english change occurs */
{
   han_change_func = func;
}

void set_ins_change_func(void (*func) (void))
/* sets auto-executing function when insert/edit change occurs */
{
   ins_change_func = func;
}

void set_han_wait_func(void (*func) (void))
/* sets auto-executing function while CPU waits for a keypress */
{
   han_wait_func = func;
}

void set_attr_han(int attr)
/* sets the attribute of the character displayed */
{
   attr_han = attr;
}

void set_color_han(int color)
/* sets foreground color of the character displayed */
{
   color_han = color;
}

void set_bkcolor_han(int color)
/* sets the background color of the character displayed */
{
   bkcolor_han = color;
}

void set_vbcolor_han(int color, int bkcolor)
/* sets foreground,background color of the character displayed */
{
   color_han = color;
   bkcolor_han = bkcolor;
}

void set_output_offset_han(int output_offset)
/* sets output offset of the character displayed */
{
   output_offset_han = output_offset;
}

void set_to_han_mode(void)
/* sets input mode to hangul mode */
{
   han_mode = 1;
}

void set_to_eng_mode(void)
/* sets input mode to english mode */
{
   han_mode = 0;
}

void enable_han_change(void)
/* enables hangul/english changing */
{
   able_to_change_han = 1;
}

void disable_han_change(void)
/* disables hangul/english changing */
{
   able_to_change_han = 0;
}

void set_twinkle_time(int twinkle_time)
/* sets cursor twinkle time, must be in the range from 0 to 255 */
{
   if(twinkle_time < 0 || twinkle_time > 255) return;
   cursor_twinkle_time = twinkle_time;
}

void set_max_strlen_han(int length)
/* sets the number of the maximum acceptable characters */
{
   if(length <= MAX_STRLEN) max_strlen_han = length;
}

void set_keyboard_type(int type)
/* sets keyboard type */
{
   bul3_mode = (type == BUL3_TYPE) ? 1 : 0;
}

void set_bgi_font_path(char *bgi_path, char *font_path)
/* sets "*.BGI", "*.FNT" path */
{
   bgi_path_han  = bgi_path;
   font_path_han = font_path;
}

