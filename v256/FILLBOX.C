#include "byte.h"
extern void uiswap(unsigned *,unsigned *);

void fill_box(x1,y1,x2,y2,color)
unsigned x1,y1,x2,y2;
BYTE color;
{
	unsigned xsize,ysize;

	if(y2<y1) uiswap(&y2,&y1);
        if(x2<x1) uiswap(&x2,&x1);
	xsize=x2-x1+1;

        asm{ push ds
             cld
             mov ax,y1
             mov ysize,ax }

loop1:	    asm{
		mov cx,xsize
		mov ax,0a000h
		mov es,ax

                mov ax,320
                mov bx,ysize
                mul bx
                add ax,x1
		mov di,ax

                mov al,color
		rep stosb

                mov ax,y2
                cmp ysize,ax
                jz end
                inc ysize
                jmp loop1    }

end:            asm pop ds
}

