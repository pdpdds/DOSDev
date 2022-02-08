/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          getch2_han(), getch3_han()                          */
/*   FILENAME           getch23.c                                           */
/*   DATE & TIME        03/11/92(WED) 12:25                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

typedef unsigned char mini;

/****************************************************************************/
/*                 Tables(to be referenced by getch?_han()                  */
/****************************************************************************/

/******************** tables for 2 BUL style keyboard ***********************/

/* 1. HANGUL code for each ASCII                          */
/*    1) a code less than 100 means that it is a JAEUM    */
/*    2) a code greater than 100 means that it is a MOEUM */
/*    3) real MOEUM code = value of key_table - 100       */

static const mini key_table1[26] =              /* not shifted */
{
  /* A    B    C    D    E    F    G    H    I    J    K    L    M */
     8, 126,  16,  13,   5,   7,  20, 113, 105, 107, 103, 129, 127,
  /* N    O    P    Q    R    S    T    U    V    W    X    Y    Z */
   120, 104, 110,   9,   2,   4,  11, 111,  19,  14,  18, 119,  17
};

static const mini key_table2[26] =              /* shifted */
{
  /* A    B    C    D    E    F    G    H    I    J    K    L    M */
     8, 126,  16,  13,   6,   7,  20, 113, 105, 107, 103, 129, 127,
  /* N    O    P    Q    R    S    T    U    V    W    X    Y    Z */
   120, 106, 112,  10,   3,   4,  12, 111,  19,  15,  18, 119,  17
};

/* 2. CHOSUNG code for JONGSUNG                        */
/*    CHOSUNG code converted from JONGSUNG by JUNGSUNG */
/*    e.q. Ðe + „a ==> Ðaa                            */
/*    1) code greated than 100 means that it should be */
/*       devided into one JONGSUNG and one CHOSUNG     */
/*       e.q. j + „á + ¬A ==> iˆõ                    */
/*    2) code division occurs referring jung_table2    */

static const mini cho_table[30] =
{
 /*           ˆA   ŒA   „D   A   „F   „G   ”A   œA   „J   „K   „L   „M   „N */
     0,   1,   2,   3, 100,   4, 101, 102,   5,   7, 103, 104, 105, 106, 107,
 /* „O   „P    A        ¤A   „T   ¬A   °A   ´A   ¸A   ÀA   ÄA   ÈA   ÌA   ÐA */
   108, 109,   8,   1,   9, 110,  11,  12,  13,  14,  16,  17,  18,  19,  20
};

/* 3. HANGUL code for double MOEUM(both for 2/3 BUL style) */
/*    e.q.  µ¡ + „a = µÁ                                   */

static const mini jung_table[7][3] =
{
 /*  …¡  „a  …Á      …¡  „  …á      …¡  ‡¡  †A */
   { 13,  3, 14 }, { 13,  4, 15 }, { 13, 29, 18 },
 /*  †  „á  †¡      †  …A  †Á      †  ‡¡  †á */
   { 20,  7, 21 }, { 20, 10, 22 }, { 20, 29, 23 },
 /*  ‡a  ‡¡  ‡ */
   { 27, 29, 28 }
};

/* 4. JONGSUNG code for CHOSUNG                        */
/*    JAEUM code in key_table1(2) is only for CHOSUNG  */
/*    and if you want a JONGSUNG code you should refer */
/*    this table                                       */
/*    e.q. When 'A' (' A') is pressed..                */
/*         By key_table1 CHOSUNG code is got to be 8,  */
/*         and JONGSUNG for that code is '”A'.         */
/*         If you want the real JONGSUNG code for ' A' */
/*         you should refer jong_table1[8] and it con- */
/*         tains 17. JONGSUNG for code 17 is ' A'      */

