#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   char hangul_code[3], hanja_code[3];
   int  ret_code;

   if(init_han() != OK_HAN) exit(1);

   puts_han("�e�a�� �e���i �e�i�i ���b�a�A�a.");

   set_max_strlen_han(2);       /* �A�� 2�a�a�� ���b�h�i �� ������ �e�a. */
   set_to_han_mode();           /* �e�i ���b �wȁ�� ���� */
   disable_han_change();        /* �e/�w ���� ǡ�a �b���A�� �g���� �e�a. */
   gets_han(hangul_code);
   puts_han("\n\n");

   ret_code = choose_hanja(hangul_code, hanja_code);
   switch(ret_code)
   {
      case -1    : puts_han("Ё�w�a�e �e�a�a ���s���a."); break;
      case -2    : puts_han("�A�����a �����s���a."); break;
      case ESC   : puts_han("��Ȃ�i �᭡�a�v�s���a."); break;
      case ENTER : printf_han("<%s> �a�i ��Ȃ�a�v�s���a.\n", hanja_code); break;
   }

   puts_han("\n\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
