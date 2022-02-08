#include "byte.h"

void memtomem(dseg,doff,sseg,soff,rep_num,not_color)
unsigned dseg,doff,sseg,soff,rep_num;
BYTE not_color;
{
	asm{ push ds
	     cld

	     mov al,not_color
	     cmp al,0
	     jz p_not

	     mov cx,rep_num
	     mov ax,sseg
	     mov ds,ax
	     mov si,soff
	     mov ax,dseg
	     mov es,ax
	     mov di,doff
	     rep movsb
	     jmp p_end     }

p_not:  asm{ mov cx,rep_num
	     mov ax,sseg
	     mov ds,ax
	     mov si,soff
	     mov ax,dseg
	     mov es,ax
	     mov di,doff   }

repeat: asm{ mov al,ds:[si]
	     cmp al,00h
	     jz skip
	     mov es:[di],al }

skip:   asm{ inc si
	     inc di
	     loop repeat }

p_end:       asm pop ds;
}
