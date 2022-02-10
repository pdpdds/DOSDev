#define _MOUSE

#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <conio.h>
#include <mem.h>
#include <dos.h>

#include "est.h"
#include "21.h"
#include "proto.h"
#include "arrow1.h"

#define AND		0
#define OR		1

#define COPY		2
#define ON  		1
#define OFF 		0

#define PRESS 		0
#define RELEASE 	1
#define PC_MOUSE 1
#define MS_MOUSE 2

#define TRUE      	0xFF
#define INDEXREGS 	0x3CE
#define VALREGS 	0x3CF
#define ENABLE  	0x0F

typedef struct {
	BYTE rb  : 1;
	BYTE mb  : 1;
	BYTE lb	 : 1;
	BYTE non : 5;
} but;

union mouse_button {
	but 	b;
	BYTE  c;
} mouseb;

typedef struct {
	BYTE bit0 :1;
	BYTE bit1 :1;
	BYTE bit2 :1;
	BYTE bit3 :1;
	BYTE bit4 :1;
	BYTE bit5 :1;
	BYTE bit6 :1;
	BYTE bit7 :1;
}BITS;

typedef union {
	BITS b;
	BYTE ch;
}B;

B b1;
extern BYTE *vid_addr;
extern WORD offsetx[];
extern int MaxX, MaxY;
extern BYTE color_mode;

//MOON
//extern WORD far mport;

extern WORD no_work;

BYTE mouse_kind;
BYTE mouse_port;
BYTE MOUSE_PORT;
BYTE button=0;
BYTE rbp=0;
BYTE lbp=0;
int mousex, mousey;
int oldx, oldy;
BYTE mouse_on_off=OFF;
BYTE mouse_installed=OFF;

int clip_minx,clip_miny,clip_maxx,clip_maxy;

#define TMBUFSIZE 40
char tmbuf[TMBUFSIZE];
char temp;
BYTE count=0;

void interrupt (*oldhandler)(void);
unsigned base;

int xm=0, ym=0;
char cursor_kind=1;
BYTE buf16x16[300];
BYTE cursor[64];
BYTE bits;
BYTE buadlo,buadhi;
BYTE irqset;
BYTE irqset2;
int m_rate=6;
BYTE mc_color=15;

