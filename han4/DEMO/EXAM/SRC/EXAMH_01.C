#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char string1[] = "�e�i�e �����m�A�� ���B�a�v�a.", string2[128];
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* string1�i �e�a�i �a�w�a�a �b */
   xputs_han(0, 0, string1);

   /* string1 ���� ���e �e�a�i �e�i�� �e�� */
   i = 0;
   while(i < strlen(string1))
   {
      if((unsigned char) string1[i] >= 0xe0)
      /* �e�a���e �e�i�� �e�ů�ǥ�a. */
      {
         hanja_to_hangul(string1 + i, string2 + i);
         i++;
      }
      else if(string1[i] & 0x80)
      /* �e�i���e 2�a���a�i ���a�e�a. */
      {
         string2[i++] = string1[i];
         string2[i]   = string1[i];
      }
      else
      /* �w���a���e 1�a���a�i ���a�e�a. */
         string2[i] = string1[i];
      i++;
   }
   /* string2�A �� ���a�i �������a. */
   string2[i] = 0;

   /* �e�i�� �e�ŖE string2�i �b */
   xputs_han(0, 1, string2);

   xputs_han(0, 4, "�a�� ǡ�a ���a�A�a.. ");
   if(!getch()) getch();
   close_han();
}
