#include "byte.h"

BYTE getpixel256(x,y)
unsigned x,y;
{
	asm{ mov ax,y
	     mov bx,x
	     mov dx,320
	     mul dx
	     add bx,ax
	     mov ax,0a000h
	     mov es,ax
	     mov al,es:[bx] }
	return(_AL);
}