BYTE near lshift_table[8]={0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
BYTE near rshift_table[9]={0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

//MOON
//void serisr(void);
//signed char readbuf(void);

static void near vClipPutImageBit(WORD x, WORD y, BYTE *buf)
{
	register WORD i;
	BYTE restr, restl;
	BYTE *offset;
	WORD xmj=0;

	if(MaxX<x+15) xmj=(x+15-MaxX)<<1;
	restl=y & 0x0007;
	restr=8-restl;
	y=y>>3;
	if(y<(MaxY>>3)-1) {
		if(color_mode) {
			OUTPUT(0, 0);
			OUTPUT(1, 0x0f);
			OUTPUT(3, 0x08);
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				OUTPUT(8, ~((buf[i] >> restl) | lshift_table[restl]));
				*offset++&=0xff;
				OUTPUT(8, ~((buf[i++] << restr) | rshift_table[restr]));
				*offset&=0xff;
				OUTPUT(8, ~((buf[i] >> restl) | lshift_table[restl]));
				*offset++&=0xff;
				OUTPUT(8, ~((buf[i++] << restr) | rshift_table[restr]));
				*offset&=0xff;
			}
		}
		else {
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				*offset++&=((buf[i] >> restl) | lshift_table[restl]);
				*offset&=((buf[i++] << restr) | rshift_table[restr]);
				*offset++&=((buf[i] >> restl) | lshift_table[restl]);
				*offset&=((buf[i++] << restr) | rshift_table[restr]);
			}
		}
		buf+=32;
		x=x-(i>>1);
		if(color_mode) {
			OUTPUT(0, mc_color);
			OUTPUT(1, 0x0f);
			OUTPUT(3, 0x10);
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				OUTPUT(8, buf[i] >> restl);
				*offset++&=0xff;
				OUTPUT(8, buf[i++] << restr);
				*offset&=0xff;
				OUTPUT(8, buf[i] >> restl);
				*offset++&=0xff;
				OUTPUT(8, buf[i++] << restr);
				*offset&=0xff;
			}
		}
		else {
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				*offset++|= buf[i] >> restl;
				*offset|= buf[i++] << restr;
				*offset++|= buf[i] >> restl;
				*offset|= buf[i++] << restr;
			}
		}
	}
	else {
		if(color_mode) {
			OUTPUT(0, 0);
			OUTPUT(1, 0x0f);
			OUTPUT(3, 0x08);
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				OUTPUT(8, ~((buf[i] >> restl) | lshift_table[restl]));
				*offset++ &= 0xff;
				if(y==(MaxY>>3)-1) {
					OUTPUT(8, ~((buf[i++] << restr) | rshift_table[restr]));
					*offset &= 0xff;
					OUTPUT(8, ~((buf[i] >> restl) | lshift_table[restl]));
					*offset &= 0xff;
				}
				else i++;
				i++;
			}
		} else {
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				*offset++ &=((buf[i] >> restl) | lshift_table[restl]);
				if(y==(MaxY>>3)-1) {
					*offset &=((buf[i++] << restr) | rshift_table[restr]);
					*offset &=((buf[i] >> restl) | lshift_table[restl]);
				}
				else i++;
				i++;
			}
		}
		buf+=32;
		x=x-(i>>1);
		if(color_mode) {
			OUTPUT(0, mc_color);
			OUTPUT(1, 0x0f);
			OUTPUT(3, 0x10);
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				OUTPUT(8, buf[i] >> restl);
				*offset++&=0xff;
				if(y==(MaxY>>3)-1){
					OUTPUT(8, buf[i++] << restr);
					*offset &=0xff;
					OUTPUT(8, buf[i] >> restl);
					*offset &=0xff;
				}
				else i++;
				i++;
			}
		}
		else {
			for(i=0 ;i<32-xmj;) {
				offset=vid_addr+offsetx[x++]+y;
				*offset++|=buf[i] >> restl;
				if(y==(MaxY>>3)-1){
					*offset|=buf[i++] << restr;
					*offset|=buf[i] >> restl;
				}
				else i++;
				i++;
			}
		}
	}
	OUTPUT(0, 0);
	OUTPUT(1, 0);
	OUTPUT(3, 0);
	OUTPUT(8, TRUE);
}

static void near vGetColorImageEByte(WORD x, WORD y, WORD endx, WORD endy, BYTE *buf)
{
	int i, j, c;
	BYTE *offset;
	BYTE *temp;

	if(!color_mode) {
		vGetImage(x, y, endx, endy, buf);
		return;
	}
	temp=buf;
	OUTPUT(5, 0);
	for(c=0;c<4;c++) {
		OUTPUT(4, c);
//		vGetImage(x, y, endx, endy, temp);
//		temp+=i;

		for(i=x ; i<endx+1 ;i++) {
			offset=vid_addr+offsetx[i]+y;
			for(j=y; j<endy+1 ; j++) *temp++=*offset++;
		}

	}
	outp(0x3cf, 0);

}

static void near vPutColorImageEByte(WORD x, WORD y, WORD endx, WORD endy, BYTE *buf)
{
	int i, j, c;
	BYTE *offset;
	BYTE *temp;

	if(!color_mode) {
		vPutImage(x, y, endx, endy, buf);
		return;
	}
	temp=buf;
	OUTPUT(8, 0xff);
	OUTPUT(5, 0);
	outp(0x3c4, 2);
	for(c=0;c<4;c++) {
		outp(0x3c5, 1<<c);
//		vPutImage(x, y, endx, endy, temp);
//		temp+=i;

		for(i=x ;i<endx+1;i++) {
			offset=vid_addr+offsetx[i]+y;
			for(j=y ; j<endy+1 ; j++) *offset++ = *temp++;
		}

	}
	outp(0x3c5, 0xf);

}

