#include <process.h>
#include "hanio.h"

void main(void)
/* parent process */
{
   int state;

   if(init_han() != OK_HAN) exit(1);
   /* ���a���a���A��e �a��Ϣ �������i �e �� �e�a. */

   puts_han("���a���a���� ��З�A�e �������a.");
   puts_han("�a�� ǡ�a ���a�e ���a���a���� ��З�S���a.");
   if(!getch()) getch();

   if((state = spawnl(P_WAIT, "exami_02.exe", NULL)) != 0)
   /* �a��Ϣ ���a�� �� �� exami_02.exe�i ��З�e�a. */
   /* �A��a �i���a�e ���� �A�����a �����e �����a. */
   {
      error_exit_msg = "exami_02.exe�i ��З�i �� ���s���a";
      error_exit_han("exami_02.exe�� main()", 1);
   }

   /* ���a���a���A�� �e�� ���a�a �a���� �a��ᕡ */
   /* ���a���a���A�e �wз�i ���� �g�a�a�� ���a�i */
   /* �a���� ���� �g�a�e ���a���a����   �A������ */
   /* �så�a.  �a�a�� gotoxy_han()�i ѡ�a�a �e */
   /* �� ���a�i �a���� ���ᴡ �e�a.              */
   gotoxy_han(0, 6);
   puts_han("���a���a���a�� ���a �֯s���a. �a�� ǡ�a á�A�a..");
   if(!getch()) getch();
   close_han();
}
