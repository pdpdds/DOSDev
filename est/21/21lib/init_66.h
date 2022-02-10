extern far int *mouse_x;
extern far int *mouse_y;
extern byte far *lb, far *rb;

#define mousex (*mouse_x)
#define mousey (*mouse_y)
#define lbp *lb
#define rbp *rb

/*
extern int mousex;
extern int mousey;
extern byte lbp;
extern byte rbp;
*/

#define getpos(mx, my) {mx=mousex; my=mousey;}


