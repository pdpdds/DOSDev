// XGRAF.CPP ////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <dos.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "global.h"
#include "template.h"
#include "xgraf.h"
#include "timer.h"

// DEFINES //////////////////////////////////////////////////////////////////

#define NUMBLOCKS 10
#define BLOCKLEN   12
#define PAUSE      30

// EXTERNALS ////////////////////////////////////////////////////////////////

extern timer_C timer;
extern byte rgb[768];

// FUNCTIONS ////////////////////////////////////////////////////////////////

// MODEX

void modex(int not13)
{
	frame();
	if (!not13)
	{
		_AX=0x0013;
		VGAINT;
	}
	frame();

	outportb(0x3c8,0);
	for (int count=0; count<768; count++)
		outportb(0x3c9,0);
	outport(SC_INDEX,0x0604);
	outport(SC_INDEX,0x0100);
	outportb(MISC_OUTPUT,0xe3);
	outport(SC_INDEX,0x0300);
	outportb(CRTC_INDEX,0x11);
	_AL=inportb(CRTC_INDEX+1)&0x7f;
	outportb(CRTC_INDEX+1,_AL);
	outport(CRTC_INDEX,0x0d06);
	outport(CRTC_INDEX,0x3e07);
	outport(CRTC_INDEX,0x4109);
	outport(CRTC_INDEX,0xea10);
	outport(CRTC_INDEX,0xac11);
	outport(CRTC_INDEX,0xdf12);
	outport(CRTC_INDEX,0x0014);
	outport(CRTC_INDEX,0xe715);
	outport(CRTC_INDEX,0x0616);
	outport(CRTC_INDEX,0xe317);
	outport(SC_INDEX,0x0f02);

	asm mov ax,0xa000
	asm mov es,ax
	asm xor ax,ax
	asm xor di,di
	asm mov cx,32768
	asm rep stosw
}

// TEXTMODE

void textmode(void)
{
	_AX=3;
	VGAINT;
}

// PUTPIXEL

void putpixel(int x,int y,byte color)
{
	if (x<0 || x>=320)
		return;
	asm mov ax,80
	asm mul y
	asm mov bx,x
	asm mov cx,bx
	asm shr bx,1
	asm shr bx,1
	asm add bx,ax
	asm mov ax,0xa000
	asm mov es,ax

	asm and cl,0x03
	asm mov ax,0x0100+MAP_MASK
	asm shl ah,cl
	asm mov dx,SC_INDEX
	asm out dx,ax

	asm mov al,color
	asm mov es:[bx],al
}

// GETPIXEL

byte getpixel(int x,int y)
{
	if (x<0 || x>=320)
		return 0;
	asm mov ax,80
	asm mul y
	asm mov bx,x
	asm mov cx,bx
	asm shr bx,1
	asm shr bx,1
	asm add bx,ax
	asm mov ax,0xa000
	asm mov es,ax
	asm mov ah,cl
	asm and ah,0x03
	asm mov al,READ_MAP
	asm mov dx,GC_INDEX
	asm out dx,ax
	asm mov al,es:[bx]
  return _AL; // Return value already in AL
}

// UPDATEPALETTE

void updatepalette(byte *pal)
{
	outportb(0x3c8,0);
	for (int count=0; count<768; count++)
	{
		if (!(count&15))
			frame();
		outportb(0x3c9,pal[count]);
	}
}

// YOFFSET

void yoffset(int yoff)
{
	static int lastyoff=28712;
	if (yoff==lastyoff)
    return;
  lastyoff=yoff;
  asm mov ax,80
  asm mul yoff
  asm mov bh,al
  asm mov ch,ah
  asm mov bl,0x0d
	asm mov cl,0x0c
	asm mov ah,1

	asm mov dx,0x3da
vent:
	asm in al,dx
	asm test al,ah
	asm jnz vent

	asm mov dx,CRTC_INDEX
	asm mov ax,bx
	asm cli
	asm out dx,ax
	asm mov ax,cx
	asm out dx,ax
	asm sti
}

// FRAME

void frame(void)
{
  while (inportb(0x3da)&8);
  while (!(inportb(0x3da)&8));
}

// SETRGB

void setrgb(byte color, byte red,byte green,byte blue)
{
	outportb(0x3c8,color);
	outportb(0x3c9,red);
  outportb(0x3c9,green);
  outportb(0x3c9,blue);
}

// BESTFIT