void vPutColorImageFarEByte(int x, int y, int ex, int ey, BYTE *buf)
{
	vPutColorImageEByte(x, y, ex, ey, buf);
}

static void near vMouseCursorOperation(int x, int y, BYTE flag)
{
	x-=xm;
	y-=ym;
	if(mouse_on_off==OFF) return;
	if(flag==ON) {
		vGetColorImageEByte(x, (y>>3), x+15, (y>>3)+2, buf16x16);
		vClipPutImageBit(x, y, cursor);
	}
	else {
		vPutColorImageEByte(x, (y>>3), x+15, (y>>3)+2, buf16x16);
	}
}

void vMCON(void)
{
	if(mouse_installed==OFF || mouse_on_off==ON) return;
	disable();
	mouse_on_off=ON;
	vMouseCursorOperation(mousex, mousey, ON);
	enable();
}

void vMCOFF(void)
{
	if(mouse_installed==OFF || mouse_on_off==OFF) return;
	disable();
	vMouseCursorOperation(mousex, mousey, OFF);
	mouse_on_off=OFF;
	enable();
}

void vSetMousePosition(int x, int y)
{
	BYTE temp;
	union REGS r;

	temp=mouse_on_off;
	vMCOFF();
	if(mouse_installed==2) {
		r.x.ax=4;
		r.x.cx=y;
		r.x.dx=x;
		int86(0x33, &r, &r);
		oldx=mousex=x;
		oldy=mousey=y;
	}
	else {
		disable();
		oldx=mousex=x;
		oldy=mousey=y;
		enable();
	}
	if(temp) vMCON();
}

void vGetMousePosition(int *x,int *y)
{
	disable();
	*x=mousex;
	*y=mousey;
	enable();
}

void vChangeMC(int x, int y, BYTE *i)
{
	BYTE temp;

	temp=mouse_on_off;
	vMCOFF();
	disable();
	memcpy(cursor, i, 64);
	xm=x;
	ym=y;
	enable();
	if(temp==ON) vMCON();
}


void vSetRange(int x, int y, int endx, int endy)
{
	int temp;
	union REGS r;

	temp=mouse_on_off;
	vMCOFF();   // all is bit unit
	if(mouse_installed==2) {
		r.x.ax=7;
		r.x.cx=y;
		r.x.dx=endy;
		int86(0x33, &r, &r);
		r.x.ax=8;
		r.x.cx=x;
		r.x.dx=endx;
		int86(0x33, &r, &r);
	}
	else {
		disable();
		clip_minx=x;
		clip_miny=y;
		clip_maxx=endx;
		clip_maxy=endy;
		enable();
	}
	if(temp==ON)vMCON();
}

void vRestoreRange(void)
{
	vSetRange(0, 0, MaxX, MaxY);
}

void vSetMouseSpeed(WORD ratio)
{
	union REGS r;
	if(mouse_installed==2) {
		r.x.ax=15;
		r.x.cx=ratio;
		r.x.dx=ratio;
		int86(0x33, &r, &r);
	}
}

BYTE pcmflag=0;

int t_rate;

