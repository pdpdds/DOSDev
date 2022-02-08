#include "byte.h"
extern BYTE getpixel(unsigned,unsigned);
extern void putpixel256(unsigned,unsigned,BYTE);

void xorputpixel256(x, y, color)
unsigned x,y;
BYTE color;
{
	color^=getpixel256(x,y);
	putpixel256(x,y,color);
}
