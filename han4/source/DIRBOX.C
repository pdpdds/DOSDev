/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.1                       */
/*   SUB-TITLE          directory box functions                             */
/*   FILENAME           dirbox.c                                            */
/*   DATE & TIME        05/13/92(WED) 20:57                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <errno.h>
#include <ctype.h>
#include <alloc.h>

#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

#define MAX_FILE   1026

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

typedef struct
{
   char name[14];
   unsigned char attrib;
} file;

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int dir_box_short(int box_x, int box_y, char *mask, char *file_name);
int dir_box_long (int box_x, int box_y, char *mask, char *file_name);

/****************************************************************************/
/*                    Prototypes of the Static Functions                    */
/****************************************************************************/

static int  get_mask(int box_x, int box_y, char *mask);
//static int  error_handler(int errval, int ax, int bp, int si);
static int  error_handler(void);
static int  fn_split(char *mask, char *drive, char *dir, char *wild_card);
static int  sort_func1(const void *a, const void *b);
static int  sort_func2(const void *a, const void *b);
static void show_one_file(int box_x, int box_y, int i, struct ffblk *temp);
static void put_msg(char *string);

/****************************************************************************/
/*                    Implementations of the C Functions                    */
/****************************************************************************/

int dir_box_short(int box_x, int box_y, char *mask, char *file_name)
/* ÑÁ·© ·¡Ÿq·i ¬åÈ‚¤h“e”a. <Esc> ˆa ’‰œá»¡¡e ESC Ÿi, <Enter> ˆa ’‰œá»¡¡e   */
/* ENTER Ÿi Ÿ¡ÈåÐe”a.						           */
{
   struct ffblk ffblk;		/* file-directory ¸÷¥¡ */
   struct dfree dtable;		/* disk free ¸÷¥¡ */
   int  number;			/* number = ¤i‰e–E ÑÁ·©(—¡BÉ¡Ÿ¡ Í¡Ðq)· ® */
   int  top_index;		/* ÑÁ·© ¤b¯a ¹A·© Àõ ÑÁ·©· ·¥•B¯a */
   int  bar_x, bar_y;		/* ¤e¸å  b”· Ñe¸ ¶áÃ¡ */
   int  prv_bar_x, prv_bar_y;   /* ¤e¸å  b”· ¤a¡ ¸å ¶áÃ¡ */
   file *files;
   char search_file[128], drive[3], dir[81], show_mask[128], org_dir[81];
   char wild_card[13], mask2[55];
   int  re_draw, move_bar, key;
   int  ret_code, i, j;
   char *screen;

   harderr(error_handler);	/* error handler Ÿi ¬é¸÷ */
   fnsplit(mask, search_file, search_file + 3, search_file + 69, search_file + 78);
   strcpy(wild_card, search_file + 69);
   strcat(wild_card, search_file + 78);
   strcpy(mask2, mask);
   if(get_mask(box_x, box_y, mask2) == ESC) return(ESC);
   if(!strchr(mask2, '?') && !strchr(mask2, '*') && findfirst(mask2, &ffblk, 0x00) != -1)
   {
      strcpy(file_name, ffblk.ff_name);
      return(ENTER);
   }

   if((files = malloc(sizeof(file)*MAX_FILE)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
      return ESC;
   }

   if((screen = save_text_han(box_x, box_y, box_x + 57, box_y + 15)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      free(files);
      return ESC;
   }

   draw_box_han(box_x,      box_y,      box_x + 57, box_y + 15, BOX_H1V1);
   draw_box_han(box_x +  1, box_y +  2, box_x + 56, box_y +  2, BOX_H1V1);
   put_eng_font(box_x,      box_y +  2, 195);
   put_eng_font(box_x + 57, box_y +  2, 180);
   draw_box_han(box_x +  1, box_y + 13, box_x + 56, box_y + 13, BOX_H1V1);
   put_eng_font(box_x,      box_y + 13, 195);
   put_eng_font(box_x + 57, box_y + 13, 180);

   getcwd(org_dir, 80);
   ret_code = -1;
   do
   {
      if(fn_split(mask2, drive, dir, wild_card) == -1)
      {
	 ret_code = ESC;
	 break;
      }

      strcpy(search_file, drive);
      strcat(search_file, dir);
      strcat(search_file, wild_card);

      /********** ÑÁ·©—i·i ˆñ¬‚Ðaµa files µA ¸á¸wÐe”a **********/

      number = 0;

      if(findfirst("*.*", &ffblk, FA_DIREC) != -1)
      {
	 if(strcmp(ffblk.ff_name, ".") && ffblk.ff_attrib == FA_DIREC)
	 {
	    strcpy(files[number].name, ffblk.ff_name);
	    strcat(files[number].name, "\\");
	    files[number++].attrib = ffblk.ff_attrib;
	 }

	 /* a á»¡ ÑÁ·©—i·i ‰­¢ ˆñ¬‚Ðe”a */
	 while(findnext(&ffblk) == 0 && strcmp(ffblk.ff_name, ".") && ffblk.ff_attrib == FA_DIREC)
	 {
	    strcpy(files[number].name, ffblk.ff_name);
	    strcat(files[number].name, "\\");
	    files[number++].attrib = ffblk.ff_attrib;
	    if(number >= MAX_FILE - 1)
	    {
	       put_msg("ÑÁ·©· ®ˆa á¢  g¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
	       break;
	    }
	 }
      }

      if(findfirst(wild_card, &ffblk, FA_ARCH) != -1)
      {
	 strcpy(files[number].name, ffblk.ff_name);
	 files[number++].attrib = ffblk.ff_attrib;

	 /* a á»¡ ÑÁ·©—i·i ‰­¢ ˆñ¬‚Ðe”a */
	 while(findnext(&ffblk) == 0)
	 {
	    strcpy(files[number].name, ffblk.ff_name);
	    files[number++].attrib = ffblk.ff_attrib;

	    if(number >= MAX_FILE)
	    {
	       put_msg("ÑÁ·©· ®ˆa á¢  g¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
	       break;
	    }
	 }
      }

      strcpy(show_mask, search_file);
      show_mask[56] = 0;
      clear_text_han(box_x + 1, box_y + 1, box_x + 56, box_y + 1);
      xputs_han(box_x + (56 - strlen(show_mask)) / 2 + 1, box_y + 1, show_mask);

      getdfree(drive[0] - 'A' + 1, &dtable);
      clear_text_han(box_x + 1, box_y + 14, box_x + 56, box_y + 14);
      xprintf_han(box_x + 6, box_y + 14, " Á· %3d ˆ        q·e µwµb %10ld ¤a·¡Ëa", number,
		  (long) dtable.df_avail * dtable.df_sclus * dtable.df_bsec);
      if(number == 0)
      {
	 xputs_han(box_x + 2, box_y + 3, "ÑÁ·©·¡ Ðaa•¡ ´ô¯s“¡”a.");
	 if(!getch()) getch();
	 ret_code = ESC;
	 break;
      }

      qsort((void *) files, number, sizeof(files[0]), sort_func1);

      /* ÑÁ·©—i·i ÂA” 4x10 ˆ e Îa¯¡Ðe”a */
      top_index = 0;
      re_draw = move_bar = 1;
      do
      {
	 if(re_draw)
	 {
	    clear_text_han(box_x + 1, box_y + 3, box_x + 56, box_y + 12);
	    for(i = 0; i < 40 && top_index + i < number; i++)
	       xputs_han((i % 4) * 14 + box_x + 2, (i / 4) + box_y + 3, files[top_index+i].name);
	    prv_bar_x = prv_bar_y = bar_x = bar_y = 0;
	    re_draw  = 0;
	    move_bar = 1;
	 }
	 if(move_bar)
	 {
	    modify_text_attr(prv_bar_x * 14 + box_x + 1,      prv_bar_y + box_y + 3,
			     prv_bar_x * 14 + box_x + 1 + 13, prv_bar_y + box_y + 3,
			     FONT_NORMAL);
	    modify_text_attr(bar_x * 14 + box_x + 1,      bar_y + box_y + 3,
			     bar_x * 14 + box_x + 1 + 13, bar_y + box_y + 3,
			     FONT_INVERSE);
	    prv_bar_x = bar_x;
	    prv_bar_y = bar_y;
	    move_bar = 0;
	 }

	 key = getch();
	 if(isalpha(key))		/* ´iÌa¥U·¡¡e */
	 {
	    for(i = 0; i < number; i++) if(files[i].name[0] == toupper(key)) break;
	    if(i < number)		/*  áŸ¡‹i¸aˆa key µÁ ˆ{·e ÑÁ·©·¡ ·¶·a¡e ·¡•· */
	    {
	       top_index = i;
	       move_bar  = re_draw = 1;
	    }
	 }
	 else if(key == _ENTER)
	 {
	    strcpy(file_name, drive);
	    strcat(file_name, dir);
	    strcat(file_name, files[top_index + bar_y * 4 + bar_x].name);
	    if(files[top_index + bar_y * 4 + bar_x].attrib == FA_DIREC)
	       strcpy(mask2, file_name);
	    else
	       ret_code = ENTER;
	    break;
	 }
	 else if(key == _ESC)
	 {
	    ret_code = ESC;
	    break;
	 }
	 else if(key == 0)		/* ÑÂ¸wÇ¡·¡¡e */
	 {
	    switch(key = getch())
	    {
	       case UP    : if(top_index + bar_y * 4 + bar_x >= 4)
			    {
			       bar_y--;
			       move_bar = 1;
			    }
			    break;
	       case DOWN  : if(top_index + bar_y * 4 + bar_x < number - 4)
			    {
			       bar_y++;
			       move_bar = 1;
			    }
			    break;
	       case LEFT  : if(top_index + bar_y * 4 + bar_x > 0)
			    {
			       bar_x--;
			       move_bar = 1;
			    }
			    break;
	       case RIGHT : if(top_index + bar_y * 4 + bar_x < number - 1)
			    {
			       bar_x++;
			       move_bar = 1;
			    }
			    break;
	       case PGUP  : if(top_index + bar_y * 4 + bar_x >= 10 * 4)
			    /* Ðe ÍA·¡»¡ ¦…œ·· ÑÁ·©·¡ ´|µA ·¶·i ˜ */
			    {
			       top_index -= 10 * 4 - bar_y * 4 - bar_x;
			       if(top_index < 0) top_index = 0;
			       re_draw = 1;
			    }
			    else if(top_index + bar_y * 4 + bar_x != 0)
			    /* Ðe ÍA·¡»¡ ¦…œ··¡ –A»¡ ´g·i ˜ */
			    {
			       top_index = 0;
			       re_draw = 1;
			    }
			    break;
	       case PGDN  : if(top_index + bar_y * 4 + bar_x < number - 10 * 4)
			    /* Ðe ÍA·¡»¡ ¦…œ·· ÑÁ·©·¡ –áµA ·¶·i ˜ */
			    {
			       top_index += 10 * 4 + bar_y * 4 + bar_x;
			       if(top_index > number - 1) top_index = number -1;
			       re_draw = 1;
			    }
			    else if(top_index + bar_y * 4 + bar_x != number - 1)
			    /* Ðe ÍA·¡»¡ ¦…œ··¡ –A»¡ ´g·i ˜ */
			    {
			       top_index = number - 1;
			       re_draw = 1;
			    }
			    break;
	       case HOME  : if(top_index + bar_y * 4 + bar_x != 0)
			    {
			       if(top_index == 0)
			       {
				  bar_x = bar_y = 0;
				  move_bar = 1;
			       }
			       else
			       {
				  top_index = 0;
				  re_draw = 1;
			       }
			    }
			    break;
	       case END   : if(top_index + bar_y * 4 + bar_x != number - 1)
			    {
			       top_index = number - 1;
			       re_draw = 1;
			    }
			    break;
	    }
	    if(move_bar)
	    {
	       if(bar_x < 0)
	       {
		  bar_x = 3;
		  bar_y--;
	       }
	       if(bar_x > 3)
	       {
		  bar_x = 0;
		  bar_y++;
	       }
	       if(bar_y < 0)
	       {
		  if(top_index > 3)
		  {
		     modify_text_attr(prv_bar_x * 14 + box_x + 1,      prv_bar_y + box_y + 3,
				      prv_bar_x * 14 + box_x + 1 + 13, prv_bar_y + box_y + 3,
				      FONT_NORMAL);
		     top_index -= 4;
		     xscrolldn_han(box_x + 2, box_y + 3, box_x + 55, box_y + 12);
		     for(i = 0; i < 4; i++)
			xputs_han(i * 14 + box_x + 2, box_y + 3, files[top_index+i].name);
		  }
		  bar_y = 0;
	       }
	       if(bar_y > 9)
	       {
		  if(number > top_index + 4 * 10)
		  {
		     modify_text_attr(prv_bar_x * 14 + box_x + 1,      prv_bar_y + box_y + 3,
				      prv_bar_x * 14 + box_x + 1 + 13, prv_bar_y + box_y + 3,
				      FONT_NORMAL);
		     top_index += 4;
		     xscrollup_han(box_x + 2, box_y + 3, box_x + 55, box_y + 12);
		     for(i = 0; i < 4 && top_index + 9 * 4 + i < number; i++)
			xputs_han(i * 14 + box_x + 2, box_y + 12, files[top_index + 9 * 4 + i].name);
		  }
		  bar_y = 9;
	       }
	    }
	 }
      }
      while(1);
   }
   while(ret_code == -1);

   restore_text_han(box_x, box_y, screen);
   free(files);
   chdir(org_dir);
   setdisk(org_dir[0] - 'A');
   return(ret_code);
}

int dir_box_long(int box_x, int box_y, char *mask, char *file_name)
/* ÑÁ·© ·¡Ÿq·i ¬åÈ‚¤h“e”a. <Esc> ˆa ’‰œá»¡¡e ESC Ÿi, <Enter> ˆa ’‰œá»¡¡e   */
/* ENTER Ÿi Ÿ¡ÈåÐe”a.						           */
{
   struct ffblk ffblk;		/* file-directory ¸÷¥¡ */
   struct dfree dtable;		/* disk free ¸÷¥¡ */
   int  number;			/* number = ¤i‰e–E ÑÁ·©(—¡BÉ¡Ÿ¡ Í¡Ðq)· ® */
   int  top_index;		/* ÑÁ·© ¤b¯a ¹A·© Àõ ÑÁ·©· ·¥•B¯a */
   int  bar_y;		        /* ¤e¸å  b”· Ñe¸ ¶áÃ¡ */
   int  prv_bar_y;              /* ¤e¸å  b”· ¤a¡ ¸å ¶áÃ¡ */
   struct ffblk *files, temp;
   char search_file[128], drive[3], dir[81], show_mask[128], org_dir[81];
   char wild_card[13], mask2[55];
   int  re_draw, move_bar, key;
   int  ret_code, i, j;
   char *screen;

   harderr(error_handler);	/* error handler Ÿi ¬é¸÷ */
   fnsplit(mask, search_file, search_file + 3, search_file + 69, search_file + 78);
   strcpy(wild_card, search_file + 69);
   strcat(wild_card, search_file + 78);
   strcpy(mask2, mask);
   if(get_mask(box_x, box_y, mask2) == ESC) return(ESC);
   if(!strchr(mask2, '?') && !strchr(mask2, '*') && findfirst(mask2, &ffblk, 0x00) != -1)
   {
      strcpy(file_name, ffblk.ff_name);
      return(ENTER);
   }

   if((files = malloc(sizeof(struct ffblk)*MAX_FILE)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
      return ESC;
   }

   if((screen = save_text_han(box_x, box_y, box_x + 40, box_y + 15)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      free(files);
      return ESC;
   }

   draw_box_han(box_x,      box_y,      box_x + 40, box_y + 15, BOX_H1V1);
   draw_box_han(box_x +  1, box_y +  2, box_x + 39, box_y +  2, BOX_H1V1);
   put_eng_font(box_x,      box_y +  2, 195);
   put_eng_font(box_x + 40, box_y +  2, 180);
   draw_box_han(box_x +  1, box_y + 13, box_x + 39, box_y + 13, BOX_H1V1);
   put_eng_font(box_x,      box_y + 13, 195);
   put_eng_font(box_x + 40, box_y + 13, 180);

   getcwd(org_dir, 80);
   ret_code = -1;
   do
   {
      if(fn_split(mask2, drive, dir, wild_card) == -1)
      {
	 ret_code = ESC;
	 break;
      }

      strcpy(search_file, drive);
      strcat(search_file, dir);
      strcat(search_file, wild_card);

      /********** ÑÁ·©—i·i ˆñ¬‚Ðaµa files µA ¸á¸wÐe”a **********/

      number = 0;

      if(findfirst("*.*", &ffblk, FA_DIREC) != -1)
      {
	 if(strcmp(ffblk.ff_name, ".") && ffblk.ff_attrib == FA_DIREC) files[number++] = ffblk;

	 /* a á»¡ ÑÁ·©—i·i ‰­¢ ˆñ¬‚Ðe”a */
	 while(findnext(&ffblk) == 0 && strcmp(ffblk.ff_name, ".") && ffblk.ff_attrib == FA_DIREC)
	 {
	    files[number++] = ffblk;
	    if(number >= MAX_FILE - 1)
	    {
	       put_msg("ÑÁ·©· ®ˆa á¢  g¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
	       break;
	    }
	 }
      }

      if(findfirst(wild_card, &ffblk, FA_ARCH) != -1)
      {
	 files[number++] = ffblk;

	 /* a á»¡ ÑÁ·©—i·i ‰­¢ ˆñ¬‚Ðe”a */
	 while(findnext(&ffblk) == 0)
	 {
	    files[number++] = ffblk;
	    if(number >= MAX_FILE)
	    {
	       put_msg("ÑÁ·©· ®ˆa á¢  g¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
	       break;
	    }
	 }
      }

      strcpy(show_mask, search_file);
      show_mask[56] = 0;
      clear_text_han(box_x + 1, box_y + 1, box_x + 39, box_y + 1);
      xputs_han(box_x + (38 - strlen(show_mask)) / 2 + 1, box_y + 1, show_mask);

      getdfree(drive[0] - 'A' + 1, &dtable);
      clear_text_han(box_x + 1, box_y + 14, box_x + 39, box_y + 14);
      xprintf_han(box_x + 2, box_y + 14, "Á· %3d ˆ  q·e µwµb %10ld ¤a·¡Ëa", number,
		  (long) dtable.df_avail * dtable.df_sclus * dtable.df_bsec);
      if(number == 0)
      {
	 xputs_han(box_x + 2, box_y + 3, "ÑÁ·©·¡ Ðaa•¡ ´ô¯s“¡”a.");
	 if(!getch()) getch();
	 ret_code = ESC;
	 break;
      }

      qsort((void *) files, number, sizeof(files[0]), sort_func2);

      top_index = 0;
      re_draw = move_bar = 1;
      do
      {
	 if(re_draw)
	 {
	    clear_text_han(box_x + 1, box_y + 3, box_x + 39, box_y + 12);
	    for(i = 0; i < 10 && top_index + i < number; i++)
	       show_one_file(box_x, box_y, i, &files[top_index + i]);
	    prv_bar_y = bar_y = 0;
	    re_draw  = 0;
	    move_bar = 1;
	 }
	 if(move_bar)
	 {
	    modify_text_attr(box_x + 1,  prv_bar_y + box_y + 3,
			     box_x + 39, prv_bar_y + box_y + 3,
			     FONT_NORMAL);
	    modify_text_attr(box_x + 1,  bar_y + box_y + 3,
			     box_x + 39, bar_y + box_y + 3,
			     FONT_INVERSE);
	    prv_bar_y = bar_y;
	    move_bar = 0;
	 }

	 key = getch();
	 if(isalpha(key))		/* ´iÌa¥U·¡¡e */
	 {
	    for(i = 0; i < number; i++) if(files[i].ff_name[0] == toupper(key)) break;
	    if(i < number)		/*  áŸ¡‹i¸aˆa key µÁ ˆ{·e ÑÁ·©·¡ ·¶·a¡e ·¡•· */
	    {
	       top_index = i;
	       move_bar  = re_draw = 1;
	    }
	 }
	 else if(key == _ENTER)
	 {
	    strcpy(file_name, drive);
	    strcat(file_name, dir);
	    strcat(file_name, files[top_index + bar_y].ff_name);
	    if(files[top_index + bar_y].ff_attrib == FA_DIREC)
	       strcpy(mask2, file_name);
	    else
	       ret_code = ENTER;
	    break;
	 }
	 else if(key == _ESC)
	 {
	    ret_code = ESC;
	    break;
	 }
	 else if(key == 0)		/* ÑÂ¸wÇ¡·¡¡e */
	 {
	    switch(key = getch())
	    {
	       case UP    : if(top_index + bar_y > 0)
			    {
			       bar_y--;
			       move_bar = 1;
			    }
			    break;
	       case DOWN  : if(top_index + bar_y < number - 1)
			    {
			       bar_y++;
			       move_bar = 1;
			    }
			    break;
	       case PGUP  : if(top_index + bar_y >= 10)
			    /* Ðe ÍA·¡»¡ ¦…œ·· ÑÁ·©·¡ ´|µA ·¶·i ˜ */
			    {
			       top_index -= 10 - bar_y;
			       if(top_index < 0) top_index = 0;
			       re_draw = 1;
			    }
			    else if(top_index + bar_y != 0)
			    /* Ðe ÍA·¡»¡ ¦…œ··¡ –A»¡ ´g·i ˜ */
			    {
			       top_index = 0;
			       re_draw = 1;
			    }
			    break;
	       case PGDN  : if(top_index + bar_y < number - 10)
			    /* Ðe ÍA·¡»¡ ¦…œ·· ÑÁ·©·¡ –áµA ·¶·i ˜ */
			    {
			       top_index += 10 + bar_y;
			       if(top_index > number - 1) top_index = number - 1;
			       re_draw = 1;
			    }
			    else if(top_index + bar_y != number - 1)
			    /* Ðe ÍA·¡»¡ ¦…œ··¡ –A»¡ ´g·i ˜ */
			    {
			       top_index = number - 1;
			       re_draw = 1;
			    }
			    break;
	       case HOME  : if(top_index + bar_y != 0)
			    {
			       if(top_index == 0)
			       {
				  bar_y = 0;
				  move_bar = 1;
			       }
			       else
			       {
				  top_index = 0;
				  re_draw = 1;
			       }
			    }
			    break;
	       case END   : if(top_index + bar_y != number - 1)
			    {
			       top_index = number - 1;
			       re_draw = 1;
			    }
			    break;
	    }
	    if(move_bar)
	    {
	       if(bar_y < 0)
	       {
		  modify_text_attr(box_x + 1,  prv_bar_y + box_y + 3,
				   box_x + 39, prv_bar_y + box_y + 3,
				   FONT_NORMAL);
		  top_index--;
		  xscrolldn_han(box_x + 2, box_y + 3, box_x + 38, box_y + 12);
		  show_one_file(box_x, box_y, 0, &files[top_index]);
		  bar_y = 0;
	       }
	       if(bar_y > 9)
	       {
		  modify_text_attr(box_x + 1,  prv_bar_y + box_y + 3,
				   box_x + 39, prv_bar_y + box_y + 3,
				   FONT_NORMAL);
		  top_index++;
		  xscrollup_han(box_x + 2, box_y + 3, box_x + 38, box_y + 12);
		  show_one_file(box_x, box_y, 9, &files[top_index + 9]);
		  bar_y = 9;
	       }
	    }
	 }
      }
      while(1);
   }
   while(ret_code == -1);

   restore_text_han(box_x, box_y, screen);
   free(files);
   chdir(org_dir);
   setdisk(org_dir[0] - 'A');
   return(ret_code);
}

static int get_mask(int box_x, int box_y, char *mask)
/*  ¯aÇaŸi ·³b¤h“e”a. <Esc> ˆa ’‰œá»¡¡e ESC Ÿi, <Enter> ˆa ’‰œá»¡¡e */
/* ENTER Ÿi Ÿ¡ÈåÐe”a.						      */
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

//static int error_handler(int errval, int ax, int bp, int si)
static int error_handler(void)
/* Ða—a¶Á´á¸â·¥ µAœáˆa ¤i¬—Ðaµv·i ˜ ¯©Ð—–A“e Ðq® */
{
   put_msg("µAœáˆa ¤i¬—Ðaµv¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   hardresume(0);	/* µAœáŸi ignore Ðe”a */
   return(0);
}

static int fn_split(char *mask, char *drive, char *dir, char *wild_card)
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
	 put_msg("—¡BÉ¡Ÿ¡ ¡w·i ¸i¡µ »¡¸÷Ðaµv¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
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

static int sort_func1(const void *a, const void *b)
{
   if( ((file *) a)->attrib == FA_DIREC && ((file *) b)->attrib != FA_DIREC )
      return(-1);
   else if( ((file *) a)->attrib != FA_DIREC && ((file *) b)->attrib == FA_DIREC )
      return(1);
   else
      return(strcmp( ((file *) a)->name, ((file *) b)->name) );
}

static int sort_func2(const void *a, const void *b)
{
   if( ((struct ffblk *) a)->ff_attrib == FA_DIREC && ((struct ffblk *) b)->ff_attrib != FA_DIREC )
      return(-1);
   else if( ((struct ffblk *) a)->ff_attrib != FA_DIREC && ((struct ffblk *) b)->ff_attrib == FA_DIREC )
      return(1);
   else
      return(strcmp( ((struct ffblk *) a)->ff_name, ((struct ffblk *) b)->ff_name) );
}

static void show_one_file(int box_x, int box_y, int i, struct ffblk *temp)
{
   xputs_han(  box_x + 2,  i + box_y + 3, temp->ff_name);
   if(temp->ff_attrib == FA_DIREC)
      put_eng_font(box_x + 2 + strlen(temp->ff_name), i + box_y + 3, '\\');
   else
      xprintf_han(box_x + 16, i + box_y + 3, "%8ld", temp->ff_fsize);
   xprintf_han(box_x + 25, i + box_y + 3, "%02d/%02d/%02d",
	       (temp->ff_fdate & 0x01ff) >> 5,
	       (temp->ff_fdate & 0x001f),
	       (((temp->ff_fdate & 0xffff) >> 9) + 80) % 100);
   xprintf_han(box_x + 34, i + box_y + 3, "%02d:%02d",
	       (temp->ff_ftime & 0xffff) >> 11,
	       (temp->ff_ftime & 0x07ff) >> 5);
}

static void put_msg(char *string)
/* ˆb¹· ¡A¯¡»¡· Â‰bµA ¬a¶w–E”a */
{
   char *screen;
   int  box_x;

   box_x = 40 - (strlen(string) + 2) / 2;
   screen = save_text_han(box_x, 10, box_x + strlen(string) + 1, 12);
   draw_box_han(box_x, 10, box_x + strlen(string) + 1, 12, BOX_H1V1);
   xputs_han(box_x + 1, 11, string);
   if(!getch()) getch();
   restore_text_han(box_x, 10, screen);
}