void interrupt vPC_MouseInt(void)
{
	disable();

	no_work=OFF;
//	serisr();
//	tmbuf[count]=(signed char)readbuf();
	if(!pcmflag) {
		if((tmbuf[0] & 0xf8) == 0x80 || count>0) count++;
	}
	else count++;
	if(!pcmflag && count==3) {
		pcmflag=1;
		count=0;
		oldx=mousex;
		oldy=mousey;
		rbp=(tmbuf[0] & 1) ^ 1;
		lbp=((tmbuf[0]>>2) & 1) ^ 1;
		mousex = mousex-tmbuf[2];

		if(mousex>clip_maxx) mousex=clip_maxx;
		if(mousex<clip_minx) mousex=clip_minx;

		mousey = mousey+tmbuf[1];

		if(mousey>clip_maxy) mousey=clip_maxy;
		if(mousey<clip_miny) mousey=clip_miny;

		if(oldx!=mousex || oldy!=mousey) {
			vMouseCursorOperation(oldx, oldy, OFF);
			vMouseCursorOperation(mousex, mousey, ON);
		}
	}
	else if(pcmflag && count==2) {
		count=0;
		pcmflag=0;
		oldx=mousex;
		oldy=mousey;

		t_rate=tmbuf[1];
		if(abs(t_rate)>3) {
			t_rate=t_rate+(t_rate*3)/m_rate;
		}
		mousex = mousex-t_rate;

		if(mousex>clip_maxx) mousex=clip_maxx;
		if(mousex<clip_minx) mousex=clip_minx;

		t_rate=tmbuf[0];
		if(abs(t_rate)>3) {
			t_rate=t_rate+(t_rate*3)/m_rate;
		}
		mousey = mousey+t_rate;

		if(mousey>clip_maxy) mousey=clip_maxy;
		if(mousey<clip_miny) mousey=clip_miny;

		if(oldx!=mousex || oldy!=mousey) {
			vMouseCursorOperation(oldx, oldy, OFF);
			vMouseCursorOperation(mousex, mousey, ON);
		}
	}
	enable();
}

void interrupt vMS_MouseInt(void)
{
	disable();

	no_work=OFF;
//	serisr();
//	tmbuf[count]=(signed char)readbuf();
	if(count==4 && (tmbuf[4] & 0xc0) != 0xc0) {
		count=3;
		enable();
		return;
	}
	count++;
	if(count==6) {
		count=3;
		oldx=mousex;
		oldy=mousey;
		b1.ch=tmbuf[4];
		lbp=b1.b.bit5;
		rbp=b1.b.bit4;
		tmbuf[5]=tmbuf[5] & 0x3f;
		tmbuf[3]=tmbuf[3] & 0x3f;

		t_rate=(signed char)(tmbuf[3] | ((temp & 0x0c)<<4));
		if(abs(t_rate)>3) {
			t_rate=t_rate+(t_rate*3)/m_rate;
		}
		mousex = mousex+t_rate;

		if(mousex>clip_maxx) mousex=clip_maxx;
		if(mousex<clip_minx) mousex=clip_minx;

		t_rate=(signed char)(tmbuf[5] | (tmbuf[4]<<6));
		if(abs(t_rate)>3) {
			t_rate=t_rate+(t_rate*3)/m_rate;
		}
		mousey = mousey+t_rate;

		if(mousey>clip_maxy) mousey=clip_maxy;
		if(mousey<clip_miny) mousey=clip_miny;

		temp=tmbuf[4];
		if(oldx!=mousex || oldy!=mousey) {
			vMouseCursorOperation(oldx, oldy, OFF);
			vMouseCursorOperation(mousex, mousey, ON);
		}
	}
	enable();
}

BYTE irqset2;

static void instisr(void interrupt(*service)(void), unsigned intnum)
{
	oldhandler=getvect(intnum);
	outp((base)+1, 1);
	irqset=inp(0x21);
	if(intnum==12) outp(0x21, irqset & ~0x10);
	else if(intnum==11) outp(0x21, irqset & ~0x08);
	irqset2=inp(0x21);
	setvect(intnum, service);
	outp((base)+4, inp((base)+4) | 8);
	inp(base);
	inp(base);
	inp(base);
}