static const mini jong_table1[21] =
{
 /*         ˆA  ŒA  A  ”A  ˜A  œA   A  ¤A  ¨A */
     0,  1,  2,  3,  5,  8,  1,  9, 17, 19,  1,
 /* ¬A  °A  ´A  ¸A  ¼A  ÀA  ÄA  ÈA  ÌA  ÐA */
    21, 22, 23, 24,  1, 25, 26, 27, 28, 29
};

/* 5. HANGUL code for double JAEUM(only for 2 BUL style) */
/*    e.q.  ˆA¡+ ˆA = ŒA                                 */
/*    1) first byte is JONGSUNG code                     */
/*    2) second byte is CHOSUNG code                     */
/*    3) resulting third byte is JONGSUNG code           */

static const mini jong_table2[11][3] =
{
 /*  ˆA  ¬A  „D */
   {  2, 11,  4 },
 /*  A  ¸A  „F      A  ÐA  „G */
   {  5, 14,  6 }, {  5, 20,  7 },
 /*  œA  ˆA  „J      œA   A  „K      œA  ¤A  „L      œA  ¬A  „M */
   {  9,  2, 10 }, {  9,  8, 11 }, {  9,  9, 12 }, {  9, 11, 13 },
 /*  œA  ÈA  „N      œA  ÌA  „O      œA  ÐA  „P */
   {  9, 18, 14 }, {  9, 19, 15 }, {  9, 20, 16 },
 /*  ¤A  ¬A  „T */
   { 19, 11, 20 }
};

/******************** tables for 3 BUL style keyboard ***********************/

/* 1. HANGUL code for each ASCII                          */
/*    1) a code less than 100 means that it is a JAEUM    */
/*    2) a code greater than 100 means that it is a MOEUM */
/*    3) real MOEUM code = value of key_table - 100       */

static const mini key3_table1[39] =             /* not shifted */
{
  /* A    B    C    D    E    F    G    H    I    J    K    L    M */
    73, 120, 110, 129, 111, 103, 127,   4,   8,  13,   2,  14,  20,
  /* N    O    P    Q    R    S    T    U    V    W    X    Y    Z */
    11,  16,  19,  71, 104,  55, 107,   5, 113,  59,  52,   7,  67,
  /* 0    1    2    3    4    5    6    7    8    9    ;    '    / */
    17,  79,  72,  69, 119, 126, 105, 112, 128, 120,   9,  18, 113
};

static const mini key3_table2[39] =             /* shifted */
{
  /* A    B    C    D    E    F    G    H    I    J    K    L    M */
    58, 120,  61,  60,  76,  53, 127,   4,   8,  13,   2,  14,  20,
  /* N    O    P    Q    R    S    T    U    V    W    X    Y    Z */
    11,  16,  19,  78, 106,  57, 107,   5,  66,  77,  70,   7,  75,
  /* 0    1    2    3    4    5    6    7    8    9    ;    '    / */
    17,  74,  72,  69, 119, 126, 105, 112, 128, 120,   9,  18, 113
};

/* 2. HANGUL code for double JAEUM(only for 3 BUL style) */
/*    e.q.  ˆA¡+ ˆA = ŒA                                 */
/*    1) three bytes are all JONGSUNG code unlike        */
/*       jong_table2.                                    */

static const mini key3_jong_table2[11][3] =
{
 /*  ˆA  ¬A  „D */
   {  2, 21,  4 },
 /*  A  ¸A  „F      A  ÐA  „G */
   {  5, 24,  6 }, {  5, 29,  7 },
 /*  œA  ˆA  „J      œA   A  „K      œA  ¤A  „L      œA  ¬A  „M */
   {  9,  2, 10 }, {  9, 17, 11 }, {  9, 19, 12 }, {  9, 21, 13 },
 /*  œA  ÈA  „N      œA  ÌA  „O      œA  ÐA  „P */
   {  9, 27, 14 }, {  9, 28, 15 }, {  9, 29, 16 },
 /*  ¤A  ¬A  „T */
   { 19, 21, 20 }
};

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

