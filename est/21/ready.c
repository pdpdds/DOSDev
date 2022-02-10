#define _READY
#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <process.h>
#include "est.h"
	#include "21.h"
	#include "proto.h"

extern BYTE *vid_addr;
extern WORD offsetx[];
extern WORD MaxX, MaxY;
extern BYTE g_dir[];
extern BYTE *our_buf;
extern BYTE font_path[];
extern BYTE swp_path[];
extern BYTE exe_path[];
extern BYTE vft_path[];
extern BYTE mouse_kind;
extern BYTE mouse_port;
extern text_window tw[];
extern BYTE cur_w;
extern BYTE dosing;
//extern unsigned long int SYSPEED;
extern int mousex;
extern int mousey;
extern BYTE lbp;
extern BYTE rbp;

extern int poption[];
extern BYTE pname[];

void interrupt (*old_int66)(void);
void interrupt our_int66(WORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD);
BYTE *int_loc;

BYTE Ext_buf[30];
BYTE E_flag=0;

BYTE bCheck21(void)
{
	union REGS in;
	BYTE tbl[6]={0xb4, 50, 0xb4, 49, 0xb4, 67};
	int i;

	if((int_loc=(BYTE *)getvect(0x66))!=NULL) {
		for(i=0;i<6;i++) {
			if(int_loc[19+i]!=tbl[i]) return 0;
		}
		in.x.ax=0;
		int86(0x66, &in, &in);
		if(in.x.ax==22 || in.x.ax==21) return 1;
	}
	return 0;
}

void vInitINT66(void)
{
	old_int66=getvect(0x66);
	setvect(0x66, our_int66);
}

void vDeinitINT66(void)
{
	setvect(0x66, old_int66);
}

int C1, C2, C3, C4;
int *ioff1, *ioff2, *ioff3, *ioff4;

