/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          default_han_change_func(),                          */
/*                      default_ins_change_func()                           */
/*   FILENAME           default.c                                           */
/*   DATE & TIME        03/11/92(WED) 19:17                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void default_han_change_func(void)
/* Ğe/µw ¸åÑÅÇ¡ˆa ’‰œá»© ˜ Ñ¡Â‰–A“e Ğq®(¸÷–E Ğq®) */
{
   int attr2, color2, bkcolor2;

   attr2       = attr_han;
   color2      = color_han;
   bkcolor2    = bkcolor_han;
   attr_han    = FONT_INVERSE;
   color_han   = LIGHTGRAY;
   bkcolor_han = BLACK;

   if(is_status_on_han)
   /* ¬wÈ œa·¥·¡ Îa¯¡–A´á ·¶·a¡e ¬wÈ œa·¥µA ·³b ¬wÈŸi Îa¯¡ */
   {
      if(han_mode)
      {
	 put_han_font(3, abs_maxy_han, 'Ğ', 'e');	/* "Ğe" */
	 put_han_font(5, abs_maxy_han, '‹', 'i');	/* "‹i" */
      }
      else
      {
	 put_han_font(3, abs_maxy_han, 'µ', 'w');	/* "µw" */
	 put_han_font(5, abs_maxy_han, '¢', '…');	/* "¢…" */
      }
   }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;

   sound(330); delay(50);
   sound(262); delay(50);
   nosound();
}

void default_ins_change_func(void)
/* ¬s·³/®¸÷ ¸åÑÅÇ¡ˆa ’‰œá»© ˜ Ñ¡Â‰–A“e Ğq®(¸÷–E Ğq®) */
{
   int attr2, color2, bkcolor2;

   attr2       = attr_han;
   color2      = color_han;
   bkcolor2    = bkcolor_han;
   attr_han    = FONT_INVERSE;
   color_han   = LIGHTGRAY;
   bkcolor_han = BLACK;

   if(is_status_on_han)
   /* ¬wÈ œa·¥·¡ Îa¯¡–A´á ·¶·a¡e ¬wÈ œa·¥µA ·³b ¬wÈŸi Îa¯¡ */
   {
      if(ins_mode)
      {
	 put_han_font(18, abs_maxy_han, '¬', 's');	/* "¬s" */
	 put_han_font(20, abs_maxy_han, '·', '³');	/* "·³" */
      }
      else
      {
	 put_han_font(18, abs_maxy_han, '®', '');	/* "®" */
	 put_han_font(20, abs_maxy_han, '¸', '÷');	/* "¸÷" */
      }
   }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}
