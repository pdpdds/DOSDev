/****************************************************************************/
/*   TITLE              ��a �A�� <������> 1.0                              */
/*   SUB-TITLE          ���� �q���i                                         */
/*   FILENAME           mylib.c                                             */
/*   DATE & TIME        92/6/6 (ɡ) 11:20                                   */
/*   PROGRAMMER         ���eѡ (ID:easyride)                                */
/****************************************************************************/

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                                �q�� ���w                                 */
/****************************************************************************/

/* MYLIB.c */
int my_popup_han ( int x , int y , char **string , int number , int *current );
int my_pulldown_han ( pulldown_menu_struct *menu_data );
int my_choose_hanja ( char *source , char *dest );
int my_get_yesno ( int y , char *string );
int my_adv_gets_han ( int x , int y , char *string , int limit , int attr , char *user_exit_key );
void draw_3dborder ( int left , int top , int right , int bottom , int is_relief );
void draw_3dbox ( int left , int top , int right , int bottom , int color );
void put_msg ( char *string , int wait_time );
int note_pad ( int box_x , int box_y , char **text , int height , int width );
//int error_handler ( int errval , int ax , int bp , int si );
int error_handler ( void );
void show_help ( void );
#if 0

/* <�e> �� �a���a�១ �q���i�i �Ÿ��e �a��Ϣ �wȁ�A�� ���w�i �� ������ ���� */
/* �e �q���i. ���� ���A �b�a�i �a�w�a�a, getimage(), putimage()�� ���e�i �� */
/* �w�a�� �����e�a.                                                         */
int  my_popup_han(int x, int y, char **string, int number, int *current);
int  my_pulldown_han(pulldown_menu_struct *menu_data);
int  my_choose_hanja(char *source, char *dest);
int  my_get_yesno(int box_y, char *string);
int  my_adv_gets_han(int x, int y, char *string, int limit, int attr,
		     char *user_exit_key);

/* ���A �b�a�A �ŝe�E �q���i */
void draw_3dborder(int left, int top, int right, int bottom, int is_relief);
void draw_3dbox(int left, int top, int right, int bottom, int color);

/* ���a */
void put_msg(char *string, int wait_time);
int  note_pad(int box_x, int box_y, char **text, int height, int width);
int  error_handler(int errval, int ax, int bp, int si);
void show_help(void);
#endif

/****************************************************************************/
/*                                �q�� ���A                                 */
/****************************************************************************/

