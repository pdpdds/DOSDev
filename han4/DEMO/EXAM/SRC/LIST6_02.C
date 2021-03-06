#include "hanio.h"
#include "extendio.h"

void change_cursor_to_box(void);

void main(void)
{
   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   /* �b�a ������ ��ᝡ �a���a. */
   change_cursor_to_box();

   puts_han("�w�� ���a �i �������a?\n\n");
   puts_han("�a�� ǡ�a ���a�A�a.");
   /* �e�� ���a�A �w�� ���i �b�e�a. */
   cursor_han = eng_cursor;             /* �e�� ���e �w�� ��ᝡ ��� */
   draw_cursor_han(get_curx_han(), get_cury_han()); /* ���i �a���a. */
   if(!getch()) getch();
   draw_cursor_han(get_curx_han(), get_cury_han()); /* ���i �����a. */

   puts_han("�e�i ���a �i �������a?\n\n");
   puts_han("�a�� ǡ�a ���a�A�a.");
   /* �e�� ���a�A �e�i ���i �b�e�a. */
   cursor_han = han_cursor;             /* �e�� ���e �e�i ��ᝡ ��� */
   draw_cursor_han(get_curx_han(), get_cury_han()); /* ���i �a���a. */
   if(!getch()) getch();
   draw_cursor_han(get_curx_han(), get_cury_han()); /* ���i �����a. */

   close_han();
}

void change_cursor_to_box(void)
{
   /* �e�i ��᷁ ����(�b�a ����) */
   unsigned char shape32[HAN_FONT_SIZE] =
   {
      0xff,0xff,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0xff,0xff
   };
   /* �w�� ��᷁ ����(�b�a ����) */
   unsigned char shape16[ENG_FONT_SIZE] =
   {
      0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
      0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
   };

   /* �A�᷁ �t�i ��� */
   han_cursor[0] = 15;
   eng_cursor[0] =  7;
   /* ���a�� �a�a�i ��� */
   memmove(han_cursor+1, shape32, HAN_FONT_SIZE);
   memmove(eng_cursor+1, shape16, ENG_FONT_SIZE);
}
