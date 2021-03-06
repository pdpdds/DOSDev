#include <dos.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* �q�� ���w */
void draw_top(void);
void show_watch();

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   draw_top();
   /* ���b ���� ���e ���e show_watch() �q���a ��З�A���� �e�a. */
   set_han_wait_func(show_watch);
   /* ���e �A�� �a�e�A �A�����i �b */
   naputs_han(0, get_abs_maxy_han(),
	      "�a�� �i�a�a ���bЁ ���A�a. �{�i ���a�e <Esc> ǡ�i ���a�A�a.",
              80, FONT_INVERSE);
   set_window_han(0, 1, get_abs_maxx_han(), get_abs_maxy_han() - 1);
   /* <Esc> ǡ�i ���i ���a�� gets_han(string) �i �e�� */
   while(1) if(gets_han(string) == ESC) break;

   close_han();
}

void draw_top(void)
/* ���e �A�� �w�e�A �i�a�� ���e�� �b�A�e �a���i �����e�a. */
{
   struct date cur_d;
   struct time cur_t;

   naputs_han(0, 0, "", 80, FONT_INVERSE);
   aputs_han((80 - 24) / 2, 0, "�e�i �a���a�១ <�e> 4.1", FONT_INVERSE);

   getdate(&cur_d); gettime(&cur_t);
   aprintf_han(1, 0, FONT_INVERSE, "%4d�e %2d�� %2d��",
               cur_d.da_year, cur_d.da_mon, cur_d.da_day);
   aprintf_han(65, 0, FONT_INVERSE, "%02d�� %02d�� %02d��",
               cur_t.ti_hour, cur_t.ti_min, cur_t.ti_sec);

   /* �a�aǡ š�a 179�A Ё�w�a�e �a��Ϣ ���a�i �b�e�a. */
   aput_eng_font(18, 0, 179, FONT_INVERSE);
   aput_eng_font(56, 0, 179, FONT_INVERSE);
   aput_eng_font(63, 0, 179, FONT_INVERSE);
   show_watch();
}

void show_watch(void)
{
   /* show_watch() �e �i�a�� ���e�i �a���a�e �q�����A ���b ���� ���e  �� */
   /* �e ���� ѡ�E�a. prv_d�e show_watch()�a ѡ�A�� �a�� �巁 �i�a   */
   /* ���A �e�a�� static �e���a�� �e�a. prv_t�� �a�e�a���a.              */
   static struct date prv_d; struct date cur_d;
   static struct time prv_t; struct time cur_t;
   static int called_num = 0;

   /* �e�� ���e�i ���e�a. */
   getdate(&cur_d); gettime(&cur_t);

   if(cur_t.ti_sec != prv_t.ti_sec)
   /* ���a �a����a�e ���i �a�� */
   {
      aprintf_han(75, 0, FONT_INVERSE, "%2d", cur_t.ti_sec);
      if(cur_t.ti_min != prv_t.ti_min)
      /* ���� �a����a�e ���i �a�� */
      {
         aprintf_han(70, 0, FONT_INVERSE, "%2d", cur_t.ti_min);
         if(cur_t.ti_hour != prv_t.ti_hour)
         /* ���a �a����a�e �I�����i ���� ���i �a�� */
         {
            if(called_num != 0)
            /* show_watch()�a ��q ѡ�A���i �� �I�����a �a�e �e�w�i */
            /* �b�� ��Ё called_num�i ��a�e�a.                       */
               play("o3l4ceg>c2");
            else
               called_num = 1;
            if(cur_t.ti_hour / 12)
               aputs_han(58, 0, "��ҁ", FONT_INVERSE);
            else
               aputs_han(58, 0, "����", FONT_INVERSE);
            if(cur_t.ti_hour % 12 == 0)
               aputs_han(65, 0, "12", FONT_INVERSE);
            else
               aprintf_han(65, 0, FONT_INVERSE, "%2d", cur_t.ti_hour % 12);
            if(cur_d.da_day != prv_d.da_day)
            /* �i�a�a �a����a�e �i�a�i �a�� */
            {
               aprintf_han(1, 0, FONT_INVERSE, "%4d�e %2d�� %2d��",
                           cur_d.da_year, cur_d.da_mon, cur_d.da_day);
               prv_d = cur_d;
            }
            prv_t.ti_hour = cur_t.ti_hour;
         }
         prv_t.ti_min = cur_t.ti_min;
      }
      prv_t.ti_sec = cur_t.ti_sec;
   }
}