int my_popup_han(int x, int y, char **string, int number, int *current)
/* popup_han() �i �Ÿ��e �a��Ϣ �ŉw�A�� �a�w�i �� ������ �����e �q��. */
/* ���e �១�e �A�A�a�e popup_han()�� �Ÿ�ӡ �{�a. popup_han()�� ���a  */
/* �� ���aЁ �� ��.                                                    */
{
   char *screen;
   unsigned size;
   int  key, stringlen, x2, y2, quit, i;

   if(number == 0)
   /* �A�A�a �a�a�� ���a�e */
   {
      key = getch_han();
      if(!key)
      {
         key = getch_han();
         if(key == LEFT || key == RIGHT) return(key);
      }
      if(key == _ENTER)
         return(ENTER);
      else if(key == _ESC)
         return(ESC);
      else
         return(0);
   }

   /* �A�A �b�a�� ͢�i ���e�e�a. */
   stringlen = strlen(string[0]);
   for(i = 0; i < number; i++)
      if(strlen(string[i]) > stringlen) stringlen = strlen(string[i]);
   x2 = x + stringlen + 1;
   y2 = y + number + 1;

   /* ���e�i ��w�e�a. */
   size = imagesize(x*8+4, y*16+13, x2*8+2, y2*16+3);
   if((screen = malloc(size)) == NULL)
   {
      put_msg("�A�����a �����s���a. �a�� ǡ�a ���a�A�a.", -1);
      return(ESC);
   }
   getimage(x*8+4, y*16+13, x2*8+2, y2*16+3, screen);

   /* ���e�i ������ �A�����i �a���a. */
   clear_text_han(x+1, y+1, x2-1, y2-1);
   if(in_vga_system)
      for(i = 0; i < 2; i++)
      {
         draw_horiz_line(x*8+6,    y*16+14+i, (x2-x-1)*8+4,  bkcolor_han);
         draw_horiz_line(x*8+6,    y2*16+1-i, (x2-x-1)*8+4,  bkcolor_han);
         draw_vert_line( x*8+6+i,  y*16+14,   (y2-y-1)*16+4, bkcolor_han);
         draw_vert_line( x2*8+i,   y*16+14,   (y2-y-1)*16+4, bkcolor_han);
      }
   draw_3dborder(x*8+4, y*16+13, x2*8+2, y2*16+3, 1);

   /* �A�A ���a�i�i �b�e�a. */
   for(i = 0; i < number; i++) aputs_han(x+1, y+1+i, string[i], FONT_NORMAL);
   aputs_han(x+1, y+1+(*current), string[*current], FONT_INVERSE);

   quit = 0;
   while(!quit)
   {
      key = getch_han();
      if(!key)
      /* �¸w ǡ�a ���v�a�e */
      {
         key = getch_han();

         if(key == UP   || key == DOWN || key == LEFT || key == RIGHT ||
            key == HOME || key == END)
            aputs_han(x+1, y+1+(*current), string[*current], FONT_NORMAL);
         /* ���a�� ǡ�A �a�a �A�A �b���i ������ǥ�a. */
         switch(key)
         {
            case UP    : *current = (number + *current - 1) % number;
                         break;
            case DOWN  : *current = (*current + 1) % number;
                         break;
            case LEFT  :
            case RIGHT : quit = 1;
                         break;
            case HOME  : *current = 0;
                         break;
            case END   : *current = number - 1;
                         break;
         }
         aputs_han(x+1, y+1+(*current), string[*current], FONT_INVERSE);
         if(quit) break;
      } /* if {}�� �{ */
      else
      /* ���e ǡ�a ���a�v�a�e */
      {
         if(key == _ENTER)
         {
            key = ENTER;
            break;
         }
         else if(key == _ESC)
         {
            key = ESC;
            break;
         }
      } /* else {}�� �{ */
   } /* while {}�� �{ */

   /* ���e�i �����e�a. */
   putimage(x*8+4, y*16+13, screen, COPY_PUT);
   free(screen);
   return(key);
}

