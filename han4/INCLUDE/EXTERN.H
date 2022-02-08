/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

/* INITHAN.C */
int init_han(void);
void close_han(void);

/* _INITHAN.C */
int _init_han(int mode);
void _close_han(int mode);

/* PUTENG.C */
void put_eng_font(int x, int y, mini code);

/* APUTENG.C */
void aput_eng_font(int x, int y, mini code, mini attr);

/* VPUTENG.C */
void vput_eng_font(int x, int y, mini code, mini color);

/* PUTHAN.C */
void put_han_font(int x, int y, mini code1, mini code2);

/* APUTHAN.C */
void aput_han_font(int x, int y, mini code1, mini code2, mini attr);

/* VPUTHAN.C */
void vput_han_font(int x, int y, mini code1, mini code2, mini color);

/* PUTCHHAN.C */
void putch_han(char *string);

/* XPUTCH.C */
void xputch_han(int x, int y, char *string);

/* APUTCH.C */
void aputch_han(int x, int y, char *string, int attr);

/* VPUTCH.C */
void vputch_han(int x, int y, char *string, int color);
void vaputch_han(int x, int y, char *string, int color, int attr);
void vbputch_han(int x, int y, char *string, int color, int bkcolor);

/* EXTENDIO.H */
#define eputch_han(x,y,code) put_eng_font(x, y, code);

/* PUTSHAN.C */
int puts_han(char *string);

/* CPUTSHAN.C */
void cputs_han(char *string);

/* XPUTS.C */
void xputs_han(int x, int y, char *string);

/* NPUTS.C */
void nputs_han(int x, int y, char *string, int length);

/* APUTS.C */
void aputs_han(int x, int y, char *string, int attr);

/* VPUTS.C */
void vputs_han(int x, int y, char *string, int color);

/* NAPUTS.C */
void naputs_han(int x, int y, char *string, int length, int attr);

/* NVPUTS.C */
void nvputs_han(int x, int y, char *string, int length, int color);

/* VAPUTS.C */
void vaputs_han(int x, int y, char *string, int color, int attr);

/* VBPUTS.C */
void vbputs_han(int x, int y, char *string, int color, int bkcolor);

/* EPUTS.C */
void eputs_han(int x, int y, char *string);
void enputs_han(int x, int y, char *string, int length);

/* ADVPUTS.C */
void adv_puts_han(char *string, int indent);

/* CNTRPUTS.C */
void center_puts_han(int y, char *string, int color, int attr);

/* PRINTF.C */
int printf_han(char *format,...);

/* XPRINTF.C */
void xprintf_han(int x, int y, char *format,...);

/* NPRINTF.C */
void nprintf_han(int x, int y, int length, char *format,...);

/* APRINTF.C */
void aprintf_han(int x, int y, int attr, char *format,...);

/* VPRINTF.C */
void vprintf_han(int x, int y, int color, char *format,...);

/* NAPRINTF.C */
void naprintf_han(int x, int y, int length, int attr, char *format,...);

/* NVPRINTF.C */
void nvprintf_han(int x, int y, int length, int color, char *format,...);

/* VAPRINTF.C */
void vaprintf_han(int x, int y, int color, int attr, char *format,...);

/* VBPRINTF.C */
void vbprintf_han(int x, int y, int color, int bkcolor, char *format,...);

/* EPRINTF.C */
void eprintf_han(int x, int y, char *format,...);
void enprintf_han(int x, int y, int limit, char *format,...);

/* GETCH23.C */
void temp_make_han(union hangul *prv, union hangul *cur);
void make_han(union hangul *prv, union hangul *cur, mini *ret_han);
void push_han(unsigned code);
void pop_han(unsigned *code);
int  getch2_han(char *ret_str, char *exit_key, int edit_cursor);
int  getch3_han(char *ret_str, char *exit_key, int edit_cursor);

/* GETCHHAN.C */
int getch_han(void);

/* GETKEY.C */
int get_key_han(int *ascii);

/* GETYESNO.C */
int  get_yesno_han(int x, int y, int attr);

/* WAITKEY.C */
int wait_key_han(int second);

/* GETSHAN.C */
int  gets_han(char *string);

/* ADVGETS.C */
int  adv_gets_han(int x, int y, char *string, int limit, int attr, char *user_exit_key);

/* GETNUMBR.C */
int  get_number_han(int x, int y, char *number, int limit, int attr, char *user_exit_key);

/* SCANF.C */
int scanf_han(char *format,...);

/* WINDOW.C */
void set_window_han(int left, int top, int right, int bottom);
void init_window_han(void);

/* CLEAR.C */
void clear_text_han(int left, int top, int right, int bottom);

/* ACLEAR.C */
void aclear_text_han(int left, int top, int right, int bottom, int attr);

/* VCLEAR.C */
void vclear_text_han(int left, int top, int right, int bottom, int color);

/* CLRSCR.C */
void clrscr_han(void);

/* ACLRSCR.C */
void aclrscr_han(int attr);

/* VCLRSCR.C */
void vclrscr_han(int color);

/* RNDCLEAR.C */
void rand_clear_text_han(int left, int top, int right, int bottom, int attr, unsigned count, unsigned delay_time);

/* SAVETEXT.C */
char *save_text_han(int left, int top, int right, int top);
void restore_text_han(int x, int y, char *scr_data);

/* MODITEXT.C */
void modify_text_attr(int left, int top, int right, int top, int attr);
void modify_text_color(int left, int top, int right, int top, int color);

/* DRAWBOX.C */
void draw_box_han(int left, int top, int right, int bottom, int border_num);

