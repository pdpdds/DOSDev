// �A���� 0 - 0x0000 �A���� 1 - 0x4000 �A���� 2 - 0x8000 �A���� 3 - 0xc000
void setscanaddress(unsigned sa)
{
    asm mov dx,03dah
lp: asm { in al,dx
	  test al,08h
	  jz lp
	  mov bx,sa
	  mov dx,03d4h
	  mov al,0dh
	  mov ah,bl
	  out dx,ax
	  mov dx,03d4h
	  mov al,0ch
	  mov ah,bh
	  out dx,ax   }
}
