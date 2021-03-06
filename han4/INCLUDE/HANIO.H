/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          header file - basic functions(50)                   */
/*   FILENAME           hanio.h                                             */
/*   DATE & TIME        06/29/92 (MON) 13:20                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* �a�� ���A�� �A���� ���I�� ���a���A�e �A�� �A�����i �b�e ҁ */
/* ���a���i ���e�a���� �e�a.                                    */

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/* init_han() �� ����t */
enum
{
   OK_HAN,
   MEMORY_NOT_ENOUGH,
   HAN_FONT_NOT_FOUND,
   ENG_FONT_NOT_FOUND,
   SPC_FONT_NOT_FOUND,
   HAN_FONT_READ_ERROR,
   ENG_FONT_READ_ERROR,
   SPC_FONT_READ_ERROR,
   INVALID_GR_CARD,
   INITGRAPH_ERROR,
   ETC_ERROR
};

/* ���� */
#define FONT_NORMAL             0x00
#define FONT_BOLD               0x01
#define FONT_OUTLINE            0x02
#define FONT_SHADOW             0x04
#define FONT_SHADE              0x08
#define FONT_UNDERLINE          0x10
#define FONT_INVERSE            0x20
#define FONT_VERT_DBL           0x40
#define FONT_HORIZ_DBL          0x80

/* �� */
#if     !defined(__COLORS)
#define __COLORS
enum
{
   BLACK,               /* �ᖁ�� �� */
   BLUE,
   GREEN,
   CYAN,
   RED,
   MAGENTA,
   BROWN,
   LIGHTGRAY,
   DARKGRAY,            /* �j�e �� */
   LIGHTBLUE,
   LIGHTGREEN,
   LIGHTCYAN,
   LIGHTRED,
   LIGHTMAGENTA,
   YELLOW,
   WHITE
};
#endif

/* �b�a ���� */
enum
{
   BOX_H1V1,
   BOX_H1V2,
   BOX_H2V1,
   BOX_H2V2,
   BOX_USER_DEFINED
};

/* ��� ���� */
enum
{
   BIG_CURSOR,
   SMALL_CURSOR,
   BOX_CURSOR,
   USER_DEFINED_CURSOR
};

/* �a�e �w�� */
enum
{
   BUL2_TYPE,
   BUL3_TYPE
};

/* �a�a�� ���᷁ �A�� �i �� �� З �� */
#define LIMITX_HAN              90
#define LIMITY_HAN              30

/* ���e�A �b�E ���a�� ���A */
#define ENG                     0x00
#define HAN1                    0x01
#define HAN2                    0x02
#define EDGE                    0x03

/* �b�� ���a ����š�a */
#define SPC_ID                  212

/* �a��Ϣ ���a */
enum
{
   INIT_HGC640_0,
   INIT_HGC640_1,
   INIT_HGC720_0,
   INIT_HGC720_1,
   INIT_VGA,
   INIT_EGA,
   INIT_EGA_MONO,
   INIT_PC24,
   INIT_ATT,
   INIT_CORONA,
   INIT_TOSHIBA,
   INIT_SPC,
   INIT_TV
};

/* �ᥡ C ѡ�Ŷw ���b�q���� ���b �a�w�e ���a�� �� */
#define MAX_STRLEN      127

/* getch2_han(), getch3_han() �� ����t */
#define ESC_PRESSED      -1
#define BS_PRESSED       -2
#define ENTER_PRESSED    -3
#define TAB_PRESSED      -4
#define CTRLQ_PRESSED    -5
#define CTRLT_PRESSED    -6
#define CTRLY_PRESSED    -7
#define HAN_CHANGED      -8
#define COMPLETE         -9
#define NOT_COMPLETE    -10
#define IGNORE          -11

/* �e�i ���b �aȂ�� �a�� */
#define MAX_HAN_STACK    10

/* ��, ��, ������ ���e �e�i�� ��, ��, ���� š�a */
#define NO_CHO            1
#define NO_JUNG           2
#define NO_JONG           1

/* ��, ��, ������ ���� ���e �e�i�� š�a */
#define NO_HAN_CODE  0x8441
#define NO_HAN_STR   0x4184

/* ��, ��, ���� �� �a�aǡ���a�� �� */
#define NO_HAN1          19
#define NO_HAN2          21
#define NO_HAN3          27
#define NO_ENG          256

/* ��, ��, ���� ͥ�a�� �� �� */
#define NO_BUL1           8
#define NO_BUL2           4
#define NO_BUL3           4

/* �e�i, �w�� ͥ�a�� �a��(�a���a �e��) */
#define HAN_FONT_SIZE    32
#define ENG_FONT_SIZE    16

/* EGA/VGA ���e�� �i �� */
#define VGA_SCR_WIDTH    80

/* �� ͡���� */
#ifndef NULL
#   define      NULL     0L
#endif

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

#define mini unsigned char

/* �e�i ���b �aȂ�� �a�a�w */
typedef struct
{
   int top;
   unsigned element[MAX_HAN_STACK];
} han_stack_struct;

/* �e�i š�a�� �a�a�w */
union hangul
{
   unsigned code;
   struct
   {
      unsigned  jong:5;
      unsigned  jung:5;
      unsigned   cho:5;
      unsigned ishan:1;
   } bit;
   mini bytes[2];
};

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

