/****************************************************************************/
/*   TITLE              Æá»a ‰A·± <™©™©·¡> 1.0                              */
/*   SUB-TITLE          ¥¥ Ïa¡‹aœ‘                                         */
/*   FILENAME           quiz.c                                              */
/*   DATE & TIME        92/6/6 (É¡) 11:20                                   */
/*   PROGRAMMER         ·¡ÑeÑ¡ (ID:easyride)                                */
/****************************************************************************/

#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <dos.h>
#include <stdlib.h>
#include <alloc.h>
#include <mem.h>
#include <time.h>
#include <dir.h>
#include "hanio.h"
#include "extendio.h"
#include "misc.h"
#include "keydef.h"
#include "mylib.h"              /* mylib· ÑA”á ÑÁ·© */

/****************************************************************************/
/*                                Ça¡ ¬w®                                */
/****************************************************************************/

/* Ðe ˆ· Æá»a ¸ažaµA ”Ðe ¬w® */
#define QUEST_LINE             3    /* Ðe ¢…¹A· º‰ ®(3º‰)                 */
#define QUEST_LENGTH          40    /* ¢…¹A Ðe º‰· ÂA” ‹©·¡(40¸a )        */
#define ITEM_PER_QUEST         3    /* Ðe ¢…¹A”w Ðw¡¢ ®(3Ðw¡¢)             */
#define ITEM_LENGTH           40    /* ˆb Ðw¡¢· ÂA” ‹©·¡(40¸a)            */

/* ‰A·±µA ”Ðe ¬w® */
#define LEVEL_NUM              5    /* e·¡•¡· ®(5ˆa»¡)                   */
#define QUIZ_PER_LEVEL        10    /* Ðe e·¡•¡”w Â‰¹A–A“e ¢…¹A ®(10ˆ)   */

/* ‹¡Èa */
#define FILE_HEADER          "Toltori V1.0\x1a"     /* Æá»a ÑÁ·©· ÑA”á     */
#define MAX_APPEND           100    /* Ðe ¤åµA ÂˆaÐi ® ·¶“e Æá»a· ®     */

/* ¸÷–E ¬‚·a¡ ¤aŽ“e  Ça¡ Ðq® */
#define default_color()      set_vbcolor_han(BLACK, LIGHTGRAY)

/****************************************************************************/
/*                                 ¸ažaÑw                                   */
/****************************************************************************/

/* Ðe ˆ· Æá»a ¸aža“e Š¹¡ÁA quiz_structÑw ¥e®µA ¸á¸w–E”a */
typedef struct
{
   char quest[QUEST_LINE][QUEST_LENGTH+1];    /* ¢…¹A      */
   char item[ITEM_PER_QUEST][ITEM_LENGTH+1];  /* Ðw¡¢      */
   char answer;                               /* ”s        */
   char level;                                /* e·¡•¡    */
   int  make_num;                             /* Â‰¹A ÒU® */
   int  wrong_num;                            /* ËiŸ¥ ÒU® */
} quiz_struct;

/****************************************************************************/
/*                                ¸åµb ¥e®                                 */
/****************************************************************************/

pulldown_menu_struct menu_data =              /* º¡A“A(Î‰”a¶… ¡A“A) •A·¡Èá */
{
   5, 0, { 1, 5, 16, 27, 38 },
   { " \xd4\xff ", " ‰A·± ¯¡¸b ", " ·AÑw ¬åÈ‚ ", " Æá»a ‰ÅŸ¡ ", " ‹¡Èa " },
   { {  0, 0, },
     {  0, 0, },
     { 11, 0, 16, 1,
     { " Š‚  ¬a ", " ­A‰¬a ", " »¡  Ÿ¡ ", " ‰Á  Ðb ", " ÄñÏAÈá ",
       " ¸÷  Ã¡ ", " ‰w  ¹A ", " µ  ®‰ ", " Ðe  ¢… ", " µw  ´á ",
       " ¬w  ¯¢ " }
     },
     {  3, 0, 26, 1,
        { " Â     ˆa ", " ®¸÷/¬b¹A ", " ÑÁ·© ·ª‹¡ " }
     },
     {  5, 0, 37, 1,
        { " ­¡Ÿ¡   a‹¡ ", " Ðw¡¢ ´e¬ã‹¡ ", " ­A¤é¯¢·a¡  ",
          " •¡¯a¡      ", " {          " }
     },
   }
};

quiz_struct *quiz;       /* Æá»a—i·¡ ¸á¸w–E ¡A¡¡Ÿ¡Ÿi ˆaŸ¡Ç¡“e Í¡·¥Èá        */
int quiz_num;            /* ¡A¡¡Ÿ¡µA ¸á¸w–E Æá»a· ®                       */
int cur_type;            /* ¬a¶w¸aµA ·Ð ¬åÈ‚–E ·AÑw                       */
int cur_level;           /* Ñe¸· e·¡•¡                                   */
int sound_on   = 1;      /* 1 ·¡¡e ­¡Ÿ¡Ÿi ‰¡, 0 ·¡¡e »¡ ´g“e”a          */
int limit_time = 10;     /* ¢…¹AŸi Î“e •A º´á»¡“e ¯¡ˆe(Ðe ¢…¹A”w 10Á¡)    */
int mix_item   = 1;      /* 1 ·¡¡e ‰A·±¯¡µA Ðw¡¢·i ¬ã‰¡, 0·¡¡e ¬ã»¡ ´g“e”a. */

/****************************************************************************/
/*                                Ðq® ¶¥Ñw                                 */
/****************************************************************************/

/* ÑÁ¡e Š¬÷‰Á ­¡Ÿ¡µA ‰Åe–E Ðq®—i */
void disp_type_level(void);
void disp_status(void);
void disp_logo(void);
void play2(char *string);

/* Æá»a ·ª‹¡µÁ ¸á¸wÐa‹¡µA ‰Åe–E Ðq®—i */
int  read_quiz(int quiz_type);
int  save_quiz(int quiz_type, int mode);
void clear_quiz(int index);

/* Æá»a Íe»³µA ‰Åe–E Ðq®—i */
void draw_edit_screen(void);
void draw_detail_screen(void);
void disp_one_quiz(int index, int pos);
void disp_quiz_detail(quiz_struct *q);
int  edit_one_quiz(int index, int mode);
void edit_quiz(int mode);

/* ‰A·±µA ‰Åe–E Ðq®—i */
void draw_game_screen(void);
void disp_game_quiz(quiz_struct *q);
int  one_level_game(void);
void game(void);

/* ‹¡Èa º¡A“AµA ‰Åe–E Ðq®—i */
void append_file(void);
void to_dos(void);
void ask_quit(void);

/****************************************************************************/
/*                                Ðq® ¥¥ÁA                                 */
/****************************************************************************/

