/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          __put_msg_han(), __get_mask_han(), __fn_split_han() */
/*   FILENAME           dirbox1.c                                           */
/*   DATE & TIME        05/13/92(WED) 20:57                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <errno.h>
#include <alloc.h>

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void __put_msg_han(char *string)
/* output message -- internal function */
{
   char *screen;
   int  box_x;

   box_x = 40 - (strlen(string) + 2) / 2;
   screen = save_text_han(box_x, 10, box_x + strlen(string) + 1, 12);
   draw_box_han(box_x, 10, box_x + strlen(string) + 1, 12, BOX_H1V1);
   xputs_han(box_x + 1, 11, string);
   if(!getch_han()) getch_han();
   restore_text_han(box_x, 10, screen);
}

int __get_mask_han(int box_x, int box_y, char *mask)
/* get file mask by user -- internal function */
/* returns --- ESC   : <Esc> key is pressed   */
/*             ENTER : <Enter> key is pressed */
{
   char *screen;
   int  ret_code, first_char, cur_mode, cur_change;

   screen = save_text_han(box_x, box_y, box_x + 57, box_y + 4);
   clear_text_han(box_x + 1, box_y + 1, box_x + 56, box_y + 3);
   draw_box_han(box_x,      box_y,     box_x + 57, box_y + 4, BOX_H1V1);
   draw_box_han(box_x +  1, box_y + 2, box_x + 56, box_y + 2, BOX_H1V1);
   put_eng_font(box_x,      box_y + 2, 195);
   put_eng_font(box_x + 57, box_y + 2, 180);

   cur_mode           = han_mode;
   cur_change         = able_to_change_han;
   han_mode           = 0;
   able_to_change_han = 0;

   xputs_han(box_x + 28 - 8, box_y + 1, "·ª·i ÑÁ·© ·¡Ÿq·e?");
   ret_code = adv_gets_han(box_x + 2, box_y + 3, mask, 54, FONT_NORMAL, "");

   able_to_change_han = cur_change;
   han_mode           = cur_mode;
   for(cur_mode = 0; cur_mode < strlen(mask); cur_mode++) mask[cur_mode] = toupper(mask[cur_mode]);

   restore_text_han(box_x, box_y, screen);
   return(ret_code);
}

int __fn_split_han(char *mask, char *drive, char *dir, char *wild_card)
/* splits path name <mask> into its components */
{
   char filename[9], ext[5], flag, cur_dir[81], test_dir[81], i;

   flag = fnsplit(mask, drive, dir, filename, ext);

   if(!(flag & DRIVE))
   {
      drive[0] = getdisk() + 'A';
      drive[1] = ':';
      drive[2] = 0;
   }
   else
      setdisk(drive[0] - 'A');

   if(!(flag & DIRECTORY))
   {
      getcwd(cur_dir, 79);
      strcpy(dir, cur_dir + 2);
      if(strcmp(dir, "\\")) strcat(dir, "\\");
   }
   else
   {
      strcpy(test_dir, drive);
      strcat(test_dir, dir);
      i = strlen(test_dir);
      if(test_dir[i - 1] == '\\' && i != 3) test_dir[i - 1] = 0;
      if(chdir(test_dir) != 0)
      {
	 __put_msg_han("—¡BÉ¡Ÿ¡ ¡w·i ¸i¡µ »¡¸÷Ðaµv¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
	 chdir(cur_dir);
	 setdisk(cur_dir[0] - 'A');
	 return -1;
      }
      else
      {
	 getcwd(cur_dir, 79);
	 strcpy(dir, cur_dir + 2);
	 if(strcmp(dir, "\\")) strcat(dir, "\\");
      }
   }

   if(flag & FILENAME)
   {
      strcpy(test_dir, drive);
      strcat(test_dir, dir);
      strcat(test_dir, filename);
      strcat(test_dir, ext);
      if(!chdir(test_dir))
      {
	 getcwd(cur_dir, 79);
	 strcpy(dir, cur_dir + 2);
	 if(strcmp(dir, "\\")) strcat(dir, "\\");
      }
      else if(filename[0])
      {
	 strcpy(wild_card, filename);
	 strcat(wild_card, ext);
      }
   }

   return 0;
}