#pragma warn -par
void interrupt our_int66(bp, di, si, ds, es, dx, cx, bx, ax)
WORD bp, di, si, ds, es, dx, cx, bx, ax;
{
	int i;
	BYTE high, low;
	BYTE *offset1;
	BYTE *offset2;

	asm {
		MOV AH, 50d
		MOV AH, 49d
		MOV AH, 67d
	}
	enable();
	switch(ax) {
		case 0 :          // is in 21th century
			if(dosing) ax=22;
			else ax=21;
			break;
		case 1 :
			cx=MaxX;
			dx=MaxY;
			break;
		case 2 :
			cx=FP_SEG(vid_addr);
			dx=FP_OFF(vid_addr);
			break;
		case 3 :
			cx=FP_SEG(offsetx);
			dx=FP_OFF(offsetx);
			break;
		case 4 :
			ax=FP_SEG(&mousex);
			bx=FP_OFF(&mousex);
			cx=FP_SEG(&mousey);
			dx=FP_OFF(&mousey);
			break;
		case 5 :
			ax=FP_SEG(&lbp);
			bx=FP_OFF(&lbp);
			cx=FP_SEG(&rbp);
			dx=FP_OFF(&rbp);
			break;
		case 6 :
			vMCOFF();
			break;
		case 7 :
			vMCON();
			break;
		case 8 :
			vSetRange(bx, cx, dx, si);
			break;
		case 9 :
			vRestoreRange();
			break;
		case 10 :          // get file name
			i=bx;
			offset1=MK_FP(cx, dx);
			offset2=MK_FP(si, di);
			bx=iFileListDlg(offset1, offset2, i);
			break;
		case 11 :
			offset1=MK_FP(dx, si);
			vWriteString(bx, cx, offset1, (BYTE)di);
			break;
		case 12 :
			ax=FP_SEG(our_buf);
			bx=FP_OFF(our_buf);
//			cx=(WORD)(((unsigned long)(SYSPEED*130))>>16);
//			dx=(WORD)(((unsigned long)(SYSPEED*130)) & 0x0ffff);
			break;
		case 13 :
			ax=FP_SEG(exe_path);
			bx=FP_OFF(exe_path);
			break;
		case 14 :
			ax=FP_SEG(font_path);
			bx=FP_OFF(font_path);
			break;
		case 15 :
			ax=FP_SEG(swp_path);
			bx=FP_OFF(swp_path);
			break;
		case 16 :
			offset1=MK_FP(es, ds);
			ax=iGetString(bx, cx, dx, si, di, offset1);
			break;
		case 17 :
			vDelay(bx);
			break;
		case 20 :
			offset1=MK_FP(cx, dx);
			vSetExt(offset1);
			break;
		case 50 :
			offset1=MK_FP(bx, cx);
			vErrorMsgDlg(offset1);
			break;
		case 51 :
			offset1=MK_FP(bx, cx);
			i=ihUserChoiceDlg(offset1);
			bx=i;
			break;
		case 52 :
			i=wCheckKeyHit();
			bx=i;
			break;
		case 53 :
			i=wGetKey(bx);
			bx=i;
			break;
		case 54 :
//			vTest();
			break;
		case  94 : vGraphClipOff(); break;
		case  95 : vGraphClipOn(bx, cx, dx, si); break;
		case  96 : vNLineXBit(bx, cx, dx, si); break;
		case  97 : vNLineYBit(bx, cx, dx, si); break;
		case  98 : vNBoxBit(bx, cx, dx, si, di); break;
		case  99 : vNBarBit(bx, cx, dx, si, di); break;
		case 106 : vLineBit(bx, cx, dx, si, di); break;
		case 113 : vXORBarBytex16(bx, cx, dx); break;
		case 114 : vBarByte(bx, cx, dx, si, di); break;

		case 120 :
			offset1=MK_FP(si, di);
			high=(BYTE)(dx>>8);
			low= (BYTE)(dx & 0x0ff);
			vPutFreeImageByte(bx, cx, high, low, offset1, ds);
			break;
		case 121 :
			offset1=MK_FP(si, di);
			vGetImage(bx, cx, dx, ds, offset1);
			break;
		case 122 :
			offset1=MK_FP(si, di);
			vPutImage(bx, cx, dx, ds, offset1);
			break;
		case 123 :
			offset1=MK_FP(dx, si);
			vPutWordImageBit(bx, cx, offset1);
			break;
		case 124 :
			vDrawPanelByte(bx, cx, dx, si, di);
			break;

		case 199 :
			C1=bx;
			C2=cx;
			C3=dx;
			break;
		case 200 :
			ioff1=(int *)MK_FP(bx, cx);
			ioff2=(int *)MK_FP(dx, si);
			break;
		case 201 : vDrawVScrollBarBit(bx, cx, dx); break;
		case 202 : vDrawHScrollBarBit(bx, cx, dx); break;
		case 203 : vDrawVThumbBit(bx, cx, dx, si, di, es); break;
		case 204 : vDrawHThumbBit(bx, cx, dx, si, di, es); break;
		case 205 :
			i=iVscrollBarHandler(C1, C2, C3, bx, cx, dx, si, di);
			bx=i;
			break;
		case 206 :
			i=iHScrollBarHandler(C1, C2, C3, bx, cx, dx, si, di);
			bx=i;
			break;
		case 207 :
			offset1=(BYTE *)MK_FP(es, ds);
			vDrawWindowByte(bx, cx, dx, si, di, offset1);
			break;
		case 208 :
			ioff3=(int *)MK_FP(bx, cx);
			ioff4=(int *)MK_FP(dx, si);
			bx=iMoveWindow(ioff1, ioff2, ioff3, ioff4);
			break;
		case 209 :
			ioff3=(int *)MK_FP(bx, cx);
			ioff4=(int *)MK_FP(dx, si);
			bx=iResizeWindow(ioff1, ioff2, ioff3, ioff4);
			break;
		case 210 :
			vSetMinWindowSize(bx, cx);
			break;
		case 220 :   // return value from calc or others
			bx=FP_SEG(Ext_buf);
			cx=FP_OFF(Ext_buf);
			break;
		case 221 :
			E_flag=bx;
			break;

		case 500 :   // printing option array
			cx=FP_SEG(poption);
			dx=FP_OFF(poption);
			break;
		case 501 :
			cx=FP_SEG(pname);
			dx=FP_OFF(pname);
			break;
		case 502 :
			cx=FP_SEG(font_path);
			dx=FP_OFF(font_path);
			break;
		case 503 :
			cx=FP_SEG(vErrorMsgDlg);
			dx=FP_OFF(vErrorMsgDlg);
			break;
		case 505 :
			cx=FP_SEG(our_buf);
			dx=FP_OFF(our_buf);
	}
}
#pragma warn +par

BYTE *out_exe[]={
	"TEDIT.EXE",
	"GEDIT.EXE",
	"CALENDAR.EXE",
	"ECD.EXE",
	"CALC.EXE",
};

void vExcuteSubEXE(BYTE n)
{
	vMCOFF();
	if(tw[cur_w].active) {
		if(!iSwapDocTofile(cur_w)) return;
	}
	vClearNodeList();
	iFreeFontBuffer(1);
	sprintf(g_dir, "%s\\%s", exe_path, out_exe[n]);
	if(spawnl(P_WAIT, g_dir, g_dir, NULL) == -1) {
		vErrorMsgfDlg(0, 1, out_exe[n]);
	}
	vInitNodeList();
	if(tw[cur_w].active) {
		iSwapFileToDoc(cur_w);
		vSetCurrent();
	}
	vMCON();
}