void main(void)
{
   int cur_top, cur_sub;

   /* Ðe‹i ÑÅ‰w·a¡ Á¡‹¡ÑÁÐe”a. */
   if(init_han() != OK_HAN)
   {
      puts("Can\'t initialize the HANGUL environment.");
      exit(2);
   }

   /* ÑÁ¡e· ˆa¡Í¢·¡ 80 Äéœñ·¡ ´a“¡¡e ¯©Ð—·i º—»¡Ðe”a. */
   if(get_abs_maxx_han() != 79)
   {
      puts_han("·¡ Ïa¡‹aœ‘·e 80 Äéœñ¶w·³“¡”a. ÐáÇIŸ¡¯aŸi ¬a¶wÐe”a¡e");
      puts_han("copy herc640.bgi herc.bgi ¡ww·i ¯©Ð—¯¡Ç¥ Ò ”a¯¡ ¯¡");
      puts_han("¸bÐa­A¶a. ´a¢ Ç¡a ’Ÿa­A¶a..");
      if(!getch()) getch();
   }

   randomize();                 /* e® ¤i¬—‹¡ Á¡‹¡ÑÁ */
   harderr(error_handler);      /* Ða—a¶Á´á µAœá Ð…—iœá ¬é¸÷ */
   status_on_han();             /* ¬wÈ œa·¥ Š¬÷ */
   disp_status();
   disp_logo();                 /* ¡‰¡ ÑÁ¡e·i ¥¡µa º…”a */

   while(1)             /* {‹¡ ¡A“AŸi ¬åÈ‚Ði ˜Œa»¡ ¢ÐeÓ¡ ¤e¥¢Ðe”a. */
   {
      /* Î‰”a¶… ¡A“AŸi Š¬÷Ðe Ò ¬a¶w¸a· ¬åÈ‚·i ¤h“e”a */
      default_color();
      vclrscr_han(BLUE);
      if(my_pulldown_han(&menu_data) == ESC) continue;
      vclrscr_han(BLUE);
      /* cur_top = ¬a¶w¸aˆa ¬åÈ‚Ðe º¡A“A· ¤åÑ¡ */
      /* cur_sub = ¬a¶w¸aˆa ¬åÈ‚Ðe ¦¡A“A· ¤åÑ¡ */
      cur_top = menu_data.current;
      cur_sub = menu_data.sub_menu[cur_top].current;

      switch(cur_top)
      {
         case 0 :       /* •¡¶‘ i ¥¡‹¡ */
            show_help();
            break;
         case 1 :       /* ‰A·± ¯¡¸b */
            game();
            break;
         case 2 :       /* ·AÑw ¬åÈ‚ */
            cur_type = cur_sub;
            disp_type_level();
            break;
         case 3 :       /* Æá»a ‰ÅŸ¡ */
            switch(cur_sub)
            {
               case 0 : /* Âˆa */
                  edit_quiz(0);
                  break;
               case 1 : /* ®¸÷/¬b¹A */
                  edit_quiz(1);
                  break;
               case 2 : /* ÑÁ·©¡¦Èá Âˆa */
                  append_file();
                  break;
            }
            break;
         case 4 :       /* ‹¡Èa */
            switch(cur_sub)
            {
               case 0 : /* ­¡Ÿ¡ Åa‹¡/a‹¡ */
                  sound_on = !sound_on;
                  /* ¦¡A“A ¢…¸aµi·i ŠaÁA */
                  if(sound_on)
                     strcpy(menu_data.sub_menu[4].string[0], " ­¡Ÿ¡   a‹¡ ");
                  else
                     strcpy(menu_data.sub_menu[4].string[0], " ­¡Ÿ¡   Åa‹¡ ");
                  play2("o4l16c");
                  break;
               case 1 : /* Ðw¡¢ ¬ã‹¡/´e ¬ã‹¡ */
                  mix_item = !mix_item;
                  /* ¦¡A“A ¢…¸aµi·i ŠaÁA */
                  if(mix_item)
		     strcpy(menu_data.sub_menu[4].string[1], " Ðw¡¢ ´e¬ã‹¡ ");
                  else
                     strcpy(menu_data.sub_menu[4].string[1], " Ðw¡¢   ¬ã‹¡ ");
                  play2("o4l16c");
                  break;
               case 2 : /* –¤é¯¢/­A¤é¯¢ ¸aÌe */
                  bul3_mode = !bul3_mode;
                  /* ¦¡A“A ¢…¸aµi·i ŠaÁA */
                  if(bul3_mode)
                     strcpy(menu_data.sub_menu[4].string[2], " –¤é¯¢·a¡  ");
                  else
                     strcpy(menu_data.sub_menu[4].string[2], " ­A¤é¯¢·a¡  ");
                  disp_status();
                  play2("o4l16c");
                  break;
               case 3 : /* •¡¯a¡ aˆa‹¡ */
                   to_dos();
                   break;
               case 4 : /* {‹¡ */
                  ask_quit();
                  break;
            }
            break;
      } /* switch {}· { */
   } /* while {}· { */
}

void disp_type_level(void)
/* Ñe¸ ¬åÈ‚–E Æá»a· ·AÑw‰Á e·¡•¡Ÿi ¬wÈ œa·¥µA Îa¯¡Ðe”a. */
{
   set_attr_han(FONT_INVERSE);
   set_vbcolor_han(LIGHTGRAY, LIGHTGREEN);

   /* Ñe¸ ¬åÈ‚–E ·AÑwµA Ð”wÐa“e ¢…¸aµi·e º¡A“A •A·¡ÈáµA ¸á¸w–A´á ·¶”a. */
   aputs_han(58, abs_maxy_han, menu_data.sub_menu[2].string[cur_type],
             FONT_INVERSE);
   aprintf_han(71, abs_maxy_han, FONT_INVERSE, "%1d", cur_level+1);

   set_attr_han(FONT_NORMAL);
   default_color();
}

void disp_status(void)
/* ÑÁ¡e ¹A·© Ða”eµA ¬wÈ œa·¥·i Îa¯¡Ðe”a. */
{
   /*  å¸á <Ðe>· ¸÷¸â·¥ ¬wÈ œa·¥·i Îa¯¡Ðe”a. */
   draw_status_han();
   /* ·AÑw‰Á e·¡•¡Ÿi Îa¯¡Ðe”a. */
   set_attr_han(FONT_INVERSE);
   set_vbcolor_han(LIGHTGRAY, LIGHTGREEN);
   /* abs_maxy_han·e ÂA” y ¹ÁÎaŸi aÈa“e ¸åµb ¥e® */
   xputs_han(68, abs_maxy_han, "¹A   ®º…");
   set_vbcolor_han(LIGHTGRAY, BLACK);
   put_eng_font(57, abs_maxy_han, 179);
   put_eng_font(66, abs_maxy_han, 179);
   put_eng_font(78, abs_maxy_han, 179);
   disp_type_level();
}

void disp_logo(void)
/* ¡‰¡ ÑÁ¡e·i ¥¡µa º…”a. */
{
   int box_x;

   set_bkcolor_han(BLUE);
   clrscr_han();
   /* in_vga_system·e EGA/VGAŸi ¬a¶wº—·© ˜ 1¡ ¬é¸÷–A“e ¸åµb ¥e® */
   if(in_vga_system)
      center_puts_han(6, " Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1 ", LIGHTBLUE,
                      FONT_HORIZ_DBL | FONT_VERT_DBL);
   else
      center_puts_han(6, " Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1 ", WHITE,
                      FONT_SHADOW | FONT_HORIZ_DBL | FONT_VERT_DBL);

   box_x = (abs_maxx_han + 1) / 2 - 20;
   set_vbcolor_han(LIGHTGRAY, CYAN);
   set_attr_han(FONT_INVERSE);

   xputs_han(box_x, 10, "                                         ");
   set_bkcolor_han(RED);
   aputs_han(box_x, 11, "  Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1 ·w¶w Ïa¡‹aœ‘ ",
             FONT_SHADOW | FONT_INVERSE);
   set_bkcolor_han(BLACK);
   draw_box_han(box_x, 12, box_x + 40, 12, BOX_H1V1);
   if(in_vga_system)
      vbputs_han(box_x, 13, "        Æá»a ‰A·± <™©™©·¡> Ver 1.0       ",
                 DARKGRAY, YELLOW);
   else
      aputs_han( box_x, 13, "        Æá»a ‰A·± <™©™©·¡> Ver 1.0       ",
                FONT_SHADE | FONT_INVERSE);
   draw_box_han(box_x, 14, box_x + 40, 14, BOX_H1V1);
   set_bkcolor_han(BLUE);
   aputs_han(box_x, 15, "        e—e·¡ : ·¡ ÑeÑ¡(easyride)        ",
             FONT_OUTLINE | FONT_INVERSE);
   xputs_han(box_x, 16, "        e—ei : 1992 ‘e 06 ¶© 06 ·©      ");
   xputs_han(box_x, 17, "                                         ");

   default_color();
   set_attr_han(FONT_NORMAL);
   /* 5Á¡ •·´e ¬a¶w¸aˆa Ç¡Ÿi ’Ÿa‹¡Ÿi ‹¡”aŸ¥”a. */
   clear_key_buff();
   if(wait_key_han(5))
   {
      if(!getch_han()) getch_han();
   }
}

void play2(char *string)
/* '­¡Ÿ¡ Åa‹¡' ¬wÈ(sound_on·¡ 1·¥ ¬wÈ)µA¬á e ­¡Ÿ¡Ÿi “e Ðq® */
{
   if(sound_on) play(string);
}

