void monitor_onoff(char onoff)
{
	asm{ mov ah,12h
	     mov al,onoff
	     mov bl,36h
	     int 10h     }
}