int my_pulldown_han(pulldown_menu_struct *menu_data)
/* pulldown_han()�i �Ÿ��e �a��Ϣ �ŉw�A�� �a�w�i �� ������ �����e �q��.  */
/* ���e �១�e �A�A�a�e pulldown_han()�� �Ÿ�ӡ �{�a. pulldown_han()�� �� */
/* �a�� ���aЁ �� ��.                                                     */
{
   char *screen;
   unsigned size;
   int key, i;

   /* ���e�i ��w�e�a. */
   size = imagesize(0, 0, abs_maxx_han*8+7, 27);
   if((screen = malloc(size)) == NULL)
   {
      /* Ή�a�� �A�A�e ���A�A�� �a���� �����A, Ή�a�� �A�A�i �a���i ���� */
      /* �� �A������ ���a�e �a�� ��З�i ���a��ǡ�e ���� �u�a.            */
      close_han();
      puts("Memory is not enough..");
      exit(1);
   }
   getimage(0, 0, abs_maxx_han*8+7, 27, screen);

   /* Ή�a�� �A�A�� �A�� �w�e�i �����e�a. */
   if(in_vga_system)
      naputs_han(0, 0, "", abs_maxx_han+1, FONT_INVERSE);
   else
      nputs_han(0, 0, "", abs_maxx_han+1);
   /* ���A�A ���a�i�i �b�e�a. */
   output_offset_han = 8;
   if(in_vga_system)
   {
      nputs_han(0, 0, "", abs_maxx_han+1);
      for(i = 0; i < menu_data->number; i++)
         xputs_han(menu_data->x[i], 0, menu_data->string[i]);
   }
   else
   {
      naputs_han(0, 0, "", abs_maxx_han+1, FONT_INVERSE);
      for(i = 0; i < menu_data->number; i++)
         aputs_han(menu_data->x[i], 0, menu_data->string[i], FONT_INVERSE);
   }
   for(i = 0; i < 2; i++)
   {
      draw_horiz_line(0,  6+i, (abs_maxx_han+1)*8, LIGHTGRAY);
      draw_horiz_line(0, 24+i, (abs_maxx_han+1)*8, LIGHTGRAY);
   }
   if(in_vga_system) draw_3dborder(0, 5, abs_maxx_han*8+7, 27, 1);

   while(1)
   {
      if(in_vga_system)
         aputs_han(menu_data->x[menu_data->current], 0,
                   menu_data->string[menu_data->current], FONT_INVERSE);
      else
         aputs_han(menu_data->x[menu_data->current], 0,
                   menu_data->string[menu_data->current], FONT_NORMAL);

      output_offset_han = 0;
      key = my_popup_han(menu_data->sub_menu[menu_data->current].box_x,
                         menu_data->sub_menu[menu_data->current].box_y,
                         menu_data->sub_menu[menu_data->current].string,
                         menu_data->sub_menu[menu_data->current].number,
                         &(menu_data->sub_menu[menu_data->current].current));

      if(key == ESC || key == ENTER) break;
      output_offset_han = 8;
      if(in_vga_system)
         aputs_han(menu_data->x[menu_data->current], 0,
                   menu_data->string[menu_data->current], FONT_NORMAL);
      else
         aputs_han(menu_data->x[menu_data->current], 0,
                   menu_data->string[menu_data->current], FONT_INVERSE);
      if(key == LEFT)
         menu_data->current = (menu_data->current - 1 + menu_data->number)
                               % menu_data->number;
      else if(key == RIGHT)
         menu_data->current = (menu_data->current + 1) % menu_data->number;
   }

   /* ���e�i �����e�a. */
   putimage(0, 0, screen, COPY_PUT);
   free(screen);
   return(key);
}

/* �e�a�i �a���a�e �����A �ŉ��E ���a�� �w�� */
#define MAX_HANJA_X   10         /* �a���� �a���a�e ���� */
#define MAX_HANJA_Y    7         /* �A���� �a���a�e ���� */

/* <�e>�� �����A�� �a�w�A�e �q�� hangul_to_hanja()�i ����e�a. �� �q���e  */
/* š�a�a source�� �e�a�i �����A�� ���� �e�a ����A ��w�e�a.             */
int hangul_to_hanja(unsigned char *string);

