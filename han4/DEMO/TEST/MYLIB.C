#include <graphics.h>
#include <stdio.h>
#include <alloc.h>
#include <hanio.h>
#include <extendio.h>
#include <keydef.h>

/****************************************************************************/
/*                                Ðq® ¶¥Ñw                                 */
/****************************************************************************/

/* <Ðe> · œa·¡§aœáŸ¡ Ðq®—i·i µÅ¸åÐe ‹aœÏ¢ ¬wÈµA¬á ·¡¶wÐi ® ·¶•¡¢ ®¸÷ */
/* Ðe Ðq®—i. ¡¡– ·³ÁA ¤b¯aŸi ¬a¶wÐa¡a, getimage(), putimage()¡ ÑÁ¡e·i ¸á */
/* ¸wÐa‰¡ ¥¢ŠÐe”a.                                                         */
int  my_popup_han(int x, int y, char **string, int number, int *current);
int  my_pulldown_han(pulldown_menu_struct *menu_data);
int  my_choose_hanja(char *source, char *dest);
int  my_get_yesno(int box_y, char *string);
int  my_adv_gets_han(int x, int y, char *string, int limit, int attr,
                     char *user_exit_key);

/* ·³ÁA ¤b¯aµA ‰Åe–E Ðq®—i */
void draw_3dborder(int left, int top, int right, int bottom, int is_relief);
void draw_3dbox(int left, int top, int right, int bottom, int color);

/* ‹¡Èa */
void put_msg(char *string, int wait_time);
int  note_pad(int box_x, int box_y, char **text, int height, int width);
int  error_handler(int errval, int ax, int bp, int si);
void show_help(void);

/****************************************************************************/
/*                                Ðq® ¥¥ÁA                                 */
/****************************************************************************/

int my_popup_han(int x, int y, char **string, int number, int *current)
/* popup_han() ·i µÅ¸åÐe ‹aœÏ¢ ÑÅ‰wµA¬á ¬a¶wÐi ® ·¶•¡¢ ®¸÷Ðe Ðq®. */
/* ÑÁ¡e ÀáŸ¡ e ¹A¶AÐa¡e popup_han()‰Á µÅ¸åÓ¡ ˆ{”a. popup_han()· ­¡¯a  */
/* µÁ §¡ŠaÐ ¥© ˆõ.                                                    */
{
   char *screen;
   unsigned size;
   int  key, stringlen, x2, y2, quit, i;

   if(number == 0)
   /* ¡A“Aˆa Ðaa•¡ ´ô·a¡e */
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

   /* ¡A“A ¤b¯a· Í¢·i ‰¬eÐe”a. */
   stringlen = strlen(string[0]);
   for(i = 0; i < number; i++)
      if(strlen(string[i]) > stringlen) stringlen = strlen(string[i]);
   x2 = x + stringlen + 1;
   y2 = y + number + 1;

   /* ÑÁ¡e·i ¸á¸wÐe”a. */
   size = imagesize(x*8+4, y*16+13, x2*8+2, y2*16+3);
   if((screen = malloc(size)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      return(ESC);
   }
   getimage(x*8+4, y*16+13, x2*8+2, y2*16+3, screen);

   /* ÑÁ¡e·i »¡¶‰¡ ÉA–Ÿ¡Ÿi ‹aŸ¥”a. */
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

   /* ¡A“A ¢…¸aµi·i Â‰bÐe”a. */
   for(i = 0; i < number; i++) aputs_han(x+1, y+1+i, string[i], FONT_NORMAL);
   aputs_han(x+1, y+1+(*current), string[*current], FONT_INVERSE);

   quit = 0;
   while(!quit)
   {
      key = getch_han();
      if(!key)
      /* ÑÂ¸w Ç¡ˆa ’‰v·a¡e */
      {
         key = getch_han();

         if(key == UP   || key == DOWN || key == LEFT || key == RIGHT ||
            key == HOME || key == END)
            aputs_han(x+1, y+1+(*current), string[*current], FONT_NORMAL);
         /* ’‰a»¥ Ç¡µA ˜aœa ¡A“A  b”Ÿi ·¡•·¯¡Ç¥”a. */
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
      } /* if {}· { */
      else
      /* ·©¤e Ç¡ˆa ’‰a¹v·a¡e */
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
      } /* else {}· { */
   } /* while {}· { */

   /* ÑÁ¡e·i ¥¢ŠÐe”a. */
   putimage(x*8+4, y*16+13, screen, COPY_PUT);
   free(screen);
   return(key);
}

