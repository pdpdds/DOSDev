// FLAMSCRL /////////////////////////////////////////////////////////////////

// Andreas 0.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <string.h>
#include <fstream.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>

#include "seg.h"
#include "xgraf.h"
#include "timer.h"

// DEFINES //////////////////////////////////////////////////////////////////

#define MINSPEED 150
#define ABSMIN	 90
#define WIDTH 40
#define HEIGHT 100
#define SCREEN 88
#define PAGEBASE (2*16000)
#define FLAME 1
#define FONTPOS (WIDTH*(HEIGHT+10))
#define MASKPOS (WIDTH*(HEIGHT+10)+8200)
#define LETTERSTART 4
#define FLAMSTART 0
#define FLAMEND   4
#define MOVSB mov al,[si]; asm mov es:[di],al; asm inc si; asm inc di

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void movef32(void);
extern "C" void smidigo(void);

// VARS

extern byte rgb[768],
       *pressed,
       callmidi,
       guson;
extern int relspeed;
extern timer_C timer;

// PROTOTYPES ///////////////////////////////////////////////////////////////

void compilemask(byte *stripe,byte *res);
void setpalette(void);
void xoffset(word xoff);
void setlogwidth(word w);
void fade(byte *plt);

// FUNCTIONS ////////////////////////////////////////////////////////////////

// FLAMSCRL

