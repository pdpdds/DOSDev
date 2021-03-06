#include "hanio.h"
#include "misc.h"

void main(void)
{
   int gr_mode;

   /* �a��Ϣ ���a�i ��Ȃ�h�e�a.                        */
   /* Q�i ��ȂЖ��a <Esc> �i ���a�e ��З�i ���a�e�a.  */
   gr_mode = choose_gr_mode();
   if(gr_mode == -1) exit(1);

   /* ��Ȃ�E �a��Ϣ ���a���� �������e�a(BGI �����e ϩ�a ���a). */
   if(_init_han(gr_mode) != OK_HAN) exit(2);

   puts_han("�e�w�a�A�a?");

   if(!getch()) getch();
   /* �e�a�� _init_han�A �a�w�e �a��Ϣ ���a�i �a���� _close_han�A ��a   */
   /* ���ᴡ �e�a.                                                       */
   _close_han(gr_mode);
}