int my_pulldown_han(pulldown_menu_struct *menu_data)
/* pulldown_han()·i µÅ¸åÐe ‹aœÏ¢ ÑÅ‰wµA¬á ¬a¶wÐi ® ·¶•¡¢ ®¸÷Ðe Ðq®.  */
/* ÑÁ¡e ÀáŸ¡ e ¹A¶AÐa¡e pulldown_han()‰Á µÅ¸åÓ¡ ˆ{”a. pulldown_han()· ­¡ */
/* ¯aµÁ §¡ŠaÐ ¥© ˆõ.                                                     */
{
   char *screen;
   unsigned size;
   int key, i;

   /* ÑÁ¡e·i ¸á¸wÐe”a. */
   size = imagesize(0, 0, abs_maxx_han*8+7, 27);
   if((screen = malloc(size)) == NULL)
   {
      /* Î‰”a¶… ¡A“A“e º¡A“A¡ ³a·¡‹¡ ˜¢…µA, Î‰”a¶… ¡A“AŸi Îa¯¡Ði ¸÷•¡ */
      /* · ¡A¡¡Ÿ¡•¡ ´ô”a¡e ´aµ ¯©Ð—·i ¹·ža¯¡Ç¡“e ˆõ·¡ u”a.            */
      close_han();
      puts("Memory is not enough..");
      exit(1);
   }
   getimage(0, 0, abs_maxx_han*8+7, 27, screen);

   /* Î‰”a¶… ¡A“A· ¹A·© ¬w”e·i Š¬÷Ðe”a. */
   if(in_vga_system)
      naputs_han(0, 0, "", abs_maxx_han+1, FONT_INVERSE);
   else
      nputs_han(0, 0, "", abs_maxx_han+1);
   /* º¡A“A ¢…¸aµi·i Â‰bÐe”a. */
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

   /* ÑÁ¡e·i ¥¢ŠÐe”a. */
   putimage(0, 0, screen, COPY_PUT);
   free(screen);
   return(key);
}

/* Ðe¸aŸi Îa¯¡Ða“e ¦…œ·µA ‰Å‰–E  Ça¡ ¬w® */
#define MAX_HANJA_X   10         /* ˆa¡¡ Îa¯¡Ða“e ¦…œ· */
#define MAX_HANJA_Y    7         /* ­A¡¡ Îa¯¡Ða“e ¦…œ· */

/* <Ðe>· ¦µA¬á ¬a¶w–A“e Ðq® hangul_to_hanja()Ÿi ¬å´åÐe”a. ·¡ Ðq®“e  */
/* Å¡—aˆa source·¥ Ðe¸aŸi ÑÁ·©µA¬á ·ª´á Ðe¸a ¤áÌáµA ¸á¸wÐe”a.             */
int hangul_to_hanja(unsigned char *string);

