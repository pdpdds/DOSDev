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
/* ���a�i�i ���b�h�e�a. */
{
   char han_str[11], attr2, tab_space;
   int  return_code, i, j;

   string[0] = 0;
   while(1)
   {
      /* �e�i�i �� ���b�h�i �� ���e�� �i�� */
      no_more_han = (strlen(string) > max_strlen_han - 2) ? 1 : 0;
      do
      {
         /* �e ���� ǡ�i ���b�h�e�a. getch?_han() �e �e�i�� �Ŭ��A�� */
         /* �g�a�� �����e�a. �១�A�� �g�e ǡ�a ���ỡ�e IGNORE ���� */
	 return_code = bul3_mode ? getch3_han(han_str, "", 0) :
				   getch2_han(han_str, "", 0);
      }
      while(return_code == IGNORE);

      /* �e�i�� ���s���� �wȁ�a �a���e ���b�h�e �i�a�i �១�e�a. ���b�h�e */
      /* �i�a�e han_str �A ��w�A�� ���e �wȁ���a.                        */
      if(return_code != NOT_COMPLETE)
      {
	 for(i = 0; i < strlen(han_str); i++)
         /* ���b�h�e �i�a�i �a�a�a�a �១�e�a. */
	 {
	    if(han_str[i] & 0x80)       /* �e�i�� ���b�A�� ���a�e */
	    {
	       if((j = strlen(string)) < max_strlen_han - 1)
	       {
		  string[j]   = han_str[i++];
		  string[j+1] = han_str[i];
		  string[j+2] = 0;
		  putch_han(string + j);
	       }
	    }
	    else                        /* �w���� ���b�A�� ���a�e */
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

      /* getch?_han() �� ����t�A �a�a ���ӡ �១ */
      switch(return_code)
      {
	 case ESC_PRESSED   :
	 case ENTER_PRESSED :   /* <Esc> �a <Enter> ǡ�a ����v�a�e */
	    putch_han("\n");
	    cursor_han = eng_cursor;
	    return(return_code == ESC_PRESSED ? ESC : ENTER);
	 case BS_PRESSED    :   /* <BS> ǡ�a ����v�a�e */
            /* getch?_han() �e <BS> ǡ�A ���e �១�i �a�� �g�a�a�� */
            /* ���� �១Ё ���ᴡ �e�a.                            */
	    if(prv_han.code == NO_HAN_STR)  /* ���s���� �e�i�� ���a�e */
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
            else                            /* ���s���� �e�i�� ���a�e */
	    {
               /* �a�� �|�A ���s�E �e�i�� �a���a. */
	       pop_han(&(prv_han.code));
	       if(han_stack.top == 0)
               /* ���s�E �e�i�� �� ���a�e */
	       {
		  cur_han.code = NO_HAN_CODE;
                  /* ���i �����a. */
		  put_eng_font(curx_han,   cury_han, ' ');
		  put_eng_font(curx_han+1, cury_han, ' ');
	       }
	       else
               /* ���s�E �e�i�� ���a�e �a�� �q�a ���a�e */
	       {
		  cur_han.bytes[0] = prv_han.bytes[1];
		  cur_han.bytes[1] = prv_han.bytes[0];
	       }
	    }
	    break;
	 case TAB_PRESSED   :   /* <Tab> ǡ�a ����v�a�e */
            /* ȓ�e�b�� �e�� x ���a�A �a�a ����e ���� �����i �s�� */
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
      /* �e�i�i �� ���b�h�i �� ���� �e�� ���s���� �e�i�� ���a�e */
      /* �e�� ���s���� �e�i�i �b�e�a.                         */
      {
	 if(curx_han == maxx_han)
         /* �e�� ���a�a �A�� �{ �i���e �{�១�i ���e �����i �s�� */
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
         /* put_han_font() �A ��Ё ���a �������a�� exist_cursor �� */
         /* �t�i �����e�a.                                           */
	 exist_cursor = 0;
      }
   }
}
