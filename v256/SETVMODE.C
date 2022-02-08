void setvmode(char mode)
{
	asm{ mov ah,00h
	     mov al,mode
	     int 10h    }
}
