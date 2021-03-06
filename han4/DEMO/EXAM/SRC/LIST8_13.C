#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   unsigned char exit_key[] = { F9, F10, 0 }, key;
   char string[41], hanja_code[3], hangul_code[3];
   int ret_value;

   if(init_han() != OK_HAN) exit(1);
   status_on_han();
   puts_han("�e�a�i ���b�h�e �w��i ���a ���e �a���a�������a.");
   puts_han("�a�w�i �� ���e ǡ�e �a�q�� �{�s���a.");
   puts_han("------------------------------------");
   puts_han("1. F9  : �e�i�i �e�a�� �e��");
   puts_han("2. F10 : �e�a�i �e�i�� �e��");
   puts_han("3. adv_gets_han()�� ǡ �a���� �a�w");

   draw_box_han(19, 9, 60, 11, BOX_H1V1);
   strcpy(string, "�e�i�� �e�a");
   while(1)
   {
      key = adv_gets_han(20, 10, string, 40, FONT_NORMAL, exit_key);
      /* adv_gets_han()�� ѡ�E ��ҁ�A adv_gets_han_index�e */
      /* �e���� ��� ��á�A ���e �����i �x�e�a.              */

      if(key == ENTER || key == ESC) break;
      if(key == F9 && (string[adv_gets_han_index] & 0x80))
      /* �e�� ��� ��á�� �i�a�a �e�i�� �� Ё�w�a�e �e�a�i ��Ȃ�h�e�a.*/
      {
         ret_value = choose_hanja(string + adv_gets_han_index, hanja_code);
         if(ret_value == ENTER)
         /* ��Ȃ�E �e�a�� š�a�� �a���a. */
         {
            string[adv_gets_han_index]     = hanja_code[0];
            string[adv_gets_han_index + 1] = hanja_code[1];
         }
      }
      else if(key == F10 && (unsigned char) string[adv_gets_han_index] >= 0xe0)
      /* �e�� ��� ��á�� �i�a�a �e�a�� ���e �e�i�� �a���a. */
      {
         hanja_to_hangul(string + adv_gets_han_index, hangul_code);
         string[adv_gets_han_index]     = hangul_code[0];
         string[adv_gets_han_index + 1] = hangul_code[1];
      }
      else
      /* <F9>, <F10> ǡ�a �១�A�� �g�e �w���A�e ���B�a�i �a���� ���a. */
      /* �b, ��᷁ ��á�i �a���� �g�e�a.                              */
      {
         adv_gets_han_index = adv_gets_han_index * (-1) - 1;
         continue;
      }

      /* <F9>, <F10> ǡ�a �១�E �w�� �a�� adv_gets_han()�� ѡ�A */
      /* �e �A���� ��� ��á�a �e���� ��� ��á �a�� �a�q�� �A���� */
      /* adv_gets_han_index�� �t�i �����e�a.                       */
      /* ���� : adv_gets_han_index = ���a�e ��� ��á * (-1) - 1   */
      adv_gets_han_index = (adv_gets_han_index + 2) * (-1) - 1;
      if(adv_gets_han_index > strlen(string)) adv_gets_han_index = strlen(string);
   }
   clrscr_han();
   cputs_han("���b�h�e �i�a�e <");
   cputs_han(string);
   cputs_han("> �����a. �a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
