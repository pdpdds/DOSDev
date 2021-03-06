#include "hanio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* �a�w�a ���� �b�a�� �����i �����e�a. user_defined_box_shape�e ��b �e�� */
   user_defined_box_shape = "012345";

   /* ���a�� �w���w�i �a���a�e�� �b�a�i �a���a. */
   for(i = 0; i < 5; i++)
   {
      draw_box_han(i*15, 1, 10+i*15, 6, i);
      switch(i)
      {
         case 0 : xputs_han( 1, 0, "BOX_H1V1");
                  break;
         case 1 : xputs_han(16, 0, "BOX_H1V2");
                  break;
         case 2 : xputs_han(31, 0, "BOX_H2V1");
                  break;
         case 3 : xputs_han(46, 0, "BOX_H2V2");
                  break;
         case 4 : xputs_han(58, 0, "USER_DEFINED_BOX");
                  break;
      }
   }

   /* �b�w�a�� �b�a�i �a���a. �����e aclear_text_han() �q���� ���� ���ᴡ */
   /* �e�a. �e�� ������ FONT_INVERSE�� ����A����a�� clear_text_han() �q */
   /* ���i �a�w�a�e FONT_NORMAL��  ������ �����A  clear_text_han() �q���� */
   /* �����e �e �E�a.(aclear_text_han �q��)                               */
   set_attr_han(FONT_INVERSE);
   for(i = 0; i < 5; i++)
   {
      draw_box_han(i*15, 7, 10+i*15, 12, i);
      aclear_text_han(i*15+1, 7+1, 10+i*15-1, 12-1, FONT_INVERSE);
   }

   /* ��ѷ�a�� �b�a�i �a���a.  �����e vclear_text_han() �q����  ���� ���ᴡ */
   /* �e�a. �e�� ���� LIGHTMAGENTA �� ����A����a�� clear_text_han()  �q�� */
   /* �i �a�w�a�e BLACK �a�� ������ �����A clear_text_han()  �q����  �����e */
   /* �e �E�a.(vclear_text_han �q��)                                        */
   set_color_han(LIGHTMAGENTA);
   for(i = 0; i < 5; i++)
   {
      draw_box_han(i*15, 13, 10+i*15, 18, i);
      vclear_text_han(i*15+1, 13+1, 10+i*15-1, 18-1, LIGHTMAGENTA);
   }

   aputs_han(0, 20, "�a�� ǡ�a ���a�A�a..", FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
