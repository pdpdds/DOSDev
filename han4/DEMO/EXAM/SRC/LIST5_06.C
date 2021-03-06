#include <ctype.h>
#include "hanio.h"
#include "extendio.h"

#define MENU_NUMBER     6       /* �A�A�� ���� */

/* static_menu() �q���� ���w */
int static_menu(void);

void main(void)
{
   int menu;                                    /* ��Ȃ�E �A�A�� ��ѡ */

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   aputs_han(33, 19, "�A�A�i  ��Ȃ�a�A�a", FONT_SHADE);
   menu = static_menu();                        /* �A�A�i ��Ȃ�h�e�a. */

   clrscr_han();
   printf_han("%d �� �A�A�i ��Ȃ�a�v�s���a. �a�� ǡ�a ���a�A�a.\n", menu);
   if(!getch()) getch();

   close_han();
}

int static_menu(void)
/* �A�A�i ��Ȃ�h�e �q�� */
{
   char *menu[MENU_NUMBER] = { "���� ����    L",        /* �A�A ���a�i */
                               "���� ��w    S",
                               "�� ����      N",
                               "���Bɡ��     D",
                               "���a��       O",
                               "�{           Q"
                             };
   char hot_key[MENU_NUMBER] = { 'L', 'S', 'N', 'D', 'O', 'Q' }; /* �u ǡ */
   char key;
   int i;

   /* �A�A ���a�i�i �a���e ҁ �b�a�� �A�����i å�a. */
   for(i = 0; i < MENU_NUMBER; i++) xputs_han(35, 10 + i, menu[i]);
   draw_box_han(33, 9, 50, 16, BOX_H1V1);

   do
   {
      key = getch();
      if(key == 0) /* �¸w ǡ�e �����e�a. */
      {
         getch();
         continue;
      }

      /* �a�e ǡ�a ���v�e�� ��a�e�a. */
      /* �១�i �w���a�A �a�� ��Ё key�i �����a�� �១�e�a. */
      for(i = 0; i < MENU_NUMBER; i++) if(toupper(key) == hot_key[i]) break;
      if(i < MENU_NUMBER) break; /* �a�e ǡ�a ���v�a�e ���a�i �a�a �a�e�a. */
   }
   while(1);    /* ���eӡ ���a�i �e���e�a. */

   return(i);   /* ��Ȃ�E �A�A�� ��ѡ�i �����e�a. */
}
