#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

#define MENU_NUMBER     7       /* �A�A�� ���� */

void main(void)
{
   char *string[MENU_NUMBER] = { " �� �i       @N ",
                                 " ���ᵡ��    @O ",
                                 " ��w�a��    @S ",
                                 " �����q�a��  @V ",
                                 " �w���a����     ",
                                 " ���a�w�w�� @F0 ",
                                 " �{          @X "
                               };
   int  current;                /* ��Ȃ�E �A�A�� ��ѡ */
   char key;                    /* �a���b�A ���a�� ǡ */

   if(init_han() != OK_HAN) exit(1);

   aputs_han(33, 19, "�A�A�i  ��Ȃ�a�A�a", FONT_SHADE);

   /* ��q�A�e 0�弁 �A�A�a ��Ȃ�A�� ������ �e ҁ �A�A�i ��Ȃ�h�e�a. */
   current = 0;
   key = popup_han(33, 8, string, MENU_NUMBER, &current);

   clrscr_han();
   switch(key)
   {
      case ESC   : puts_han("<Esc> ǡ�i ���a�v�s���a.");
                   break;
      case LEFT  : puts_han("<Left> ǡ�i ���a�v�s���a.");
                   break;
      case RIGHT : puts_han("<Right> ǡ�i ���a�v�s���a.");
                   break;
      case ENTER : printf_han("<%s> �A�A�i ��Ȃ�a�v�s���a.\n", string[current]);
                   break;
   }
   puts_han(" �a�� ǡ�a ���a�A�a..");

   if(!getch()) getch();
   close_han();
}
