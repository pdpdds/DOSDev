#include <dos.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* Ðq® ¶¥Ñw */
int is_leap_year(int year);
int get_last_day(int year, int month);
int zeller(int year, int month, int day);
void draw_one_calendar_screen(int x, int y);
void show_one_calendar(int x, int y, int year, int month);
void show_calendars(void);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   show_calendars();

   close_han();
}

int is_leap_year(int year)
/* yearˆa ·E‘e·¡¡e 1·i, ´a“¡¡e 0·i Ÿ¡ÈåÐe”a. */
{
   if(year % 400 == 0)
      return(1);
   else if(year % 100 == 0)
      return(0);
   else if(year % 4 == 0)
      return(1);
   else
      return(0);
}

int get_last_day(int year, int month)
/* year ‘e month ¶©·  a»¡ b i·i ®•¸a¡ Ÿ¡ÈåÐe”a. */
{
   if(month == 2)
   {
      if(is_leap_year(year))
         return(29);
      else
         return(28);
   }
   else if(month == 2 || month == 4 || month == 6 || month == 9 || month == 11)
      return(30);
   else
      return(31);
}

int zeller(int year, int month, int day)
/* Zeller· Ðq®Ÿi ·¡¶wÐaµa year ‘e month ¶© day ·©· ¶a·©·i ‰¬eÐe”a. */
/* ¶a·©·e ·©¶a·©·e 0, ¶©¶a·©·e 1‰Á ˆ{·e ¯¢·a¡ Ÿ¡Èå–E”a.               */
{
   int year_of_century, century, day_of_week;

   /* Zeller· Ðq®µA ·¡¶wÐi ® ·¶•¡¢ yearµÁ monthŸi ¥¡¸÷ */
   if(month == 1 || month == 2) year--;
   month = (month + 9) % 12 + 1;  /* Jan = 11, Feb = 12, Mar = 1, ...  */
   year_of_century = year % 100;  /* µ : 91 of 1991                   */
   century = year / 100;          /* µ : 19 of 1991                   */
   day_of_week = ((13*month-1)/5 + day + year_of_century + year_of_century/4 +
                 century/4 - 2*century) % 7;
   if(day_of_week < 0) day_of_week = (day_of_week + 7) % 7;
   return(day_of_week);
}

void draw_one_calendar_screen(int x, int y)
/* (x, y)µA Ðe ”i· ”ib·i ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. */
{
   draw_box_han(x, y, x+23, y+9, BOX_H1V1);
   naputs_han(x+1, y+2, " ·© ¶© ÑÁ ® ¡¢ ‹q É¡ ", 22, FONT_SHADE);
}

void show_one_calendar(int x, int y, int year, int month)
/* (x, y)µA year ‘e month ¶©· ”ib·i Â‰bÐe”a. */
{
   struct date cur_d;
   int first_day_of_week, i, x1, y1;

   getdate(&cur_d);
   xprintf_han(x, y-2, "   %4d ‘e  %2d ¶©   ", year, month);
   clear_text_han(x, y, x+20, y+5);
   /* year ‘e month ¶© Àõ¼i· ¶a·©·i ŠÐe”a. */
   first_day_of_week = zeller(year, month, 1);
   for(i = 0; i < get_last_day(year, month); i++)
   {
      x1 = x + ((first_day_of_week + i) % 7) * 3;
      y1 = y + (first_day_of_week + i) / 7;
      if(cur_d.da_year == year && cur_d.da_mon == month && cur_d.da_day == i+1)
      /* Ñe¸·©·e µb¬w·a¡ Îa¯¡Ðe”a. */
         aprintf_han(x1, y1, FONT_INVERSE, "%2d", i+1);
      else
         xprintf_han(x1, y1, "%2d", i+1);
   }
}

void show_calendars(void)
/* ÐeŒá¤åµA ­A ˆ· ”ib·i ¥¡µa º“e ºÐq® */
{
   static struct date d;
   int year, month, l_year, l_month, r_year, r_month;
   int ascii, extend, quit, re_draw;
   char *screen;

   if((screen = save_text_han(0, 3, 79, 18)) == NULL)
   {
      error_exit_msg = "¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a";
      error_exit_han("show_calendars()", 1);
   }

   /* ÑÁ¡e Š¬÷ */
   clear_text_han(0, 3, 79, 18);
   draw_box_han( 1, 4, 78, 17, BOX_H1V1);
   draw_box_han( 2, 6, 77, 6, BOX_H1V1);
   put_eng_font( 1, 6, 195);
   put_eng_font(78, 6, 180);
   aputs_han(34, 5, "  e‘e ”ib ", FONT_SHADE);
   draw_one_calendar_screen( 2, 7);
   draw_one_calendar_screen(28, 7);
   draw_one_calendar_screen(54, 7);
   naputs_han(0, get_abs_maxy_han(), "  Up/Down/PgUp/PgDn:‘e•¡ ¬åÈ‚   "
              "Left/Right:”i ¬åÈ‚   Space:Ñe¸ ¬åÈ‚   Esc:{",
              get_abs_maxx_han()+1, FONT_INVERSE);

   re_draw = 1;
   quit = 0;
   /* ÂAÁ¡µA“e Ñe¸·©·i ‹¡º…·a¡ ”ib·i Â‰bÐe”a. */
   getdate(&d);
   year  = d.da_year;
   month = d.da_mon;

   do
   {
      if(re_draw)
      {
         l_month = (month + 10) % 12 + 1;
         r_month = month % 12 + 1;
         l_year = (month ==  1) ? year - 1 : year;
         r_year = (month == 12) ? year + 1 : year;
         show_one_calendar( 2+2, 7+3, l_year, l_month);
         show_one_calendar(28+2, 7+3,   year,   month);
         show_one_calendar(54+2, 7+3, r_year, r_month);
         re_draw = 0;
      }

      ascii = getch();
      if(ascii == 0)
      {
         extend = getch();
         switch(extend)
         {
            case LEFT :         /* Ðe”i ¸å·a¡ */
               if(year > 0 || month > 2)
               {
                  if(month > 1)
                     month--;
                  else
                  {
                     month = 12;
                     year--;
                  }
               }
               re_draw = 1;
               break;
            case RIGHT :        /* Ðe”i Ò¡ */
               if(year < 9999 || month < 11)
               {
                  if(month < 12)
                     month++;
                  else
                  {
                     month = 1;
                     year++;
                  }
               }
               re_draw = 1;
               break;
            case UP :           /* 1 ‘e ¸å·a¡ */
               if(year > 1) year--;
               re_draw = 1;
               break;
            case DOWN :         /* 1‘e Ò¡ */
               if(year < 9998) year++;
               re_draw = 1;
               break;
            case PGUP :         /* 100‘e ¸å·a¡ */
               if(year > 100) year -= 100;
               re_draw = 1;
               break;
            case PGDN :         /* 100‘e Ò¡ */
               if(year < 9898) year += 100;
               re_draw = 1;
               break;
         }
      }
      else
      {
         switch(ascii)
         {
            case _SPACE :       /* Ñe¸·©¡ */
               getdate(&d);
               year  = d.da_year;
               month = d.da_mon;
               re_draw = 1;
               break;
            case _ESC   :       /* { */
               quit = 1;
               break;
         }
      }
   }
   while(!quit);
   restore_text_han(0, 3, screen);
}