/* ͥ�a�i �a��ǡ�e ͡���� */
extern mini (*han1_font)[NO_HAN1+1][HAN_FONT_SIZE],
            (*han2_font)[NO_HAN2+1][HAN_FONT_SIZE],
	    (*han3_font)[NO_HAN3+1][HAN_FONT_SIZE],
            (*eng_font)[ENG_FONT_SIZE],
            (*spc_font)[HAN_FONT_SIZE],
            or_font[HAN_FONT_SIZE];
extern mini (*org_han1_font)[NO_HAN1+1][HAN_FONT_SIZE],
	    (*org_han2_font)[NO_HAN2+1][HAN_FONT_SIZE],
            (*org_han3_font)[NO_HAN3+1][HAN_FONT_SIZE],
            (*org_eng_font)[ENG_FONT_SIZE],
            (*org_spc_font)[HAN_FONT_SIZE];

/* �a�a������i �a��ǡ�e ͡���� */
extern mini (*scr_buff)[LIMITX_HAN][4];

/* ���A ���e �e���i */
extern mini han_cursor[HAN_FONT_SIZE+1],
            eng_cursor[ENG_FONT_SIZE+1];
extern mini *cursor_han;
extern char exist_cursor, draw_cursor;
extern int  cursor_twinkle_time;

/* �a��Ϣ ���a�A ���e �e���i */
extern unsigned video_seg;
extern char in_vga_system;
extern char stay_in_gr_mode;

/* ���a�A ���e �e���i */
extern int abs_maxx_han, abs_maxy_han;
extern int minx_han, maxx_han, miny_han, maxy_han;
extern int curx_han, cury_han;

/* �b ���U */
extern int output_offset_han;

/* �e�� ���� */
extern mini attr_han;

/* �e�� ��w��, ���w�� */
extern mini color_han, bkcolor_han;

/* ���b �wȁ�A ���e �e���i */
extern char han_mode;
extern char ins_mode;
extern char bul3_mode;

/* �e�i ���b�A ���e �e���i */
extern mini no_more_han;
extern int  adv_gets_han_index;
extern int  max_strlen_han;
extern han_stack_struct han_stack;
extern union hangul prv_han, cur_han;

/* �e/�w ���ŵA ���e �e���i */
extern char able_to_change_han;
extern mini han_change_code;
extern mini han_change_flag;
extern char is_han_change_extend;

/* �wȁ �a�� �a�� �a�� */
extern char is_status_on_han;

/* ͥ�a ������ BGI ������ ́�a */
extern char *font_path_han;
extern char *bgi_path_han;

/* �a�w�a ���� ���a�� �b�a�� �����i �a��ǡ�e ͡���� */
extern char *user_defined_box_shape;

/* �A�� �A�����i �a��ǡ�e ͡���� */
extern char *error_exit_msg;

/* �b�� �q�� ͡����i */
extern void (*han_change_func) (void);
extern void (*ins_change_func) (void);
extern void (*han_wait_func) (void);
extern void (*redirect_func) (char *);

/* �e�a ����A ���e �e���i */
extern struct
{
   unsigned code;
   char font[HAN_FONT_SIZE];
} *hanja_buff;
extern unsigned hanja_buff_ptr;
extern unsigned hanja_buff_size;

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int  init_han(void);
void close_han(void);
int  _init_han(int mode);
void _close_han(int mode);
void set_han_change_key(int code, int is_extend, int flag);
void set_bgi_font_path(char *bgi_path, char *font_path);

void putch_han(char *string);
int  puts_han(char *string);
void cputs_han(char *string);
int  printf_han(char *format,...);
int  scanf_han(char *format,...);
int  gets_han(char *string);
void clrscr_han(void);
void gotoxy_han(int x, int y);

void set_window_han(int left, int top, int right, int bottom);
void init_window_han(void);
int  get_abs_maxx_han(void);
int  get_abs_maxy_han(void);
int  get_maxx_han(void);
int  get_maxy_han(void);
int  get_minx_han(void);
int  get_miny_han(void);
int  get_curx_han(void);
int  get_cury_han(void);
int  get_attr_han(void);
int  get_color_han(void);
int  get_bkcolor_han(void);
void set_attr_han(int attr);
void set_color_han(int color);
void set_bkcolor_han(int color);
void set_vbcolor_han(int color, int bkcolor);

void status_on_han(void);
void status_off_han(void);
void draw_status_han(void);
void set_to_han_mode(void);
void set_to_eng_mode(void);
void enable_han_change(void);
void disable_han_change(void);
int  is_han_mode(void);
int  get_keyboard_type(void);
void set_keyboard_type(int type);
void default_han_change_func(void);
void default_ins_change_func(void);

void draw_box_han(int left, int top, int right, int bottom, int border_num);
char *save_text_han(int left, int top, int right, int bottom);
void restore_text_han(int x, int y, char *scr_data);

void temp_make_han(union hangul *prv, union hangul *cur);
void make_han(union hangul *prv, union hangul *cur, mini *ret_han);
void push_han(unsigned code);
void pop_han(unsigned *code);

/* SETGR.C */
void set_to_gr_mode ( int mode );
void set_to_text_mode ( int mode );

/* STRTRIM.C */
char *lefttrim ( char *string );
char *righttrim ( char *string );

/* PLAY.C */
//const int note_freq [( MAX_OCTAVE +1 );
int play ( char *string );

/* KEYFLAG.C */
unsigned char get_keyflag_han ( void );
int is_keyflag_han ( unsigned char flag );
void set_keyflag_han ( unsigned char flag );
void toggle_keyflag_han ( unsigned char flag );

#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>

#undef mini
