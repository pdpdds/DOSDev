/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          header file - more extended functions(95)           */
/*   FILENAME           extendio.h                                          */
/*   DATE & TIME        06/29/92 (MON) 13:30                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/* pulldown_han() ·a¡ Š¬÷Ði ® ·¶“e º¡A“A, ¦¡A“A· ® */
#define MAX_MAIN_MENU   30
#define MAX_SUB_MENU    30

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

#define mini unsigned char

/* menu_han() · ·¥¸a· ¸ažaÑw */
typedef struct
{
   char number, current, vertical, save, border_num, attr;
   char box_x1, box_y1, box_x2, box_y2;
   char *x, *y;
   char **string;
   char *key;
   void (**func) (void);
   char auto_menu;
} menu_struct;

/* pulldown_han() · ·¥¸a· ¸ažaÑw */
typedef struct
{
   int  number, current;
   int  x[MAX_MAIN_MENU];
   char *(string[MAX_MAIN_MENU]);
   struct
   {
      int  number, current;
      int  box_x, box_y;
      char *(string[MAX_SUB_MENU]);
   } sub_menu[MAX_MAIN_MENU];
} pulldown_menu_struct;

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void put_eng_font(int x, int y, mini code);
void aput_eng_font(int x, int y, mini code, mini attr);
void vput_eng_font(int x, int y, mini code, mini color);
void put_han_font(int x, int y, mini code1, mini code2);
void aput_han_font(int x, int y, mini code1, mini code2, mini attr);
void vput_han_font(int x, int y, mini code1, mini code2, mini color);

void xputch_han(int x, int y, char *string);
void aputch_han(int x, int y, char *string, int attr);
void vputch_han(int x, int y, char *string, int color);
void vaputch_han(int x, int y, char *string, int color, int attr);
void vbputch_han(int x, int y, char *string, int color, int bkcolor);
#define eputch_han(x,y,code) put_eng_font(x, y, code);

void xputs_han(int x, int y, char *string);
void nputs_han(int x, int y, char *string, int length);
void aputs_han(int x, int y, char *string, int attr);
void vputs_han(int x, int y, char *string, int color);
void naputs_han(int x, int y, char *string, int length, int attr);
void nvputs_han(int x, int y, char *string, int length, int color);
void vaputs_han(int x, int y, char *string, int color, int attr);
void vbputs_han(int x, int y, char *string, int color, int bkcolor);
void eputs_han(int x, int y, char *string);
void enputs_han(int x, int y, char *string, int length);

void xprintf_han(int x, int y, char *format,...);
void nprintf_han(int x, int y, int length, char *format,...);
void aprintf_han(int x, int y, int attr, char *format,...);
void vprintf_han(int x, int y, int color, char *format,...);
void naprintf_han(int x, int y, int length, int attr, char *format,...);
void nvprintf_han(int x, int y, int length, int color, char *format,...);
void vaprintf_han(int x, int y, int color, int attr, char *format,...);
void vbprintf_han(int x, int y, int color, int bkcolor, char *format,...);
void eprintf_han(int x, int y, char *format,...);
void enprintf_han(int x, int y, int limit, char *format,...);

void clear_text_han(int left, int top, int right, int bottom);
void aclear_text_han(int left, int top, int right, int bottom, int attr);
void vclear_text_han(int left, int top, int right, int bottom, int color);
void aclrscr_han(int attr);
void vclrscr_han(int color);
void rand_clear_text_han(int left, int top, int right, int bottom,
			 int attr, unsigned count, unsigned delay_time);

int get_scr_buff_code(int x, int y);
int get_scr_buff_id(int x, int y);
int get_scr_buff_attr(int x, int y);
int get_scr_buff_color(int x, int y);
int get_scr_buff_bkcolor(int x, int y);

int  getch_han(void);
int  getch2_han(char *ret_str, char *exit_key, int edit_cursor);
int  getch3_han(char *ret_str, char *exit_key, int edit_cursor);
int  adv_gets_han(int x, int y, char *string, int limit, int attr, char *user_exit_key);
int  get_number_han(int x, int y, char *number, int limit, int attr, char *user_exit_key);
int  get_yesno_han(int x, int y, int attr);
int  wait_key_han(int second);
int  get_max_strlen_han(void);
void set_max_strlen_han(int length);

void adv_puts_han(char *string, int indent);
void center_puts_han(int y, char *string, int color, int attr);
void error_exit_han(char *func_name, int exit_code);

void draw_cursor_han(int x, int y);
void hide_cursor_han(void);
void show_cursor_han(void);
void set_cursor_han(int han_no, char *han_shape, int eng_no, char *eng_shape);
void set_twinkle_time(int twinkle_time);

void draw_var_box_han(int left, int top, int right, int bottom,
		      int border_num, int attr, int delay_time);
void draw_bit_map_box_han(int left, int top, int right, int bottom,
			  char *bit_map);
int  dir_box_long(int box_x, int box_y, char *mask, char *file_name);
int  dir_box_short(int box_x, int box_y, char *mask, char *file_name);
void draw_horiz_line(int left, int top, int width, int color);
void draw_vert_line(int left, int top, int height, int color);

int  menu_han(menu_struct *menu_data);
int  popup_han(int x, int y, char **string, int number, int *current);
int  pulldown_han(pulldown_menu_struct *menu_data);

int  replace_font(char *han_font_file, char *eng_font_file, char *spc_font_file);
int  read_han_font(char **font_ptr, char *han_font_file);
int  read_eng_font(char **font_ptr, char *eng_font_file);
int  read_spc_font(char **font_ptr, char *spc_font_file);
void set_font(char *han_font_ptr, char *eng_font_ptr, char *spc_font_ptr);
void free_font(char *han_font_ptr, char *eng_font_ptr, char *spc_font_ptr);
void return_to_org_font(int return_han, int return_eng, int return_spc);

int  choose_hanja(char *hangul_code, char *hanja_code);
void hanja_to_hangul(char *hanja_code, char *hangul_code);

void modify_text_attr(int left, int top, int right, int bottom, int attr);
void modify_text_color(int left, int top, int right, int bottom, int color);

unsigned get_video_seg(void);
int  is_vga_mode(void);
void setactivepage_han(int page);
int  get_output_offset_han(void);
void set_output_offset_han(int output_offset);

void set_han_change_func(void (*func) (void));
void set_ins_change_func(void (*func) (void));
void set_han_wait_func(void (*func) (void));

void modify_image(mini *dest, mini *src, mini bytes, mini attr);
void putimage_8(int x, int y, mini *code);
void putimage_16(int x, int y, mini *code);
void zoomed_out_8(int x, int y, mini *bit_map, mini attr);
void zoomed_out_16(int x, int y, mini *bit_map, mini attr);
void xscrollup_han(int left, int top, int right, int bottom);
void xscrolldn_han(int left, int top, int right, int bottom);

#undef mini
