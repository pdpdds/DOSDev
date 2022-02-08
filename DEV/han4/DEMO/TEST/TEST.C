#include <dos.h>
#include "hanio.h"

/* Ðq® ¶¥Ñw */
int  is_leap_year(int year);
int  get_last_day(int year, int month);
int  valid_date(int year, int month, int day);
long get_passed_day(int year, int month, int day, int start_year);
int  zeller(int year, int month, int day);

void main(void)
{
   struct date today;
   char *(day_of_week[7]) = { "·©", "¶©", "ÑÁ", "®", "¡¢", "‹q", "É¡" };
   int i,b_year, b_month, b_day, c_year;

   if(init_han() != OK_HAN) exit(1);

   /* Ñe¸·©·i todayµA ¸á¸w */
   getdate(&today);
   puts_han("”w¯¥· ¬—‘e¶©·©·i ´·b·a¡ ·³bÐa­A¶a.(YYYY MM DD)");
   /* ¬—‘e¶©·©·i ·³b¤h“e”a.(¤aŸa‰A ·³bÐi ˜Œa»¡) */
   while(1)
   {
      scanf_han("%d %d %d", &b_year, &b_month, &b_day);
      if(valid_date(b_year, b_month, b_day))
         break;
      else
         puts_han("\bi¼aˆa ¤aŸa»¡ ¡µÐs“¡”a. ”a¯¡ ·³bÐa­A¶a.");
   }

   /* i¼aµA ‰ÅÐe ˆb¹· ¸÷¥¡Ÿi Â‰bÐe”a. */
   puts_han("\n------------------------------------");
   printf_han("¬—‘e¶©·©  : %4d ‘e %2d ¶© %2d ·© ", b_year, b_month, b_day);
   printf_han("(%s)\n", day_of_week[zeller(b_year, b_month, b_day)]);
   printf_han("µ¡“i i¼a : %4d ‘e %2d ¶© %2d ·© ",
              today.da_year, today.da_mon, today.da_day);
   printf_han("(%s)\n",
              day_of_week[zeller(today.da_year, today.da_mon, today.da_day)]);
   printf_han("µ©Ð“e ");
   if(is_leap_year(today.da_year))
      printf_han("·E‘e·³“¡”a.\n");
   else
      printf_han("Íw‘e·³“¡”a.\n");
   printf_han("·¡¤å ”i·e %d ·©Œa»¡ ·¶¯s“¡”a.\n",
              get_last_day(today.da_year, today.da_mon));
   /* 1800‘e¦Èá ‰w‰ÁÐe i¼a ®¡¦Èá µ¡“iŒa»¡ ¬i´aµ¥ ‹¡ˆe·i ‰¬eÐe”a.*/
   printf_han("”w¯¥·e %ld ·©ˆe ¬i´aµÖ¯s“¡”a.\n",
          get_passed_day(today.da_year, today.da_mon, today.da_day, 1800) -
              get_passed_day(b_year, b_month, b_day,1800));
  for(i=1;i<3;i++)
      printf_han("1993 %d ¶© %d ·© Œa»¡‰w‰Á·©®“e %ld ·©\n",
         i,(i%2)*30+1,get_passed_day(today.da_year,i,(i%2)*30+1, 1992));

   puts_han("------------------------------------\n");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

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
/* year ‘e <month> ¶©·  a»¡ b i·i ®•¸a¡ Ÿ¡ÈåÐe”a. */
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

int valid_date(int year, int month, int day)
/* year ‘e month ¶© day ·©·¡œa“e i¼aˆa ´ô·a¡e 0·i */
/* ·¶·a¡e 1·i Ÿ¡ÈåÐe”a.                            */
{
   if(year < 0 || month < 1 || month > 12) return(0);
   if(day  < 1 || day   > get_last_day(year, month)) return(0);
   return(1);
}

long get_passed_day(int year, int month, int day, int start_year)
/* start_year‘e 1¶© 1·©¦Èá year ‘e month ¶© day ·©Œa»¡ */
/* ‰w‰Á–E i ®Ÿi Ÿ¡ÈåÐe”a.                             */
{
   long passed_day;
   int i;

   passed_day = day - 1;
   for(i = start_year; i < year; i++)
      if(is_leap_year(i)) passed_day += 366; else passed_day += 365;
   for(i = 1; i < month; i++)
      passed_day += get_last_day(year, i);
   return(passed_day);
}

int zeller(int year, int month, int day)
/* Zeller· Ðq®Ÿi ·¡¶wÐaµa year ‘e month ¶© day ·©· ¶a·©·i ‰¬eÐe”a. 
*/
/* ¶a·©·e ·©¶a·©·e 0, ¶©¶a·©·e 1‰Á ˆ{·e ¯¢·a¡ Ÿ¡Èå–E”a.               
*/
{
 int year_of_century, century, day_of_week;

   /* Zeller· Ðq®µA ·¡¶wÐi ® ·¶•¡¢ year µÁ monthŸi ¥¡¸÷ */
   if(month == 1 || month == 2) year--;
   month = (month + 9) % 12 + 1;  /* Jan = 11, Feb = 12, Mar = 1, ...*/
   year_of_century = year % 100;  /* µ : 91 of 1991*/
   century = year / 100;          /* µ : 19 of 1991*/
   day_of_week = ((13*month-1)/5 + day + year_of_century + year_of_century/4 +
                 century/4 - 2*century) % 7;
   if(day_of_week < 0) day_of_week = (day_of_week + 7) % 7;
   return(day_of_week);
}