han_stack_struct han_stack;                     /* HANGUL stack         */
union hangul prv_han = { NO_HAN_STR },          /* previous HANGUL code */
	     cur_han = { NO_HAN_CODE };         /* current HANGUL code  */
mini no_more_han;       /* no more characters can be accepted in gets_han() */
mini hcode1, hcode2, hcode3;

void (*han_change_func) (void) = (void (*) (void)) default_han_change_func;
/* will be called when hangul/english mode is toggled */
void (*ins_change_func) (void) = (void (*) (void)) default_ins_change_func;
/* will be called when insert/delete mode is toggled */
void (*han_wait_func) (void) = (void (*) (void)) NULL;
/* will be called when the CPU waits for the user's input */

/********************** Variables about hangul input ************************/

char han_mode;             /* 1, if current input mode is hangul mode   */
char ins_mode;             /* 1, if current input mode is insert mode   */
char bul3_mode;            /* 1, if current keyboard type is 3 BQL type */

char able_to_change_han = 1;   /* 1, if user can change input mode          */
mini han_change_code = _SPACE; /* ascii code of hangul/english changing key */
mini han_change_flag = LEFT_SHIFT;/* key flag of hangul/english changing key*/
char is_han_change_extend;     /* 1, if han_change_code is an extended code */

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void temp_make_han(union hangul *prv, union hangul *cur)
{
   push_han(prv->code);
   prv->bytes[0] = cur->bytes[1];
   prv->bytes[1] = cur->bytes[0];
}

void make_han(union hangul *prv, union hangul *cur, mini *ret_han)
{
   han_stack.top = 0;
   push_han(NO_HAN_STR);
   ret_han[0] = prv->bytes[0];
   ret_han[1] = prv->bytes[1];
   ret_han[2] = 0;
   prv->bytes[0] = cur->bytes[1];
   prv->bytes[1] = cur->bytes[0];
}

void push_han(unsigned code)
{
   if(han_stack.top == MAX_HAN_STACK) return;
   han_stack.element[(han_stack.top)++] = code;
}

void pop_han(unsigned *code)
{
   if(han_stack.top == 0) return;
   *code = han_stack.element[--(han_stack.top)];
}

