/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_to_han_mode(), set_to_eng_mode(),               */
/*                      enable_han_change(), disable_han_change(),          */
/*                      get_keyboard_type(), set_keyboard_type(),           */
/*                      get_video_seg(), is_han_mode(), is_vga_mode()       */
/*   FILENAME           mode.c                                              */
/*   DATE & TIME        03/11/92(WED) 18:52                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_to_han_mode(void)
/* Ðe‹i ·³b ¬wÈ¡ ¸åÑÅÐe”a. */
{
   han_mode = 1;
}

void set_to_eng_mode(void)
/*  µw¢… ·³b ¬wÈ¡ ¸åÑÅÐe”a. */
{
   han_mode = 0;
}

void enable_han_change(void)
/* Ðe/µw ¸åÑÅÇ¡ˆa ¸b•·–A•¡¢ Ðe”a. */
{
   able_to_change_han = 1;
}

void disable_han_change(void)
/* Ðe/µw ¸åÑÅÇ¡ˆa ¸b•·–A»¡ ´g•¡¢ Ðe”a. */
{
   able_to_change_han = 0;
}

int get_keyboard_type(void)
/* ¸aÌe· ¹·ŸAŸi Ÿ¡ÈåÐe”a. */
{
   return(bul3_mode);
}

void set_keyboard_type(int type)
/* ¸aÌe· ¹·ŸAŸi ¬é¸÷Ðe”a. */
{
   bul3_mode = (type == BUL3_TYPE) ? 1 : 0;
}

unsigned get_video_seg(void)
/* ¢…¸aa ‹aœÏ¢·¡ Â‰b–A“e §¡—¡µ¡œ‘· ­A‹a¡EËaˆt·i Ÿ¡ÈåÐe”a. */
{
   return(video_seg);
}

int is_han_mode(void)
/* Ðe‹i ·³b ¬wÈ·¡¡e 1 ·i Ÿ¡ÈåÐe”a. */
{
   return(han_mode);
}

int is_vga_mode(void)
/* EGA/VGA ¡¡—a¡ Á¡‹¡ÑÁ–A´ö·a¡e 1 ·i Ÿ¡ÈåÐe”a. */
{
   return(in_vga_system);
}