/* BITBOX.C */
void draw_bit_map_box_han(int left, int top, int right, int bottom, char *bit_map)

/* VARBOX.C */
void draw_var_box_han(int left, int top, int right, int bottom, int border_num, int attr, int delay_time);

/* GOTOXY.C */
void gotoxy_han(int x, int y);

/* GETCORDI.C */
int get_abs_maxx_han(void);
int get_abs_maxy_han(void);
int get_maxx_han(void);
int get_maxy_han(void);
int get_minx_han(void);
int get_miny_han(void);
int get_curx_han(void);
int get_cury_han(void);

/* ATTRCOLR.C */
int  get_attr_han(void);
void set_attr_han(int attr);
int  get_color_han(void);
int  get_bkcolor_han(void);
void set_color_han(int color);
void set_bkcolor_han(int color);
void set_vbcolor_han(int color, int bkcolor);

/* MODE.C */
void set_to_han_mode(void);
void set_to_eng_mode(void);
void enable_han_change(void);
void disable_han_change(void);
int  get_keyboard_type(void);
void set_keyboard_type(int type);
unsigned get_video_seg(void);
int  is_vga_mode(void);
int  is_han_mode(void);

/* DEFAULT.C */
void default_han_change_func(void);
void default_ins_change_func(void);

/* SETCHKEY.C */
void set_han_change_key(int code, int is_extend, int flag);

/* STATUS.C */
void draw_status_han(void);
void status_on_han(void);
void status_off_han(void);

/* GETBUFF.C */
int  get_scr_buff_code(int x, int y);
int  get_scr_buff_id(int x, int y);
int  get_scr_buff_attr(int x, int y);
int  get_scr_buff_color(int x, int y);
int  get_scr_buff_bkcolor(int x, int y);

/* KEYFLAG.C */
mini get_keyflag_han(void);
int  is_keyflag_han(mini flag);
void set_keyflag_han(mini flag);
void toggle_keyflag_han(mini flag);

/* HANBASE.C */
int  replace_font(char *han_font_file, char *eng_font_file, char *spc_font_file);
void set_cursor_han(int han_no, char *han_shape, int eng_no, char *eng_shape);
void modify_image(unsigned char *dest, unsigned char *src, unsigned char bytes, unsigned char attr);
void putimage_8(int x, int y, unsigned char *code);
void putimage_16(int x, int y, unsigned char *code);
void zoomed_out_8(int x, int y, unsigned char *font, unsigned char attr);
void zoomed_out_16(int x, int y, unsigned char *font, unsigned char attr);

/* FONTMNG.C */
int  read_han_font(char **font_ptr, char *han_font_file);
int  read_eng_font(char **font_ptr, char *eng_font_file);
int  read_spc_font(char **font_ptr, char *spc_font_file);
void set_font(char *han_font_ptr, char *eng_font_ptr, char *spc_font_ptr);
void free_font(char *han_font_ptr, char *eng_font_ptr, char *spc_font_ptr);
void return_to_org_font(int return_han, int return_eng, int return_spc);

/* SETPATH.C */
void set_bgi_font_path(char *bgi_path, char *font_path);

/* XSCROLUP.C */
void xscrollup_han(int left, int top, int right, int top);

/* XSCROLDN.C */
void xscrolldn_han(int left, int top, int right, int top);

/* OFFSET.C */
int  get_output_offset_han(void);
void set_output_offset_han(int output_offset);

/* MAXSTRLN.C */
int  get_max_strlen_han(void);
void set_max_strlen_han(int length);

/* KORCHI.C */
void hanja_to_hangul(char *hanja_code, char *hangul_code);

/* CHHANJA.C */
void choose_hanja(char *hangul_code, char *hanja_code);

/* DIRBOX2.C */
int dir_box_short(int box_x, int box_y, char *mask, char *file_name);

/* DIRBOX3.C */
int dir_box_long(int box_x, int box_y, char *mask, char *file_name);

/* SETGR.C */
void set_to_gr_mode(int mode);
void set_to_text_mode(int mode);

/* ACTPAGE.C */
void setactivepage_han(int page);

/* CHOOSEGR.C */
int choose_gr_mode(void);

/* CURSOR.C */
void hide_cursor_han(void);
void show_cursor_han(void);
void draw_cursor_han(int x, int y);

/* SETTWINK.C */
void set_twinkle_time(int twinkle_time);

/* VERTLINE.C */
void draw_vert_line(int left, int top, int height, int color);

/* HORILINE.C */
void draw_horiz_line(int left, int top, int width, int color);

/* SETCHF.C */
void set_han_change_func(void (*func) (void));
void set_ins_change_func(void (*func) (void));

/* SETWAITF.C */
void set_han_wait_func(void (*func) (void));

/* KEYBUFF.C */
void clear_key_buff(void);

/* PLAY.C */
int play(char *string);

/* STRTRIM.C */
char *lefttrim(char *string);
char *righttrim(char *string);

/* ISNUMBER.C */
int is_number(char *string);

/* WHATBYTE.C */
int what_byte(char *string, int chk_pos);

/* ARRNGSTR.C */
int arrange_string(char *string, int width);

/* CNVTHAN.C */
int convert_han_code(char *string, int source_code, int dest_code);

/* MENU.C */
int menu_han(menu_struct *menu_data);

/* POPUP.C */
int popup_han(int x, int y, char **string, int number, int *current);

/* PULLDOWN.C */
int pulldown_han(pulldown_menu_struct *menu_data);

/* ERROR.C */
void error_exit_han(char *func_name, int exit_code);
