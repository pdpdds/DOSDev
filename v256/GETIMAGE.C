#include "byte.h"
extern void uiswap(unsigned *, unsigned *);

void getimage256 (x1,y1,x2,y2,image)
unsigned x1,y1,x2,y2;
BYTE *image;
{
	unsigned yy,count=3,data_count;

        if(y1>y2) uiswap(&y1,&y2);
        if(x1>x2) uiswap(&x1,&x2);

	yy=x2-x1; image[2]=(BYTE)(y2-y1);
	if(yy > 255) { yy-=255; image[0]=255; image[1]=(BYTE)yy;}
	else { image[0]=(BYTE)yy; image[1]=0;}

	data_count=(unsigned)(image[0]+image[1]+1);

	 asm{
	     push ds
	     mov ax,y1
	     mov yy,ax
	     cld      }

loop1:   asm{
	     mov cx,data_count
	     mov ax,0a000h

	     mov ds,ax
	     mov ax,320
	     mov bx,yy
	     mul bx
	     add ax,x1
	     mov si,ax

	     les di,image
	     add di,count
	     rep movsb

	     mov ax,data_count
	     add count,ax

	     mov ax,y2
	     cmp yy,ax
	     jz end
	     inc yy
	     jmp loop1 }

end:    asm pop ds;
}