int getch2_han(char *ret_str, char *exit_key, int edit_cursor)
{
   clock_t time1, time2;
   mini index, ascii_code, scan_code, hcode4, hcode, flag;
   int  ret_code, i;

   ret_str[0] = 0;
   ret_code   = NOT_COMPLETE;

   if(no_more_han)
   {
      han_stack.top = 0;
      prv_han.code  = NO_HAN_STR;
      cur_han.code  = NO_HAN_CODE;
   }

   if(prv_han.code != NO_HAN_STR)
   /* when it is combinating hangul, the cursor should be hangul cursor */
   /* unconditionally.                                                  */
   /* Otherwise follow the instruction by checking <edit_cursor>        */
   /* This is for moving cursor through string in adv_gets_han()        */
      cursor_han = han_cursor;
   else if(edit_cursor == 0)
      cursor_han = ((prv_han.code == NO_HAN_STR) ? eng_cursor : han_cursor);
   else if(edit_cursor == 1)
      cursor_han = han_cursor;
   else if(edit_cursor == 2)
      cursor_han = eng_cursor;
   draw_cursor_han(curx_han,cury_han);

   time1 = clock();
   while(!kbhit())
      if(((time2 = clock()) - time1) > cursor_twinkle_time)
      {
	 draw_cursor_han(curx_han, cury_han);
         if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
         time1 = time2;
      }
   if(exist_cursor) draw_cursor_han(curx_han, cury_han);  /* hide cursor */
   flag = peekb(0, 0x0417);
   if((ascii_code = getch()) == 0)
      scan_code = getch();
   else
      scan_code = 0;

   if((scan_code  == han_change_code &&  is_han_change_extend) ||
      (ascii_code == han_change_code && !is_han_change_extend))
   {
      if(han_change_flag == 0 || (flag & han_change_flag))
      {
         if(able_to_change_han)
         {
	    han_mode = !han_mode;
	    ret_code = HAN_CHANGED;
	    if(han_change_func != (void (*) (void)) NULL) han_change_func();
	 }
	 else
	    return(IGNORE);
      }
   }
   else if(ascii_code == _BS)
      return(BS_PRESSED);

   if(han_mode && ((ascii_code >= 'A' && ascii_code <= 'Z') ||
                   (ascii_code >= 'a' && ascii_code <= 'z')))
   {
      if(ascii_code >= 0x61)
         index = ascii_code - 0x61;
      else
         index = ascii_code - 0x41;
      if(flag & 0x03)
         hcode = key_table2[index];
      else
         hcode = key_table1[index];

      if(hcode < 100)
      {
	 if(cur_han.bit.jung == NO_JUNG)        /* if chosung */
	 {
	    if(cur_han.bit.cho != NO_CHO)
	    {
               cur_han.bit.cho  = hcode;
	       cur_han.bit.jong = NO_JONG;
               make_han(&prv_han, &cur_han, ret_str);
               ret_code = COMPLETE;
            }
            else
            {
               cur_han.bit.cho  = hcode;
               cur_han.bit.jong = NO_JONG;
	       temp_make_han(&prv_han, &cur_han);
            }
         }                                      /* end of chosung */
	 else                                   /* if jongsung */
         {
            if(cur_han.bit.jong == NO_JONG)
            {
               cur_han.bit.jong = jong_table1[hcode];
               if(cur_han.bit.jong == NO_JONG)
               {
		  cur_han.bit.cho  = hcode;
                  cur_han.bit.jung = NO_JUNG;
		  make_han(&prv_han, &cur_han, ret_str);
                  ret_code = COMPLETE;
               }
               else
                  temp_make_han(&prv_han, &cur_han);
            }
            else
            {
	       i = 0;
               while(i <= 10 && (jong_table2[i][0] != cur_han.bit.jong ||
                                 jong_table2[i][1] != hcode) ) i++;
	       if(i <= 10)
               {
                  cur_han.bit.jong = jong_table2[i][2];
                  temp_make_han(&prv_han, &cur_han);
               }
               else
               {
                  cur_han.bit.cho  = hcode;
                  cur_han.bit.jung = NO_JUNG;
		  cur_han.bit.jong = NO_JONG;
                  make_han(&prv_han, &cur_han, ret_str);
                  ret_code = COMPLETE;
               }
            }
         }                                      /* end of jongsung */
      }
      else                                      /* if jungsung */
      {
         hcode -= 100;
         if(cur_han.bit.cho == NO_CHO && cur_han.bit.jung == NO_JUNG)
	 {
            cur_han.bit.jung = hcode;
            cur_han.bit.jong = NO_JONG;
            temp_make_han(&prv_han, &cur_han);
         }
         else if(cur_han.bit.jong == NO_JONG)
         {
            if(cur_han.bit.jung == NO_JUNG)
            {
	       cur_han.bit.jung = hcode;
               temp_make_han(&prv_han, &cur_han);
            }
            else
            {
               i = 0;
               while(i <= 6 && (jung_table[i][0] != cur_han.bit.jung ||
                                jung_table[i][1] != hcode) ) i++;
	       if(i <= 6)
               {
                  cur_han.bit.jung = jung_table[i][2];
		  temp_make_han(&prv_han, &cur_han);
               }
               else
               {
                  cur_han.bit.cho  = NO_CHO;
                  cur_han.bit.jung = hcode;
                  cur_han.bit.jong = NO_JONG;
                  make_han(&prv_han, &cur_han, ret_str);
                  ret_code = COMPLETE;
	       }
            }
         }
         else
         {
            hcode4 = cho_table[cur_han.bit.jong];
            cur_han.bit.jong = NO_JONG;
            if(hcode4 >= 100)
	    {
               hcode4 -= 100;
               cur_han.bit.jong = jong_table2[hcode4][0];
	       hcode4 = jong_table2[hcode4][1];
            }
            ret_str[0] = cur_han.bytes[1];
            ret_str[1] = cur_han.bytes[0];
            ret_str[2] = 0;
            ret_code   = COMPLETE;
            prv_han.bytes[0] = 0x80 | (hcode4 << 2);
            prv_han.bytes[1] = 'A';
            han_stack.top = 0;
	    push_han(NO_HAN_STR);
            cur_han.bit.cho  = hcode4;
            cur_han.bit.jung = hcode;
            cur_han.bit.jong = NO_JONG;
            temp_make_han(&prv_han, &cur_han);
         }
      }                                         /* end of jungsung */
   }
   else                                         /* if ASCII */
   {
      if(!ascii_code)
      {
	 for(i = 0; i < strlen(exit_key); i++)
	    if(scan_code == exit_key[i]) break;
	 if(i < strlen(exit_key))
	    ret_code = i;
	 else
	    return(IGNORE);
      }
      else if(ascii_code & 0x80)
	 return(IGNORE);

      han_stack.top = 0;
      cur_han.code  = NO_HAN_CODE;
      i = 0;
      if(prv_han.code != NO_HAN_STR)
      {
         ret_str[i++] = prv_han.bytes[0];
         ret_str[i++] = prv_han.bytes[1];
         ret_str[i]   = 0;
      }
      prv_han.code = NO_HAN_STR;

      switch(ascii_code)
      {
	 case 0      : break;
	 case _CTRLQ : return(CTRLQ_PRESSED);
	 case _CTRLT : return(CTRLT_PRESSED);
	 case _CTRLY : return(CTRLY_PRESSED);
	 case _TAB   : return(TAB_PRESSED);
	 case _ESC   : ret_code = ESC_PRESSED;
                       break;
         case _ENTER : ret_code = ENTER_PRESSED;
                       break;
	 case _SPACE : if(ret_code == HAN_CHANGED) break;
         default     : ret_str[i++] = ascii_code;
                       ret_str[i++] = 0;
                       ret_code = COMPLETE;
                       break;
      }
   }                                            /* end of ASCII */

   return(ret_code);
}

