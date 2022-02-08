/********************************************************/
/* Keyboard processing...                               */
/********************************************************/
#include <dos.h>

int ESC,CTRL,LSHIFT,ALT,SPACE,UP,DOWN,LEFT,RIGHT;

void interrupt (*oldhandler)(void);

void interrupt handler(void)
{
unsigned far *key=(unsigned far *)1050;

disable();
switch(inportb(0x60)){
	case 1:  ESC=1;    break;   case 1+128:  ESC=0;    break;
	case 29: CTRL=1;   break;   case 29+128: CTRL=0;   break;
	case 42: LSHIFT=1; break;   case 42+128: LSHIFT=0; break;
	case 56: ALT=1;    break;   case 56+128: ALT=0;    break;
	case 57: SPACE=1;  break;   case 57+128: SPACE=0;  break;
	case 72: UP=1;     break;   case 72+128: UP=0;     break;
	case 80: DOWN=1;   break;   case 80+128: DOWN=0;   break;
	case 75: LEFT=1;   break;   case 75+128: LEFT=0;   break;
	case 77: RIGHT=1;  break;   case 77+128: RIGHT=0;  break;
        default:;
		     }
*key=*(key+1);
enable();
(*oldhandler)();
}

void setkeyint(void)
{
   pokeb(0,0x417,0x00);
   asm{ mov ah,03h
        mov al,05h
        mov bx,001fh
        int 16h
      }
   oldhandler = getvect(9);
   setvect(9,handler);
}

void resetkeyint(void)
{
   setvect(9,oldhandler);

/* keyboard reset routine
   asm{ in al,61h
        mov ah,al
        or al,80h
        out 61h,al
        mov al,ah
        out 61h,al  */
  asm{
        mov ah,03h
        mov al,05h
        mov bx,0000h
        int 16h
      }
   pokeb(0,0x417,0x20);
}