void flamscrl(void)
{
	char text[]=" WELCOME TO THE SUPERUNKNOWN    ";
  int tempdi,
      tempax,
      tempbx,
      tempcx,
      tempdx,
      tempsi;
	byte tempch,
	     fadepal[64*3],
       size=0,
       doit=1,
       dec_it=0,
       bokstavnr=0,
       blockpos=0,
       maskpos=0,
       index=0,
       blapal[768],
       randtab[256],
       frame_byte=0;
	word startframe=10,
       aa=0,
       pb=0,
       bokstavln=0,
       adr=0,
       frames=0;
	int x,
      y,
      delta;

	for(x=0;x<256;x++)
		randtab[x]=random(2)*16+48;

	seg_C flammeseg;
	byte *flamme=flammeseg.ptr;
	long p_P= (long) flamme;

	seg_C dataseg;
	byte *data=dataseg.ptr;
	long data_P= (long) data;

	seg_C lineseg(20);
	byte *linja=lineseg.ptr;

	word b;
		for(b=0;b<65535;b++)
		flamme[b]=0;

	FILE *brixfil = fopen(BRIXFILE, "rb" );

	byte cc;
	for(int yy=0;yy<16*14;yy++)
	for(int xx=0;xx<16;xx++)
	{
		fread(&cc,1,1,brixfil);
		data[adr]=cc;
		adr++;
	}
	fclose(brixfil);

	FILE *palfil = fopen(FLAMEPAL,"rb");
	fread(&rgb,1,32,palfil);
	fread(&rgb,1,768,palfil);
	fclose(palfil);

	for(x=0;x<768;x++) rgb[x]>>=2;

	adr=WIDTH*(HEIGHT+10);

	FILE *maskfil = fopen(MASKFILE,"rb");

	for(yy=0;yy<15*28;yy++)
	for(int xx=0;xx<19;xx++)
	{
		fread(&cc,1,1,maskfil);
		if (xx<16 && xx)
    {
			flamme[adr]=cc;
			data[adr]=cc;
			adr++;
		}
	}
	fclose(maskfil);

	outport(SC_INDEX,0x0f02);
	frame();

	asm mov ax,0xa000
	asm mov es,ax
	asm xor ax,ax
	asm xor di,di
	asm mov cx,32768
	asm rep stosb
	asm mov cx,32768
	asm rep stosb

	setlogwidth(320+32);
	setpalette();

	seg_C rleseg(35);
	byte *rle = rleseg.ptr;

	rle[0]=0;
	rle[1]=0;
  rle[2]=0;
	rle[3]=16;
	rle[4]=0;
	rle[6]=0;
  rle[5]=3*SCREEN;
	rle[7]=2*16;
	rle[8]=64;

	for(x=0;x<80;x++)
	for(y=0;y<31;y++)
		flamme[MASKPOS+y+x*31]=rle[y];

	while (text[size]!='\0')
	{
		if (text[size]==' ') text[size]='@';
		size++;
	}

  while(bokstavnr<size-1)
  {
	  frames++;
	  frame_byte+=17;
	  pb^=PAGEBASE;

		doit^=1;
		if(doit)
		{
			dec_it^=1;

			asm mov bh,dec_it
			asm push ds
			asm mov cx,((HEIGHT)+2)*WIDTH
			asm lds di,p_P
			asm add di,WIDTH
D1:
			asm mov ax,[di]
			asm add ah,al
			asm mov al,ah
			asm add ah,[di-1]
			asm add ah,[di+WIDTH]
			asm jz alla
			asm sub ah,bh
			asm shr ah,2
alla:
			asm xor bh,1
			asm add al,[di+2]
			asm add al,[di+WIDTH+1]
			asm jz ahla
			asm sub al,bh
			asm shr al,2
ahla:
			asm cmp al,64
			asm ja al_over
			asm jmp check_ah
al_over:
			asm mov al,63
check_ah:
			asm cmp ah,64
			asm ja ah_over
			asm jmp D2
ah_over:
			asm mov ah,63
D2:
			asm xchg al,ah
			asm mov [di-WIDTH],ax

			asm add di,2
			asm sub cx,2
			asm jz D3
			asm jmp D1
D3:
			asm pop ds

			for(int j=0;j<WIDTH/2+1;j++)
			{
				asm mov ax,j
				asm add al,frame_byte
				asm mov index,al
				if(frames>startframe && bokstavnr<(size-7))
					delta=randtab[index];
				else
					delta=0;

				flamme[(HEIGHT+1)*WIDTH+(j<<1)+1]=delta;
				flamme[(HEIGHT+1)*WIDTH+(j<<1)]  =delta;
			}
		}


	  blockpos=aa%4;

	  if(!blockpos)
	  {
		  bokstavln++;
		  bokstavln%=15;

		  if(!bokstavln)
		  {
			  bokstavnr++;
			  bokstavnr%=size;
		  }
		  bokstavln+=(text[bokstavnr]-64)*15l;
	  }

	  if(!(blockpos))
	  {
		  maskpos++;
		  maskpos%=15;
		  adr=maskpos*15l+((text[bokstavnr]-64)*15*15);
		  if (!adr)
        adr=15;
		  for(y=0;y<12;y++)
		  linja[y]=flamme[FONTPOS+adr+y+3-15];
		  compilemask(linja,rle);
	  }

	  for(y=0;y<31;y++)
	  {
		  flamme[MASKPOS+y+81*31]=rle[y];
		  flamme[MASKPOS+y+80*31]=rle[y];
	  }


	  asm pusha
	  asm push ds

	  asm lds si,flamme
	  asm add si,MASKPOS
	  movef32(); // From ASMZOOM.ASM

	  asm pop ds
	  asm popa


	  callmidi=0;
	  if (guson)
		  smidigo();
	  frame();
	  callmidi=guson;

	  byte startplan=0;
	  byte sluttplan=1;
	  byte span=3;

	  for(byte plan=startplan; plan<=sluttplan; plan++)
	  {
		  byte ahmask=span<<(plan*2);
		  for(byte start=FLAMSTART;start<FLAMEND;start++)
		  {
		    int retn = 2-4*(start&1);
		    int plussi=plan*(retn/2)+(WIDTH-1)*(start&1);

			  asm mov di,aa
			  asm add di,2
			  asm add di,pb
			  asm add di,SCREEN*240-SCREEN*HEIGHT*2
			  asm xor ax,ax
			  asm mov al,start
			  asm mov bl,WIDTH/2
			  asm mul bl
			  asm add di,ax

			  asm mov tempdi,di
			  asm mov ax,0xa000
			  asm mov es,ax
			  asm pusha
			  asm push ds
			  asm lds si,p_P
			  asm mov bx,si
			  asm add bx,MASKPOS

			  asm add si,plussi
			  asm xor ah,ah
			  asm mov al,start
			  asm mov dx,31*WIDTH/2
			  asm mul dx
			  asm add bx,ax

			  asm mov al,2
			  asm mov ah,ahmask
			  asm mov dx,SC_INDEX
			  asm out dx,ax
			  asm mov dh,WIDTH/2
			  asm mov tempch,ch
			  asm cld

s_newline:
			  asm mov ch,[bx]
			  asm or ch,ch
			  asm jnz okay
			  asm add di,1
			  asm add si,retn
			  asm add bx,31
			  asm dec dh
			  asm jz nomore
			  asm jmp s_newline
okay:
			  asm mov tempbx,bx
			  asm inc bx
			  asm mov tempdi,di
			  asm mov tempsi,si
s_newmask:
			  asm mov ax,[bx]
			  asm add di,ax
			  asm add bx,2
			  asm mov ax,[bx]
			  asm shr ax,1
			  asm add si,ax
			  asm add bx,2
			  asm mov cl,[bx]
			  asm inc bx
			  asm mov al,[bx]
			  asm inc bx
s_newpix:
			  asm mov ah,[si]
			  asm add si,WIDTH
			  asm or ah,al
			  asm mov es:[di],ah
			  asm add di,SCREEN
			  asm mov es:[di],ah
			  asm add di,SCREEN
			  asm dec cl
			  asm jnz s_newpix
			  asm dec ch
			  asm jnz s_newmask
			  asm mov bx,tempbx
			  asm add bx,31
			  asm mov si,tempsi
			  asm add si,retn
			  asm mov di,tempdi
			  asm inc di
			  asm dec dh
			  asm jnz s_newline
nomore:
			  asm pop ds
			  asm popa
		  }
	  }

	  if((blockpos<2))
	  {
		  word tempsomething;
		  asm mov ax,15
		  asm mov bx,bokstavln
		  asm mul bx
		  asm push ds
		  asm lds si,data_P
		  asm mov tempsomething,ax
		  asm mov di,SCREEN-LETTERSTART
		  asm add di,aa
		  asm add di,pb
		  asm mov ax,0xa000
		  asm mov es,ax
		  asm mov ax,0x0102
		  asm mov dx,SC_INDEX
		  asm mov cl,0
new_mask:
		  asm mov dx,SC_INDEX
		  asm mov al,02
		  asm out dx,ax
b0:
		  asm mov ch,0
b1:
		  asm mov dl,16
		  asm mov bx,WIDTH*(HEIGHT+10)
		  asm add bl,ch
		  asm add bx,tempsomething
		  asm mov si,bx
		  asm mov dh,[si]
		  asm cmp dh,'.'
		  asm jne b1_1
		  asm mov dh,0xc
		  asm jmp b2
b1_1:
		  asm cmp dh,' '
		  asm jne b1_2
		  asm mov dh,0xd
		  asm jmp b2
b1_2:
		  asm sub dh,49
		  asm cmp dh,10
		  asm jb  b2
		  asm sub dh,7
b2:
		  asm mov bl,dh
		  asm xor bh,bh
		  asm shl bx,8
		  asm add bl,cl
		  asm mov si,bx
new_b_pix:
		  asm MOVSB
		  asm add si,3
		  asm MOVSB
		  asm add si,3
		  asm MOVSB
		  asm add si,3
		  asm MOVSB
		  asm add si,3
		  asm add di,SCREEN-4
		  asm dec dl
		  asm jnz new_b_pix
		  asm inc ch
		  asm cmp ch,15
		  asm jne b1
		  asm cmp ah,8
		  asm je nomore2
		  asm sub di,SCREEN*240
		  asm shl ah,1
		  asm inc cl
		  asm jmp new_mask
nomore2:
		  asm pop ds

		  pb^=PAGEBASE;

		  asm mov ax,15
		  asm mov bx,bokstavln
		  asm mul bx
		  asm push ds
		  asm lds si,data_P
		  asm mov tempsomething,ax
		  asm mov di,SCREEN-LETTERSTART
		  asm add di,aa
		  asm add di,pb
		  asm mov ax,0xa000
		  asm mov es,ax
		  asm mov ax,0x0102
		  asm mov dx,SC_INDEX
		  asm mov cl,0
new_mask2:
		  asm mov dx,SC_INDEX
		  asm mov al,02
		  asm out dx,ax
b02:
		  asm mov ch,0
b12:
		  asm mov dl,16
		  asm mov bx,WIDTH*(HEIGHT+10)
		  asm add bl,ch
		  asm add bx,tempsomething
		  asm mov si,bx
		  asm mov dh,[si]
		  asm cmp dh,'.'
		  asm jne b1_12
		  asm mov dh,0xc
		  asm jmp b22
b1_12:
		  asm cmp dh,' '
		  asm jne b1_22
		  asm mov dh,0xd
		  asm jmp b22
b1_22:
		  asm sub dh,49
		  asm cmp dh,10
		  asm jb  b22
		  asm sub dh,7
b22:
		  asm mov bl,dh
		  asm xor bh,bh
		  asm shl bx,8
		  asm add bl,cl
		  asm mov si,bx
new_b_pix2:
		  asm MOVSB
		  asm add si,3
		  asm MOVSB
		  asm add si,3
		  asm MOVSB
		  asm add si,3
		  asm MOVSB
		  asm add si,3
		  asm add di,SCREEN-4
		  asm dec dl
		  asm jnz new_b_pix2
		  asm inc ch
		  asm cmp ch,15
		  asm jne b12
		  asm cmp ah,8
		  asm je nomore22
		  asm sub di,SCREEN*240
		  asm shl ah,1
		  asm inc cl
		  asm jmp new_mask2
nomore22:
		  asm pop ds

		  pb^=PAGEBASE;
	  }

	  aa++;
	  aa++;
	  b++;
	  b%=(256/4);
	  xoffset(aa+pb);
  }

thend:
	outport(SC_INDEX,0x0f02);
	frame();

	asm mov ax,0xa000
	asm mov es,ax
	asm xor ax,ax
	asm xor di,di
	asm mov cx,32768
	asm rep stosb
	asm mov cx,32768
	asm rep stosb

	setlogwidth(320);
	xoffset(0);
}

