#include "byte.h"

void getcolorregister(RegNum,Red,Green,Blue)
unsigned RegNum;
BYTE *Red,*Green,*Blue;
{
	asm{ mov ax,1015h
	     mov bx,RegNum
	     int 10h       }
	*Red   = _DH;
	*Green = _CH;
	*Blue  = _CL;
}
