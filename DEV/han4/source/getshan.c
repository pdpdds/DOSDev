/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          gets_han()                                          */
/*   FILENAME           getshan.c                                           */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int gets_han(char *string)
/* ¢…¸aµi·i ·³b¤h“e”a. */
{
   char han_str[11], attr2, tab_space;
   int  return_code, i, j;

   string[0] = 0;
   while(1)
   {
      /* Ðe‹i·i ”á ·³b¤h·i ® ·¶“e»¡ ‰i¸÷ */
      no_more_han = (strlen(string) > max_strlen_han - 2) ? 1 : 0;
      do
      {
         /* Ðe ˆ· Ç¡Ÿi ·³b¤h“e”a. getch?_han() ·e Ðe‹i·¡ µÅ¬÷–A»¡ */
         /* ´g´a•¡ Ÿ¡ÈåÐe”a. ÀáŸ¡–A»¡ ´g“e Ç¡ˆa ’‰œá»¡¡e IGNORE Ÿ¡Èå */
	 return_code = bul3_mode ? getch3_han(han_str, "", 0) :
				   getch2_han(han_str, "", 0);
      }
      while(return_code == IGNORE);

      /* Ðe‹i·¡ ¹¡Ðsº—·¥ ¬wÈˆa ´a“¡¡e ·³b¤h·e ‹i¸aŸi ÀáŸ¡Ðe”a. ·³b¤h·e */
      /* ‹i¸a“e han_str µA ¸á¸w–A´á ·¶“e ¬wÈ·¡”a.                        */
      if(return_code != NOT_COMPLETE)
      {
	 for(i = 0; i < strlen(han_str); i++)
         /* ·³b¤h·e ‹i¸aŸi ÐaaÐaa ÀáŸ¡Ðe”a. */
	 {
	    if(han_str[i] & 0x80)       /* Ðe‹i·¡ ·³b–A´á ·¶·a¡e */
	    {
	       if((j = strlen(string)) < max_strlen_han - 1)
	       {
		  string[j]   = han_str[i++];
		  string[j+1] = han_str[i];
		  string[j+2] = 0;
		  putch_han(string + j);
	       }
	    }
	    else                        /* µw¢…·¡ ·³b–A´á ·¶·a¡e */
	    {
	       if((j = strlen(string)) < max_strlen_han)
	       {
		  string[j]   = han_str[i];
		  string[j+1] = 0;
		  putch_han(string + j);
	       }
	    }
	 }
      }

      /* getch?_han() · Ÿ¡ÈåˆtµA ˜aœa ¸â¸éÓ¡ ÀáŸ¡ */
      switch(return_code)
      {
	 case ESC_PRESSED   :
	 case ENTER_PRESSED :   /* <Esc> a <Enter> Ç¡ˆa ’‰œá¹v·a¡e */
	    putch_han("\n");
	    cursor_han = eng_cursor;
	    return(return_code == ESC_PRESSED ? ESC : ENTER);
	 case BS_PRESSED    :   /* <BS> Ç¡ˆa ’‰œá¹v·a¡e */
            /* getch?_han() ·e <BS> Ç¡µA ”Ðe ÀáŸ¡Ÿi Ða»¡ ´g·a£a¡ */
            /* »¢¸ó ÀáŸ¡Ð º´á´¡ Ðe”a.                            */
	    if(prv_han.code == NO_HAN_STR)  /* ¹¡Ðsº—·¥ Ðe‹i·¡ ´ô”a¡e */
	    {
	       if(strlen(string) > 0)
	       {
		  if(curx_han == minx_han && cury_han != miny_han)
		  {
		     curx_han = maxx_han;
		     cury_han--;
		     if(scr_buff[cury_han][curx_han][1] == EDGE) curx_han--;
		  }
		  else
		     curx_han--;
		  attr2    = attr_han;
		  attr_han = FONT_NORMAL;
		  if(scr_buff[cury_han][curx_han][1] == HAN2)
		  {
		     put_eng_font(curx_han, cury_han, ' ');
		     curx_han--;
		     put_eng_font(curx_han, cury_han, ' ');
		     string[strlen(string)-2] = 0;
		  }
		  else
		  {
		     put_eng_font(curx_han, cury_han, ' ');
		     string[strlen(string)-1] = 0;
		  }
		  attr_han = attr2;
	       }
            }
            else                            /* ¹¡Ðsº—·¥ Ðe‹i·¡ ·¶”a¡e */
	    {
               /* ¤a¡ ´|µA ¹¡Ðs–E Ðe‹i¡ ¤aŽ…”a. */
	       pop_han(&(prv_han.code));
	       if(han_stack.top == 0)
               /* ¹¡Ðs–E Ðe‹i·¡ ”á ´ô”a¡e */
	       {
		  cur_han.code = NO_HAN_CODE;
                  /* Äá¬áŸi »¡¶…”a. */
		  put_eng_font(curx_han,   cury_han, ' ');
		  put_eng_font(curx_han+1, cury_han, ' ');
	       }
	       else
               /* ¹¡Ðs–E Ðe‹i·¡ ·¶”a¡e ´a»¢ q´a ·¶”a¡e */
	       {
		  cur_han.bytes[0] = prv_han.bytes[1];
		  cur_han.bytes[1] = prv_han.bytes[0];
	       }
	    }
	    break;
	 case TAB_PRESSED   :   /* <Tab> Ç¡ˆa ’‰œá¹v·a¡e */
            /* È“ˆe‰b‰Á Ñe¸ x ¹ÁÎaµA ˜aœa ¸â¸éÐe ®· ‰·¤‚·i ¬s·³ */
	    tab_space = (((curx_han >> 3) + 1) << 3) - curx_han;
	    while(tab_space-- > 0)
	    {
	       if((j = strlen(string)) >= max_strlen_han)
		  break;
	       else
	       {
                  if(curx_han > maxx_han) break;
		  string[j]   = ' ';
		  string[j+1] = 0;
		  putch_han(string + j);
	       }
	    }
	    break;
      }

      no_more_han = (strlen(string) > max_strlen_han - 2) ? 1 : 0;
      if(!no_more_han && prv_han.code != NO_HAN_STR)
      /* Ðe‹i·i ”á ·³b¤h·i ® ·¶‰¡ Ñe¸ ¹¡Ðsº—·¥ Ðe‹i·¡ ·¶·a¡e */
      /* Ñe¸ ¹¡Ðsº—·¥ Ðe‹i·i Â‰bÐe”a.                         */
      {
	 if(curx_han == maxx_han)
         /* Ñe¸ ¹ÁÎaˆa ¹A·© { µi·¡¡e {ÀáŸ¡Ÿi ¶áÐe ‰·¤‚·i ¬s·³ */
	 {
	    put_eng_font(curx_han, cury_han, ' ');
	    scr_buff[cury_han][curx_han][1] = EDGE;
	    curx_han = minx_han;
	    if(cury_han == maxy_han)
	       xscrollup_han(minx_han, miny_han, maxx_han, maxy_han);
	    else
	       cury_han++;
	 }
	 put_han_font(curx_han, cury_han, prv_han.bytes[0], prv_han.bytes[1]);
         /* put_han_font() µA ·Ð Äá¬áˆa »¡¶¡»¡£a¡ exist_cursor · */
         /* ˆt·i ¥¡¸÷Ðe”a.                                           */
	 exist_cursor = 0;
      }
   }
}