int my_choose_hanja(char *source, char *dest)
/* choose_hanja()�i �Ÿ��e �a��Ϣ �ŉw�A�� �a�w�i �� ������ �����e �q��. */
/* ���e �១�e �A�A�a�e choose_hanja()�� �Ÿ�ӡ �{�a. choose_hanja()��   */
/* ���a�� ���aЁ �� ��.                                                  */
{
   unsigned char buffer[MAX_HANJA_X*MAX_HANJA_Y][2], key;
   int number, cur_ptr, hanja_x, i;
   int box_x1, box_y1, box_x2, box_y2;
   int cur_hanja, prv_hanja;
   char *screen;
   unsigned size;

   /* �e�� �e�a ����i �a��ǡ�e ͡����i cur_ptr�A ��w */
   cur_ptr = hanja_buff_ptr;
   /* š�a�a source�� �e�a�i �����A�� ���e�a. */
   if((number = hangul_to_hanja(source)) == 0) return -1;
   hanja_x = number / MAX_HANJA_Y + 1;
   if(hanja_x < 5) hanja_x = 5;

   /* ���e �e�a�i buffer�A ��w�e�a. */
   buffer[0][0] = source[0];
   buffer[0][1] = source[1];
   for(i = 1; i < number + 1 && i < MAX_HANJA_X * MAX_HANJA_Y; i++)
   {
      buffer[i][0] = (hanja_buff[cur_ptr].code & 0xff00) >> 8;
      buffer[i][1] = (hanja_buff[cur_ptr].code & 0x00ff);
      cur_ptr = (cur_ptr + 1) % hanja_buff_size;
   }

   /* �b�a�� ���a�i ���e�e�a. */
   box_x1 = (abs_maxx_han - hanja_x * 3 - 3) / 2;
   box_y1 = (abs_maxy_han - MAX_HANJA_Y - 4) / 2;
   box_x2 = box_x1 + hanja_x * 3 + 2;
   box_y2 = box_y1 + MAX_HANJA_Y + 3;

   /* ���e�i ��w�e�a. */
   size = imagesize(box_x1*8, box_y1*16, (box_x2+1)*8-1, (box_y2+1)*16-1);
   if((screen = malloc(size)) == NULL) return -2;
   getimage(box_x1*8, box_y1*16, (box_x2+1)*8-1, (box_y2+1)*16-1, screen);

   /* ���e�i �����e�a. */
   draw_3dbox(box_x1, box_y1, box_x2, box_y2, LIGHTGRAY);
   draw_horiz_line(box_x1*8+4, (box_y1+2)*16+8, (hanja_x*3+2)*8-2, BLUE);
   aputs_han(box_x1+(box_x2-box_x1-3)/2, box_y1+1, "�e�a", FONT_NORMAL);
   for(i = 0; i < number + 1; i++) aput_han_font(box_x1+2+(i%hanja_x)*3,
                                                 box_y1+3+(i/hanja_x),
                                                 buffer[i][0], buffer[i][1],
                                                 FONT_NORMAL);
   /* �a�w�a�� ��Ȃ�i �h�e�a. */
   prv_hanja = 0;
   cur_hanja = 1;
   while(1)
   {
      if(cur_hanja != prv_hanja)
      {
         aput_han_font(box_x1+2+(prv_hanja%hanja_x)*3,
                       box_y1+3+(prv_hanja/hanja_x),
                       buffer[prv_hanja][0], buffer[prv_hanja][1],
                       FONT_NORMAL);
         aput_han_font(box_x1+2+(cur_hanja%hanja_x)*3,
                       box_y1+3+(cur_hanja/hanja_x),
                       buffer[cur_hanja][0], buffer[cur_hanja][1],
                       FONT_INVERSE);
         prv_hanja = cur_hanja;
      }

      key = getch_han();
      if(key == _ENTER)
      {
         dest[0] = buffer[cur_hanja][0];
         dest[1] = buffer[cur_hanja][1];
         dest[2] = 0;   /* NULL terminator */
         break;
      }
      else if(key == _ESC)
         break;
      else if(key == 0)
      {
         switch(getch_han())
         {
            case UP    : cur_hanja -= hanja_x;   break;
            case DOWN  : cur_hanja += hanja_x;   break;
            case LEFT  : cur_hanja--;            break;
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

   /* ���e�i �����e�a. */
   putimage(box_x1*8, box_y1*16, screen, COPY_PUT);
   free(screen);
   return((key == _ESC) ? ESC : ENTER);
}

int my_get_yesno(int y, char *string)
/* get_yesno_han()�i �Ÿ��e �a��Ϣ �ŉw�A�� �a�w�i �� ������ �����e �q��.  */
/* get_yesno_han()�� �Ÿ�ӡ �a�a�a. y�e �b�a�� y ���a���� string�e �b�a��  */
/* �����A �a���A�e ���a�i���a.                                             */
/* ���a�� ǡ�A �a�a YES, NO, ESC�i �����e�a.                               */
{
   int box_x, cur, move_bar, key, quit;
   char *screen;
   unsigned size;

   /* �b�a�� x ���a�i ���e�e�a. */
   box_x = (abs_maxx_han - (strlen(string) + 2)) / 2;

   /* ���e�i ��w�e�a. */
   size = imagesize(box_x*8, y*16, (box_x+strlen(string)+2)*8-1, (y+5)*16-1);
   if((screen = malloc(size)) == NULL) return(ESC);
   getimage(box_x*8, y*16, (box_x+strlen(string)+2)*8-1, (y+5)*16-1, screen);

   /* ���e�i �����e�a. */
   draw_3dbox(box_x, y, box_x+strlen(string)+1, y+3, LIGHTGRAY);
   output_offset_han = 12;
   vbputs_han(box_x+1, y, string, BLACK, LIGHTGRAY);
   draw_horiz_line(box_x*8+5, (y+2)*16, strlen(string)*8+4, BLACK);
   output_offset_han = 4;
   vbputs_han(box_x+1+3, y+2, "��", BLACK, LIGHTGRAY);
   vbputs_han(box_x+strlen(string)-9, y+2, "�a����", BLACK, LIGHTGRAY);

   cur = quit = 0;
   move_bar = 1;
   while(!quit)
   {
      if(move_bar)
      {
         /* �e�� �b���i ������ǥ�a. */
         if(cur == 0)
         {
            modify_text_attr(box_x+1, y+2, 39, y+2, FONT_INVERSE);
            modify_text_attr(abs_maxx_han/2, y+2, box_x+strlen(string), y+2,
                             FONT_NORMAL);
         }
         else
         {
            modify_text_attr(box_x+1, y+2, 39, y+2, FONT_NORMAL);
            modify_text_attr(abs_maxx_han/2, y+2,
                             box_x+strlen(string), y+2, FONT_INVERSE);
         }
      }
      if(quit) break;

      if((key = getch_han()) == 0)
      {
         switch(getch_han())
         {
            case LEFT  :
            case RIGHT : cur = (cur + 1) % 2; break;
         }
      }
      else
      {
         switch(key)
         {
            case 'y'    :
            case 'Y'    : cur = 0; quit = 1; break;
            case 'n'    :
            case 'N'    : cur = 1; quit = 1; break;
            case _ENTER :
            case _ESC   : quit = 1; break;
         }
      }
   }


   /* ���e�i �����e�a. */
   putimage(box_x*8, y*16, screen, COPY_PUT);
   free(screen);

   output_offset_han = 0;
   if(key == 'y' || key == 'Y') return(YES);
   if(key == 'n' || key == 'n') return(NO);
   if(key == _ENTER) return(cur == 0 ? YES : NO);
   return(ESC);
}

int my_adv_gets_han(int x, int y, char *string, int limit, int attr,
                    char *user_exit_key)
/* adv_gets_han()�� �i ǡ ���w�i ���w�a�a �e�a�i ���b�i �� ������ �e�e */
/* �q��. �a�ỡ ���w�� �a�w��e adv_gets_han()�� �Ÿ�ӡ �{�a.            */
{
   unsigned char exit_key[50] = { F9, F10, 0 }, key;
   char hanja_code[3], hangul_code[3];
   int ret_value;

   /* exit_key�A �a�w�a�a �����e �i ǡ�i �������a. */
   strcat(exit_key, user_exit_key);
   while(1)
   {
      key = adv_gets_han(x, y, string, limit, attr, exit_key);
      /* adv_gets_han()�� ѡ�E ��ҁ�A adv_gets_han_index�e */
      /* �e���� ��� ��á�A ���e �����i �x�e�a.              */

      if(key != F9 && key != F10) break;
      if(key == F9 && (string[adv_gets_han_index] & 0x80))
      /* �e�� ��� ��á�� �i�a�a �e�i�� �� Ё�w�a�e �e�a�i ��Ȃ�h�e�a.*/
      {
         ret_value = my_choose_hanja(string+adv_gets_han_index, hanja_code);
         if(ret_value == ENTER)
         /* ��Ȃ�E �e�a�� š�a�� �a���a. */
         {
            string[adv_gets_han_index]     = hanja_code[0];
            string[adv_gets_han_index + 1] = hanja_code[1];
         }
      }
      else if(key == F10 && (unsigned char) string[adv_gets_han_index] >= 0xe0)
      /* �e�� ��� ��á�� �i�a�a �e�a�� ���e �e�i�� �a���a. */
      {
         hanja_to_hangul(string+adv_gets_han_index, hangul_code);
         string[adv_gets_han_index]     = hangul_code[0];
         string[adv_gets_han_index + 1] = hangul_code[1];
      }
      else
      /* <F9>, <F10> ǡ�a �១�A�� �g�e �w���A�e ���B�a�i �a���� ���a. */
      /* �b, ��᷁ ��á�i �a���� �g�e�a.                              */
      {
         adv_gets_han_index = adv_gets_han_index * (-1) - 1;
         continue;
      }

      /* <F9>, <F10> ǡ�a �១�E �w�� �a�� adv_gets_han()�� ѡ�A */
      /* �e �A���� ��� ��á�a �e���� ��� ��á �a�� �a�q�� �A���� */
      /* adv_gets_han_index�� �t�i �����e�a.                       */
      /* ���� : adv_gets_han_index = ���a�e ��� ��á * (-1) - 1   */
      adv_gets_han_index = (adv_gets_han_index + 2) * (-1) - 1;
      if(adv_gets_han_index > strlen(string)) adv_gets_han_index = strlen(string);
   }
   return(key);
}

void draw_3dborder(int left, int top, int right, int bottom, int is_relief)
/* EGA/VGA�A�� ���A �b�a�i �e�a�e �A ϩ�a�e �A�����i �a���e �q��. */
/* left, top, right, bottom�e ���� �a��Ϣ ���a�a.                 */
/* is_relief�a 1���e ���b, 0���e �q�b�a�� �A�����i �a���a.        */
{
   int width, height;

   if(!in_vga_system)
   {
      width  = right - left;
      height = bottom - top - 1;

      draw_vert_line(left,    top,   height, WHITE);
      draw_vert_line(right-1, top,   height, WHITE);
      draw_vert_line(right,   top+1, height, WHITE);

      draw_horiz_line(left,   top,      width,  WHITE);
      draw_horiz_line(left+1, bottom,   width,  WHITE);
      draw_horiz_line(left,   bottom-1, width,  WHITE);
   }
   else
   {
      width  = right - left + 1;
      height = bottom - top + 1;

      if(is_relief)
      {
         draw_horiz_line(left,   top,      width,   WHITE);
         draw_horiz_line(left,   bottom,   width,   DARKGRAY);
         draw_horiz_line(left+1, bottom-1, width-2, DARKGRAY);

         draw_vert_line(left,    top,   height,   WHITE);
         draw_vert_line(left+1,  top+1, height-2, WHITE);
         draw_vert_line(right,   top,   height,   DARKGRAY);
         draw_vert_line(right-1, top+1, height-2, DARKGRAY);
      }
      else
      {
         draw_horiz_line(left,   top,      width,   DARKGRAY);
         draw_horiz_line(left,   bottom,   width,   WHITE);
         draw_horiz_line(left+1, bottom-1, width-2, WHITE);

         draw_vert_line(left,    top,   height,   DARKGRAY);
         draw_vert_line(left+1,  top+1, height-2, DARKGRAY);
         draw_vert_line(right,   top,   height,   WHITE);
         draw_vert_line(right-1, top+1, height-2, WHITE);
      }
   }
}

void draw_3dbox(int left, int top, int right, int bottom, int color)
/* �锁 ���a�� (left, top) - (right, bottom) �w�b�A color�A �����E ���a�� */
/* ���A �b�a�i �a���e �q��                                                */
{
   int x1, y1, x2, y2;

   /* left, top, right, bottom�A Ё�w�a�e �a��Ϣ ���a�i ���e */
   x1     = left << 3;
   y1     = top  << 4;
   x2     = (right  << 3) + 7;
   y2     = (bottom << 4) + 15;

   vclear_text_han(left, top, right, bottom, color);
   if(in_vga_system)
   {
      draw_3dborder(x1,   y1,   x2,   y2,   1);
      draw_3dborder(x1+4, y1+8, x2-4, y2-8, 0);
   }
   else
   {
      draw_3dborder(x1, y1, x2, y2, 0);
      draw_horiz_line(x1+5, y1+9, x2-x1-9,  WHITE);
      draw_horiz_line(x1+5, y2-9, x2-x1-9,  WHITE);
      draw_vert_line( x1+5, y1+9, y2-y1-17, WHITE);
      draw_vert_line( x2-5, y1+9, y2-y1-17, WHITE);
   }
}

void put_msg(char *string, int wait_time)
/* �A�����i �b�e ҁ wait_time�� ���e�e �a�w�a�� ǡ ���b�i ���a���a. */
/* wait_time�� -1���e ���� ǡ ���b�i ���a���a.                        */
{
   char *screen;
   unsigned size, box_x;

   /* �b�a�� x ���a�i ���e�e�a. */
   box_x = (abs_maxx_han - (strlen(string) + 2)) / 2;

   /* ���e�i ��w�e�a. */
   size = imagesize(box_x*8, 10*16, (box_x+strlen(string)+2)*8-1, 13*16-1);
   if((screen = malloc(size)) == NULL) return;
   getimage(box_x*8, 10*16, (box_x+strlen(string)+2)*8-1, 13*16-1, screen);

   /* ���e�i �����e�a. */
   draw_3dbox(box_x, 10, box_x+strlen(string)+1, 12, LIGHTGRAY);
   vbputs_han(box_x + 1, 11, string, BLACK, LIGHTGRAY);

   /* ǡ�i ���b�h�e�a. */
   if(wait_key_han(wait_time))
   {
      if(!getch()) getch();
   }

   /* ���e�i �����e�a. */
   putimage(box_x*8, 10*16, screen, COPY_PUT);
   free(screen);
}

int note_pad(int box_x, int box_y, char **text, int height, int width)
/* �����a width�� ���a�i�i height�� ���b�h�a text�A ��w�e�a.    */
/* ����A�e �t�e ���a�i �� �a�w�E ǡ�� š�a���A ENTER, ESC, CTR  */
/* LEND �� �a�a���a.                                             */
{
   unsigned char exit_key[] = { UP, DOWN, CTRLPGUP, CTRLPGDN, CTRLEND, 0 };
   int cur_line, key, index;

   cur_line = 0;
   adv_gets_han_index = 0 * (-1) - 1;
   while(1)
   {
      key = my_adv_gets_han(box_x, box_y+cur_line, text[cur_line],
                            width, attr_han, exit_key);
      /* adv_gets_han()�� ѡ�E ��ҁ�A�e adv_gets_han_index�e �e�� */
      /* �� ��� ��á�A ���e �����i �x�e�a.                         */

      if(key == ESC || key == CTRLEND)
         break;
      else if(key == ENTER)
      {
         /* �A�� �{ ���A�� <Enter> ǡ�i ���a�e ���b�i ���a�e�a. */
         if(++cur_line == height) break;
      }
      else if(key == UP)
      /* �e �� �ᝡ ���� */
      {
         /* �A�� �� ���A�� <Up> ǡ�i ���a�e ���b�i ���a�e�a. */
         if(cur_line-- == 0) break;
      }
      else if(key == DOWN)
      /* �e �� �a���� ���� */
      {
         /* �A�� �{ ���A�� <Down> ǡ�i ���a�e ���b�i ���a�e�a. */
         if(++cur_line == height) break;
      }
      else if(key == CTRLPGUP)
      /* �A�� �� ���� ���� */
         cur_line = 0;
      else if(key == CTRLPGDN)
      /* �A�� �{ ���� ���� */
         cur_line = height - 1;

      /* �a�� adv_gets_han()�� ѡ�A���i ���� �A�� ��� ��á�i ��  */
      /* ���e�a.                                                    */
      /* <Enter>, <Ctrl>+<PgUp>, <Ctrl>+<PgDn> -> �A�� �� �i        */
      /* <Up>, <Down>                          -> �e�� ��� ��á    */
      /* �e�� �����e ���� ���a�i �����a �A�� ��� ��á���a �l�i  �� */
      /* �e ���a�i�� �A�� �{�A ��á�a���� �����e�a.                 */
      /* ���� : adv_gets_han_index = ���a�e ��� ��á * (-1) - 1    */
      if(key == ENTER || key == CTRLPGUP || key == CTRLPGDN)
         adv_gets_han_index = 0 * (-1) - 1;
      else
      {
         if(strlen(text[cur_line]) < adv_gets_han_index)
         /* ���� �����e ���� ���a�i �����a �� �l�a�e ���a�i�� �A�� �{�A */
            adv_gets_han_index = strlen(text[cur_line]) * (-1) - 1;
         else
         /* ��ӡ ���a�e �e�� ��� ��á�A */
            adv_gets_han_index = adv_gets_han_index * (-1) - 1;
      }
   }
   return(key);
}

//int error_handler(int errval, int ax, int bp, int si)
int error_handler(void)
/* �a�a����ⷥ �A��a �i���a�v�i �� ��З�A�e �q��  */
/* ���a�����A 4���� �w���a �a���e�A �����a�e �E�a.  */
{
   put_msg("�A��a �i���a�v�s���a. �a�� ǡ�a ���a�A�a.", -1);
   hardresume(0);       /* �A��i �����e�a. */
   return(0);
}

/* �����i�� ������ ���e �a���A �ŝe�E ���a�� �w�� */
#define HELP_LINE       14                   /* ���e�A �a���A�e �� ��    */
#define MAX_PAGE         4                   /* �a���i �� ���e �A���� �� */
#define MAX_HELP        HELP_LINE * MAX_PAGE /* �a���i �� ���e �� ��     */

void show_help(void)
/* quiz.hlp �����i ����� �����i�i ���a ���a. */
{
   FILE *stream;
   char (*help)[68+2], *screen;
   int  cur_page, re_draw, line_no, key, i;

   /* �����i�A ϩ�a�e �A�����i �i�w�e�a. */
   if((help = malloc((68+2)*MAX_HELP)) == NULL)
   {
      put_msg("�A�����a �����s���a. �a�� ǡ�a ���a�A�a.", -1);
      return;
   }

   /* �����i ���� quiz.hlp�� ���w�i ���� help�A ��w�e�a. */
   if((stream = fopen("quiz.hlp", "rt")) == NULL)
   {
      put_msg("�����i �����i ���i �� ���s���a. �a�� ǡ�a ���a�A�a.", -1);
      return;
   }
   line_no = 0;
   while(!feof(stream))
      if(line_no >= MAX_HELP || fgets(help[line_no], 70, stream) == NULL)
         break;
      else
      {
         /* ���a�i�� �{�A ���e '\n' ���a�e �b�A�e�a. */
         i = strlen(help[line_no]);
         if(help[line_no][i - 1] == '\n') help[line_no][i - 1] = 0;
	 line_no++;
      }
   fclose(stream);

   /* ���e�i �����e�a. */
   draw_3dbox(4, 2, abs_maxx_han-4, 2+HELP_LINE+3, LIGHTGRAY);
   set_output_offset_han(12);
   vputs_han(35, 2, "�� �� �i", RED);
   draw_horiz_line(4*8+5, 4*16-2,  (abs_maxx_han-9)*8+5, BLUE);
   draw_horiz_line(4*8+5, 18*16+2, (abs_maxx_han-9)*8+5, BLUE);
   set_output_offset_han(4);
   vputs_han(8, 18, "Up/PgUp : �| �A����   Down/PgDn : �a�q �A����"
                    "   Esc : �a�e ����", YELLOW);
   set_output_offset_han(0);

   /* help�A ��w�E �����i�i ���a ���a. */
   cur_page = 0;
   re_draw  = 1;
   while(1)
   {
      if(re_draw)
      /* �A�����i �a�����a�e �����i�i ���� �a���e�a. */
      {
         for(i = 0; i < HELP_LINE; i++)
            if(cur_page*HELP_LINE+i >= line_no)
               nputs_han(6, 4+i, "", 68);
            else
               nputs_han(6, 4+i, help[cur_page*HELP_LINE+i], 68);
      }

      /* �a�w�a�� ǡ ���b�i �១�e�a. */
      key = getch_han();
      if(key == _ESC)
         break;
      else if(key == 0)
      {
	 switch(getch_han())
         {
            case UP   :
            case PGUP : if(cur_page > 0) cur_page--;
                        re_draw = 1;
                        break;
            case DOWN :
            case PGDN : if(cur_page < MAX_PAGE - 1) cur_page++;
                        re_draw = 1;
                        break;
         }
      }
   }

   /* help�A �i�w�E �A�����i ���ᯡǥ�a. */
   free(help);
}

