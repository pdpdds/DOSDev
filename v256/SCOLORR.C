#include "byte.h"

void setcolorregister(RegNum,Red,Green,Blue)
unsigned RegNum;
BYTE Red,Green,Blue;
{
   asm{ mov ax,1010h
	mov bx,RegNum
	mov dh,Red
	mov ch,Green
	mov cl,Blue
	int 10h      }
}