// SETPALETTE

void setpalette()
{
  int  i;

  _SI = (word)&rgb[0];
  asm mov cx,768
  asm mov dx,0x03c8
  asm xor al,al
  asm out dx,al
  asm inc dx
l1:
  asm outsb
  asm dec cx
  asm jnz l1
}

// XOFFSET

void xoffset(word xoff)
{
	asm mov ax,xoff
	asm mov bh,al
	asm mov ch,ah
	asm mov bl,0x0d
	asm mov cl,0x0c
	asm mov ah,8
	asm mov dx,CRTC_INDEX
	asm mov ax,bx
	asm cli
	asm out dx,ax
	asm mov ax,cx
	asm out dx,ax
	asm sti
}

// SETLOGWIDTH

void setlogwidth(word w)
{
	asm	mov   cx,w
	asm	mov   dx,CRTC_INDEX
	asm	mov   al,0x13
	asm	out   dx,al
	asm	inc   dx
	asm	mov   ax,cx
	asm	shr   ax,3
	asm	out   dx,al
}

// STRIPTYPE

byte striptype(byte in)
{
	if (in=='.')
		return 64;
	if (in==' ')
		return 0;
	return 255;
}

// COMPILEMASK

void compilemask(byte *stripe,byte *res)
{
	byte here;
	int treff=0;
	word last_one=0;
	word last_onef=0;
	for (int scan=0; scan<12; scan++)
	{
		here=striptype(stripe[scan]);
		if (relspeed<ABSMIN)
			here=255;
		if (here<255 && (here || relspeed>MINSPEED))
		{
			word adr=SCREEN*16*(scan)+SCREEN*8;
			word adrf=WIDTH*16*(scan)+WIDTH*8;
			res[1+treff*6]=(adr-last_one);
			res[2+treff*6]=(adr-last_one)>>8;
			res[3+treff*6]=(adrf-last_onef);
			res[4+treff*6]=(adrf-last_onef)>>8;
			byte len=0;
			while (striptype(stripe[scan])==here && scan<12)
			{
				len++;
				scan++;
			}
			res[5+treff*6]=len*8;
			res[6+(treff++)*6]=here;
			last_one=adr+SCREEN*len*16;
			last_onef=adrf+WIDTH*len*16;
		}
	}
	res[0]=treff;
}

// FADE

void fade(byte *plt)
{
  _SI = (word) &plt[0];

  asm mov cx,64*3
  asm mov dx,0x03c8
  asm mov al,64
  asm xor ah,ah
  asm out dx,al
  asm inc dx
l1:
  asm outsb
  asm dec cx
  asm jnz l1
}

