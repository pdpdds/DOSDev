#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

#define MENU_NUMBER     7       /* �A�A�� ���� */

void main(void)
{
   menu_struct menu_data =
   {
      MENU_NUMBER, 0, 1, 0, BOX_H1V1, FONT_NORMAL,  33, 8, 50, 16,
   };
   char menu_data_x[MENU_NUMBER] = { 34, 34, 34, 34, 34, 34, 34, },
        menu_data_y[MENU_NUMBER] = {  9, 10, 11, 12, 13, 14, 15, };
   char *(menu_data_string[MENU_NUMBER]) = { " �� �i       @N ",
                                             " ���ᵡ��    @O ",
                                             " ��w�a��    @S ",
                                             " �����q�a��  @V ",
                                             " �w���a����     ",
                                             " ���a�w�w�� @F0 ",
                                             " �{          @X "
                                           };
   char key;                                    /* �a���b�A ���a�� ǡ */

   /* �������A�� �g�e menu_data �a���� �t�i �����e�a. */
   menu_data.x      = menu_data_x;
   menu_data.y      = menu_data_y;
   menu_data.string = menu_data_string;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   aputs_han(33, 19, "�A�A�i  ��Ȃ�a�A�a", FONT_SHADE);
   key = menu_han(&menu_data);

   clrscr_han();
   switch(key)
   {
      case ESC   : puts_han("<Esc> ǡ�i ���a�v�s���a. �a�� ǡ�a ���a�A�a.");
                   break;
      case LEFT  : puts_han("<Left> ǡ�i ���a�v�s���a. �a�� ǡ�a ���a�A�a.");
                   break;
      case RIGHT : puts_han("<Right> ǡ�i ���a�v�s���a. �a�� ǡ�a ���a�A�a.");
                   break;
      case DOWN  : puts_han("<Down> ǡ�i ���a�v�s���a. �a�� ǡ�a ���a�A�a.");
                   break;
      case ENTER : printf_han("<%s> �A�A�i ��Ȃ�a�v�s���a. �a�� ǡ�a ���a�A�a.\n",
                              menu_data.string[menu_data.current]);
                   break;
   }

   if(!getch()) getch();
   close_han();
}
