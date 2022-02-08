#ifndef __SCREEN_H
#define __SCREEN_H

extern char far *g_screen;

void screen_init(void);
void screen_close(void);
void screen_clear(char register ascii, char color);
void screen_fill_region(char ascii, char color, unsigned int start, char w, char h);
void screen_blit(const char *data, unsigned int start, char w, char h);
void screen_wait_retrace(void);
void screen_color(char color);
void screen_print_text(char x, char y, const char *text);
void screen_print_hex16(char x, char y, unsigned int value);
void screen_print_uint(char x, char y, unsigned int value);
void screen_print_bar(char x, char y, char l1, char l2);

#define screen_offset_xy(x, y)          (((y) * 80 + (x)) * 2)
#define screen_offset_yx(y, x)          (((y) * 80 + (x)) * 2)

#endif