byte bestfit(int resr, int resg, int resb)
{
	long best=100000000l;
	byte hit,*rp2=rgb;
	for (int test=0; test<256; test++)
	{
		int r2=*(rp2++);
		r2<<=8;
		long bomr=resr-r2;

		int g2=*(rp2++);
		g2<<=8;
		long bomg=resg-g2;

		int b2=*(rp2++);
		b2<<=8;
		long bomb=resb-b2;

		bomb*=bomb;
		bomr*=bomr;
		bomg*=bomg;
		long sumbom=bomr+bomg+bomb;
		if (sumbom<best)
		{
			best=sumbom;
			hit=test;
			if (!best)
				test=256;
		}
	}
	return hit;
}

// SETPALETTE

void setpalette(char *filnavn)
{
	FILE *infile=fopen(filnavn,"rb");
	fread(rgb,1,768,infile);
	updatepalette(rgb);
	fclose(infile);
}

// PUTABSPIXEL

void putabspixel(word adr,byte plan, byte color,word sseg)
{
	asm mov ax,sseg
	asm mov es,ax
	asm mov cl,plan

	asm and cl,0x03
	asm mov ax,0x0100+MAP_MASK
	asm shl ah,cl
	asm mov dx,SC_INDEX
	asm out dx,ax
	asm mov al,color
	asm mov bx,adr
	asm mov es:[bx],al
}

// BYTT0

void switch0(word from, word to)
{
	to=19200;
	from=0;
	yoffset(to/80);
	int full[NUMBLOCKS];
	for (int count=0; count<NUMBLOCKS; count++)
		full[count]=1*(count-NUMBLOCKS-1);
	asm pusha
	asm push ds
  asm cld
	do
	{
		word adr=to;
		word fromadr=from;
		for (int BLOCK=0; BLOCK<NUMBLOCKS; BLOCK++)
		{
			int f=full[BLOCK];
			if (f>=0 && f<BLOCKLEN)
			{
				if (f)
				{
					asm mov ax,0xa000
					asm mov es,ax
					asm mov ds,ax

					word tadr=adr+80*(BLOCKLEN-f-1);
					int ff=f*80;
					asm mov di,tadr
					asm mov si,di
					asm add si,80
					asm mov cx,ff
					asm rep movsb
				}
				word pl=fromadr+80*f;
				asm mov ax,0xa000
				asm mov es,ax
				asm mov ds,ax
				asm mov di,adr
				asm add di,80*(BLOCKLEN-1);
				asm mov si,pl
				asm mov cx,80
				asm rep movsb
			}
			full[BLOCK]++;
			adr+=80*BLOCKLEN;
			fromadr+=80*BLOCKLEN;
		}
		frame();
		frame();
	}while (full[0]<BLOCKLEN);
	asm pop ds
	asm popa
}

void switch1(word from, word to)
{
	to=19200;
	from=0;
	yoffset(to/80);
	int full[NUMBLOCKS];
	for (int count=0; count<NUMBLOCKS; count++)
		full[count]=1*(BLOCKLEN+count);
	asm pusha
	asm push ds
	asm cld
	do
	{
		long tid=timer.readtimer();
		word adr=to;
		word fromadr=from;
		for (int BLOCK=0; BLOCK<NUMBLOCKS; BLOCK++)
		{
			int f=full[BLOCK];
			if (f>0 && f<=BLOCKLEN)
			{
				for (int ned=0; ned<f-1; ned++)
				{
					asm mov ax,0xa000
					asm mov es,ax
					asm mov ds,ax

					word tadr=adr+80*(BLOCKLEN-ned)-80;
					asm mov di,tadr
					asm mov si,di
					asm add si,-80
					asm mov cx,80
					asm rep movsb
				}
				word pl=fromadr+80*(BLOCKLEN-f);
				word pl0=adr+80*(BLOCKLEN-f);
				asm mov ax,0xa000
				asm mov es,ax
				asm mov ds,ax
				asm mov di,pl0
				asm mov si,pl
				asm mov cx,80
				asm rep movsb
			}
			full[BLOCK]--;
			adr+=80*BLOCKLEN;
			fromadr+=80*BLOCKLEN;
		}
		while (timer.elapsed(tid,timer.readtimer())<PAUSE);
	}while (full[NUMBLOCKS-1]);
	asm pop ds
	asm popa
}

// SWITCHPAGE

void switchpage(word from,word to,byte method)
{
	asm mov dx,GC_INDEX
	asm mov ax,BIT_MASK
	asm out dx,ax
	asm mov dx,SC_INDEX+1
	asm mov al,0x0f
	asm out dx,al

	switch(method)
	{
		case 0: switch0(from,to); break;
		case 1: switch1(from,to); break;
	}

	asm mov dx,GC_INDEX+1
	asm mov al,0xff
	asm out dx,al
}

