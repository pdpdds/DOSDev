#include "byte.h"

void putimage256 (x1,y1,mode,image)
unsigned x1,y1;
int mode;
BYTE *image;
{
	unsigned yy,ysize,count=3,data_count;

	ysize=(unsigned)(image[2]+y1);
	data_count=(unsigned)(image[0]+image[1]+1);

	 asm{push ds
	     mov ax,y1
	     mov yy,ax
	     cld
	     mov ax,mode
	     cmp ax,1
	     jz loop1   }

loop2:   asm{mov cx,data_count
	     lds si,image
	     add si,count
	     mov ax,0a000h
	     mov es,ax
	     mov ax,320
	     mov bx,yy
	     mul bx
	     add ax,x1
	     mov di,ax
             rep movsb
	     mov ax,data_count
	     add count,ax
	     mov ax,ysize
	     cmp yy,ax
	     jz end
	     inc yy
	     jmp loop2        }

loop1:   asm{mov cx,data_count
	     lds si,image
	     add si,count
	     mov ax,0a000h
	     mov es,ax
	     mov ax,320
	     mov bx,yy
	     mul bx
	     add ax,x1
	     mov di,ax }

repeat: asm{ mov al,ds:[si]
	     cmp al,00h
	     jz skip
	     mov es:[di],al }

skip:   asm{ inc si
	     inc di
	     loop repeat
	     mov ax,data_count
	     add count,ax
	     mov ax,ysize
	     cmp yy,ax
	     jz end
	     inc yy
	     jmp loop1 }

end:    asm pop ds;
}

