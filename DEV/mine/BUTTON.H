#define YES -2
#define NO -1

#define UP    0x4800
#define DOWN  0x5000


void button_up_s(int left,int top,int right,int bottom,int color,int yes_no);
void button_down_s(int left,int top,int right,int bottom,int color,int yes_no);
void button_no_fill(int left,int top,int right,int bottom,int up_down);
void panel_s(int left,int top,int right,int bottom,int color);
int line_x(int left,int right,int y,int color);
int line_y(int top,int bottom,int x,int color);
void box(int left,int top,int right,int bottom,int color);
int box_fill(int left,int top,int right,int bottom,int color);
int box_fill_x8(int left,int top,int right,int bottom,int color);
int box_fill_pattern(int left,int top,int right,int bottom,int color,int bcolor,int pattern);
void get_image_x8(int left,int top,int right,int bottom,char far *buff);
void put_image_x8(int left,int top,char far *buff,int mode);
