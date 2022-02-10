#define  _OVLMAN
#include <stdio.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <mem.h>
#include <alloc.h>
#include "est.h"
	#include "21.h"
	#include "proto.h"

void far *OVLaddr=0;
BYTE far *OVLorigin=0;

void interrupt (*oint60)(void);
void interrupt our_int60(WORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD);
void vInitINT60(void)
{
	oint60=getvect(0x60);
	setvect(0x60, our_int60);
}

void vDeinitINT60(void)
{
	setvect(0x60, oint60);
}

extern unsigned long int Scount;

#pragma warn -par
void interrupt our_int60(bp, di, si, ds, es, dx, cx, bx, ax)
WORD bp, di, si, ds, es, dx, cx, bx, ax;
{
	switch(ax) {
		case 0 : OVLaddr=MK_FP(cx, dx); break;
		case 1 :
			cx=FP_SEG(vErrorMsgfDlg);
			dx=FP_OFF(vErrorMsgfDlg);
			break;
		case 2 :
			cx=FP_SEG(ihUserChoiceDlg);
			dx=FP_OFF(ihUserChoiceDlg);
			break;
		case 3 :
			cx=FP_SEG(&Scount);
			dx=FP_OFF(&Scount);
			break;
		//  here must gives address of address in '21st century'
		//  by moudule such as graphics, mouse, window, text, math, etc.
		default : break;
	}
}
#pragma warn +par

long lGetFileSize(BYTE *path)
{
	 int handle;
	 long size=0;

	 handle = open(path, O_BINARY);
	 size=filelength(handle);
	 close(handle);
	 if(size==-1) size=0;
	 return size;
}

void huge *pLoadModule(BYTE *path)  // load a overlay module
{
	int arg[2];
	union REGS r;
	struct SREGS s;
	char far *func;
	long size;

	size=lGetFileSize(path);
	if(size>(WORD)0xefff || size==0) {
		vErrorMsgfDlg(0, 4, "21­A‹¡¶w Overlay");
		return 0;
	}
	func=(char far *)pGetMem((WORD)size);
	if(func==NULL) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	arg[0]=FP_SEG(func)+1;
	arg[1]=FP_SEG(func)+1;

	r.h.ah=0x4b;
	r.h.al=3;
	r.x.bx=FP_OFF(arg);
	r.x.dx=FP_OFF(path);

	s.ds=FP_SEG(path);
	s.es=FP_SEG(arg);
	intdosx(&r, &r, &s);
	if(r.x.ax==0x01 || r.x.ax==0x02 || r.x.ax==0x08) {
		vErrorMsgfDlg(0, 5, "Overlay");
		return 0;
	}
	OVLorigin=func;
	return (char far *)MK_FP(arg[0], 0);
}

void far *pInitOverlay(BYTE *path) // get overlay functions address
{
	void huge (*tfunc)(void);

	tfunc=pLoadModule(path);
	if(tfunc==0) return 0;
	OVLaddr=(BYTE *)0;
	(*tfunc)();                  // initialize function, set func number and
	return OVLorigin;            // address of overlay moudule(used when free)
}

void huge *pGetModuleAddr(void)
{
	return (void huge *)OVLaddr;
}
