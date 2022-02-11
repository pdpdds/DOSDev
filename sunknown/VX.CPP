// VX.CPP ///////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <dos.h>
#include <stdlib.h>

#include "vx.h"

// METHODS //////////////////////////////////////////////////////////////////

// FRAME

void vx_C::frame(void)
{
  while (inportb(0x3da)&8);
  while (!(inportb(0x3da)&8));
}

// WRITEPIXEL

void vx_C::writepixel(word pos,byte plane, byte color)
{
  asm mov cl,plane
  asm mov ax,0x0100+MAP_MASK
  asm shl ah,cl
  asm mov dx,SC_INDEX
  asm out dx,ax
  asm mov di,pos
  asm mov ax,0xa000
  asm mov es,ax
  asm mov al,color
  asm mov es:[di],al
}

// SETOFFSET

void vx_C::setoffset(word newoffset)
{
  if (offset==newoffset)
    return;
  if ((offset&255)==(newoffset&255))
  {
    asm mov ax,newoffset
    asm mov al,0x0c
    asm mov dx,CRTC_INDEX
    asm out dx,ax
  }
  else
  {
    asm mov ax,newoffset
    asm mov bh,al
    asm mov ch,ah
    asm mov bl,0x0d
    asm mov cl,0x0c
    asm mov dx,CRTC_INDEX
    asm mov ax,bx
    asm cli
    asm out dx,ax
    asm mov ax,cx
    asm out dx,ax
    asm sti
  }
  offset=newoffset;
}

// SETRGB

void vx_C::setrgb(byte color, byte red,byte green,byte blue)
{
  outportb(0x3c8,color);
  outportb(0x3c9,red);
  outportb(0x3c9,green);
  outportb(0x3c9,blue);
}

byte vx_C::getpixel(word x,word y)
{
  word offset_2=offset;
  asm mov ax,80
  asm mul y
  asm mov bx,x
  asm mov cx,bx
  asm shr bx,1
  asm shr bx,1
  asm add bx,offset_2
  asm add bx,ax
  asm mov ax,0xa000
  asm mov es,ax
  asm mov ah,cl
  asm and ah,0x03
  asm mov al,READ_MAP
  asm mov dx,GC_INDEX
  asm out dx,ax
  asm mov al,es:[bx]
  return _AL; // Return value already in AL
}

// BLOCKFILL

void vx_C::blockfill(word start,word end,byte data)
{
	asm mov  dx,SC_INDEX
	asm mov  al,MAP_MASK
	asm out  dx,al
	asm inc  dx

	asm mov  al,0x0f
	asm out  dx,al

	asm mov ax,0xa000
	asm mov es,ax
	asm mov di,start
	asm mov cx,end
	asm sub cx,start
	asm inc cx
	asm mov al,data
	asm rep stosb

	asm mov dx,GC_INDEX+1
	asm mov al,0xff
	asm out dx,al
}