int getch3_han(char *ret_str, char *exit_key, int edit_cursor)
{
   clock_t time1, time2;
   mini index, ascii_code, scan_code, hcode, flag;
   int  ret_code, key_code, i;

   ret_str[0] = 0;
   ret_code  = NOT_COMPLETE;

   if(no_more_han)
   {
      han_stack.top = 0;
      prv_han.code  = NO_HAN_STR;
      cur_han.code  = NO_HAN_CODE;
   }

   if(prv_han.code != NO_HAN_STR)
   /* when it is combinating hangul, the cursor should be hangul cursor */
   /* unconditionally.                                                  */
   /* Otherwise follow the instruction by checking <edit_cursor>        */
   /* This is for moving cursor through string in adv_gets_han()        */
      cursor_han = han_cursor;
   else if(edit_cursor == 0)
      cursor_han = ((prv_han.code == NO_HAN_STR) ? eng_cursor : han_cursor);
   else if(edit_cursor == 1)
      cursor_han = han_cursor;
   else if(edit_cursor == 2)
      cursor_han = eng_cursor;
   draw_cursor_han(curx_han, cury_han);

   time1 = clock();
   while(!kbhit())
      if(((time2 = clock()) - time1) > cursor_twinkle_time)
      {
	 draw_cursor_han(curx_han, cury_han);
	 if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
	 time1 = time2;
      }
   if(exist_cursor) draw_cursor_han(curx_han, cury_han);  /* hide cursor */
   flag       = peekb(0, 0x0417);
   key_code   = bioskey(0);
   ascii_code = key_code & 0xff;
   scan_code  = key_code >> 8;

   if((scan_code  == han_change_code &&  is_han_change_extend) ||
      (ascii_code == han_change_code && !is_han_change_extend))
   {
      if(han_change_flag == 0 || (flag & han_change_flag))
      {
	 if(able_to_change_han)
	 {
	    han_mode = !han_mode;
	    ret_code = HAN_CHANGED;
	    if(han_change_func != (void (*) (void)) NULL) han_change_func();
	 }
	 else
	    return(IGNORE);
      }
   }
   else if(ascii_code == _BS)
      return(BS_PRESSED);

   i = 1;                     /* i == 1 means that a HANGUL key is struck */
   if(han_mode)
   {
      if(ascii_code >= 'a' && ascii_code <= 'z') ascii_code -= 0x20;
      if(ascii_code >= 'A' && ascii_code <= 'Z')
	 index = ascii_code - 0x41;
      else if(ascii_code >= '0' && ascii_code <= '9' && scan_code >= 2 && scan_code <= 11)
	 index = ascii_code - 22;
      else if(ascii_code == ';')
	 index = 36;
      else if(ascii_code == '\'')
	 index = 37;
      else if(ascii_code == '/')
	 index = 38;
      else if(ascii_code == '!')
	 index = 27;
      else
	 i = 0;

      if(flag & (LEFT_SHIFT | RIGHT_SHIFT))
      {
	 switch(ascii_code)
	 {
	    case 'T' : ascii_code = ';';  i = 0; break;
	    case 'Y' : ascii_code = '<';  i = 0; break;
	    case 'U' : ascii_code = '7';  i = 0; break;
	    case 'I' : ascii_code = '8';  i = 0; break;
	    case 'O' : ascii_code = '9';  i = 0; break;
	    case 'P' : ascii_code = '>';  i = 0; break;
	    case 'G' : ascii_code = '/';  i = 0; break;
	    case 'H' : ascii_code = '\''; i = 0; break;
	    case 'J' : ascii_code = '4';  i = 0; break;
	    case 'K' : ascii_code = '5';  i = 0; break;
	    case 'L' : ascii_code = '6';  i = 0; break;
	    case 'B' : ascii_code = '!';  i = 0; break;
	    case 'N' : ascii_code = '0';  i = 0; break;
	    case 'M' : ascii_code = '1';  i = 0; break;
	    case '<' : ascii_code = '2';  i = 0; break;
	    case '>' : ascii_code = '3';  i = 0; break;
	 }
      }
   }
   else
      i = 0;

   if(i)                                        /* if hangul */
   {
      if(flag & (LEFT_SHIFT | RIGHT_SHIFT))
	 hcode = key3_table2[index];
      else
	 hcode = key3_table1[index];
      if(hcode < 50)
      /* if chosung is struck */
      {
	 if(cur_han.code == NO_HAN_CODE)
         {
            cur_han.bit.cho = hcode;
            temp_make_han(&prv_han, &cur_han);
         }
         else
         {
            if(cur_han.bit.jung == NO_JUNG && cur_han.bit.jong == NO_JONG &&
               cur_han.bit.cho == hcode)
            /* check for double chosung */
            {
               if(cur_han.bit.cho ==  2 || cur_han.bit.cho ==  5 ||
                  cur_han.bit.cho ==  9 || cur_han.bit.cho == 11 ||
                  cur_han.bit.cho == 14)
               {
                  cur_han.bit.cho = hcode + 1;
                  temp_make_han(&prv_han, &cur_han);
               }
               else
               {
                  cur_han.bit.cho  = hcode;
                  cur_han.bit.jung = NO_JUNG;
                  cur_han.bit.jong = NO_JONG;
                  make_han(&prv_han, &cur_han, ret_str);
                  ret_code = COMPLETE;
               }
            }
            else
            {
               cur_han.code    = NO_HAN_CODE;
               cur_han.bit.cho = hcode;
               make_han(&prv_han, &cur_han, ret_str);
               ret_code = COMPLETE;
            }
         }
      }
      else if(hcode < 100)
      /* if jongsung is struck */
      {
         hcode -= 50;
         if(cur_han.bit.jung == NO_JUNG && cur_han.bit.cho != NO_CHO)
         {
            cur_han.bit.cho  = NO_CHO;
            cur_han.bit.jung = NO_JUNG;
            cur_han.bit.jong = hcode;
            make_han(&prv_han, &cur_han, ret_str);
            ret_code = COMPLETE;
         }
         else if(cur_han.bit.jong == NO_JONG)
         {
            cur_han.bit.jong = hcode;
            temp_make_han(&prv_han, &cur_han);
         }
         else
         {
            i = 0;
            while(i <= 10 && (key3_jong_table2[i][0] != cur_han.bit.jong ||
                              key3_jong_table2[i][1] != hcode)) i++;
            if(i <= 10)
            {
               cur_han.bit.jong = key3_jong_table2[i][2];
               temp_make_han(&prv_han, &cur_han);
            }
            else
            {
               cur_han.bit.cho  = NO_CHO;
               cur_han.bit.jung = NO_JUNG;
               cur_han.bit.jong = hcode;
               make_han(&prv_han, &cur_han, ret_str);
               ret_code = COMPLETE;
            }
         }
      }
      else
      /* if jungsung is struck */
      {
         hcode -= 100;
         if(cur_han.bit.jong != NO_JONG)
         {
            cur_han.code     = NO_HAN_CODE;
            cur_han.bit.jung = hcode;
            make_han(&prv_han, &cur_han, ret_str);
            ret_code = COMPLETE;
         }
         else if(cur_han.bit.jung == NO_JUNG)
         {
            cur_han.bit.jung = hcode;
            temp_make_han(&prv_han, &cur_han);
         }
         else
	 {
	    i = 0;
	    while(i <= 6 && (jung_table[i][0] != cur_han.bit.jung ||
                             jung_table[i][1] != hcode) ) i++;
            if(i <= 6)
            {
               cur_han.bit.jung = jung_table[i][2];
               temp_make_han(&prv_han, &cur_han);
            }
            else
            {
               cur_han.code     = NO_HAN_CODE;
               cur_han.bit.jung = hcode;
               make_han(&prv_han, &cur_han, ret_str);
               ret_code = COMPLETE;
            }
	 }
      }
   }
   else                         /* if ASCII */
   {
      if(!ascii_code)
      {
	 for(i = 0; i < strlen(exit_key); i++)
	    if(scan_code == exit_key[i]) break;
	 if(i < strlen(exit_key))
	    ret_code = i;
	 else
	    return(IGNORE);
      }
      else if(ascii_code & 0x80)
	 return(IGNORE);

      han_stack.top = 0;
      cur_han.code  = NO_HAN_CODE;
      i             = 0;
      if(prv_han.code != NO_HAN_STR)
      {
         ret_str[i++] = prv_han.bytes[0];
         ret_str[i++] = prv_han.bytes[1];
         ret_str[i]   = 0;
      }
      prv_han.code = NO_HAN_STR;

      switch(ascii_code)
      {
	 case 0      : break;
	 case _CTRLQ : return(CTRLQ_PRESSED);
	 case _CTRLT : return(CTRLT_PRESSED);
	 case _CTRLY : return(CTRLY_PRESSED);
	 case _TAB   : return(TAB_PRESSED);
	 case _ESC   : ret_code = ESC_PRESSED;
                       break;
         case _ENTER : ret_code = ENTER_PRESSED;
                       break;
	 case _SPACE : if(ret_code == HAN_CHANGED) break;
	 default     : ret_str[i++] = ascii_code;
                       ret_str[i++] = 0;
                       ret_code = COMPLETE;
                       break;
      }
   }

   return(ret_code);
}
