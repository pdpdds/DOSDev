
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *string = "�������e ���� ��, �b�a�i �e ���a�a ��a ���a�v�e���� ���� "
                  "�g�v�a. �a�e \"�� �b���w�i�� ��i �a�e ���������a �a�i��"
                  "�a�A �a�q�a�� ���i �w�ⷡ �{�e ���e�i�� ��ɷ���i �����A ��"
                  "�a �񋩈�a\"�� ��a���� �a �A������ �q�e �១�A �ᵩ�v�i "
                  "�������a. �a��a �b�� �a�t�e ���b�� �i���a.";
   char buff[51];
   int index = 0, buff_index;

   if(init_han() != OK_HAN) exit(1);

   while(string[index] != 0)    /* ���a�i�� �{�� �a�� ���e �e���a�a. */
   {
      /* 50�a�� ���� buff�A ���aЁ �� ҁ buff�i ���i�e�a. */
      buff_index = 0;
      while(buff_index < 50 && string[index] != 0)
         buff[buff_index++] = string[index++];
      buff[buff_index] = 0;

      /* ������ �s���E �w�� �a ���e�q string�� index�i �q����ǥ�a. */
      index -= arrange_string(buff, 50);
      puts_han(buff);
   }

   puts_han("\n�a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();

   close_han();
}