int read_quiz(int quiz_type)
/* Æá»a ÑÁ·©·i ·ª´á —i·¥”a. µAœáˆa ¤i¬—Ða¡e 0·i Ÿ¡ÈåÐe”a. */
{
   FILE *stream;
   char file_name[20], header[20], result;
   long f_length;

   quiz_num = 0;
   result   = 1;
   /* ¢…¹A ·AÑwµA ˜aœa quiz00.dta, quiz01.dta,       Ÿi ·ª´á —i·¥”a */
   sprintf(file_name, "quiz%02d.dta", quiz_type);
   if((stream = fopen(file_name, "rb")) == NULL)
   /* ÑÁ·©·¡ ´ô·a¡e ¬¡ ¬—¬÷Ðe”a. */
   {
      stream = fopen(file_name, "wb");
      result = fwrite(FILE_HEADER, sizeof(FILE_HEADER), 1, stream) == 1;
   }
   else
   /* ÑÁ·©·¡ ·¶·a¡e ·ª´á¬á quizµA ¸á¸wÐe”a. */
   {
      /* ÑA”áˆa ·©Ã¡Ði ˜ e ·ª“e”a. */
      if(fread(header, sizeof(FILE_HEADER), 1, stream) == 1 &&
         !strcmp(header, FILE_HEADER))
      {
         /* f_length“e ÑÁ·© Ça‹¡µA¬á ÑA”á Ça‹¡Ÿi ¨… ˆt */
         f_length = filelength(fileno(stream)) - sizeof(FILE_HEADER);
         if(f_length > 0)
         /* Æá»aˆa Ðaa ·¡¬w ¸á¸w–A´á ·¶·a¡e ¡A¡¡Ÿ¡Ÿi Ði”wÐa‰¡ ·ª´á —i·¥”a. */
         {
	    quiz = (quiz_struct *) malloc(f_length);
            if(quiz == NULL)
            {
               put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
               result = 0;
            }
            else
            {
               result = fread(quiz, f_length, 1, stream) == 1;
               if(result == 0) free(quiz);
               /* ·ª´á —i·¥ Æá»a· ˆ•®“e quiz_numµA ¸á¸wÐe”a. */
               quiz_num = f_length / sizeof(quiz_struct);
            }
	 }
      }
      else
	 result = 0;
   }
   fclose(stream);

   if(!result)
   /* µAœáˆa ¤i¬—Ð–·a¡e */
   {
      put_msg("¸aža ÑÁ·©·i ·ª·i ®ˆa ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..", -1);
      quiz_num = 0;
   }
   return(result);
}

