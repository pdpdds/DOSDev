#include "hanio.h"
#include "misc.h"

void main(void)
{
   int gr_mode;

   /* �a��Ϣ ���a�i ��Ȃ�h�e�a.                        */
   /* Q�i ��ȂЖ��a <Esc>�i ���a�e ��З�i ���a�e�a.   */
   gr_mode = choose_gr_mode();
   if(gr_mode == -1) exit(1);

   /* ���e �wȁ�i ��Ȃ�E �a��Ϣ ���a�� �a���a.           */
   /* ���� ���e�e �������� �g�e�a(BGI �����e ϩ�a ���a). */
   set_to_gr_mode(gr_mode);
   if(!getch()) getch();

   /* ���e �wȁ�i �a�� �B�a�a ���a�� �a���a.               */
   /* �e�a�� set_to_gr_mode�A �a�w�e �a��Ϣ ���a�i �a����  */
   /* set_to_text_mode�A ��a ���ᴡ �e�a.                 */
   set_to_text_mode(gr_mode);
   puts("\n\nagain in text mode..");
}
