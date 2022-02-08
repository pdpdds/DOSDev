/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          putch_han()                                         */
/*   FILENAME           putchhan.c                                          */
/*   DATE & TIME        07/03/92(FRI) 01:10                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

/* putch_han() ·a¡ Â‰bÐi •A·¡Èáˆa ñ´áµ© ˜ a”a Ñ¡Â‰–A“e Ðq®(Â‰b */
/* ¸»¡Ð··i ÀáŸ¡Ða“e Ðq®)Ÿi ˆaŸ¡Ç¡“e Í¡·¥Èá                         */
void (*redirect_func) (char *) = (void (*) (char *)) NULL;

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void putch_han(char *string)
/* Ñe¸ ¹ÁÎaµA Ðe ˆ· ¢…¸aŸi Â‰bÐe”a. */
{
   char redirect_data[3];
   int tab_space;

   if((redirect_data[0] = string[0]) & 0x80)
   /* Â‰bÐi ¢…¸aˆa Ðe‹i·¡¡e */
   {
      if(curx_han == maxx_han)
      /* Ñe¸ ¹ÁÎaˆa ¹A·© { µi·¡¡e {ÀáŸ¡Ÿi ¶áÐe ‰·¤‚·i ¬s·³ */
      {
	 scr_buff[cury_han][curx_han][0] = ' ';
	 scr_buff[cury_han][curx_han][1] = EDGE;
	 scr_buff[cury_han][curx_han][2] = attr_han;
	 scr_buff[cury_han][curx_han][3] = color_han | (bkcolor_han << 4);
	 curx_han = minx_han;
	 if(cury_han == maxy_han)
	    xscrollup_han(minx_han, miny_han, maxx_han, maxy_han);
	 else
	    cury_han++;
      }
      /* Ðe‹i Ðe ¸aŸi Â‰b */
      put_han_font(curx_han, cury_han, string[0], string[1]);
      curx_han++; curx_han++;
      redirect_data[1] = string[1];
      redirect_data[2] = 0;
   }
   else
   /* Â‰bÐi ¢…¸aˆa µw¢…·¡¡e */
   {
      switch(string[0])
      {
	 case '\7' : /* ¥I ­¡Ÿ¡ */
                     sound(890);
		     delay(70);
		     nosound();
		     break;
	 case '\t' : /* È“(tab) */
                     tab_space = (((curx_han >> 3) + 1) << 3) - curx_han;
		     while(tab_space-- > 0)
		     {
			if(curx_han > maxx_han) break;
			put_eng_font(curx_han, cury_han, ' ');
                        curx_han++;
		     }
		     break;
	 case '\r' : /* ÄŸ¡»¡ Ÿ¡Èå(CR) */
                     curx_han = 0;
		     break;
	 case '\n' : /* ¬ º‰(new line, CR/LF) */
                     curx_han = maxx_han + 1;
		     break;
	 case '\b' : /* ¤‚¯aÍA·¡¯a(BS) */
                     if(curx_han > minx_han)
		     {
                        if(scr_buff[cury_han][curx_han][1] == HAN2) put_eng_font(curx_han, cury_han, ' ');
                        curx_han--;
                        if(scr_buff[cury_han][curx_han][1] == HAN2)
                        {
                           put_eng_font(curx_han, cury_han, ' ');
                           curx_han--;
                           put_eng_font(curx_han, cury_han, ' ');
                        }
                        else
                           put_eng_font(curx_han, cury_han, ' ');
		     }
		     break;
	 default   : /* µw¢… Ðe ¸aŸi Â‰b */
                     put_eng_font(curx_han, cury_han, string[0]);
		     curx_han++;
		     break;
      }
      redirect_data[1] = 0;
   }

   if(curx_han > maxx_han) /* ˆÐ—·i Ð´¡Ðe”a¡e */
   {
      curx_han = minx_han;
      if(cury_han == maxy_han)
         xscrollup_han(minx_han, miny_han, maxx_han, maxy_han);
      else
         cury_han++;
   }

   /* Â‰b ¸»¡Ð··i ÀáŸ¡Ða“e Ðq®¡ Â‰bÐe ¢…¸aŸi ñ‹¥”a. */
   if(redirect_func != (void (*) (char *)) NULL) redirect_func(redirect_data);
}
