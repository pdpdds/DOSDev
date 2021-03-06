#include <dir.h>
#include "hanio.h"
#include "extendio.h"

void main(int argc, char **argv)
{
   char drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE], ext[MAXEXT];
   char path[MAXDRIVE+MAXDIR];

   /* argv[0] �A�� �a���a���� ��З�A�e �a�a���a, ���Bɡ���i ���e�a. */
   fnsplit(argv[0], drive, dir, name, ext);
   strcpy(path, drive);
   strcat(path, dir);
   /* BGI ������ ͥ�a �����i �a���a���� ��З�A�e ���Bɡ���A�� */
   /* �x���� �e ҁ init_han()�i ѡ�e�a.                     */
   set_bgi_font_path(path, path);

   /* �e�i �ŉw�a�� �������e ҁ �b�� �e���i�� �t�i �b */
   if(init_han() != OK_HAN)
   {
      printf("Can\'t initialize Hangul environment..\n");
      printf("drive      : %s\n",   drive);
      printf("directory  : %s\n",   dir);
      printf("program    : %s%s\n", name, ext);
      printf("BGI & font : %s\n",   path);
   }
   else
   {
      printf_han("�a�a���a�w : %s\n",   drive);
      printf_han("���Bɡ���w : %s\n",   dir);
      printf_han("��З�����w : %s%s\n", name, ext);
      printf_han("�a���a���� ��З�E ���Bɡ���e %s�����a.\n\n", path);
      puts_han("�a�� ǡ�a ���a�A�a..");
      if(!getch()) getch();
      close_han();
   }
}