int my_choose_hanja(char *source, char *dest)
/* choose_hanja()Ÿi µÅ¸åÐe ‹aœÏ¢ ÑÅ‰wµA¬á ¬a¶wÐi ® ·¶•¡¢ ®¸÷Ðe Ðq®. */
/* ÑÁ¡e ÀáŸ¡ e ¹A¶AÐa¡e choose_hanja()µÁ µÅ¸åÓ¡ ˆ{”a. choose_hanja()·   */
/* ­¡¯aµÁ §¡ŠaÐ ¥© ˆõ.                                                  */
{
   unsigned char buffer[MAX_HANJA_X*MAX_HANJA_Y][2], key;
   int number, cur_ptr, hanja_x, i;
   int box_x1, box_y1, box_x2, box_y2;
   int cur_hanja, prv_hanja;
   char *screen;
   unsigned size;

   /* Ñe¸ Ðe¸a ¤áÌáŸi ˆaŸ¡Ç¡“e Í¡·¥ÈáŸi cur_ptrµA ¸á¸w */
   cur_ptr = hanja_buff_ptr;
   /* Å¡—aˆa source·¥ Ðe¸aŸi ÑÁ·©µA¬á ·ª“e”a. */
   if((number = hangul_to_hanja(source)) == 0) return -1;
   hanja_x = number / MAX_HANJA_Y + 1;
   if(hanja_x < 5) hanja_x = 5;

   /* ·ª·e Ðe¸aŸi bufferµA ¸á¸wÐe”a. */
   buffer[0][0] = source[0];
   buffer[0][1] = source[1];
   for(i = 1; i < number + 1 && i < MAX_HANJA_X * MAX_HANJA_Y; i++)
   {
      buffer[i][0] = (hanja_buff[cur_ptr].code & 0xff00) >> 8;
      buffer[i][1] = (hanja_buff[cur_ptr].code & 0x00ff);
      cur_ptr = (cur_ptr + 1) % hanja_buff_size;
   }

   /* ¤b¯a· ¹ÁÎaŸi ‰¬eÐe”a. */
   box_x1 = (abs_maxx_han - hanja_x * 3 - 3) / 2;
   box_y1 = (abs_maxy_han - MAX_HANJA_Y - 4) / 2;
   box_x2 = box_x1 + hanja_x * 3 + 2;
   box_y2 = box_y1 + MAX_HANJA_Y + 3;

   /* ÑÁ¡e·i ¸á¸wÐe”a. */
   size = imagesize(box_x1*8, box_y1*16, (box_x2+1)*8-1, (box_y2+1)*16-1);
   if((screen = malloc(size)) == NULL) return -2;
   getimage(box_x1*8, box_y1*16, (box_x2+1)*8-1, (box_y2+1)*16-1, screen);

   /* ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_3dbox(box_x1, box_y1, box_x2, box_y2, LIGHTGRAY);
   draw_horiz_line(box_x1*8+4, (box_y1+2)*16+8, (hanja_x*3+2)*8-2, BLUE);
   aputs_han(box_x1+(box_x2-box_x1-3)/2, box_y1+1, "Ðe¸a", FONT_NORMAL);
   for(i = 0; i < number + 1; i++) aput_han_font(box_x1+2+(i%hanja_x)*3,
                                                 box_y1+3+(i/hanja_x),
                                                 buffer[i][0], buffer[i][1],
                                                 FONT_NORMAL);
   /* ¬a¶w¸a· ¬åÈ‚·i ¤h“e”a. */
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

   /* ÑÁ¡e·i ¥¢ŠÐe”a. */
   putimage(box_x1*8, box_y1*16, screen, COPY_PUT);
   free(screen);
   return((key == _ESC) ? ESC : ENTER);
}

