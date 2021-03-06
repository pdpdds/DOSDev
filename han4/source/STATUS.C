/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          draw_status_han(), status_on_han(),                 */
/*                      status_off_han()                                    */
/*   FILENAME           status.c                                            */
/*   DATE & TIME        03/11/92(WED) 18:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_status_han(void)
/* �wȁ �a���A �����i �a���e�a. */
{
   int attr2, color2, bkcolor2, i;

   /* �wȁ �a���� �a�a�a ���� �g�a�e �a�� ���� */
   if(!is_status_on_han) return;

   attr2       = attr_han;
   color2      = color_han;
   bkcolor2    = bkcolor_han;
   attr_han    = FONT_INVERSE;
   color_han   = LIGHTGRAY;
   bkcolor_han = BLACK;

   for(i = 0; i <= abs_maxx_han; i++) put_eng_font(i, abs_maxy_han, ' ');
   put_eng_font( 1, abs_maxy_han, 179);
   put_eng_font( 8, abs_maxy_han, 179);
   put_eng_font(16, abs_maxy_han, 179);
   put_eng_font(23, abs_maxy_han, 179);

   /* �e/�w �wȁ �a�� */
   if(han_mode)
   {
      put_han_font(3, abs_maxy_han, '�', 'e');	/* "�e" */
      put_han_font(5, abs_maxy_han, '�', 'i');	/* "�i" */
   }
   else
   {
      put_han_font(3, abs_maxy_han, '�', 'w');	/* "�w" */
      put_han_font(5, abs_maxy_han, '�', '�');  /* "��" */
   }

   /* 2 �鯢/3 �鯢 �wȁ �a�� */
   bul3_mode ? put_eng_font(10, abs_maxy_han, '3')
	     : put_eng_font(10, abs_maxy_han, '2');
   put_han_font(11, abs_maxy_han, '�', '�');	/* "��" */
   put_han_font(13, abs_maxy_han, '�', '�');	/* "��" */

   /* �s��/���� �wȁ �a�� */
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

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}

void status_on_han(void)
/* �wȁ �a���� �a�a�a�A �e�a. */
{
   /* ���� �wȁ �a���� �a�a�a ���a�e �a�� ���� */
   if(is_status_on_han) return;

   if(cury_han == abs_maxy_han)
   {
      xscrollup_han(0, 0, abs_maxx_han, abs_maxy_han);
      cury_han--;
   }
   maxy_han--;

   is_status_on_han = 1;
   draw_status_han();
}

void status_off_han(void)
/* �wȁ �a���i �a�a���A �e�a. */
{
   int i;

   /* �wȁ �a���� �a�a�a ���� �g�a�e �a�� ���� */
   if(!is_status_on_han) return;

   maxy_han++;
   for(i = 0; i <= abs_maxx_han; i++) put_eng_font(i, abs_maxy_han, ' ');
   is_status_on_han = 0;
}