void vInstallSerialInt(void)
{
	clip_minx=0; clip_miny=0; clip_maxx=MaxX; clip_maxy=MaxY;

	outp((base)+4, inp((base)+4) & ~3);

	bits=inp((base)+3);
	bits |= 0x80;
	outp(base+3, bits);
	outp(base+3, 0x83);

	buadlo = (inp(base + 0));
	buadhi = (inp(base + 1));

	outp(base, 0x60);
	outp(base+1, 0x00);
	outp(base+3, inp(base+3) & 0x7f);
	if(mouse_kind==PC_MOUSE) instisr(vPC_MouseInt, MOUSE_PORT);
	else if(mouse_kind==MS_MOUSE) instisr(vMS_MouseInt,MOUSE_PORT);
	outp(base+4, inp(base+4) | 3);
}

void huge vMouseIntService(void)
{
	disable();
	button=_BX;
	mousex=_DX;
	mousey=_CX;
	lbp=button&1;
	rbp=(button>>1)&1;
	no_work=OFF;
	if(oldx!=mousex || oldy!=mousey) {
		vMouseCursorOperation(oldx, oldy, OFF);
		vMouseCursorOperation(mousex, mousey, ON);
		oldx=mousex;
		oldy=mousey;
	}
	enable();
}

void vInstallEventHandler(void huge *userfunc, int mask)
{
	union REGS r;
	struct SREGS sr;

	r.x.ax=12;
	r.x.cx=mask;
	r.x.dx=FP_OFF(userfunc);
	sr.es=FP_SEG(userfunc);
	int86x(0x33, &r, &r, &sr);
}


void vDeinitMouse(void)
{
	union REGS r;
	struct SREGS sr;

	if(mouse_installed==OFF) return;
	if(mouse_installed==2) {
		r.x.ax=12;
		r.x.cx=0;
		r.x.dx=0;
		sr.es=0;
		int86x(0x33, &r, &r, &sr);
	}
	else {
		if(MOUSE_PORT==12) {
			if(irqset & 0x10) {
				outp(base + 4, inp(base + 4) & ~0x08);
				outp(0x21, inp(0x21) | 0x10);
				outp(base+1, 0);
			}
		}
		else {
			if(irqset & 0x08) {
				outp(base + 4, inp(base + 4) & ~0x08);
				outp(0x21, inp(0x21) | 0x08);
				outp(base+1, 0);
			}
		}
		setvect(MOUSE_PORT, oldhandler);
		outp(base + 3, bits);
		outp(base + 0, buadlo);
		outp(base + 1, buadhi);
		outp(base + 3, inp(base+3) & 0x7f);
	}
}

int *PORT1=(int *)0x00000400L;
int *PORT2=(int *)0x00000402L;

int iInitMouse(BYTE kind, BYTE port)
{
	union REGS r;
	mouse_installed=OFF;
	mouse_kind=kind;
	mouse_port=port;

	r.x.ax=0;
	int86(0x33, &r, &r);
	if((int)r.x.ax==-1) {     // -1 means mouse is,0 means mouse isn't
		mouse_installed=2;
		mouse_kind=kind;
		mouse_port=port;
		vChangeMC(0, 0, m_cursor[0]);
		vInstallEventHandler(vMouseIntService, 0x1f);
		vRestoreRange();
		return -1;
	}
	if(mouse_kind==0) return -1;
	if(port==1) {
		MOUSE_PORT=12;
//		base=mport=*(PORT1);
		base=*(PORT1);
	}
	else if(port==2) {
		MOUSE_PORT=11;
//		base=mport=*(PORT2);
		base=*(PORT2);
	}
	else return 0;
	vChangeMC(0, 0, m_cursor[0]);
	setmem(tmbuf, TMBUFSIZE, 0);
	vInstallSerialInt();
	if(MOUSE_PORT==12) {
		if(irqset & 0x10) {
			if(irqset2 & 0x10) return 100;
		}
	}
	else {
		if(irqset & 0x08) {
			if(irqset2 & 0x08) return 100;
		}
	}
	mouse_installed=ON;
	return -1;
}

