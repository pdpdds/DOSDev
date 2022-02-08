/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          popup_han()                                         */
/*   FILENAME           popup.c                                             */
/*   DATE & TIME        03/14/92(SAT) 19:40                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int popup_han(int x, int y, char **string, int number, int *current)
/* �s�� �A�A �q�� */
{
   char *screen, key, attr2, quit;
   int i, length, x2, y2;

   /* �A�A�a 0 �����e �A�A�i �a���a�� �g�� �a�� ���b�i �h�e�a. */
   if(number == 0)
   {
      if((key = getch_han()) == _ENTER)         /* <Enter> ǡ�a ���ỡ�e */
	 return(ENTER);
      else if(key == _ESC)                      /* <Esc> ǡ�a ���ỡ�e */
	 return(ESC);
      else if(!key)                             /* �¸wǡ�a ���ỡ�e */
      {
	 key = getch_han();
	 if(key == LEFT || key == RIGHT) return(key);
      }
      return(0);
   }

   /* �A�A �b�a�� �A�� �a�e ���e�� ���១ ���a�i ���e�e�a. */
   length = strlen(string[0]);
   for(i = 0; i < number; i++)
      if(strlen(string[i]) > length) length = strlen(string[i]);
   x2 = x + length + 1;
   y2 = y + number + 1;

   /* �b�a�� ���a�a �b�a ��ѡ�a �i�� �����A�e error_exit_han() ѡ */
   if(x < 0 || x > abs_maxx_han || x2 < 0 || x2 > abs_maxx_han ||
      y < 0 || y > abs_maxy_han || y2 < 0 || y2 > abs_maxy_han)
   {
      error_exit_msg = "out of range error";
      error_exit_han("popup_han", 1);
   }

   /* ���e�� �e�� �����i ��w�e�a. */
   screen = save_text_han(x, y, x2 + 1, y2 + 1);
   if(screen == NULL)
   {
      error_exit_msg = "memory is not enough";
      error_exit_han("popup_han", 1);
   }
   attr2    = attr_han;
   attr_han = FONT_NORMAL;

   /* ���e�i ���� �a�q �A�A �b�a�i �a���� �A�����i å�a. */
   clear_text_han(x, y, x2, y2);
   output_offset_han = 12;
   for(i = y;     i <= y2;     i++) put_eng_font(x2 + 1, i,  176);
   for(i = x + 1; i <= x2 + 1; i++) put_eng_font(i,      y2, 176);
   output_offset_han = 0;
   draw_box_han(x, y, x2, y2, BOX_H1V1);

   /* �A�A ���a�i�i �b�e�a. */
   for(i = 0; i < number; i++) xputs_han(x + 1, y + 1 + i, string[i]);
   attr_han = FONT_INVERSE;
   xputs_han(x + 1, y + 1 + *current, string[*current]);
   attr_han = FONT_NORMAL;

   quit = 0;
   while(1)
   {
      key = getch_han();
      if(!key)          /* �¸wǡ�a ���ỡ�e */
      {
	 key = getch_han();

         /* �A�A ����ǡ�a ���ỡ�e �e�� ��Ȃ�E �A�A�i */
         /* FONT_NORMAL �� �����a�� �a��              */
	 if(key == UP || key == DOWN || key == LEFT || key == RIGHT ||
            key == HOME || key == END)
	    xputs_han(x + 1, y + 1 + *current, string[*current]);
         /* ���ụ ǡ�A �a�a �e�� ��Ȃ�E �A�A�� ��ѡ�i �i���e�a. */
	 switch(key)
	 {
	    case UP    : *current = (number + *current - 1) % number;
			 break;
	    case LEFT  : quit = 1;
			 break;
	    case DOWN  : *current = (*current + 1) % number;
			 break;
	    case RIGHT : quit = 1;
			 break;
	    case HOME  : *current = 0;
			 break;
	    case END   : *current = number - 1;
			 break;
	 }
         /* ���� ��Ȃ�E �A�A�i FONT_INVERSE �� �����a�� �a�� */
	 attr_han = FONT_INVERSE;
	 xputs_han(x + 1, y + 1 + *current, string[*current]);
	 attr_han = FONT_NORMAL;
	 if(quit) break;
      }
      else              /* ���eǡ�a ���ỡ�e */
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
      }
   }

   /* ���e�� �����i �����e�a. */
   attr_han = attr2;
   restore_text_han(x, y, screen);

   return(key);
}
