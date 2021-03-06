#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char han_shape[] =   /* ���a ������ �e�i ��� */
   {
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0xff, 0xff,
      0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x08
   };
   char eng_shape[] =   /* ���a ������ �w�� ��� */
   {
      0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff,
      0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18
   };
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   puts_han("�e ��ᷳ���a. <Enter> ǡ�i ���a�A�a..");
   gets_han(string);

   set_cursor_han(BOX_CURSOR, NULL, BOX_CURSOR, NULL);
   puts_han("�b�a ������ ��ᷳ���a. <Enter> ǡ�i ���a�A�a..");
   gets_han(string);

   set_cursor_han(USER_DEFINED_CURSOR, han_shape,
                  USER_DEFINED_CURSOR, eng_shape);
   puts_han("���a ������ ��ᷳ���a. <Enter> ǡ�i ���a�A�a..");
   gets_han(string);

   close_han();
}
