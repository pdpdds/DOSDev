#include "byte.h"

void putpixel256( x, y, color)
unsigned x,y;
BYTE color;
{
	asm{ mov cl,color
	     mov ax,y
	     mov bx,x
	     mov dx,320
	     mul dx
	     add bx,ax
	     mov ax,0a000h
	     mov es,ax
	     mov es:[bx],cl }
}
