/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          vputch_han(), vaputch_han(), vbputch_han()          */
/*   FILENAME           vputch.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void vputch_han(int x, int y, char *string, int color)
/* (x, y) µA color · ¸å‰w¬‚·a¡ Ðe ˆ· ¢…¸aŸi Â‰bÐe”a. */
{
   if(string[0] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      vput_han_font(x, y, string[0], string[1], color);
   else                 /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
      vput_eng_font(x, y, string[0], color);
}

void vaputch_han(int x, int y, char *string, int color, int attr)
/* (x, y) µA color · ¸å‰w¬‚‰Á attr · ­¢¬÷·a¡ Ðe ˆ· ¢…¸aŸi Â‰bÐe”a. */
{
   int attr2;

   attr2    = attr_han;
   attr_han = attr;
   if(string[0] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      vput_han_font(x, y, string[0], string[1], color);
   else                 /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
      vput_eng_font(x, y, string[0], color);
   attr_han = attr2;
}

void vbputch_han(int x, int y, char *string, int color, int bkcolor)
/* (x, y) µA color · ¸å‰w¬‚‰Á bkcolor · ¤‰w¬‚·a¡ Ðe ˆ· ¢…¸aŸi Â‰bÐe”a. */
{
   int bkcolor2;

   bkcolor2    = bkcolor_han;
   bkcolor_han = bkcolor;
   if(string[0] & 0x80) /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
      vput_han_font(x, y, string[0], string[1], color);
   else                 /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
      vput_eng_font(x, y, string[0], color);
   bkcolor_han = bkcolor2;
}
