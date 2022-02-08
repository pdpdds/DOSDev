/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          adv_gets_han()                                      */
/*   FILENAME           advgets.c                                           */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <string.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

int adv_gets_han_index = 0;                        /* first cursor position */

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int adv_gets_han(int x, int y, char *string, int limit, int attr, char *user_exit_key)
/* WordStar-type string input function */
{
   static unsigned char exit_key[60] = { LEFT, RIGHT, CTRLLEFT, CTRLRIGHT,
					 HOME, END,   INS,      DEL };
   char temp[100], han_str[11], second_ascii;
   int  index, length, edit_cursor, is_first, return_code;
   int  cur_x, cur_y, attr2;

   /* backup current state and set state */
   attr2       = attr_han;
   cur_x       = curx_han;
   cur_y       = cury_han;
   cury_han    = y;
   attr_han    = attr;

   no_more_han = 0;
   return_code = NOT_COMPLETE;

   if(limit > abs_maxx_han + 1 || strlen(string) > limit ||
      strlen(user_exit_key) > 50) return ESC;
   strcpy(temp, string);    	 /* copy <string> to <temp>                  */
   exit_key[8] = 0;		 /* very Important!!!!!!!!!!!!!!!!!!!!!!!!!! */
   strcat(exit_key, user_exit_key);	           /* append <user_exit_key> */

   /* check if the index is already assigned */
   index = adv_gets_han_index * (-1) - 1;
   if(index >= 0 && index <= strlen(temp) && index <= limit)
      is_first = 0;
   else
   {
      is_first = 1;
      index    = strlen(temp);
   }

   do
   {
      /* check <temp> */
      length = strlen(temp);
      if(index > limit || length > limit) exit(1);

      /* display <temp>, correct <index> and choose cursor */
      nputs_han(x, y, temp, limit);
      if(is_first)
      {
	 attr_han = attr_han ^ FONT_INVERSE;
	 nputs_han(x, y, temp, strlen(temp));
	 attr_han = attr;
      }

      if(scr_buff[y][x+index][1] == HAN2) index--;
      curx_han = x + index;
      edit_cursor = (scr_buff[y][x+index][1] == HAN1) ? 1 : 2;

      /* input one character */
      do
      {
	 return_code = bul3_mode ? getch3_han(han_str, exit_key, edit_cursor)
				 : getch2_han(han_str, exit_key, edit_cursor);
      }
      while(return_code == IGNORE);

      if(is_first) attr_han = attr;

      if(return_code >= 0) return_code = exit_key[return_code];
      if(return_code == DEL)
      /* <Del> key is pressed? */
      {
	 if(scr_buff[cury_han][curx_han][1] == HAN1)
	    memmove(temp+index, temp+index+2, (length+1)-(index+2));
	 else
	    memmove(temp+index, temp+index+1, (length+1)-(index+1));
      }
      else if(return_code != BS_PRESSED && prv_han.code != NO_HAN_STR &&
	      strlen(han_str) == 0)
      /* Hangul is combinating? */
      {
	 if(is_first && limit >= 2)
	 /* if first stroke, clear already inputted string and copy it */
	 {
	    temp[0]  = prv_han.bytes[0];
	    temp[1]  = prv_han.bytes[1];
	    temp[2]  = 0;
	    index    = 0;
	    is_first = 0;
	    continue;
	 }
	 if(index > limit - 2)
	 /* if there is no space for Hangul, clear stack and ignore it */
	 {
	    han_stack.top = 0;
	    prv_han.code  = NO_HAN_STR;
	    cur_han.code  = NO_HAN_CODE;
	    continue;
	 }
	 if(han_stack.top == 1 && ins_mode)
	 /* if first Hangul and insert mode, insert space for it */
	 {
	    memmove(temp+index+2, temp+index, (length+1)-index);
	    if(scr_buff[y][x+limit-2][1] == HAN2) temp[limit-1] = 0;
	 }

	 if(strlen(temp) > limit) temp[limit] = 0;
	 if(strlen(temp) < index+2 && !ins_mode && index+2 <= limit) temp[index+2] = 0;
	 temp[index]   = prv_han.bytes[0];
	 temp[index+1] = prv_han.bytes[1];
	 if(scr_buff[cury_han][curx_han+2][1] == HAN2 &&
	    !ins_mode && index+2 < limit)
	    temp[index+2] = ' ';
      }
      else if(strlen(han_str) == 1)
      /* an ASCII character is inputted? */
      {
	 if(is_first && limit >= 1)
	 /* if first stroke, clear already inputted string and copy it */
	 {
	    temp[0]  = han_str[0];
	    temp[1]  = 0;
	    index    = 1;
	    is_first = 0;
	    continue;
	 }
	 if(index == limit) continue;
	 if(ins_mode)
	 /* if insert mode, insert space for it */
	 {
	    memmove(temp+index+1, temp+index, (length+1)-index);
	    if(scr_buff[y][x+limit-1][1] == HAN2) temp[limit-1] = 0;
	    if(strlen(temp) > limit) temp[limit] = 0;
	    temp[index++] = han_str[0];
         }
         else
	 {
            if(strlen(temp) < index+1 && index+1 <= limit) temp[index+1] = 0;
            temp[index++] = han_str[0];
            if(scr_buff[cury_han][curx_han+1][1] == HAN2) temp[index] = ' ';
	 }
      }
      else if(strlen(han_str) == 2)
      /* a Hangul is completed? */
      {
	 temp[index]   = han_str[0];
	 temp[index+1] = han_str[1];
	 if(prv_han.code != NO_HAN_STR)
	 /* if Hangul is combinating yet */
	 {
	    if(index <= limit-4)
	    {
	       if(ins_mode)
	       {
		  memmove(temp+index+4, temp+index+2, (length+1)-(index+2));
		  if(scr_buff[y][x+limit-2][1] == HAN2) temp[limit-1] = 0;
		  if(strlen(temp) > limit) temp[limit] = 0;
	       }
	       index++; index++;
               if(!ins_mode && scr_buff[cury_han][curx_han+3][1] == HAN1)
		  temp[index+2] = ' ';
            }
            if(strlen(temp) < index+2 && !ins_mode && index+2 <= limit) temp[index+2] = 0;
            temp[index]   = prv_han.bytes[0];
	    temp[index+1] = prv_han.bytes[1];
	    if(strlen(temp) > limit) temp[limit] = 0;
         }
	 else
	 /* there is no Hangul combinating */
         {
	    if(return_code != LEFT && return_code != RIGHT && !strchr(user_exit_key, return_code)) index +=  2;
	 }
      }
      else if(strlen(han_str) == 3)
      /* a Hangul is completed and an ASCII character is inputted? */
      {
	 if(is_first) exit(1);

	 temp[index]   = han_str[0];
	 temp[index+1] = han_str[1];
	 if(index > limit - 3)
	 /* if there is no space for 3 characters, clear stack and ignore it */
	 {
	    han_stack.top = 0;
	    prv_han.code  = NO_HAN_STR;
	    cur_han.code  = NO_HAN_CODE;
	    continue;
	 }
	 index += 2;
	 if(ins_mode)
	 {
	    memmove(temp+index+1, temp+index, (length+1)-index);
	    if(scr_buff[y][x+limit-1][1] == HAN2) temp[limit-1] = 0;
	    if(strlen(temp) > limit) temp[limit] = 0;
	 }

	 if(!ins_mode && strlen(temp) < index+1 && index+1 <= limit)
	    temp[index+1] = 0;
	 temp[index] = han_str[2];
	 index++;
      }
      else if(strlen(han_str) > 3)
	 exit(1);

      if(is_first) is_first = 0;
      switch(return_code)
      {
	 case TAB_PRESSED   :
	 case HAN_CHANGED   :
	 case NOT_COMPLETE  :
	 case COMPLETE      :
	    break;
	 case LEFT  :
	    if(index > 0)
	    {
	       index--;
	       if(scr_buff[cury_han][curx_han-1][1] == HAN2) index--;
	    }
            break;
         case RIGHT :
	    if(index < strlen(temp))
            {
               index++;
               if(scr_buff[cury_han][curx_han][1] == HAN1) index++;
            }
            break;
         case CTRLLEFT :
            while((index > 0) && (temp[index-1] == ' ')) index--;
	    while((index > 0) && (temp[index-1] != ' ')) index--;
            if(scr_buff[y][x+index-1][1] == HAN2) index--;
            break;
	 case CTRLRIGHT :
	    while((index < strlen(temp)) && (temp[index] != ' ')) index++;
	    while((index < strlen(temp)) && (temp[index] == ' ')) index++;
            break;
         case HOME  :
	    index = 0;
	    break;
         case END   :
	    righttrim(temp);
	    index = strlen(temp);
	    break;
         case BS_PRESSED    :
            if (prv_han.code != NO_HAN_STR)
            {
               pop_han(&(prv_han.code));
               if(han_stack.top == 0)
	       {
                  cur_han.code = NO_HAN_CODE;
		  memmove(temp+index, temp+index+2, (strlen(temp)+1)-index);
	       }
	       else
	       {
		  cur_han.bytes[0] = prv_han.bytes[1];
		  cur_han.bytes[1] = prv_han.bytes[0];
		  temp[index]      = prv_han.bytes[0];
		  temp[index+1]    = prv_han.bytes[1];
	       }
	    }
	    else
	    {
	       if(scr_buff[cury_han][curx_han-1][1] == HAN2)
	       {
		  memmove(temp+index-2, temp+index, (strlen(temp)+1)-index);
		  index -= 2;
	       }
	       else if(index > 0)
	       {
		  memmove(temp+index-1, temp+index, (strlen(temp)+1)-index);
		  index--;
	       }
	    }
	    break;
	 case CTRLQ_PRESSED :
	    if(!(second_ascii = getch())) getch();
	    /* <Ctrl>+<Q>+<Y> */
	    if(second_ascii == 'Y' || second_ascii == 'y') temp[index] = 0;
	    break;
	 case CTRLT_PRESSED :
	    second_ascii = index;	/* save current index */
	    while((index < strlen(temp)) && (temp[index] != ' ')) index++;
	    while((index < strlen(temp)) && (temp[index] == ' ')) index++;
	    memmove(temp+second_ascii, temp+index, (length+1)-second_ascii);
	    index = second_ascii;	/* restore current index */
            break;
	 case CTRLY_PRESSED :
	    temp[0] = index = 0;
	    break;
	 case INS           :
	    ins_mode = !ins_mode;
            if(ins_change_func != (void (*) (void)) 0) ins_change_func();
	 case DEL           :
	    break;
	 case ESC_PRESSED   :
	 case ENTER_PRESSED :
	 default            :
	    if(return_code == ESC_PRESSED)
	       return_code = ESC;
	    else if(return_code == ENTER_PRESSED)
	       return_code = ENTER;
	    else if(strchr(user_exit_key, return_code) == NULL)
	       exit(1);

	    /* restore state, display accepted string and return */
	    curx_han = cur_x;
	    cury_han = cur_y;
	    attr_han = attr2;
	    righttrim(temp);
	    naputs_han(x, y, temp, limit, attr);
	    strcpy(string, temp);
	    cursor_han = eng_cursor;
	    adv_gets_han_index = index;
	    return(return_code);
      }
   }
   while(1);
}
