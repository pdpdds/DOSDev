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
/* œa»¡ ·¡¶A· ¡A¡¡Ÿ¡ ¡¡•I¡ ÄñÌa·©–A¡e µAœá ¡A¯¡»¡Ÿi Â‰bÐe Ò */
/* ÄñÌa·©·i º—”eÐa•¡¢ Ðe”a.                                    */

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/* init_han() · Ÿ¡Èåˆt */
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

/* ­¢¬÷ */
#define FONT_NORMAL             0x00
#define FONT_BOLD               0x01
#define FONT_OUTLINE            0x02
#define FONT_SHADOW             0x04
#define FONT_SHADE              0x08
#define FONT_UNDERLINE          0x10
#define FONT_INVERSE            0x20
#define FONT_VERT_DBL           0x40
#define FONT_HORIZ_DBL          0x80

/* ¬‚ */
#if     !defined(__COLORS)
#define __COLORS
enum
{
   BLACK,               /* ´á–¶… ¬‚ */
   BLUE,
   GREEN,
   CYAN,
   RED,
   MAGENTA,
   BROWN,
   LIGHTGRAY,
   DARKGRAY,            /* ¤j·e ¬‚ */
   LIGHTBLUE,
   LIGHTGREEN,
   LIGHTCYAN,
   LIGHTRED,
   LIGHTMAGENTA,
   YELLOW,
   WHITE
};
#endif

/* ¤b¯a ¡¡´· */
enum
{
   BOX_H1V1,
   BOX_H1V2,
   BOX_H2V1,
   BOX_H2V2,
   BOX_USER_DEFINED
};

/* Äá¬á ¡¡´· */
enum
{
   BIG_CURSOR,
   SMALL_CURSOR,
   BOX_CURSOR,
   USER_DEFINED_CURSOR
};

/* ¸aÌe Ñw¯¢ */
enum
{
   BUL2_TYPE,
   BUL3_TYPE
};

/* ¯aÇaŸ¥ ¤áÌá· ÂA” µi ® £¹ Ð— ® */
#define LIMITX_HAN              90
#define LIMITY_HAN              30

/* ÑÁ¡eµA Â‰b–E ¢…¸a· ¹·ŸA */
#define ENG                     0x00
#define HAN1                    0x01
#define HAN2                    0x02
#define EDGE                    0x03

/* Ëb® ¢…¸a ·¥¯¢Å¡—a */
#define SPC_ID                  212

/* ‹aœÏ¢ ¡¡—a */
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

/* Èá¥¡ C Ñ¡ÑÅ¶w ·³bÐq®¡ ·³b ˆa“wÐe ¢…¸a· ® */
#define MAX_STRLEN      127

/* getch2_han(), getch3_han() · Ÿ¡Èåˆt */
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

/* Ðe‹i ·³b ¯aÈ‚· Ça‹¡ */
#define MAX_HAN_STACK    10

/* Á¡, º—, ¹·¬÷·¡ ´ô“e Ðe‹i· Á¡, º—, ¹·¬÷ Å¡—a */
#define NO_CHO            1
#define NO_JUNG           2
#define NO_JONG           1

/* Á¡, º—, ¹·¬÷·¡ ¡¡– ´ô“e Ðe‹i· Å¡—a */
#define NO_HAN_CODE  0x8441
#define NO_HAN_STR   0x4184

/* Á¡, º—, ¹·¬÷ £¹ ´a¯aÇ¡¢…¸a· ® */
#define NO_HAN1          19
#define NO_HAN2          21
#define NO_HAN3          27
#define NO_ENG          256

/* Á¡, º—, ¹·¬÷ Í¥Ëa· ¤é ® */
#define NO_BUL1           8
#define NO_BUL2           4
#define NO_BUL3           4

/* Ðe‹i, µw¢… Í¥Ëa· Ça‹¡(¤a·¡Ëa ”e¶á) */
#define HAN_FONT_SIZE    32
#define ENG_FONT_SIZE    16

/* EGA/VGA ÑÁ¡e· µi ® */
#define VGA_SCR_WIDTH    80

/* é Í¡·¥Èá */
#ifndef NULL
#   define      NULL     0L
#endif

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

#define mini unsigned char

/* Ðe‹i ·³b ¯aÈ‚· ¸ažaÑw */
typedef struct
{
   int top;
   unsigned element[MAX_HAN_STACK];
} han_stack_struct;

/* Ðe‹i Å¡—a· ¸ažaÑw */
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

/* Í¥ËaŸi ˆaŸ¡Ç¡“e Í¡·¥Èá */
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

/* ¯aÇaŸ¥¤áÌáŸi ˆaŸ¡Ç¡“e Í¡·¥Èá */
extern mini (*scr_buff)[LIMITX_HAN][4];

/* Äá¬áµA ‰ÅÐe ¥e®—i */
extern mini han_cursor[HAN_FONT_SIZE+1],
            eng_cursor[ENG_FONT_SIZE+1];
extern mini *cursor_han;
extern char exist_cursor, draw_cursor;
extern int  cursor_twinkle_time;

/* ‹aœÏ¢ ¡¡—aµA ‰ÅÐe ¥e®—i */
extern unsigned video_seg;
extern char in_vga_system;
extern char stay_in_gr_mode;

/* ¹ÁÎaµA ‰ÅÐe ¥e®—i */
extern int abs_maxx_han, abs_maxy_han;
extern int minx_han, maxx_han, miny_han, maxy_han;
extern int curx_han, cury_han;

/* Â‰b µ³­U */
extern int output_offset_han;

/* Ñe¸ ­¢¬÷ */
extern mini attr_han;

/* Ñe¸ ¸å‰w¬‚, ¤‰w¬‚ */
extern mini color_han, bkcolor_han;

/* ·³b ¬wÈµA ‰ÅÐe ¥e®—i */
extern char han_mode;
extern char ins_mode;
extern char bul3_mode;

/* Ðe‹i ·³bµA ‰ÅÐe ¥e®—i */
extern mini no_more_han;
extern int  adv_gets_han_index;
extern int  max_strlen_han;
extern han_stack_struct han_stack;
extern union hangul prv_han, cur_han;

/* Ðe/µw ¸åÑÅµA ‰ÅÐe ¥e®—i */
extern char able_to_change_han;
extern mini han_change_code;
extern mini han_change_flag;
extern char is_han_change_extend;

/* ¬wÈ œa·¥ Îa¯¡ µa¦ */
extern char is_status_on_han;

/* Í¥Ëa ÑÁ·©‰Á BGI ÑÁ·©· Ì¯a */
extern char *font_path_han;
extern char *bgi_path_han;

/* ¬a¶w¸a ¸÷· §¡Ëa “ ¤b¯a· ¡¡´··i ˆaŸ¡Ç¡“e Í¡·¥Èá */
extern char *user_defined_box_shape;

/* µAœá ¡A¯¡»¡Ÿi ˆaŸ¡Ç¡“e Í¡·¥Èá */
extern char *error_exit_msg;

/* ˆb¹· Ðq® Í¡·¥Èá—i */
extern void (*han_change_func) (void);
extern void (*ins_change_func) (void);
extern void (*han_wait_func) (void);
extern void (*redirect_func) (char *);

/* Ðe¸a ¤áÌáµA ‰ÅÐe ¥e®—i */
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

#undef mini
