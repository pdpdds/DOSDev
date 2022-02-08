/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          modify_text_attr(), modify_text_color()             */
/*   FILENAME           moditext.c                                          */
/*   DATE & TIME        07/03/92(FRI) 13:30                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void modify_text_attr(int left, int top, int right, int bottom, int attr)
/* ÑÁ¡e· ·©¦¦…· ­¢¬÷·i ¤aŽ…”a. */
{
   int attr2, color2, bkcolor2, x, y;

   attr2    = attr_han;
   color2   = color_han;
   bkcolor2 = bkcolor_han;
   attr_han = attr;

   for(y = top; y <= bottom; y++)
      for(x = left; x <= right; x++)
      {
         /* (x, y) µA Â‰b–E ¢…¸a· ­¢¬÷ e ¤aŽ´á ”a¯¡ Â‰b */
	 color_han   = scr_buff[y][x][3] & 0x0bf;
	 bkcolor_han = (scr_buff[y][x][3] >> 4) & 0x0f;
	 if(scr_buff[y][x][1] == HAN1)
	 {
	    put_han_font(x, y, scr_buff[y][x][0], scr_buff[y][x+1][0]);
	    x++;
	 }
	 else if(scr_buff[y][x][1] == HAN2)
         {
            /* Ðe‹i·¡ º—ˆeµA¬á ¸iv·i ˜“e ´| ¤a·¡ËaŸi ·ª´á¬á Â‰b */
            if(x == left) put_han_font(x - 1, y, scr_buff[y][x-1][0],
                                                 scr_buff[y][x][0]);
         }
	 else
	    put_eng_font(x, y, scr_buff[y][x][0]);
      }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}

void modify_text_color(int left, int top, int right, int bottom, int color)
/* ÑÁ¡e· ·©¦¦…· ¸å‰w¬‚·i ¤aŽ…”a. */
{
   int attr2, color2, bkcolor2, x, y;

   attr2     = attr_han;
   color2    = color_han;
   bkcolor2  = bkcolor_han;
   color_han = color;

   for(y = top; y <= bottom; y++)
      for(x = left; x <= right; x++)
      {
         /* (x, y) µA Â‰b–E ¢…¸a· ¸å‰w¬‚ e ¤aŽ´á ”a¯¡ Â‰b */
	 attr_han    = scr_buff[y][x][2];
	 bkcolor_han = (scr_buff[y][x][3] >> 4) & 0x0f;
	 if(scr_buff[y][x][1] == HAN1)
	 {
	    put_han_font(x, y, scr_buff[y][x][0], scr_buff[y][x+1][0]);
	    x++;
	 }
	 else if(scr_buff[y][x][1] == HAN2)
         {
           /* Ðe‹i·¡ º—ˆeµA¬á ¸iv·i ˜“e ´| ¤a·¡ËaŸi ·ª´á¬á Â‰b */
            if(x == left) put_han_font(x - 1, y, scr_buff[y][x-1][0],
                                                 scr_buff[y][x][0]);
         }
 	 else
	    put_eng_font(x, y, scr_buff[y][x][0]);
      }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}
