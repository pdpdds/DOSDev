#include "hanio.h"

void main(void)
/* child process */
{
   int state;

   return;

   stay_in_gr_mode = 1;
   /* �����A �a�e init_han() �q���i ѡЁ�� �a��Ϣ ������ ��˥�i ����a. */
   if((state = init_han()) != OK_HAN) exit(state);

   /* �t�� ��Ё���� �g�e �e���i�� �t�i ���� �����e�a. */
   in_vga_system = 1;
   video_seg     = 0xa000;
   abs_maxx_han  = maxx_han = 79;
   abs_maxy_han  = maxy_han = 29;

   /* ���a���a���A�� �e�� ���a�a �a���� �a��ᕡ ���a���a���A�e �wз�i ���� */
   /* �g�a�a�� ���a�i �a���� ���� �g�a�e ���a���a���� �A������ �så�a. �a�a */
   /* �� gotoxy_han()�i ѡ�a�a �e�� ���a�i �a���� ���ᴡ �e�a.            */
   gotoxy_han(0, 3);
   puts_han("���a���a���� ��З�A�e �������a.");
   puts_han("�a�� ǡ�a ���a�e ���a���a���a�� ���a�s���a..");
   if(!getch()) getch();

   /* stay_in_gr_mode�a 1���a�� �a��Ϣ ���a�� �� �� ���a�e�a. */
   close_han();
}
