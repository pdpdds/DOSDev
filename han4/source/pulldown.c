/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          pulldown_han()                                      */
/*   FILENAME           pulldown.c                                          */
/*   DATE & TIME        04/02/92(THU) 21:15                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int pulldown_han(pulldown_menu_struct *menu_data)
/* Î‰”a¶… ¡A“A Ðq® */
{
   char *screen, attr2;
   int  ret_code, i;

   /* º¡A“Aˆa Îa¯¡–A“e ¹A·© Àõ º‰‰Á Ñe¸ ­¢¬÷·i ¸á¸w */
   if((screen = save_text_han(0, 0, abs_maxx_han, 0)) == NULL)
   {
      error_exit_msg = "memory is not enough";
      error_exit_han("pulldown_han", 1);
   }
   attr2 = attr_han;

   /* º¡A“AŸi Îa¯¡ */
   attr_han = FONT_INVERSE;
   nputs_han(0, 0, "", abs_maxx_han + 1);
   for(i = 0; i < menu_data->number; i++)
      xputs_han(menu_data->x[i], 0, menu_data->string[i]);

   while(1)
   {
      /* Ñe¸ º¡A“AŸi FONT_NORMAL · ­¢¬÷·a¡ Îa¯¡ */
      attr_han = FONT_NORMAL;
      xputs_han(menu_data->x[menu_data->current], 0,
                menu_data->string[menu_data->current]);

      ret_code = popup_han(menu_data->sub_menu[menu_data->current].box_x,
			   menu_data->sub_menu[menu_data->current].box_y,
			   menu_data->sub_menu[menu_data->current].string,
			   menu_data->sub_menu[menu_data->current].number,
			   &(menu_data->sub_menu[menu_data->current].current));
      if(ret_code == ESC || ret_code == ENTER) break;

      /* Ñe¸ º¡A“AŸi FONT_INVERSE · ­¢¬÷·a¡ Îa¯¡ */
      attr_han = FONT_INVERSE;
      xputs_han(menu_data->x[menu_data->current], 0,
		menu_data->string[menu_data->current]);

      /* ’‰œá»¥ Ç¡µA ˜aœa Ñe¸ ¬åÈ‚–E ¡A“A· ¤åÑ¡Ÿi ‰i¸÷Ðe”a. */
      if(ret_code == LEFT)
	 menu_data->current = (menu_data->current - 1 + menu_data->number) % menu_data->number;
      else if(ret_code == RIGHT)
	 menu_data->current = (menu_data->current + 1) % menu_data->number;
   }

   /* ÑÁ¡e‰Á ­¢¬÷·i ¥¢ŠÐe”a. */
   attr_han = attr2;
   restore_text_han(0, 0, screen);

   return(ret_code);
}
