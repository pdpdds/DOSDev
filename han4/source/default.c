/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          default_han_change_func(),                          */
/*                      default_ins_change_func()                           */
/*   FILENAME           default.c                                           */
/*   DATE & TIME        03/11/92(WED) 19:17                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void default_han_change_func(void)
/* �e/�w ����ǡ�a ���ứ �� ѡ�A�e �q��(�����E �q��) */
{
   int attr2, color2, bkcolor2;

   attr2       = attr_han;
   color2      = color_han;
   bkcolor2    = bkcolor_han;
   attr_han    = FONT_INVERSE;
   color_han   = LIGHTGRAY;
   bkcolor_han = BLACK;

   if(is_status_on_han)
   /* �wȁ �a���� �a���A�� ���a�e �wȁ �a���A ���b �wȁ�i �a�� */
   {
      if(han_mode)
      {
	 put_han_font(3, abs_maxy_han, '�', 'e');	/* "�e" */
	 put_han_font(5, abs_maxy_han, '�', 'i');	/* "�i" */
      }
      else
      {
	 put_han_font(3, abs_maxy_han, '�', 'w');	/* "�w" */
	 put_han_font(5, abs_maxy_han, '�', '�');	/* "��" */
      }
   }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;

   sound(330); delay(50);
   sound(262); delay(50);
   nosound();
}

void default_ins_change_func(void)
/* �s��/���� ����ǡ�a ���ứ �� ѡ�A�e �q��(�����E �q��) */
{
   int attr2, color2, bkcolor2;

   attr2       = attr_han;
   color2      = color_han;
   bkcolor2    = bkcolor_han;
   attr_han    = FONT_INVERSE;
   color_han   = LIGHTGRAY;
   bkcolor_han = BLACK;

   if(is_status_on_han)
   /* �wȁ �a���� �a���A�� ���a�e �wȁ �a���A ���b �wȁ�i �a�� */
   {
      if(ins_mode)
      {
	 put_han_font(18, abs_maxy_han, '�', 's');	/* "�s" */
	 put_han_font(20, abs_maxy_han, '�', '�');	/* "��" */
      }
      else
      {
	 put_han_font(18, abs_maxy_han, '�', '�');	/* "��" */
	 put_han_font(20, abs_maxy_han, '�', '�');	/* "��" */
      }
   }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}