int my_get_yesno(int y, char *string)
/* get_yesno_han()·i µÅ¸åÐe ‹aœÏ¢ ÑÅ‰wµA¬á ¬a¶wÐi ® ·¶•¡¢ ®¸÷Ðe Ðq®.  */
/* get_yesno_han()‰Á µÅ¸åÓ¡ ”aŸa”a. y“e ¤b¯a· y ¹ÁÎa·¡‰¡ string·e ¤b¯a·  */
/* ¦µA Îa¯¡–A“e ¢…¸aµi·¡”a.                                             */
/* ’‰a»¥ Ç¡µA ˜aœa YES, NO, ESCŸi Ÿ¡ÈåÐe”a.                               */
{
   int box_x, cur, move_bar, key, quit;
   char *screen;
   unsigned size;

   /* ¤b¯a· x ¹ÁÎaŸi ‰¬eÐe”a. */
   box_x = (abs_maxx_han - (strlen(string) + 2)) / 2;

   /* ÑÁ¡e·i ¸á¸wÐe”a. */
   size = imagesize(box_x*8, y*16, (box_x+strlen(string)+2)*8-1, (y+5)*16-1);
   if((screen = malloc(size)) == NULL) return(ESC);
   getimage(box_x*8, y*16, (box_x+strlen(string)+2)*8-1, (y+5)*16-1, screen);

   /* ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_3dbox(box_x, y, box_x+strlen(string)+1, y+3, LIGHTGRAY);
   output_offset_han = 12;
   vbputs_han(box_x+1, y, string, BLACK, LIGHTGRAY);
   draw_horiz_line(box_x*8+5, (y+2)*16, strlen(string)*8+4, BLACK);
   output_offset_han = 4;
   vbputs_han(box_x+1+3, y+2, "µ", BLACK, LIGHTGRAY);
   vbputs_han(box_x+strlen(string)-9, y+2, "´a“¡µ¡", BLACK, LIGHTGRAY);

   cur = quit = 0;
   move_bar = 1;
   while(!quit)
   {
      if(move_bar)
      {
         /* ¤e¸å  b”Ÿi ·¡•·¯¡Ç¥”a. */
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


   /* ÑÁ¡e·i ¥¢ŠÐe”a. */
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
/* adv_gets_han()· ÈiÂ‰ Ç¡ ‹¡“w·i ·¡¶wÐaµa Ðe¸aŸi ·³bÐi ® ·¶•¡¢  e—e */
/* Ðq®. a á»¡ ‹¡“w‰Á ¬a¶w¤ó·e adv_gets_han()‰Á µÅ¸åÓ¡ ˆ{”a.            */
{
   unsigned char exit_key[50] = { F9, F10, 0 }, key;
   char hanja_code[3], hangul_code[3];
   int ret_value;

   /* exit_keyµA ¬a¶w¸aˆa ¸÷·Ðe ÈiÂ‰ Ç¡Ÿi ”õ¦›·¥”a. */
   strcat(exit_key, user_exit_key);
   while(1)
   {
      key = adv_gets_han(x, y, string, limit, attr, exit_key);
      /* adv_gets_han()·¡ Ñ¡Â‰–E »¢ÒµA adv_gets_han_index“e */
      /* Ñe¸· Äá¬á ¶áÃ¡µA ”Ðe ¸÷¥¡Ÿi ˆx“e”a.              */

      if(key != F9 && key != F10) break;
      if(key == F9 && (string[adv_gets_han_index] & 0x80))
      /* Ñe¸ Äá¬á ¶áÃ¡· ‹i¸aˆa Ðe‹i·© ˜ Ð”wÐa“e Ðe¸aŸi ¬åÈ‚¤h“e”a.*/
      {
         ret_value = my_choose_hanja(string+adv_gets_han_index, hanja_code);
         if(ret_value == ENTER)
         /* ¬åÈ‚–E Ðe¸a· Å¡—a¡ ¤aŽ…”a. */
         {
            string[adv_gets_han_index]     = hanja_code[0];
            string[adv_gets_han_index + 1] = hanja_code[1];
         }
      }
      else if(key == F10 && (unsigned char) string[adv_gets_han_index] >= 0xe0)
      /* Ñe¸ Äá¬á ¶áÃ¡· ‹i¸aˆa Ðe¸a·© ˜“e Ðe‹i¡ ¤aŽ…”a. */
      {
         hanja_to_hangul(string+adv_gets_han_index, hangul_code);
         string[adv_gets_han_index]     = hangul_code[0];
         string[adv_gets_han_index + 1] = hangul_code[1];
      }
      else
      /* <F9>, <F10> Ç¡ˆa ÀáŸ¡–A»¡ ´g·e ‰w¶µA“e ·¥•B¯aŸi ‹a”¡ –…”a. */
      /* »b, Äá¬á· ¶áÃ¡Ÿi ¤aŽ»¡ ´g“e”a.                              */
      {
         adv_gets_han_index = adv_gets_han_index * (-1) - 1;
         continue;
      }

      /* <F9>, <F10> Ç¡ˆa ÀáŸ¡–E ‰w¶ ”a¯¡ adv_gets_han()·¡ Ñ¡Â‰–A */
      /* ¡e ÂAÁ¡· Äá¬á ¶áÃ¡ˆa Ñe¸· Äá¬á ¶áÃ¡ ¤a¡ ”a·q·¡ –A•¡¢ */
      /* adv_gets_han_index· ˆt·i ¹¡¸÷Ðe”a.                       */
      /* ‰·¯¢ : adv_gets_han_index = ¶¥Ða“e Äá¬á ¶áÃ¡ * (-1) - 1   */
      adv_gets_han_index = (adv_gets_han_index + 2) * (-1) - 1;
      if(adv_gets_han_index > strlen(string)) adv_gets_han_index = strlen(string);
   }
   return(key);
}

void draw_3dborder(int left, int top, int right, int bottom, int is_relief)
/* EGA/VGAµA¬á ·³ÁA ¤b¯aŸi  e—a“e •A Ï©¶aÐe ÉA–Ÿ¡Ÿi ‹aŸ¡“e Ðq®. */
/* left, top, right, bottom·e ¡¡– ‹aœÏ¢ ¹ÁÎa”a.                 */
/* is_reliefˆa 1·¡¡e ´·ˆb, 0·¡¡e ·qˆb·a¡ ÉA–Ÿ¡Ÿi ‹aŸ¥”a.        */
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
/* ¸é” ¹ÁÎa¡ (left, top) - (right, bottom) µwµbµA colorµA »¡¸÷–E ¬‚·a¡ */
/* ·³ÁA ¤b¯aŸi ‹aŸ¡“e Ðq®                                                */
{
   int x1, y1, x2, y2;

   /* left, top, right, bottomµA Ð”wÐa“e ‹aœÏ¢ ¹ÁÎaŸi ‰¬e */
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
/* ¡A¯¡»¡Ÿi Â‰bÐe Ò wait_timeÁ¡ •·´e e ¬a¶w¸a· Ç¡ ·³b·i ‹¡”aŸ¥”a. */
/* wait_time·¡ -1·¡¡e ‰­¢ Ç¡ ·³b·i ‹¡”aŸ¥”a.                        */
{
   char *screen;
   unsigned size, box_x;

   /* ¤b¯a· x ¹ÁÎaŸi ‰¬eÐe”a. */
   box_x = (abs_maxx_han - (strlen(string) + 2)) / 2;

   /* ÑÁ¡e·i ¸á¸wÐe”a. */
   size = imagesize(box_x*8, 10*16, (box_x+strlen(string)+2)*8-1, 13*16-1);
   if((screen = malloc(size)) == NULL) return;
   getimage(box_x*8, 10*16, (box_x+strlen(string)+2)*8-1, 13*16-1, screen);

   /* ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_3dbox(box_x, 10, box_x+strlen(string)+1, 12, LIGHTGRAY);
   vbputs_han(box_x + 1, 11, string, BLACK, LIGHTGRAY);

   /* Ç¡Ÿi ·³b¤h“e”a. */
   if(wait_key_han(wait_time))
   {
      if(!getch()) getch();
   }

   /* ÑÁ¡e·i ¥¢ŠÐe”a. */
   putimage(box_x*8, 10*16, screen, COPY_PUT);
   free(screen);
}

int note_pad(int box_x, int box_y, char **text, int height, int width)
/* ‹©·¡ˆa width·¥ ¢…¸aµi·i heightˆ ·³b¤h´a textµA ¸á¸wÐe”a.    */
/* Ÿ¡Èå–A“e ˆt·e ¹·žaÐi ˜ ¬a¶w–E Ç¡· Å¡—a·¥•A ENTER, ESC, CTR  */
/* LEND º— Ðaa·¡”a.                                             */
{
   unsigned char exit_key[] = { UP, DOWN, CTRLPGUP, CTRLPGDN, CTRLEND, 0 };
   int cur_line, key, index;

   cur_line = 0;
   adv_gets_han_index = 0 * (-1) - 1;
   while(1)
   {
      key = my_adv_gets_han(box_x, box_y+cur_line, text[cur_line],
                            width, attr_han, exit_key);
      /* adv_gets_han()·¡ Ñ¡Â‰–E »¢ÒµA“e adv_gets_han_index“e Ñe¸ */
      /* · Äá¬á ¶áÃ¡µA ”Ðe ¸÷¥¡Ÿi ˆx“e”a.                         */

      if(key == ESC || key == CTRLEND)
         break;
      else if(key == ENTER)
      {
         /* ¹A·© { º‰µA¬á <Enter> Ç¡Ÿi ’Ÿa¡e ·³b·i ¹·žaÐe”a. */
         if(++cur_line == height) break;
      }
      else if(key == UP)
      /* Ðe º‰ ¶á¡ ·¡•· */
      {
         /* ¹A·© Àõ º‰µA¬á <Up> Ç¡Ÿi ’Ÿa¡e ·³b·i ¹·žaÐe”a. */
         if(cur_line-- == 0) break;
      }
      else if(key == DOWN)
      /* Ðe º‰ ´aœ¡ ·¡•· */
      {
         /* ¹A·© { º‰µA¬á <Down> Ç¡Ÿi ’Ÿa¡e ·³b·i ¹·žaÐe”a. */
         if(++cur_line == height) break;
      }
      else if(key == CTRLPGUP)
      /* ¹A·© Àõ º‰¡ ·¡•· */
         cur_line = 0;
      else if(key == CTRLPGDN)
      /* ¹A·© { º‰¡ ·¡•· */
         cur_line = height - 1;

      /* ”a¯¡ adv_gets_han()·¡ Ñ¡Â‰–A´ö·i ˜· ÂAÁ¡ Äá¬á ¶áÃ¡Ÿi ¹¡  */
      /* ¸÷Ðe”a.                                                    */
      /* <Enter>, <Ctrl>+<PgUp>, <Ctrl>+<PgDn> -> ¹A·© Àõ µi        */
      /* <Up>, <Down>                          -> Ñe¸ Äá¬á ¶áÃ¡    */
      /*  e´¢ ·¡•·Ðe º‰· ¢…¸aµi ‹©·¡ˆa ÂAÁ¡ Äá¬á ¶áÃ¡¥¡”a ¼l·i  ˜ */
      /* “e ¢…¸aµi· ¹A·© {µA ¶áÃ¡Ða•¡¢ ¹¡¸÷Ðe”a.                 */
      /* ‰·¯¢ : adv_gets_han_index = ¶¥Ða“e Äá¬á ¶áÃ¡ * (-1) - 1    */
      if(key == ENTER || key == CTRLPGUP || key == CTRLPGDN)
         adv_gets_han_index = 0 * (-1) - 1;
      else
      {
         if(strlen(text[cur_line]) < adv_gets_han_index)
         /* ¬¡ ·¡•·Ðe º‰· ¢…¸aµi ‹©·¡ˆa ”á ¼l”a¡e ¢…¸aµi· ¹A·© {µA */
            adv_gets_han_index = strlen(text[cur_line]) * (-1) - 1;
         else
         /* Â—¦…Ó¡ ‹©”a¡e Ñe¸ Äá¬á ¶áÃ¡µA */
            adv_gets_han_index = adv_gets_han_index * (-1) - 1;
      }
   }
   return(key);
}

int error_handler(int errval, int ax, int bp, int si)
/* Ða—a¶Á´á¸â·¥ µAœáˆa ¤i¬—Ðaµv·i ˜ ¯©Ð—–A“e Ðq®  */
/* ÄñÌa·©¯¡µA 4ˆ· ‰w‰¡ˆa aµ¡“e•A ¢¯¡Ða¡e –E”a.  */
{
   put_msg("µAœáˆa ¤i¬—Ðaµv¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
   hardresume(0);       /* µAœáŸi ¢¯¡Ðe”a. */
   return(0);
}

/* •¡¶‘ i· ¦…œ·‰Á ÑÁ¡e Ça‹¡µA ‰Åe–E  Ça¡ ¬w® */
#define HELP_LINE       14                   /* ÑÁ¡eµA Îa¯¡–A“e º‰ ®    */
#define MAX_PAGE         4                   /* Îa¯¡Ði ® ·¶“e ÍA·¡»¡ ® */
#define MAX_HELP        HELP_LINE * MAX_PAGE /* Îa¯¡Ði ® ·¶“e º‰ ®     */

void show_help(void)
/* quiz.hlp ÑÁ·©·i ·ª´á¬á •¡¶‘ i·i ¥¡µa º…”a. */
{
   FILE *stream;
   char (*help)[68+2], *screen;
   int  cur_page, re_draw, line_no, key, i;

   /* •¡¶‘ iµA Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”wÐe”a. */
   if((help = malloc((68+2)*MAX_HELP)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      return;
   }

   /* •¡¶‘ i ÑÁ·© quiz.hlp· ¶w·i ·ª´á helpµA ¸á¸wÐe”a. */
   if((stream = fopen("ctools.hlp", "rt")) == NULL)
   {
      put_msg("•¡¶‘ i ÑÁ·©·i ·ª·i ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      return;
   }
   line_no = 0;
   while(!feof(stream))
      if(line_no >= MAX_HELP || fgets(help[line_no], 70, stream) == NULL)
         break;
      else
      {
         /* ¢…¸aµi· {µA ¦›·e '\n' ¢…¸a“e ¬b¹AÐe”a. */
         i = strlen(help[line_no]);
         if(help[line_no][i - 1] == '\n') help[line_no][i - 1] = 0;
         line_no++;
      }
   fclose(stream);

   /* ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_3dbox(4, 2, abs_maxx_han-4, 2+HELP_LINE+3, LIGHTGRAY);
   set_output_offset_han(12);
   vputs_han(35, 2, "•¡ ¶‘  i", RED);
   draw_horiz_line(4*8+5, 4*16-2,  (abs_maxx_han-9)*8+5, BLUE);
   draw_horiz_line(4*8+5, 18*16+2, (abs_maxx_han-9)*8+5, BLUE);
   set_output_offset_han(4);
   vputs_han(8, 18, "Up/PgUp : ´| ÍA·¡»¡   Down/PgDn : ”a·q ÍA·¡»¡"
                    "   Esc : ‹a e ¥¡‹¡", YELLOW);
   set_output_offset_han(0);

   /* helpµA ¸á¸w–E •¡¶‘ i·i ¥¡µa º…”a. */
   cur_page = 0;
   re_draw  = 1;
   while(1)
   {
      if(re_draw)
      /* ÍA·¡»¡Ÿi ¤aŽ´ö”a¡e •¡¶‘ i·i ¬¡ Îa¯¡Ðe”a. */
      {
         for(i = 0; i < HELP_LINE; i++)
            if(cur_page*HELP_LINE+i >= line_no)
               nputs_han(6, 4+i, "", 68);
            else
               nputs_han(6, 4+i, help[cur_page*HELP_LINE+i], 68);
      }

      /* ¬a¶w¸a· Ç¡ ·³b·i ÀáŸ¡Ðe”a. */
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

   /* helpµA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá¯¡Ç¥”a. */
   free(help);
}
