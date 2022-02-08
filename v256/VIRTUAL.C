#include <dos.h>
#include "byte.h"

void virtualscreen (x1,y1,x2,y2,vx,vy,mode,image)
unsigned x1,y1,x2,y2,vx,vy;
int mode;
BYTE *image;
{
    unsigned yy,sseg=FP_SEG(image),count=abs(x2-x1)+1;

      if(y2<y1) uiswap(&y2,&y1);

	asm{push ds
	     mov ax,y1
	     mov yy,ax
	     cld
	     mov ax,mode
	     cmp ax,1
	     jz loop1   }
loop2:
	asm{
	     mov cx,count

	     mov ax,sseg
	     mov ds,ax
	     mov ax,0a000h
	     mov es,ax

	     mov ax,320
	     mov bx,yy
	     sub bx,y1
	     add bx,vy
	     mul bx
	     add ax,vx
	     mov di,ax

	     mov ax,320
	     mov bx,yy
	     mul bx
	     mov bx,x1
	     add bx,4
	     add ax,bx
	     mov si,ax

	     rep movsb

	     mov ax,y2
	     cmp yy,ax
             jz end
	     inc yy
	     jmp loop2
	   }
loop1:
	asm{
	     mov cx,count

	     mov ax,sseg
	     mov ds,ax
	     mov ax,0a000h
	     mov es,ax

	     mov ax,320
	     mov bx,yy
	     sub bx,y1
	     add bx,vy
	     mul bx
	     add ax,vx
	     mov di,ax

	     mov ax,320
	     mov bx,yy
	     mul bx
	     mov bx,x1
	     add bx,4
	     add ax,bx
	     mov si,ax }

repeat: asm{ mov al,ds:[si]
	     cmp al,00h
	     jz skip
	     mov es:[di],al }

skip:   asm{ inc si
	     inc di
	     loop repeat
	     mov ax,y2
	     cmp yy,ax
	     jz end
	     inc yy
	     jmp loop1
	    }

end:         asm pop ds;
}

