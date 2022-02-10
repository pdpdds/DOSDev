extern far int *mouse_x;
extern far int *mouse_y;
extern unsigned char far *lb, far *rb;

#define mousex (*mouse_x)
#define mousey (*mouse_y)
#define lbp *lb
#define rbp *rb
#define getpos(mx, my) {mx=mousex; my=mousey;}