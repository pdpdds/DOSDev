#include "byte.h"
extern void line256(unsigned,unsigned,unsigned,unsigned,int,BYTE);

void box256 (x1,y1,x2,y2,mode,color)
unsigned x1,y1,x2,y2;
int mode;
BYTE color;
{
	line256(x1,y1,x2,y1,mode,color);
	line256(x2,y1+1,x2,y2,mode,color);
	line256(x2-1,y2,x1,y2,mode,color);
	line256(x1,y2-1,x1,y1+1,mode,color);
}

