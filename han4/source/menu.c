/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          menu_han()                                          */
/*   FILENAME           menu.c                                              */
/*   DATE & TIME        12/15/91(SUN) 12:25                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int menu_han(menu_struct *menu_data)
/* ¤ñ¶w ¡A“A Ðq® */
{
   char *screen, key, inverse_attr, attr2, quit;
   int i;

   /* ¡A“Aˆa 0 ˆ·¡¡e ¡A“AŸi Îa¯¡Ða»¡ ´g‰¡ ¤a¡ ·³b·i ¤h“e”a. */
   if(menu_data->number == 0)
   {
      if((key = getch_han()) == _ENTER)         /* <Enter> Ç¡ˆa ’‰œá»¡¡e */
	 return(ENTER);
      else if(key == _ESC)                      /* <Esc> Ç¡ˆa ’‰œá»¡¡e */
	 return(ESC);
      else if(!key)                             /* ÑÂ¸wÇ¡ˆa ’‰œá»¡¡e */
      {
	 key = getch_han();
         if(key == LEFT || key == RIGHT) return(key);
      }
      return(0);
   }

   /* ¤b¯a· ¹ÁÎaa ¤b¯a ¤åÑ¡ˆa ¸i¡µ »¡¸÷–A¡e error_exit_han() Ñ¡Â‰ */
   if(menu_data->box_x1 < 0 || menu_data->box_x1 > abs_maxx_han ||
      menu_data->box_x2 < 0 || menu_data->box_x2 > abs_maxx_han ||
      menu_data->box_y1 < 0 || menu_data->box_y1 > abs_maxy_han ||
      menu_data->box_y2 < 0 || menu_data->box_y2 > abs_maxy_han ||
      (menu_data->border_num != -1 && (menu_data->box_x1 - 1 < 0 ||
                                       menu_data->box_x2 + 1 > abs_maxx_han ||
                                       menu_data->box_y2 + 1 > abs_maxy_han)))
   {
      error_exit_msg = "out of range error";
      error_exit_han("menu_han", 1);
   }

   /* save == 1 ·¡¡e ÑÁ¡e·i ¸á¸wÐe”a. */
   if(menu_data->save)
   {
      if(menu_data->border_num == -1)
	 screen = save_text_han(menu_data->box_x1, menu_data->box_y1,
                                menu_data->box_x2, menu_data->box_y2);
      else
	 screen = save_text_han(menu_data->box_x1,     menu_data->box_y1,
                                menu_data->box_x2 + 1, menu_data->box_y2 + 1);
      if(screen == NULL)        /* ¡A¡¡Ÿ¡ˆa ¦¹¢Ða¡e */
      {
         error_exit_msg = "memory is not enough";
	 error_exit_han("menu_han", 1);
      }
   }

   /* ÑÁ¡e·i »¡¶…”a. */
   attr2        = attr_han;
   attr_han     = menu_data->attr;
   inverse_attr = attr_han ^ FONT_INVERSE;
   aclear_text_han(menu_data->box_x1, menu_data->box_y1,
                   menu_data->box_x2, menu_data->box_y2, menu_data->attr);

   /* border_num != -1 ·¡¡e ¡A“A ¤b¯aŸi ‹aŸ¡‰¡ ÉA–Ÿ¡Ÿi Ã¥”a. */
   if(menu_data->border_num != -1)
   {
      output_offset_han = 12;
      for(i = menu_data->box_y1; i <= menu_data->box_y2; i++)
         put_eng_font(menu_data->box_x2 + 1, i, 176);
      for(i = menu_data->box_x1 + 1; i <= menu_data->box_x2 + 1; i++)
         put_eng_font(i, menu_data->box_y2, 176);
      output_offset_han = 0;
      draw_box_han(menu_data->box_x1, menu_data->box_y1,
                   menu_data->box_x2, menu_data->box_y2, menu_data->border_num);
   }

   /* ¡A“A ¢…¸aµi·i Â‰bÐe”a. */
   for(i = 0; i < menu_data->number; i++)
      xputs_han(menu_data->x[i], menu_data->y[i], menu_data->string[i]);
   attr_han = inverse_attr;
   xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
             menu_data->string[menu_data->current]);

   quit = 0;
   while(1)
   {
      /* Ç¡Ÿi ’Ÿa»¡ ´g´a•¡ ¸a•··a¡ ¯©Ð—–A“e ¡A“Aˆa ·¶·a¡e */
      /* ‹a ¡A“Aˆa ¬åÈ‚–A•¡¢ Ða‰¡ ´a“¡¡e Ç¡Ÿi ·³b¤h“e”a.  */
      if(menu_data->auto_menu != 0)
      {
	 attr_han = menu_data->attr;
	 xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
                   menu_data->string[menu_data->current]);
	 menu_data->current = menu_data->auto_menu - 1;
         attr_han = inverse_attr;
	 xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
                   menu_data->string[menu_data->current]);
	 key = _ENTER;
      }
      else
	 key = getch_han();

      if(!key)          /* ÑÂ¸wÇ¡ˆa ’‰œá»¡¡e */
      {
	 key = getch_han();

         /* ¡A“A ·¡•·Ç¡ˆa ’‰œá»¡¡e Ñe¸ ¬åÈ‚–E ¡A“AŸi attr · ­¢¬÷·a¡ Îa¯¡ */
	 if(key == UP || key == DOWN || key == LEFT || key == RIGHT ||
            key == HOME || key == END)
         {
	    attr_han = menu_data->attr;
	    xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
                      menu_data->string[menu_data->current]);
         }
         /* ’‰œá»¥ Ç¡µA ˜aœa Ñe¸ ¬åÈ‚–E ¡A“A· ¤åÑ¡Ÿi ‰i¸÷Ðe”a. */
	 switch(key)
	 {
	    case UP    : if(menu_data->vertical) menu_data->current = (menu_data->current - 1 + menu_data->number) % menu_data->number;
	                 break;
	    case DOWN  : if(!menu_data->vertical)
	                    quit = 1;
	                 else
	                    menu_data->current = (menu_data->current + 1) % menu_data->number;
	                 break;
	    case LEFT  : if(menu_data->vertical)
	                    quit = 1;
	                 else
	                    menu_data->current = (menu_data->current - 1 + menu_data->number) % menu_data->number;
	                 break;
	    case RIGHT : if(menu_data->vertical)
	                    quit = 1;
	                 else
	                    menu_data->current = (menu_data->current + 1) % menu_data->number;
	                 break;
	    case HOME  : menu_data->current = 0;
	                 break;
	    case END   : menu_data->current = menu_data->number - 1;
	                 break;
	 }
         /* ¬¡ ¬åÈ‚–E ¡A“AŸi inverse_attr · ­¢¬÷·a¡ Îa¯¡ */
         attr_han = inverse_attr;
	 xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
                   menu_data->string[menu_data->current]);
	 if(quit) break;
      }
      else              /* ·©¤eÇ¡ˆa ’‰œá»¡¡e */
      {
	 if(key == _ENTER)
         {
            key = ENTER;
	    break;
         }
	 else if(key == _ESC)
         {
            key = ESC;
	    break;
         }
	 else if(menu_data->key != NULL)
         /* ÐuÇ¡ˆa »¡¸÷–A´á ·¶·a¡e ÀáŸ¡Ðe”a. */
         {
            for(i = 0; i < menu_data->number; i++)
               if(key == menu_data->key[i]) break;
            if(i < menu_data->number)
            {
	       attr_han = menu_data->attr;
               xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
                         menu_data->string[menu_data->current]);
               menu_data->current = i;
               attr_han = inverse_attr;
               xputs_han(menu_data->x[menu_data->current], menu_data->y[menu_data->current],
                         menu_data->string[menu_data->current]);
               key = ENTER;
               break;
            }
         }
      }
   }

   /* ¸a•··a¡ ¯©Ð—–A•¡¢ »¡¸÷–E Ðq®Ÿi ¯©Ð—Ðe”a. */
   attr_han = attr2;
   if(key == ENTER && menu_data->func != (void (**) (void)) NULL &&
      menu_data->func[menu_data->current] != (void (*) (void)) NULL)
      (menu_data->func[menu_data->current])();

   /* save == 1 ·¡¡e ÑÁ¡e·i ¥¢ŠÐe”a. */
   if(menu_data->save) restore_text_han(menu_data->box_x1, menu_data->box_y1, screen);

   return(key);
}
