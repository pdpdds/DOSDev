/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          choose_hanja()                                      */
/*   FILENAME           chhanja.c                                           */
/*   DATE & TIME        05/10/92(SUN) 19:17                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/* ���e�A �a���i �e�a�� �� */
#define MAX_HANJA_X   10
#define MAX_HANJA_Y    7

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

/* �e�i�i �e�a�� �a���� ��Ё ͥ�a �����i ����� �e�a ����A */
/* ��w�e �q��(<�e> �� ���� �q��)                           */
int hangul_to_hanja(unsigned char *string);

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int choose_hanja(char *source, char *dest)
/* �e�a�i ��Ȃ�h�e�a. */
{
   static unsigned char buffer[MAX_HANJA_X*MAX_HANJA_Y][2];
   unsigned char attr2, key;
   int number, cur_ptr, hanja_x, i;
   int box_x1, box_y1, box_x2, box_y2;
   int cur_hanja, prv_hanja;
   char *screen;

   attr2 = attr_han;
   attr_han = FONT_NORMAL;

   /* �e�a ����A source �A Ё�w�a�e �e�a�� ͥ�a�i ���e�a. */
   cur_ptr = hanja_buff_ptr;
   /* source �A Ё�w�a�e �e�a�a �a�a�� ���a�e -1 �i ���� */
   if((number = hangul_to_hanja(source)) == 0) return -1;
   /* ���e �e�a�� ���A �a�a �a���� �a���A�e �e�a�� ���i ���� */
   hanja_x = number / MAX_HANJA_Y + 1;
   if(hanja_x < 5) hanja_x = 5;

   /* buffer �A source �� �a�A Ё�w�a�e �e�a�� š�a�i ��w�e�a. */
   buffer[0][0] = source[0];
   buffer[0][1] = source[1];
   for(i = 1; i < number + 1 && i < MAX_HANJA_X * MAX_HANJA_Y; i++)
   {
      buffer[i][0] = (hanja_buff[cur_ptr].code & 0xff00) >> 8;
      buffer[i][1] = (hanja_buff[cur_ptr].code & 0x00ff);
      cur_ptr = (cur_ptr + 1) % hanja_buff_size;
   }

   /* �e�a�i �a���a�� ���e ���e�i �����e�a. */
   box_x1 = (abs_maxx_han - hanja_x * 3 - 3) / 2;
   box_y1 = (abs_maxy_han - MAX_HANJA_Y - 4) / 2;
   box_x2 = box_x1 + hanja_x * 3 + 2;
   box_y2 = box_y1 + MAX_HANJA_Y + 3;

   if((screen = save_text_han(box_x1, box_y1, box_x2, box_y2)) == NULL) return -2;
   clear_text_han(box_x1, box_y1, box_x2, box_y2);
   draw_box_han(box_x1, box_y1,     box_x2, box_y2,     BOX_H1V1);
   draw_box_han(box_x1, box_y1 + 2, box_x2, box_y1 + 2, BOX_H1V1);
   put_eng_font(box_x1, box_y1 + 2, 195);
   put_eng_font(box_x2, box_y1 + 2, 180);
   xputs_han(box_x1 + (box_x2 - box_x1 - 3) / 2, box_y1 + 1, "�e�a");

   /* buffer �� ���w�i �a���e�a. */
   for(i = 0; i < number + 1; i++) put_han_font(box_x1 + 2 + (i % hanja_x) * 3,
						box_y1 + 3 + (i / hanja_x),
						buffer[i][0], buffer[i][1]);
   /* �e�a�i ��Ȃ�h�e�a. */
   prv_hanja = 0;
   cur_hanja = 1;
   while(1)
   {
      if(cur_hanja != prv_hanja)
      /* �a�w�a�a ����ǡ�i �����a�e �e�� ��Ȃ�E �e�a�i �a���a. */
      {
         attr_han = FONT_NORMAL;
	 put_han_font(box_x1 + 2 + (prv_hanja % hanja_x) * 3,
		      box_y1 + 3 + (prv_hanja / hanja_x),
		      buffer[prv_hanja][0], buffer[prv_hanja][1]);
         attr_han = FONT_INVERSE;
	 put_han_font(box_x1 + 2 + (cur_hanja % hanja_x) * 3,
		      box_y1 + 3 + (cur_hanja / hanja_x),
		      buffer[cur_hanja][0], buffer[cur_hanja][1]);
	 prv_hanja = cur_hanja;
      }

      /* ǡ�i ���b�h�a �១�e�a. */
      if((key = getch_han()) == _ENTER)     /* <Enter> ǡ�a ���ụ �w�� */
      {
	 dest[0] = buffer[cur_hanja][0];
	 dest[1] = buffer[cur_hanja][1];
	 dest[2] = 0;
	 break;
      }
      else if(key == _ESC)                  /* <Esc> ǡ�a ���ụ �w�� */
	 break;
      else if(key == 0)                     /* �¸wǡ�a ���ụ �w�� */
      {
	 switch(getch_han())
	 {
	    case UP    : cur_hanja -= hanja_x;   break;
	    case DOWN  : cur_hanja += hanja_x;   break;
	    case LEFT  : cur_hanja--; 	         break;
	    case RIGHT : cur_hanja++;            break;
	    case HOME  : cur_hanja = 1;          break;
	    case END   : cur_hanja = number;     break;
	 }
	 if(cur_hanja < 0)
	    cur_hanja = 0;
	 else if(cur_hanja > number)
	    cur_hanja = number;
      }
   }

   attr_han = attr2;
   restore_text_han(box_x1, box_y1, screen);
   return((key == _ESC) ? ESC : ENTER);
}