int save_quiz(int quiz_type, int mode)
/* Ñe¸ ¡A¡¡Ÿ¡µA ·¶“e Æá»a •A·¡ÈáŸi Æá»a ÑÁ·©¡ ¸á¸wÐe”a. */
/* mode = 0·¡¡e ÑÁ·©· {µA ÂˆaÐa‰¡, 1·¡¡e ”ü´á ³e”a.    */
/* µAœáˆa ¤i¬—Ða¡e 0·i Ÿ¡ÈåÐe”a.                          */
{
   FILE *stream;
   struct ffblk ffblk;
   char file_name[20], result;

   /* ¢…¹A ·AÑwµA ˜aœa quiz00.dta, quiz01.dta,       µA ¸á¸wÐe”a. */
   sprintf(file_name, "quiz%02d.dta", quiz_type);
   if(mode == 0 && findfirst(file_name, &ffblk, 0) == 0)
   /* Âˆa ¬wÈ·¥•A ÑÁ·©·¡ ·¶“e ‰w¶ */
   {
      result = (stream = fopen(file_name, "ab")) != NULL;
   }
   else
   /* Âˆa ¬wÈ»¡ e ÑÁ·©·¡ ´ôˆáa, ”ü´á ³a‹¡ ¬wÈ·¥ ‰w¶ */
   {
      result = (stream = fopen(file_name, "wb")) != NULL;
      if(result) result = fwrite(FILE_HEADER, sizeof(FILE_HEADER), 1, stream) == 1;
   }

   /* quiz_numˆ· Æá»aŸi ¸á¸wÐe”a. */
   result = fwrite(quiz, quiz_num*sizeof(quiz_struct), 1, stream) == 1;
   if(!result) put_msg("¸aža ÑÁ·©µA ³i ®ˆa ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
   fclose(stream);

   return(result);
}

void clear_quiz(int index)
/* ¡A¡¡Ÿ¡µA¬á ·ª´á —i·¥ Æá»a º— index¤å¼· ¶w·i »¡¶…”a. */
{
   int i;

   for(i = 0; i < QUEST_LINE; i++)    strcpy(quiz[index].quest[i], "");
   for(i = 0; i < ITEM_PER_QUEST; i++) strcpy(quiz[index].item[i], "");
   /* ”s·¡ -1¤å·¡œa“e ˆõ·e ·³b–A»¡ ´g·e Æá»aœa“e ˆõ·i ›uÐe”a. */
   quiz[index].answer = quiz[index].level = -1;
   quiz[index].make_num = quiz[index].wrong_num = 0;
}

void draw_edit_screen(void)
/* Æá»aŸi Íe»³Ða‹¡ ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. */
{
   draw_3dbox(0, 0, abs_maxx_han, abs_maxy_han-1, LIGHTGRAY);
   xputs_han( 2, 1, "Á·      ˆ");
   xputs_han(64, 1, "Ñe¸      ¤å¼");
   draw_horiz_line(5, 36, 78*8+5, BLACK);
   set_output_offset_han(8);
   xputs_han(1, 2, "         ¢…   ¹A                 (1)             (2)"
                   "             (3)       A L");
   draw_horiz_line(5, 60, 78*8+5, BLACK);
   draw_vert_line(27*8+6, 36, 24, BLACK);
   draw_vert_line(43*8+6, 36, 24, BLACK);
   draw_vert_line(59*8+6, 36, 24, BLACK);
   draw_vert_line(75*8+6, 36, 24, BLACK);
   draw_vert_line(77*8+6, 36, 24, BLACK);
   set_output_offset_han(0);
}

void draw_detail_screen(void)
/* Æá»a· ¶w·i ·³bÐa‹¡ ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. */
{
   int i;

   draw_3dbox(17, 10, 62, 19, LIGHTGRAY);
   draw_horiz_line(17*8+5, 14*16+8, 45*8-3, BLUE);
   draw_vert_line( 20*8+6, 10*16+9, 9*16-3, BLUE);
   vputs_han(18, 11, "¢…", LIGHTGREEN);
   vputs_han(18, 13, "¹A", LIGHTGREEN);
   /* ¸åˆb¢…¸a¡ 1, 2, 3 ·i Â‰bÐe”a. */
   for(i = 0; i < ITEM_PER_QUEST; i++)
      vprintf_han(18, 15+i, LIGHTGREEN, "%c%c", SPC_ID, i+'1');
   vputs_han(18, 18, "”s",     LIGHTGREEN);
   vputs_han(40, 18, "e·¡•¡", LIGHTGREEN);
}

void disp_one_quiz(int index, int pos)
/* ¡A¡¡Ÿ¡µA ¸á¸w–E Æá»a º— index ¤å¼ Æá»aŸi Íe»³ ÑÁ¡e· */
/* pos¤å¼ ¶áÃ¡µA Îa¯¡Ðe”a.                              */
{
   int i;

   if(index >= quiz_num)
   /* ¶w·¡ ´ô“e Æá»a·© ˜“e ‰·¤‚ ¢…¸aŸi Â‰bÐe”a. quiz_num·e ¶w·¡ */
   /* ·¶“e Æá»a(·ª´á —i·¥ Æá»a, ™¡“e Âˆa ·³b–E Æá»a)· ®·¡”a.      */
   {
      nputs_han(1, 4+pos, "", 26);
      for(i = 0; i < ITEM_PER_QUEST; i++) nputs_han(28+16*i, 4+pos, "", 15);
      xputs_han(76, 4+pos, " ");
      xputs_han(78, 4+pos, " ");
   }
   else
   /* ¶w·¡ ·¶“e Æá»a·© ˜“e ‹a ¶w·i Îa¯¡Ðe”a. */
   {
      nputs_han(1, 4+pos, quiz[index].quest[0], 26);
      for(i = 0; i < ITEM_PER_QUEST; i++)
         nputs_han(28+16*i, 4+pos, quiz[index].item[i], 15);
      xprintf_han(76, 4+pos, "%1d", quiz[index].answer+1);
      xprintf_han(78, 4+pos, "%1d", quiz[index].level+1);
   }
}

void disp_quiz_detail(quiz_struct *q)
/* Ðe ˆ· Æá»a *q· ¶w·i Æá»a ·³b ÑÁ¡eµA Îa¯¡Ðe”a. */
{
   int i;

   for(i = 0; i < QUEST_LINE;     i++)
      nputs_han(21, 11+i, q->quest[i], QUEST_LENGTH);
   for(i = 0; i < ITEM_PER_QUEST; i++)
      nputs_han(21, 15+i, q->item[i],  ITEM_LENGTH);
   if(q->level == -1)
   /* ¶w·¡ ´ô“e Æá»a·© ˜“e ”s‰Á e·¡•¡Ÿi Îa¯¡Ða»¡ ´g“e”a. ¶w·¡ ´ô“e */
   /* Æá»a“e level·¡ -1¡ ¬é¸÷–E”a.                                      */
   {
      xputs_han(21, 18, " ");
      xputs_han(47, 18, " ");
   }
   else
   /* ¶w·¡ ·¶“e Æá»a·© ˜“e ”s‰Á e·¡•¡Ÿi Îa¯¡Ðe”a. */
   {
      xprintf_han(21, 18, "%1d", q->answer+1);
      xprintf_han(47, 18, "%1d", q->level+1);
   }
}

int edit_one_quiz(int index, int mode)
/* index ¤å¼ Æá»aŸi Íe»³(Âˆa/®¸÷/¬b¹A)Ðe”a.            */
/* mode = 0·¡¡e Âˆa ¬wÈ, 1·¡¡e ®¸÷/¬b¹A ¬wÈˆa –E”a.   */
/* Ðaa ·¡¬w Íe»³Ðaµv·i ˜“e ENTERŸi, Ðaa•¡ Íe»³Ða»¡ ´g  */
/* ´v·i ˜“e ESCŸi Ÿ¡ÈåÐe”a.                              */
{
   char exit_key[] = { UP, DOWN, HOME, END, 0 }, number[2];
   char *(text[QUEST_LINE]); /* note_pad()¡ ·³b¤h‹¡ ¶áÐe Í¡·¥Èá ¤µi  */
   quiz_struct temp;         /* ·³b¤h·e ¶w·i ¸á¸wÐa“e ·±¯¡ ¥e®      */
   int edited_num;           /* ¯©¹A¡ Íe»³–E Æá»a· ®                 */
   int cur_item;             /* Ñe¸ Íe»³ º—·¥ Ðw¡¢                     */
   char *screen;
   unsigned size;
   int key, quit, i;

   /* index¤å¼ Æá»a· Íe»³·i ¶áÐ tempµA ¥¢¬aÐe”a. */
   temp = quiz[index];
   /* note_pad()¡ ¢…¹AŸi ·³b¤h·i ® ·¶•¡¢ quest[0], quest[1], */
   /* quest[2]· ¬å– ¤å»¡Ÿi Í¡·¥Èá ¤µi textµA ¸á¸wÐe”a.        */
   for(i = 0; i < QUEST_LINE; i++) text[i] = temp.quest[i];

   /* ·¡ Ðq®ˆa Ñ¡Â‰–I ˜“e ¤aÈwµA ‹aœÏ¢·¡ ´ô·a£a¡ getimage()¡ ÑÁ¡e·i  */
   /* ¸á¸wÐi Ï©¶aˆa ´ô”a.                                                 */
   if((screen = save_text_han(17, 10, 62, 19)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      return ESC;
   }

   /* temp· ¶w·i ·³bÐa‹¡ ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_detail_screen();
   disp_quiz_detail(&temp);

   cur_item = edited_num = quit = 0;
   /* Âˆa ¬wÈµA¬á“e <Esc> Ç¡Ÿi ’Ÿa»¡ ´g“e Ðe, ‰­¢ ·³b¤h“e”a. */
   while(!quit)
   {
      if(mode == 0 && quiz_num >= MAX_APPEND)
      /* Âˆa ¬wÈµA¬á“e Ðe ¤åµA MAX_APPENDˆŸi Á¡‰ÁÐaµa ÂˆaÐi ® ´ô”a.  */
      /* MAX_APPENDˆ· Æá»aŸi ÂˆaÐi ¡A¡¡Ÿ¡ e Ði”w–A´á ·¶‹¡ ˜¢…·¡”a.    */
      {
         put_msg("Ðe ¤åµA 100ˆŸi Á¡‰ÁÐaµa ÂˆaÐi ® ´ô¯s“¡”a. "
                 "´a¢ Ç¡a ’Ÿa­A¶a.", -1);
         break;
      }

      switch(cur_item)
      /* Ñe¸ ¬åÈ‚–E Ðw¡¢µA ˜aœa ¸â¸éÐe ·³b Ðq®Ÿi ¯©Ð—¯¡Ç¥”a. */
      {
         case 0 :       /* ¢…¹A ·³b */
            key = note_pad(21, 11, text, QUEST_LINE, QUEST_LENGTH);
            /* <Ctrl>+<End>Ÿi ’Ÿa¡e ¹A·©  a»¡ b º‰µA¬á <Enter> Ç¡Ÿi ’Ÿe  */
            /* ˆõ‰Á •··©Ða‰A Âá‹sÐe”a.                                     */
            if(key == CTRLEND) key = ENTER;
            break;
         case 1 :
         case 2 :
         case 3 :       /* Ðw¡¢ ·³b */
            if(strlen(temp.item[cur_item - 1]) < adv_gets_han_index)
            /* ¬¡ ·¡•·Ðe º‰· ¢…¸aµi ‹©·¡ˆa ”á ¼l”a¡e ¢…¸aµi· */
            /* ¹A·© {µA Äá¬áŸi ¶áÃ¡¯¡Ç¥”a.                      */
               adv_gets_han_index = strlen(temp.item[cur_item - 1]) * (-1) - 1;
            else
            /* Â—¦…Ó¡ ‹©”a¡e Ñe¸ Äá¬á· ¶áÃ¡Ÿi ‹a”¡ ·A»¡Ðe”a. */
               adv_gets_han_index = adv_gets_han_index * (-1) - 1;
            key = my_adv_gets_han(21, 14+cur_item, temp.item[cur_item - 1],
                                  ITEM_LENGTH, FONT_NORMAL, exit_key);
            break;
         case 4 :       /* ”s ·³b */
            do
            {
               if(temp.answer == -1)
               /* ”s·i ·³bÐe ¸â·¡ ´ô·a¡e é ¢…¸aŸi Îa¯¡Ðe Ò ·³b·i ¤h“e”a. */
                  number[0] = 0;
               else
               /* ”s·i ·³bÐe ¸â·¡ ·¶·a¡e ‹a ”s·i Îa¯¡Ðe Ò ·³b·i ¤h“e”a. */
               {
                  number[0] = temp.answer + '1';
                  number[1] = 0;
               }
               key = my_adv_gets_han(21, 18, number, 1, FONT_NORMAL, exit_key);
               temp.answer = number[0] - '1';
               if(temp.answer < 0 || temp.answer >= ITEM_PER_QUEST) temp.answer = -1;
            }
            /* ”s·¡ ¹A”¡ ·³b–A»¡ ´g´v“e•A, <Enter> Ç¡Ÿi  ’‰œöˆáa  ´á˜å */
            /* ‹i¸aŸi ·³bÐ–·i ˜“e ”a¯¡ ·³b¤h“e”a. ”s·i ·³bÐa»¡ ´g·e ¬w */
            /* È·¡¡e ·¡•·Ç¡¡ Ðw¡¢·i ·¡•·Ði ® ·¶”a.                      */
            while(temp.answer == -1 && (key == ENTER || strlen(number) != 0));
            break;
         case 5 :       /* e·¡•¡ ·³b */
            do
            {
               if(temp.level == -1)
                  number[0] = 0;
               else
               {
                  number[0] = temp.level + '1';
                  number[1] = 0;
               }
               key = my_adv_gets_han(47, 18, number, 1, FONT_NORMAL, exit_key);
               temp.level = number[0] - '1';
               if(temp.level < 0 || temp.level >= LEVEL_NUM) temp.level = -1;
            }
            while(temp.level == -1 && (key == ENTER || strlen(number) != 0));
            break;
      }

      /* ÂA¹·¸â·a¡ ’‰a»¥ Ç¡µA ”Ðe ÀáŸ¡Ÿi Ðe”a. */
      switch(key)
      {
         case UP    :   /* Ðe ”e‰ ¶õ Ðw¡¢·a¡ ·¡•· */
            cur_item = (cur_item + 5) % 6;
            break;
         case ENTER :   /* ¹A·©  a»¡ b Ðw¡¢µA¬á <Enter> Ç¡Ÿi ’‰œö·i  ˜ e */
                        /* ·³b·¡ µÅža–E ˆõ·a¡ ˆeºÐa‰¡ ‹a ¶AµA“e Ðw¡¢·i */
                        /* Ðe ”e‰ ´aœ¡ ·¡•·¯¡Ç¥”a.                     */
            if(cur_item == 5)
            {
               if(temp.answer == -1)
               /* ”s·i ·³bÐa»¡ ´g´v·a¡e ·³bÐa•¡¢ Ðe”a. e·¡•¡Ÿi ·³b */
               /* ¤h“e Ðw¡¢µA¬á <Enter> Ç¡Ÿi ’‰œö·i ˜ e  ¯©Ð—–A“e žË¥ */
               /* ·¡£a¡ e·¡•¡“e ¤aŸa‰A  ·³b–A´á ·¶·i ®¤cµA ´ô”a.    */
               /* ˜aœa¬á level·e ˆñ¬aÐi Ï©¶aˆa ´ô”a.                    */
               {
                  play2("o2l16eee");
                  put_msg("”s·i ¸÷Ð º­A¶a", 2);
                  break;
               }
               play2("o4l16ceg");

               /* ·³b¤h·e Æá»a ¸ažaŸi ¸á¸wÐe”a. */
               quiz[index + edited_num] = temp;
               for(i = 0; i < QUEST_LINE; i++)
                  strcpy(quiz[index + edited_num].quest[i], temp.quest[i]);
               edited_num++;

               /* Âˆa ¬wÈµA¬á“e ‰­¢ ·³b¤h‰¡ ®¸÷/¬b¹A ¬wÈµA¬á“e ¹·ža */
               if(mode == 0)
               {
                  temp = quiz[index + edited_num];
                  for(i = 0; i < QUEST_LINE; i++) text[i] = temp.quest[i];
                  disp_quiz_detail(&temp);
                  quiz_num++;
                  xprintf_han(69, 1, "%4d", index + edited_num + 1);
                  xprintf_han( 5, 1, "%4d", quiz_num);
                  cur_item = 0;
               }
               else
                  quit = 1;
               break;
            }
         case DOWN  :   /* Ðe ”e‰ ´aœ Ðw¡¢·a¡ ·¡•· */
            cur_item = (cur_item + 1) % 6;
            break;
         case HOME  :   /* ¹A·© Àõ Ðw¡¢·a¡ ·¡•· */
            cur_item = 0;
            break;
         case END   :   /* ¹A·© { Ðw¡¢·a¡ ·¡•· */
            cur_item = 5;
            break;
         case ESC   :   /* Íe»³·i Âá­¡Ða‰¡ {…”a. */
            quit = 1;
            break;
      }
   }

   /* ÑÁ¡e·i ¥¢ŠÐe”a */
   restore_text_han(17, 10, screen);
   return(edited_num == 0 ? ESC : ENTER);
}

void edit_quiz(int mode)
/* Ñe¸ ¬åÈ‚–E ·AÑw· Æá»aŸi Íe»³(Âˆa/®¸÷/¬b¹A)Ðe”a.    */
/* mode = 0 ·¡¡e Âˆa ¬wÈ, 1 ·¡¡e ®¸÷/¬b¹A ¬wÈˆa –E”a. */
{
   /* cur_pos  = Íe»³ ÑÁ¡eµA¬á ¤e¸å  b”· Ñe¸ ¶áÃ¡          */
   /* prv_pos  = Íe»³ ÑÁ¡eµA¬á ¤e¸å  b”· ¤a¡ ¸å ¶áÃ¡       */
   /* top_quiz = Íe»³ ÑÁ¡eµA¬á ¹A·© ¬w”eµA ·¶“e Æá»a· ·¥•B¯a */
   /* ex) Ñe¸ ÑÁ¡eµA¬á ¤e¸å  b”ˆa ¶áÃ¡Ðe Æá»a· ·¥•B¯a“e    */
   /*     top_quiz + cur_pos·¡”a.                             */
   static int cur_pos, prv_pos, top_quiz;
   /* max_disp = Ñe ‹aœÏ¢ ´á”“ÈáµA¬á Ðe ÑÁ¡eµA Îa¯¡Ði ® ·¶“e Æá»a· ® */
   /* re_draw  = 1 ·¡¡e Íe»³ ÑÁ¡e·i ”a¯¡ Š¬÷Ðe”a.                       */
   /* move_bar = 1 ·¡¡e cur_pos¡ ¤e¸å  b”Ÿi ·¡•·¯¡Ç¥”a.                */
   int  max_disp, re_draw, move_bar;
   int  key, i;

   if(mode == 0)
   /* Âˆa ¬wÈ·¡¡e MAX_APPENDˆ· Æá»aŸi ·³b¤h·i ¡A¡¡Ÿ¡Ÿi Ði”wÐe”a. */
   {
      quiz = (quiz_struct *) malloc(MAX_APPEND*sizeof(quiz_struct));
      if(quiz == NULL) return;
      for(i = 0; i < MAX_APPEND; i++) clear_quiz(i);
      quiz_num = 0;
   }
   else
   /* ®¸÷ ¬wÈ·¡¡e  å¸á Ñe¸ ¬åÈ‚–E ·AÑw· Æá»aŸi ÑÁ·©µA¬á ·ª´á —i·¥”a.*/
   /* Ï©¶aÐe ¡A¡¡Ÿ¡“e read_quiz()· ¦µA¬á quizµA Ði”w–E”a.           */
   {
      if(!read_quiz(cur_type)) return;
      if(quiz_num == 0)
      /* ®¸÷Ði Æá»aˆa Ðaa•¡ ´ô·i ˜“e ¤a¡ Ÿ¡ÈåÐe”a. ·¡ ‰w¶ read_qui */
      /* z()· ¦µA¬á ¡A¡¡Ÿ¡ˆa Ði”w–A»¡ ´g“e”a.                       */
      {
         put_msg("®¸÷Ði Æá»aˆa Ðaa•¡ ´ô¯s“¡”a", 2);
         return;
      }
   }

   /* Æá»aŸi Íe»³Ða‹¡ ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_edit_screen();
   if(mode == 0)
      vputs_han(33, 1, "Æá »a  Â ˆa", RED);
   else
      vputs_han(29, 1, "Æá »a  ® ¸÷ / ¬b ¹A", RED);
   xprintf_han( 5, 1, "%4d", quiz_num);

   max_disp = abs_maxy_han - 6;
   re_draw = 1;
   top_quiz = cur_pos = prv_pos = 0;

   do
   {
      if(re_draw)       /* ÑÁ¡e·i ”a¯¡ Š¬÷Ð´¡ Ðe”a¡e */
      {
         for(i = 0; i < max_disp; i++) disp_one_quiz(top_quiz+i, i);
         re_draw  = 0;
         move_bar = 1;
      }
      if(move_bar)      /* ¤e¸å  b”Ÿi ·¡•·¯¡Åa´¡ Ðe”a¡e */
      {
         modify_text_attr(1, 4+prv_pos, 78, 4+prv_pos, FONT_NORMAL);
         modify_text_attr(1, 4+cur_pos, 78, 4+cur_pos, FONT_INVERSE);
         xprintf_han(69, 1, "%4d", top_quiz+cur_pos+1);
         prv_pos = cur_pos;
         move_bar = 0;
      }

      if((key = getch_han()) == 0)
      /* ÑÂ¸w Ç¡ˆa ’‰a»¥ ‰w¶ */
      {
         switch(getch_han())
         {
            case UP       :     /* ¤a¡ ¶õ Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               if(cur_pos > 0)
               {
                  cur_pos--;
                  move_bar = 1;
               }
               else if(top_quiz > 0)
               {
                  top_quiz--;
                  modify_text_attr(1, 4, 78, 4, FONT_NORMAL);
                  xscrolldn_han(1, 4, 78, 3+max_disp);
                  disp_one_quiz(top_quiz, 0);
                  move_bar = 1;
               }
               break;
            case DOWN     :     /* ¤a¡ ´aœ Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               if(top_quiz + cur_pos < quiz_num - 1)
               {
                  if(cur_pos == max_disp - 1)
                  {
                     top_quiz++;
                     modify_text_attr(1, 4+cur_pos, 78, 4+cur_pos, FONT_NORMAL);
                     xscrollup_han(1, 4, 78, 3+max_disp);
                     disp_one_quiz(top_quiz+cur_pos, max_disp-1);
                     move_bar = 1;
                  }
                  else
                  {
                     cur_pos++;
                     move_bar = 1;
                  }
               }
               break;
            case PGUP     :     /* Ðe ÍA·¡»¡ ´| Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               if(top_quiz >= max_disp)
               {
                  top_quiz -= max_disp;
                  re_draw = 1;
               }
               else if(top_quiz == 0)
               {
                  cur_pos  = 0;
                  move_bar = 1;
               }
               else
               {
                  top_quiz = cur_pos = 0;
                  re_draw  = 1;
               }
               break;
            case PGDN     :     /* Ðe ÍA·¡»¡ ´aœ Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               if(top_quiz < quiz_num - max_disp)
               {
                  top_quiz += max_disp;
                  if(top_quiz + cur_pos > quiz_num - 1)
                     cur_pos = (quiz_num - 1) - top_quiz;
                  re_draw = 1;
               }
               else
               {
                  cur_pos  = (quiz_num - 1) - top_quiz;
                  move_bar = 1;
               }
               break;
            case HOME     :     /* ÑÁ¡e· ¹A·© Àõ Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               cur_pos  = 0;
               move_bar = 1;
               break;
            case END      :     /* ÑÁ¡e· ¹A·© { Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               if(quiz_num >= top_quiz + max_disp)
                  cur_pos = max_disp - 1;
               else
                  cur_pos = (quiz_num - 1) - top_quiz;
               move_bar = 1;
               break;
            case CTRLHOME :     /* ¹A·© Àõ Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               top_quiz = cur_pos = 0;
               re_draw  = 1;
               break;
            case CTRLEND  :     /* ¹A·© { Æá»a¡ ·¡•· */
               if(quiz_num == 0) break;
               top_quiz = quiz_num - 1;
               cur_pos  = 0;
               re_draw  = 1;
               break;
            case INS      :     /* Æá»aŸi Âˆa */
               if(mode != 0) break;       /* Âˆa ¬wÈˆa ´a“¡¡e ¢¯¡ */
               i = quiz_num;
               if(edit_one_quiz(quiz_num, mode) == ENTER)
               /* Ðaaœa•¡ Âˆa–A´ö·a¡e ÑÁ¡e·i ¸Š¬÷Ðe”a. */
               {
                  top_quiz = i;
                  cur_pos  = 0;
                  re_draw  = 1;
               }
               break;
            case DEL      :     /* ¤e¸å  b”ˆa ·¶“e ¶áÃ¡· Æá»aŸi ¬b¹A */
               if(quiz_num == 0) break;
               if(top_quiz + cur_pos != quiz_num - 1)
                  memmove(&(quiz[top_quiz+cur_pos]), &(quiz[top_quiz+cur_pos+1]),
                          sizeof(quiz_struct)*(quiz_num-top_quiz-cur_pos-1));
               quiz_num--;
               clear_quiz(quiz_num);
               if(top_quiz + cur_pos >= quiz_num)
               {
                  if(cur_pos > 0)
                     cur_pos--;
                  else if(top_quiz > 0)
                     top_quiz--;
               }
               xprintf_han( 5, 1, "%4d", quiz_num);
               xprintf_han(69, 1, "%4d", top_quiz+cur_pos+1);
               re_draw = 1;
               break;
         } /* switch {}· { */
      } /* if {}· { */
      else
      /* ·©¤e Ç¡ˆa ’‰a»¥ ‰w¶ */
      {
         switch(key)
         {
            case _ENTER   :     /* ¤e¸å  b”ˆa ·¶“e ¶áÃ¡· Æá»aŸi ®¸÷ */
               if(quiz[top_quiz+cur_pos].answer != -1 &&
                  edit_one_quiz(top_quiz+cur_pos, 0) == ENTER)
               {
                  disp_one_quiz(top_quiz+cur_pos, cur_pos);
                  move_bar = 1;
               }
               break;
            case _ESC   :       /* Íe»³·i ¹·ža */
               break;
         } /* switch {} · { */
      } /* else {}· { */
   } /* do {} while· { */
   while(key != _ESC);

   /* Íe»³–E ¶w·i ÑÁ·©µA ¸á¸wÐe Ò quizµA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá¯¡Ç¥”a. */
   save_quiz(cur_type, mode);
   if(quiz != NULL) free(quiz);
}

/* ‰A·±¯¡µA q·e ¯¡ˆe —w·i Îa¯¡Ði ˜ ¬a¶w–A“e  b”µA ‰ÅÐe  Ça¡ ¬w®  */
#define BAR_X           21 * 8                    /*  b”· x ¹ÁÎa    */
#define BAR_Y           17 * 16 + 4               /*  b”· y ¹ÁÎa    */
#define BAR_WIDTH       (62 - 21) * 8             /*  b”· ÂA” ‹©·¡ */
#define BAR_HEIGHT      8                         /*  b”· ‘¼·¡      */

void draw_game_screen(void)
/* ‰A·±·i ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. ‰A·±·¡ ¯¡¸b–I ˜ Ðe ¤å Ñ¡Â‰–E”a. */
{
   int i, j;

   draw_3dbox(17, 7, 62, 15, LIGHTGRAY);
   draw_horiz_line(17*8+5, 11*16+8, 45*8-3, BLUE);
   draw_vert_line( 20*8+5,  7*16+9, 8*16-3, BLUE);
   vputs_han(18,  8, "¢…", LIGHTGREEN);
   vputs_han(18, 10, "¹A", LIGHTGREEN);
   for(i = 0; i < ITEM_PER_QUEST; i++)
   /* ¸åˆb¢…¸a(Ëb®¢…¸a)¡ 1, 2, 3 ·i Â‰bÐe”a. */
      vprintf_han(18, 12+i, LIGHTGREEN, "%c%c", SPC_ID, i+'1');

   /* ¸ñ®Ìe ÑÁ¡e·i Š¬÷Ðe”a. */
   vclear_text_han(17, 17, 62, 19, LIGHTGRAY);
   /* ¡¡œ ¯¡‰(Ëb® ¢…¸a)Ÿi Îa¯¡Ðe”a. */
   vputch_han(18, 17, "\xd4\x01", LIGHTGREEN);
   /* ¸åˆb ¢…¸a(Ëb® ¢…¸a)¡ "O", "X"Ÿi Îa¯¡Ðe”a. */
   vputch_han(18, 18, "\xd4\x0a", LIGHTGREEN);
   vputch_han(18, 19, "\xd4\x0b", LIGHTGREEN);
   draw_3dborder(17*8-2, 17*16-1, 63*8+2, 20*16+2, 1);

   /* ‰w‰Á–E ¯¡ˆe,  xÓ¥ ®, ËiŸ¥ ®Ÿi Îa¯¡Ða“e  b”Ÿi ‹aŸ¥”a. */
   for(i = 0; i < 3; i++)
   {
      if(in_vga_system)
      {
         for(j = 0; j < BAR_HEIGHT; j++)
            draw_horiz_line(BAR_X, BAR_Y+16*i+j, BAR_WIDTH, LIGHTGREEN);
      }
      draw_3dborder(BAR_X-2, BAR_Y+16*i-2,
                    BAR_X+BAR_WIDTH+2, BAR_Y+BAR_HEIGHT+16*i+2, 0);
   }
}

void disp_game_quiz(quiz_struct *q)
/* ‰A·± ÑÁ¡eµA Æá»a *q· ¶w·i Îa¯¡Ðe”a. ¢…¹Aˆa ¤aŽé ˜ a”a Ñ¡Â‰–E”a. */
{
   int i;

   for(i = 0; i < QUEST_LINE;     i++)
      nputs_han(21,  8+i, q->quest[i], QUEST_LENGTH);
   for(i = 0; i < ITEM_PER_QUEST; i++)
      nputs_han(21, 12+i, q->item[i],  ITEM_LENGTH);

   /* ‰w‰Á–E ¯¡ˆe·i Îa¯¡Ða“e  b”Ÿi ¬¡ ‹aŸ¥”a. */
   draw_3dborder(BAR_X-2, BAR_Y-2, BAR_X+BAR_WIDTH+2, BAR_Y+BAR_HEIGHT+2, 0);
   if(in_vga_system)
   {
      for(i = 0; i < BAR_HEIGHT; i++)
         draw_horiz_line(BAR_X, BAR_Y+i, BAR_WIDTH, LIGHTGREEN);
   }
   else
   {
      nputs_han(21, 17, "", 62-21+1);
      draw_3dborder(BAR_X-2, BAR_Y-2, BAR_X+BAR_WIDTH+2, BAR_Y+BAR_HEIGHT+2, 0);
   }
}

/* Â‰¹AÐi Æá»a“e ·±·¡ ¬åÈ‚Ða“e•A  e´¢ ·¡£¡ Â‰¹A–E ˆõ·i ¬åÈ‚Ðe ÒU®ˆa */
/* MAX_RETRY¤å ·¡¬w·¡¡e Æá»a—i·i ®…Àa¸â·a¡ ˆñ¬‚Ðe Ò ¬åÈ‚Ðe”a.        */
#define MAX_RETRY       10

int one_level_game(void)
/* Ñe¸ e·¡•¡· ‰A·±·i Ðe”a.                                */
/* Ÿ¡Èåˆt = UP   --> Ñe¸ e·¡•¡· ‰A·±·i É·‰ÁÐ–·i ˜        */
/*          DOWN --> Ñe¸ e·¡•¡· ‰A·±·i É·‰ÁÐa»¡ ¡µÐ–·i ˜ */
/*          ESC  --> ‰A·± •¡º— <Esc> Ç¡Ÿi ’‰œö·i ˜          */
/*                   ¡A¡¡Ÿ¡ˆa ¦¹¢Ði ˜                      */
{
   /* total_num   = Ñe¸ e·¡•¡µA ­¢Ða“e Æá»a· ® */
   /* make_num    = ·¡ ‰A·±µA¬á Â‰¹A–E Æá»a· ®   */
   /* wrong_num   = ·¡ ‰A·±µA¬á ËiŸ¥ Æá»a· ®     */
   /* index       = Â‰¹AÐi Æá»a· ·¥•B¯a           */
   int total_num, make_num, wrong_num, index;
   /* made        = Â‰¹A µa¦Ÿi ´ia º…”a.                                   */
   /* ex) made[0] == 1 -> 0 ¤å¼ Æá»a“e ·¡ ‰A·±µA¬á ·¡£¡ Â‰¹A–A´ö”a“e ›u     */
   /* quiz_ptr    = Ñe¸ e·¡•¡µA ­¢Ða“e Æá»a—i·i ˆaŸ¡Ç¡“e Í¡·¥Èá· ¤µi     */
   /* ex) quiz_ptr[0]  -> Ñe¸ e·¡•¡µA ­¢Ða“e Àõ¤å¼ Æá»aŸi ˆaŸ¡Ç¡“e Í¡·¥Èá */
   /* wrong_ratio = ËiŸ¥ ® / Â‰¹A–E ® */
   char *made;
   quiz_struct **quiz_ptr;
   int wrong_ratio;
   clock_t time1, time2;
   char temp[ITEM_LENGTH+1];
   int key, answer, cur_flag, guess, i, j;

   /* Ñe¸ e·¡•¡µA ­¢Ða“e Æá»a· ®Ÿi total_numµA ¸á¸wÐe”a. */
   total_num = 0;
   for(i = 0; i < quiz_num; i++) if(quiz[i].level == cur_level) total_num++;
   if(total_num == 0)
   /* Ñe¸ e·¡•¡µA ­¢Ða“e Æá»aˆa Ðaa•¡ ´ô·a¡e ¤a¡ Ÿ¡ÈåÐe”a. */
   {
      put_msg("º…§¡–E Æá»aˆa Ðaa•¡ ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      return ESC;
   }

   /* made µÁ quiz_ptrµA Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”wÐe”a. */
   if((made     = malloc(total_num))              == NULL ||
      (quiz_ptr = malloc(sizeof(quiz)*total_num)) == NULL)
   {
      put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      if(made     != NULL) free(made);
      if(quiz_ptr != NULL) free(quiz_ptr);
      return ESC;
   }
   /* made “e ¡¡– 0·a¡ Á¡‹¡ÑÁÐe”a. */
   memset(made, 0, total_num);
   /* quiz_ptrµA Ñe¸ e·¡•¡µA ­¢Ða“e Æá»aŸi ˆaŸ¡Ç¡“e Í¡·¥ÈáŸi ¸á¸wÐe”a.*/
   for(total_num = 0, i = 0; i < quiz_num; i++)
      if(quiz[i].level == cur_level) quiz_ptr[total_num++] = &quiz[i];

   /* ¬a¶w¸a· Íe·Ÿi ¶áÐ <NumLock>·i On¯¡Ç¥”a. */
   cur_flag = get_keyflag_han();
   set_keyflag_han(cur_flag | NUM_LOCK);

   make_num = wrong_num = 0;
   while(1)
   {
      /* Â‰¹AÐi Æá»aŸi ¢¸b¶á¡ ©³–A, ·¡£¡ Â‰¹AÐe ˆõ·e ©³»¡ ´g“e”a. */
      /* MAX_RETRY¤å ¯¡•¡Ð•¡ ©³»¡ ¡µÐa¡e ®…Àa¸â·a¡ ˆñ¬‚Ðe”a.      */
      for(i = 0; i < MAX_RETRY; i++)
      {
         index = random(total_num);
         if(!made[index]) break;
      }
      if(i >= MAX_RETRY)
      /* MAX_RETRY¤å  eµA ©³»¡ ¡µÐ–·a¡e ®…Àa¸â·a¡ ˆñ¬‚Ðe”a. */
      {
         for(i = 0; i < total_num; i++) if(!made[i])
                                        {
                                           index = i;
                                           break;
                                        }
         /* ‹aœ•¡ ´ô·a¡e ”á ·¡¬w Â‰¹AÐi Æá»aˆa ´ô”a“e ¡A¯¡»¡Ÿi Â‰bÐe”a. */
         if(i >= total_num)
         {
            put_msg("Æá»aˆa ”á ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
            break;
         }
      }

      /* mix_item·¡ 1·¡¡e Ðw¡¢·i ¢¸b¶á¡ ¬ã“e”a. */
      if(mix_item)
      {
         answer = quiz_ptr[index]->answer;
         i = random(ITEM_PER_QUEST);
         j = random(ITEM_PER_QUEST);
         /* i¤å Ðw¡¢‰Á j¤å Ðw¡¢·i –á¤aŽ…”a. */
         if(i != j)
         {
            strcpy(temp,                     quiz_ptr[index]->item[i]);
            strcpy(quiz_ptr[index]->item[i], quiz_ptr[index]->item[j]);
            strcpy(quiz_ptr[index]->item[j], temp);
            if(i == answer)
               quiz_ptr[index]->answer = j;
            else if(j == answer)
               quiz_ptr[index]->answer = i;
         }
      }

      /* Â‰¹A–E Æá»a· Á· Â‰¹A ÒU®Ÿi 1 »wˆa¯¡Ç¥”a. ·¡  ÒU®µÁ  ËiŸ¥ */
      /* ÒU®“e ¸aža ÑÁ·©·i »¡¶»¡ ´g“e Ðe µw¶¥Ó¡ ’¸â–A“e•A <™©™©·¡>*/
      /* 1.0µA¬á“e ·¡¶w–A»¡ ´g“e”a.  ÒµA aµ© ¤á¸åµA¬á“e ·¡  ¸÷¥¡Ÿi */
      /* ¤aÈw·a¡ ¸aº ËiŸ¡“e ¢…¹A“e ¸aº Â‰¹AÐa“e ‹¡“w·i ý·i  ‰ÒB */
      /* ·¡”a.                                                       */
      (quiz_ptr[index]->make_num)++;

      make_num++;                       /* Â‰¹A–E Æá»a· ®Ÿi 1 »wˆa¯¡Ç¥”a.*/
      made[index] = 1;                  /* Â‰¹A–A´ö”a“e Îa¯¡Ÿi Ðe”a.       */
      disp_game_quiz(quiz_ptr[index]);  /* ¢…¹AŸi Îa¯¡Ðe”a.                */

      guess = -1;
      key   = 0;
      clear_key_buff();
      time1 = clock();
      /* ”s·i ¬åÈ‚Ða“e Ç¡Ÿi ’Ÿi ˜Œa»¡ ‰­¢ ¯¡ˆe·i ˆñ¬aÐaµa */
      /* ‰w‰Á–E ¯¡ˆe·i  b”¡ Îa¯¡Ðe”a.                      */
      do
      {
         while(!kbhit())
         {
            time2 = clock();
            if((time2 - time1) / CLK_TCK > limit_time)
            /* ‰w‰Á–E ¯¡ˆe·¡ limit_time·i Á¡‰ÁÐa¡e žÏaŸi ¨a¹a aµ¥”a. */
               break;
            else
            {
               /* ‰w‰Á–E ¯¡ˆeµA Ð”wÐa“e ‹©·¡·  b”Ÿi ‹aŸ¥”a. */
               i = (time2 - time1) / (float) (limit_time * CLK_TCK) * BAR_WIDTH;
               for(j = 0; j < BAR_HEIGHT; j++)
                  draw_horiz_line(BAR_X, BAR_Y+j, i, RED);
            }
         }
         if(kbhit())
         {
            if((key = getch_han()) == 0)
               getch_han();
            else if(key == _ESC)
            {
               key = ESC;
               break;
            }
            guess = key - '1';  /* guess = ¬a¶w¸aˆa ¬åÈ‚Ðe Ðw¡¢· ¤åÑ¡ */
         }
         else
            break;
      }
      while(guess < 0 || guess > ITEM_PER_QUEST - 1);

      if(key == ESC) break;
      if(guess == quiz_ptr[index]->answer)
      /*  x“e Ðw¡¢·i ¬åÈ‚Ð–”a¡e */
      {
         /*  xÓ¥ ®µA Ð”wÐa“e ‹©·¡·  b”Ÿi ‹aŸ¥”a. */
         i = (make_num - wrong_num) / (float) QUIZ_PER_LEVEL * BAR_WIDTH;
         for(j = 0; j < BAR_HEIGHT; j++)
            draw_horiz_line(BAR_X, BAR_Y+16+j, i, RED);
         play2("o4l16ceg");
      }
      else
      /* ËiŸ¥ Ðw¡¢·i ¬åÈ‚Ð–”a¡e */
      {
         /* Â‰¹A–E Æá»a· Á· ËiŸ¥ ÒU®Ÿi 1 »wˆa¯¡Ç¥”a. */
         (quiz_ptr[index]->wrong_num)++;
         /* ËiŸ¥ Æá»a· ®Ÿi 1 »wˆa¯¡Ç¥”a. */
         wrong_num++;
         /* ËiŸ¥ ®µA Ð”wÐa“e ‹©·¡·  b”Ÿi ‹aŸ¥”a. */
         i = wrong_num / (float) QUIZ_PER_LEVEL * BAR_WIDTH;
         for(j = 0; j < BAR_HEIGHT; j++)
            draw_horiz_line(BAR_X, BAR_Y+32+j, i, RED);
         play2("o2l16eeee");
      }

      /*  a»¡ b ¢…¹Aˆa Â‰¹A–A´ö”a¡e žÏaŸi ¨a¹a aˆe”a. */
      if(make_num >= QUIZ_PER_LEVEL) break;
   }

   /* ¶¥œ· Ç¡¥¡—a Ïiœ‹a¡ ¥¢Š¯¡Ç¥”a. */
   set_keyflag_han(cur_flag);

   if(key != ESC)
   {
      /* ‰A·± ‰i‰ÁŸi Â‰bÐe”a. */
      draw_3dbox(31, 10, 49, 16, LIGHTGRAY);
      set_output_offset_han(12);
      vputs_han(38, 10, "‰i‰Á", LIGHTGREEN);
      draw_horiz_line(31*8+5, 12*16-2, (49-31)*8-3, BLUE);
      set_output_offset_han(0);
      xputs_han(33, 12, "Æá»a ® :    ˆ");
      xputs_han(33, 13, " xÓ¥ ® :    ˆ");
      xputs_han(33, 14, "ËiŸ¥ ® :    ˆ");
      vprintf_han(43, 12, RED, "%2d", make_num);
      vprintf_han(43, 13, RED, "%2d", make_num-wrong_num);
      vprintf_han(43, 14, RED, "%2d", wrong_num);
      draw_horiz_line(31*8+5, 15*16+2, (49-31)*8-3, BLUE);
      set_output_offset_han(4);
      wrong_ratio = (float) wrong_num / make_num * 100.;
      if(wrong_ratio == 0)
         vputs_han(32, 15, "””eÓ¡ Ò‰ŸWÐs“¡”a", RED);
      else if(wrong_ratio > 20)
         vputs_han(34, 15, "¹½»¡ ´g¯s“¡”a",     RED);
      else
         vputs_han(34, 15, "¸i Ða­v¯s“¡”a",     RED);
      set_output_offset_han(0);
      if(!getch_han()) getch_han();
      vclear_text_han(31, 10, 49, 16, BLUE);
      if(wrong_ratio > 20)
      /* 20 % Ÿi Á¡‰ÁÐaµa ËiŸ¡¡e e·¡•¡Ÿi 1 ˆq­¡¯¡Ç¥”a. */
        key = DOWN;
      else
      /* 80 % ·¡¬w·i  xÓ¡¡e e·¡•¡Ÿi 1 »wˆa¯¡Ç¥”a. */
        key = UP;
   }

   /* madeµÁ quiz_ptrµA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá¯¡Ç¥”a. */
   free(made);
   free(quiz_ptr);
   return(key);
}

void game(void)
/* Ñe¸ ¬åÈ‚–E ·AÑw· ‰A·±·i Ðe”a. */
{
   char *(menu_str[LEVEL_NUM]) =
   {
      " ¹A 1 ®º… ", " ¹A 2 ®º… ", " ¹A 3 ®º… ", " ¹A 4 ®º… ", " ¹A 5 ®º… "
   };
   char *screen, string[20];
   int  result, i;

   /* e·¡•¡Ÿi ¬åÈ‚¤h“e”a. */
   if(my_popup_han(32, 10, menu_str, LEVEL_NUM, &cur_level) == ESC) return;
   /* Ñe¸ ¬åÈ‚–E ·AÑw· Æá»aŸi ·ª“e”a */
   if(!read_quiz(cur_type)) return;

   while(1)
   {
      /* ‰A·± ÑÁ¡e·i Š¬÷Ðe”a. */
      disp_type_level();
      draw_game_screen();

      /* Ñe¸ e·¡•¡· ‰A·±·i Ðe”a. */
      sprintf(string, "¹A %1d ®º…", cur_level+1);
      put_msg(string, 1);
      if((result = one_level_game()) == ESC) break;
      if(result == UP)
      /* ‰A·±·i É·‰ÁÐ–·i ˜“e e·¡•¡Ÿi 1 »wˆa¯¡Ç¥”a. */
      {
         vclrscr_han(BLUE);
         if(cur_level == LEVEL_NUM - 1)
         {
            put_msg("¡¡—e ‰Á¸÷·i ”a É·‰ÁÐa­v¯s“¡”a. Â‚ÐaÐs“¡”a.", -1);
            break;
         }
         else
            cur_level++;
      }
      else if(result == DOWN)
      /* ‰A·±·i É·‰ÁÐa»¡ ¡µÐ–·i ˜“e e·¡•¡Ÿi 1 ˆq­¡¯¡Ç¥”a. */
      {
         if(cur_level != 0) cur_level--;
      }
   }

   /* quizµA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá¯¡Ç¥”a. */
   if(quiz_num > 0) free(quiz);
}

void append_file(void)
/* ”aŸe Æá»a ÑÁ·©·i ·ª´á¬á Ñe¸ ¬åÈ‚–E ·AÑwµA ÂˆaÐe”a. */
{
   FILE *stream1, *stream2;
   char source_file[60], dest_file[20], string[100], header[20], result;
   long f_length;

   /* —¡BÉ¡Ÿ¡Ÿi ¥¡µa º… Ò, ·ª·i ÑÁ·©·i ¬åÈ‚¤h“e”a. */
   if(dir_box_short((80-58)/2, 7, "*.dta", source_file) == ESC) return;
   sprintf(string, "%s ÑÁ·©·i ·ª´á —i·¡‰V¯s“¡Œa?", source_file);
   if(my_get_yesno(11, string) != YES) return;

   /* ¬a¶w¸aˆa ¬åÈ‚Ðe ÑÁ·©·i read mode¡ µ¡ÏeÐa‰¡ Ñe¸ ¬åÈ‚–E ·AÑwµA */
   /* ˜aœa quiz00.dta, quiz01.dta,        Ÿi append mode¡ µ¡ÏeÐe”a. */
   sprintf(dest_file, "quiz%02d.dta", cur_type);
   if((stream1 = fopen(source_file, "rb")) == NULL ||
      (stream2 = fopen(dest_file,   "ab")) == NULL ||
      fread(header, sizeof(FILE_HEADER), 1, stream1) != 1 ||
      strcmp(header, FILE_HEADER))
   {
      if(stream1 != NULL) fclose(stream1);
      if(stream2 != NULL) fclose(stream2);
      put_msg("Æá»a ÑÁ·©·i ·ª·i ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
      return;
   }

   /* f_length“e ÑÁ·© Ça‹¡µA¬á ÑA”á Ça‹¡Ÿi ¨… ˆt */
   f_length = filelength(fileno(stream1)) - sizeof(FILE_HEADER);
   if(f_length != 0)
   {
      /* source_fileµA¬á Æá»aŸi ·ª´á dest_fileµA ÂˆaÐe”a. */
      if((quiz = (quiz_struct *) malloc(f_length)) == NULL)
      {
         put_msg("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
         result = 0;
      }
      else
      {
         result = fread(quiz, f_length, 1, stream1) == 1;
         if(result == 1) result = fwrite(quiz, f_length, 1, stream2) == 1;
         free(quiz);
      }
   }

   fclose(stream1);
   fclose(stream2);
   if(result)
   {
      play("o4l16go5c");
      put_msg("ÂˆaÐ–¯s“¡”a", 2);
   }
   else
      put_msg("ÂˆaÐi ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
}

void to_dos(void)
/* •¡¯a ®É·i Ðe”a. */
{
   int graph_mode, result;

   graph_mode = getgraphmode(); /* Ñe¸· ‹aœÏ¢ ¡¡—aŸi ¸á¸wÐe”a.   */
   restorecrtmode();            /* ÉB¯aËa ¡¡—a¡ •©a ‘½“e”a.       */
   puts("Type EXIT to return to Toltori...");
   result = system("");         /* •¡¯a¡ ¨a¹a aˆe”a.              */
   setgraphmode(graph_mode);    /* ¶¥œ· ‹aœÏ¢ ¡¡—a¡ •©a ‘½“e”a.*/
   if(result == -1) put_msg("•¡¯a¡ aˆi ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.", -1);
   disp_status();
}

void ask_quit(void)
/* Ïa¡‹aœ‘· ¯©Ð—·i ¹·žaÐe”a. */
{
   if(my_get_yesno(11, "     {¯¡‰V¯s“¡Œa?     ") == YES)
   {
      close_han();
      exit(1);
   }
}
