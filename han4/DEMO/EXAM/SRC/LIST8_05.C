#include <dos.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* Ðq® ¶¥Ñw */
void draw_top(void);
void show_watch();

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   draw_top();
   /* ·³b ”‹¡ ¯¡ˆe •·´e show_watch() Ðq®ˆa ¯©Ð—–A•¡¢ Ðe”a. */
   set_han_wait_func(show_watch);
   /* ÑÁ¡e ¹A·© Ða”eµA ¡A¯¡»¡Ÿi Â‰b */
   naputs_han(0, get_abs_maxy_han(),
	      "´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a. {·i a¡e <Esc> Ç¡Ÿi ’Ÿa­A¶a.",
              80, FONT_INVERSE);
   set_window_han(0, 1, get_abs_maxx_han(), get_abs_maxy_han() - 1);
   /* <Esc> Ç¡Ÿi ’Ÿi ˜Œa»¡ gets_han(string) ·i ¤e¥¢ */
   while(1) if(gets_han(string) == ESC) break;

   close_han();
}

void draw_top(void)
/* ÑÁ¡e ¹A·© ¬w”eµA i¼aµÁ ¯¡ˆe·¡ Â‰b–A“e œa·¥·i Š¬÷Ðe”a. */
{
   struct date cur_d;
   struct time cur_t;

   naputs_han(0, 0, "", 80, FONT_INVERSE);
   aputs_han((80 - 24) / 2, 0, "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1", FONT_INVERSE);

   getdate(&cur_d); gettime(&cur_t);
   aprintf_han(1, 0, FONT_INVERSE, "%4d‘e %2d¶© %2d·©",
               cur_d.da_year, cur_d.da_mon, cur_d.da_day);
   aprintf_han(65, 0, FONT_INVERSE, "%02d¯¡ %02d¦… %02dÁ¡",
               cur_t.ti_hour, cur_t.ti_min, cur_t.ti_sec);

   /* ´a¯aÇ¡ Å¡—a 179µA Ð”wÐa“e ‹aœÏ¢ ¢…¸aŸi Â‰bÐe”a. */
   aput_eng_font(18, 0, 179, FONT_INVERSE);
   aput_eng_font(56, 0, 179, FONT_INVERSE);
   aput_eng_font(63, 0, 179, FONT_INVERSE);
   show_watch();
}

void show_watch(void)
{
   /* show_watch() “e i¼aµÁ ¯¡ˆe·i Îa¯¡Ða“e Ðq®·¥•A ·³b ”‹¡ ¯¡ˆe  •· */
   /* ´e ‰­¢ Ñ¡Â‰–E”a. prv_d“e show_watch()ˆa Ñ¡Â‰–A‹¡ ¤a¡ ¸å· i¼a   */
   /* ·¥•A ¤e—a¯¡ static ¥e®µa´¡ Ðe”a. prv_t•¡  aÀeˆa»¡”a.              */
   static struct date prv_d; struct date cur_d;
   static struct time prv_t; struct time cur_t;
   static int called_num = 0;

   /* Ñe¸ ¯¡ˆe·i ŠÐe”a. */
   getdate(&cur_d); gettime(&cur_t);

   if(cur_t.ti_sec != prv_t.ti_sec)
   /* Á¡ˆa ¤aŽá´ö·a¡e Á¡Ÿi Îa¯¡ */
   {
      aprintf_han(75, 0, FONT_INVERSE, "%2d", cur_t.ti_sec);
      if(cur_t.ti_min != prv_t.ti_min)
      /* ¦…·¡ ¤aŽá´ö·a¡e ¦…·i Îa¯¡ */
      {
         aprintf_han(70, 0, FONT_INVERSE, "%2d", cur_t.ti_min);
         if(cur_t.ti_hour != prv_t.ti_hour)
         /* ¯¡ˆa ¤aŽá´ö·a¡e ¥I­¡Ÿ¡Ÿi ‰¡ ¯¡Ÿi Îa¯¡ */
         {
            if(called_num != 0)
            /* show_watch()ˆa Àá·q Ñ¡Â‰–A´ö·i ˜ ¥I­¡Ÿ¡ˆa a“e Ñe¬w·i */
            /*  b‹¡ ¶áÐ called_num·i ˆñ¬aÐe”a.                       */
               play("o3l4ceg>c2");
            else
               called_num = 1;
            if(cur_t.ti_hour / 12)
               aputs_han(58, 0, "µ¡Ò", FONT_INVERSE);
            else
               aputs_han(58, 0, "µ¡¸å", FONT_INVERSE);
            if(cur_t.ti_hour % 12 == 0)
               aputs_han(65, 0, "12", FONT_INVERSE);
            else
               aprintf_han(65, 0, FONT_INVERSE, "%2d", cur_t.ti_hour % 12);
            if(cur_d.da_day != prv_d.da_day)
            /* i¼aˆa ¤aŽá´ö·a¡e i¼aŸi Îa¯¡ */
            {
               aprintf_han(1, 0, FONT_INVERSE, "%4d‘e %2d¶© %2d·©",
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