#define _WINFUNC2
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <mem.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <sys\stat.h>
#include "est.h"

#include "21.h"
#include "proto.h"
#include "graph.h"
#include "mouse.h"

#define MAXTWIN			6
#define MAXFWIN     10
#define ON          1
#define OFF         0

BYTE cur_w=1;
extern int MaxX, MaxY;
extern char *our_buf;
extern int bminx, bminy, bmaxx, bmaxy;
extern BYTE key_flag;
extern int top_x;
extern BYTE disp_japan;
extern BYTE han_kbd;
extern BYTE mac_use;
extern BYTE block_task;

text_window tw[MAXTWIN];
text_window def_tw;
Mem tm[MAXTWIN];
BYTE nicks[MAXTWIN][30];
int WL[MAXTWIN][3];    // window length save
extern BYTE g_drive[];
extern BYTE g_dir[];
extern BYTE g_file[];
extern BYTE g_ext[];

char *key_name[]={
	"µw¢… £¡Š‚  ", //0
	"Ðe‹i 2¤é¯¢ ", //1

	"Ðe‹i 3¤é¯¢ ", //2

	"µw¢… —a¥¡œb", //3
	"µw¢… µwŠ‚  ", //4
	"µw¢… Ïaœw¯a", //5
	"µw¢… •¢·©  ", //6
	"µw¢… ¯a¶Á•E", //7
	"µw¢… ·¡ÈŸ¡", //8
	"µw¢… ‘¡Ÿa¶Á", //9
	"µw¢… œaË¥  ", //10
	"µw¢… ¯aÍA·¥", //11
	"µw¢…¯aÍA·¥2", //12
	"µw¢… •E aÇa", //13
	"µw¢…•E aÇa2", //14

	"HIRA :     ", //15
	"KATA :     ", //
	"¸a.Ó¡œaÄaa", //
	"¸a.ÄaÈaÄaa", //

	"¤e.œá¯¡´a  ", //19
	"¤e.¥BÈá¢…¸a", //
	"¤e.®Ðb‹¡Ñ¡", //
	"¤e.•¡Ñw    ", //

	"¸å.‹aŸ¡¯a  ", //23
	"¸å.¶¥¢…¸a  ", //
	"¸å.‰ÉÑ¡¢…¸a", //
	"¸å.œá¯¡´a  ", //
	"¸å.‰¡´á¸a¡¡", //
	"¸å.‹¡Ñ¡¢…¸a", //
	"¸å.‹aŸ±•¡Ñw", //
	"¸å.¬å¢…¸a  ", //30
	"¸å.¢…¸w¦Ñ¡", //
//	"¤e.¬a¶w¸a  ", //
//	"¸å.¬a¶w¸a  ", //

	"µw¢… £¡Š‚  ", //0
	"Ðe‹i 2¤é¯¢ ", //1
	"...........", //2
	"...........", //3
	"...........", //4
	"...........", //5
	"...........", //6
	"...........", //7
	"...........", //8
	"...........", //9
	"...........", //10
};

BYTE *extra_task[]={
	"§iœâ¬é¸÷.. ",
	"¬å‹aŸ¡‹¡   ",
  "·¡•·       ",
	"»¡¶‹¡     ",
	"\xff\xe1-1~0 ¬åÈ‚",
	"Yes/No/Quit",
	"‰­¢..     ",
	"­A¡§iœâ.. ",
  "‰i‰Á¬s·³.. ",
};

void vDispKeyInputStatus(void)
{
	if(mac_use==1) {
		iPrintf(tw[cur_w].x+20, tw[cur_w].y+1, 0, "{%s}", key_name[tw[cur_w].h_e]);
	}
	else {
		iPrintf(tw[cur_w].x+20, tw[cur_w].y+1, 0, "[%s]", key_name[tw[cur_w].h_e]);
	}
}

void vDispExtraStatus(int i)
{
	if(mac_use==1) {
		iPrintf(tw[cur_w].x+20, tw[cur_w].y+1, 0, "{%s}", extra_task[i]);
	}
	else {
		iPrintf(tw[cur_w].x+20, tw[cur_w].y+1, 0, "[%s]", extra_task[i]);
	}
}

void vDispRowStatus(void)
{
	iPrintf(tw[cur_w].x+20, tw[cur_w].y+14, 0, "%4u¡e", tw[cur_w].cp & 0x1fff);
	iPrintf(tw[cur_w].x+20, tw[cur_w].y+21, 0, "%5uÐ—", tw[cur_w].cr);
}

void vDispColumeStatus(void)
{
	iPrintf(tw[cur_w].x+20, tw[cur_w].y+29, 0,"%3uµi", tw[cur_w].cc & 0x02ff);
}

void vDispInsertStatus(void)
{
	vWriteString(tw[cur_w].x+20, tw[cur_w].y+35, \
								(int)(tw[cur_w].ins & 1) ? "¬s·³" : "®¸÷", tw[cur_w].ins>>1);
}

void vResetTextWindowValue(BYTE i, BYTE flag)
{
	memcpy(&(tw[i]), &def_tw, sizeof(text_window));
	if(flag && i) tw[i].x=i*16;
	else tw[i].x=16;
	tw[i].y=0;
	tw[i].cx=tw[i].x+52;
	tw[i].cy=tw[i].y+1;
	if(flag && i) tw[i].lx=(MaxX-15)/8-(i-1)*2;
	else tw[i].lx=(MaxX-15)/8;
	tw[i].ly=(MaxY+1)/8;
	tw[i].total=1;
	tw[i].cc=1;
	tw[i].cr=1;
	tm[i]=0;
	setmem(nicks[i], 30, 0);
	sprintf(tw[i].headline, "NONAME0%d.GWP", i);
}

void vInitWindowValue(void)
{
	WORD i;
	def_tw.x=16+6*16;
	def_tw.y=0;
	def_tw.cx=def_tw.x+52;
	def_tw.cy=def_tw.y+1;
	def_tw.lx=(MaxX-15)/8-6*2;
	def_tw.ly=(MaxY+1)/8;
	def_tw.bl=0;
	def_tw.cr=1;
	def_tw.cc=1;
	def_tw.cp=1;
	def_tw.total=1;
	def_tw.modify=OFF;
	def_tw.active=OFF;
	def_tw.h_e=han_kbd;
	def_tw.ins=1;
	def_tw.scr_c=0;

	def_tw.repeat=1;
	setmem(def_tw.prn_range,   2, 0xff);
	setmem(def_tw.prn_range+2, 2, 0);
	def_tw.prn_range[5]=15;
	def_tw.prn_range[6]=0;
	def_tw.prn_range[7]=1;

	def_tw.prn_dens=2;
	setmem(def_tw.tab, 30, 0x80);

	for(i=1;i<MAXTWIN;i++) {
		vResetTextWindowValue(i, 1);
		WL[i][0]=0;
	}
	memcpy(&(tw[0]), &def_tw, sizeof(text_window));
	tw[0].x=16+6*16;
	tw[0].y=0;
	tw[0].cx=tw[0].x+52;
	tw[0].cy=tw[0].y+1;
	tw[0].lx=(MaxX-15)/8-6*2;
	tw[0].ly=(MaxY+1)/8;
	strcpy(tw[0].headline, "·±¯¡¸á¸w");
	vInitDlgWindow();
}

extern unsigned long Scount;

int iHScrollBarHandler(int x, int y, int ey, int total, int sub,
						 int old_sub, int vHomeKey, int vExit)
{
	int mx,my,ly,ty,gap,i,old_i;
	char mode;
	long int temp;
	unsigned long time;

	if(lbp==0 || total<1) return -100;
	else {
		i=sub;
		old_i=old_sub;
		if(total==0) total=1;
		temp=y+16+((long int)(ey-y-48)*(sub-vHomeKey))/(total);
		ly=(int)temp & 0xfffe;
		getpos(mx,my);
		if(mx<x || mx>x+16) return -100;
		if(my>y && my<ly)	mode=1;
		else if(my>ly+16 && my<ey) mode=2;
		else if(my>ly && my<ly+16) mode=3;
		else return -100;
		if(mode==1) if(i==vHomeKey) return -100;
		if(mode==2) if(i==vExit) return -100;
		if(mode<3) {
			vMCOFF();
			if(mode==1) vXORBarBit(x+1, y+1,x+13,y+14);
			else vXORBarBit(x+1, ey-14,x+13,ey-1);
			if(mode==1) {
				if(i>vHomeKey) i--;
			}
			else {
				if(i<vExit) i++;
			}
			vDrawHThumbBit(x,y,ey,total,i-vHomeKey,old_i-vHomeKey);
			vWLineYBit(x,y,ey);
			old_i=i;
			vMCON();
			time=Scount;
			while(Scount<time+9) {
				if(lbp==0) goto OUT;
			}
			for(;;) {
				if(mode==1) {
					if(i>vHomeKey) i--;
				}
				else {
					if(i<vExit) i++;
				}
				vMCOFF();
				vDrawHThumbBit(x,y,ey,total,i-vHomeKey,old_i-vHomeKey);
				vWLineYBit(x,y,ey);
				old_i=i;
				vMCON();
				time=Scount;
				while(Scount==time);
				if(lbp==0) {
OUT:			vMCOFF();
					if(mode==1) vXORBarBit(x+1, y+1,x+13,y+14);
					else vXORBarBit(x+1, ey-14,x+13,ey-1);
					vMCON();
					return i;
				}
			}
		}
		if(mode==3) {
			vMCOFF();
			gap=my-ly;
ST: 	ty=my;
			vXORBoxBit(x+1,ly+(ty-ly)-gap,x+13,ly+15+(ty-ly)-gap);
			vMCON();
			for(;;) {
				getpos(mx,my);
				if(my-gap<y+16) my=y+16+gap;
				else if(my-gap> ey-32) my=gap+ey-32;
				if(ty!=my) {
					vMCOFF();
					vXORBoxBit(x+1,ly+(ty-ly)-gap,x+13,ly+15+(ty-ly)-gap);
					vXORBoxBit(x+1,ly+(my-ly)-gap,x+13,ly+15+(my-ly)-gap);
					vMCON();
					ty=my;
				}
				if(mx<x-2 || mx>x+18) {
					vMCOFF();
					vXORBoxBit(x+1,ly+(my-ly)-gap,x+13,ly+15+(my-ly)-gap);
					vMCON();
					for(;;) {
						getpos(mx,my);
						if(mx>x-3 && mx<x+19 && my>y+16 && my<ey-16) {
							gap=8;
							vMCOFF();
							goto ST;
						}
						else if(lbp==0)	return -100;
					}
				}
				if(lbp==0) {
					temp=(long int)(my-y-16-gap)*total;
					sub=(int)(temp/(ey-y-48)+vHomeKey);
					vMCOFF();
					vXORBoxBit(x+1,ly+(my-ly)-gap,x+13,ly+15+(my-ly)-gap);
					vDrawHThumbBit(x,y,ey,total,sub-vHomeKey,old_sub-vHomeKey);
					vWLineYBit(x,y,ey);
					old_i=sub;
					vMCON();
					if(sub<vHomeKey) sub=vHomeKey;
					else if(sub>vExit) sub=vExit;
					return sub;
				}
			}
		}
		else return -100;
	}
}

BYTE Editing=0;

int iVscrollBarHandler(int x, int ex, int y, int total, int sub,
						 int old_sub, int vHomeKey, int vExit)
{
	int mx, my, lx, tx, gap, i, old_i;
	char mode;
	long int temp;
	unsigned long time;

	if(lbp==0 || total<1) return -100;
	else {
		i=sub;
		old_i=old_sub;
		if(total==0) total=1;
		temp=x+16+(((long int)(ex-x-48)*(sub-vHomeKey))/(total) & 0xfffe);
//		lx=(int)temp & 0xfffe;
		lx=(int)temp;
		getpos(mx,my);
		if(mx>x && mx<lx &&  my>y && my<y+16)	{
			mode=1;
		}
		else if(mx>lx+16 && mx<ex && my>y && my<y+16) {
			mode=2;
		}
		else if(mx>lx && mx<lx+16 && my>y && my<y+16) mode=3;
		else return -100;
		if(mode==1) if(i==vHomeKey) return -100;
		if(mode==2) if(i==vExit) return -100;
		if(mode<3) {
			vMCOFF();
			if(mode==1) vXORBarBit(x+1, y+1,x+13,y+12);
			else vXORBarBit(ex-13, y+1, ex-1,y+12);
			if(mode==1) {
				if(i>vHomeKey) {
					i--;
					if(Editing) vLineUpDown(0);
				}
			}
			else {
				if(i<vExit) {
					i++;
					if(Editing) vLineUpDown(1);
				}
			}
			vMCOFF();
			vDrawVThumbBit(x, ex, y,total,i-vHomeKey,old_i-vHomeKey);
			old_i=i;
			vMCON();
			time=Scount;
			while(Scount<time+9) {
				if(lbp==0) {
					if(Editing) i=-100;
					goto OUT;
				}
			}
			vSetRange(x, y, ex, y+14);
			for(;;) {
				if(mode==1) {
					if(i>vHomeKey) {
						i--;
						if(Editing) vLineUpDown(0);
					}
				}
				else {
					if(i<vExit) {
						i++;
						if(Editing) vLineUpDown(1);
					}
				}
				vMCOFF();
				vDrawVThumbBit(x, ex, y, total, i-vHomeKey, old_i-vHomeKey);
				old_i=i;
				vMCON();
				while(i==vHomeKey && mode==1) if(lbp==0) goto OUT;
				while(i==vExit && mode==2) if(lbp==0) goto OUT;
				if(!Editing) {
					time=Scount;
					while(Scount==time);
				}
				if(lbp==0) {
OUT:			vMCOFF();
					vRestoreRange();
					if(mode==1) vXORBarBit(x+1, y+1,x+13,y+12);
					else vXORBarBit(ex-13, y+1,ex-1,y+12);
					vMCON();
					return i;
				}
			}
		}
		if(mode==3) {
			vMCOFF();
			gap=mx-lx;
ST: 	tx=mx;
			vXORBoxBit(lx+(tx-lx)-gap, y+1, lx+15+(tx-lx)-gap, y+13);
			vMCON();
			for(;;) {
				getpos(mx,my);
				if(mx-gap<x+16) mx=x+16+gap;
				else if(mx-gap> ex-32) mx=gap+ex-32;
				if(tx!=mx) {
					vMCOFF();
					vXORBoxBit(lx+(tx-lx)-gap, y+1, lx+15+(tx-lx)-gap, y+13);
					vXORBoxBit(lx+(mx-lx)-gap, y+1, lx+15+(mx-lx)-gap, y+13);
					vMCON();
					tx=mx;
				}
				if(my<y-2 || my>y+18) {
					vMCOFF();
					vXORBoxBit(lx+(mx-lx)-gap, y+1, lx+15+(mx-lx)-gap, y+13);
					vMCON();
					for(;;) {
						getpos(mx,my);
						if(mx>x-3 && mx<ex-16 && my>y && my<y+16) {
							gap=8;
							vMCOFF();
							goto ST;
						}
						else if(lbp==0)	return -100;
					}
				}
				if(lbp==0) {
					temp=(long int)(mx-x-16-gap)*total;
					sub=(int)(temp/(ex-x-48)+vHomeKey);
					vMCOFF();
					vXORBoxBit(lx+(mx-lx)-gap, y+1, lx+15+(mx-lx)-gap, y+13);
					vDrawVThumbBit(x, ex, y, total, sub-vHomeKey, old_sub-vHomeKey);
					old_i=sub;
					vMCON();
					if(sub<vHomeKey) sub=vHomeKey;
					else if(sub>vExit) sub=vExit;
					return sub;
				}
			}
		}
		else return -100;
	}
}

BYTE show_rod=0;


void vRestoreWindowSize(int n)
{
	if(WL[n][0] && tw[n].active && WL[n][0]==1) {
		if(WL[n][1] == tw[n].x) tw[n].lx=WL[n][2];
		WL[n][0]=0;
	}
}

void vSaveWindowSize(int n)
{
	if(tw[n].active && WL[n][0]==0) {
		WL[n][0]=1;
		WL[n][1]=tw[n].x;
		WL[n][2]=tw[n].lx;
	}
}

void vDrawTextWindow(BYTE n)
{
	int x, y, lx, ly;
	int blx, by, tl, i;
	BYTE name[80];

	x=tw[n].x;	 y=tw[n].y;
	lx=tw[n].lx; ly=tw[n].ly;
	blx=lx<<3;   by=y<<3;
	if(disp_japan && n==cur_w) {
		vSaveWindowSize(n);
		if(x+blx>MaxX-90) {
			if(lx-(x+blx-MaxX+90)/8-1<24) {
				x-=x+blx-MaxX+90;
			}
			else lx-=(x+blx-MaxX+90)/8+1;
		}
		tw[n].lx=lx;
		blx=lx<<3;
		tw[n].x=x;
	}
	else {
		vRestoreWindowSize(n);
		lx=tw[n].lx;
		blx=lx*8;
	}
	tl=ly-4-8;
	if(strlen(tw[n].headline) > tl) {
		iFnSplit(tw[n].headline);
		for(i=0; i<tl-4-13; i++) name[i]=tw[n].headline[i];
		name[i]=0;
		strcat(name, "...\\");
		strcat(name, g_file);
		strcat(name, g_ext);
	}
	else strcpy(name, tw[n].headline);
	vDrawWindowByte(x, y, lx, ly, 3, name);

	if(show_rod) bminx=x+51;
	else bminx=x+51-14;
	bminy=by+7;
	bmaxx=x+blx-4;
	bmaxy=((y+ly)<<3)-16;

	vDispScriptStatus();
	iPrintf(x+4, y+4, 0x04, "[%d]", n);
	vDrawVScrollBarBit(x+36, x+blx-16, (y+ly-2)<<3);
	vDrawVThumbBit(x+36, x+blx-16, (y+ly-2)<<3,
					 tw[n].total-1, tw[n].cr-1, 0);
	vWLineYBit(x+36, by+3, ((y+ly)<<3)-3);
	vSaveCR(tw[n].total, tw[n].cr);

	if(show_rod) vPutRod(n, 0);
	vMCON();
}

int ws_loc=0;
BYTE wstack[6];

void vPushWindow(BYTE wnum)
{
	wstack[ws_loc]=wnum;
	ws_loc++;
}

int iPopWindow(void)
{
	if(ws_loc>0) ws_loc--;
	else return 0xff;
	return wstack[ws_loc];
}


void vRedrawAllWindow(void)
{
	BYTE i;

	vMCOFF();
	vFillScreen(0xaaaa, 0x5555);
	vDrawTopMenu();
	for(i=0;i<ws_loc-1;i++) {
		if(tw[wstack[i]].active) vSwapTextWindowArea(0, wstack[i]);
	}
	if(disp_japan) vDrawKeyMap();
}

int iCloseTextWindow(int new)
{
	BYTE wnum, flag=1;
	int i;

	if(!tw[cur_w].active) return 0;
	if(tw[cur_w].modify) {
		if(cur_w==0) {
			if(!iSwapDocTofile(0)) vErrorMsgfDlg(0, 2, "µe¯s¸w");
		}
		else {
			vRestoreEdit();
			vDispText();
			i=iUserChoiceDlg("¸á¸w·i Ða¯¡‰V·s“¡Œa?");
			if(i==0) {
				if(iSaveTextWindow()!=1) return 0;
			}
			else if(i==-1) return 0;
			else flag=0;
		}
	}
	if(new) iPopWindow();
	vFreeMem(&(tm[cur_w]));
	tw[cur_w].active=OFF;
	if(cur_w!=0 && flag) {
		tw[cur_w].ly=0;
		vSaveClosedTextWindow(tw[cur_w].headline);
	}
	vRedrawAllWindow();
	if(!new) iPopWindow();
	if(cur_w) vResetNodeList();
	if(new) {
		if(ws_loc<1) return 1;
		wnum=wstack[ws_loc-1];
		if(!iSwapFileToDoc(wnum)) {
			vErrorMsgfDlg(0, 1, "¯aµÓ");
			vExit();
		}
		vMCOFF();
		cur_w=wnum;
		vDrawTextWindow(cur_w);
		tw[cur_w].active=ON;
		vSetCurrent();
	}
	return 1;
}

int iCloseAllTextWindow(void)
{
	int i;

	for(i=ws_loc;i>=0;i--) {
		if(tw[wstack[i]].active) {
			if(iCloseTextWindow(1)==0) return 0;
		}
	}
	return 1;
}

int iCheckTextWindowArea(int mx, int my)
{
	int i;
	BYTE wnum;

	// stack processing needed here
	for(i=ws_loc-1 ; i>=0; i--) {
		wnum=wstack[i];
		if(mx>tw[wnum].x && mx<tw[wnum].x+tw[wnum].lx*8
			 && my>tw[wnum].y*8 && my<(tw[wnum].y+tw[wnum].ly)*8) {
			return wnum;
		}
	}
	return 0xff;
}

int iMoveWindow(int *px, int *py, int *plx, int *ply)
{
	int oox, ooy;
	int i, ox, oy, tx, ty;
	int x, y, lx, ly;

	x=(*px);
	y=(*py)*8;
	lx=(*plx)*8;
	ly=(*ply)*8;
	vMCOFF();
	for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
	ox=mousex;
	oy=mousey;
	oox=x;
	ooy=y;
	for(;;) {
		vMCON();
		getpos(tx, ty);
		if(ox!=tx || oy!=ty) {
			vMCOFF();
			for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
			x+=tx-ox;
			y+=ty-oy;
			if(x<0) x=0;  if(y<0) y=0;
			if(x+lx>MaxX) x=MaxX-lx;
			if(y+ly>MaxY+1) y=MaxY-ly+1;
			ox=tx; oy=ty;
			for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
			vMCON();
		}
		if(lbp==0) {
			vMCOFF();
			for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
			if(oox==x && ooy==y) {
							return 0;
			}
			else {
				*px=x;
				*py=y/8;
				*plx=lx/8;
				*ply=ly/8;
			}
			vMCON();
			return 1;
		}
	}
}


int iMoveTextWindow(BYTE n)
{
	int i;
	int x, y, lx, ly;
	x=tw[n].x;
	y=tw[n].y;
	lx=tw[n].lx;
	ly=tw[n].ly;

	i=iMoveWindow(&x, &y, &lx, &ly);
	if(i==1) {
		vMCOFF();
		tw[n].cx+=x-tw[n].x;
		top_x+=x-tw[n].x;
		tw[n].cy+=y-tw[n].y;
		tw[n].x=x;
		tw[n].y=y;
		vRedrawAllWindow();
		vDrawTextWindow(n);
		vMCON();
		return 1;
	}
	else {
		return i;
	}
}

int wminx;
int wminy;

void vSetMinWindowSize(int x, int y)
{
	wminx=x;
	wminy=y;
}

int iResizeWindow(int *px, int *py, int *plx, int *ply)
{
	int oolx, ooly;
	int ox, oy, tx, ty;
	int x, y, lx, ly, i;

	x=*px;
	y=(*py)*8;
	lx=(*plx)*8;
	ly=(*ply)*8;

	vMCOFF();
	for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
	ox=mousex;
	oy=mousey;
	oolx=lx;
	ooly=ly;
	for(;;) {
		vMCON();
		getpos(tx, ty);
		if(ox!=tx || oy!=ty) {
			vMCOFF();
			for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
			lx+=tx-ox;
			ly+=ty-oy;
			if(lx<wminx) lx=wminx;
			if(ly<wminy) ly=wminy;
			if(lx+x>MaxX+1) lx=MaxX-x;
			if(ly+y>MaxY+1) ly=MaxY-y+1;
			ox=tx; oy=ty;
			for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
			vMCON();
		}
		if(lbp==0) {
			vMCOFF();
			for(i=1; i<3; i++) vXORBoxBit(x+i, y+i, x+lx-i, y+ly-i);
			if(oolx==lx && ooly==ly) return 0;
			*px=x;
			*py=y/8;
			*plx=lx/8;
			*ply=ly/8;
			return 1;
		}
	}
}

int iResizeTextWindow(BYTE n)
{
	int x, y, lx, ly, i;

	x=tw[n].x;
	y=tw[n].y;
	lx=tw[n].lx;
	ly=tw[n].ly;

	vSetMinWindowSize(180, 320);
	i=iResizeWindow(&x, &y, &lx, &ly);
	if(i==1) {
		tw[n].lx=lx;
		tw[n].ly=ly;
		vRedrawAllWindow();
		vSyncCR(iSyncDC());
		vDrawTextWindow(n);
		vMCON();
		return 1;
	}
	else {
		vMCON();
		return i;
	}
}

void vMaximizeWindow(void)
{
	vMCOFF();
	tw[cur_w].cx+=16-tw[cur_w].x;
	tw[cur_w].cy-=tw[cur_w].y;
	top_x+=16-tw[cur_w].x;
	tw[cur_w].x=0;
	tw[cur_w].y=0;
	tw[cur_w].lx=(MaxX+1)/8;
	tw[cur_w].ly=(MaxY+1)/8;
	vDrawTextWindow(cur_w);
	vMCON();
}

void vChangeWindow(BYTE wnum)
{
	int i;

	vEndKeyTask();
	if(!tw[wnum].active || wnum==cur_w) return;
	if(!iSwapDocTofile(cur_w)) {
		vErrorMsgfDlg(0, 2, "¯aµÓ");
		return;
	}
	if(!iSwapFileToDoc(wnum)) {
		vErrorMsgfDlg(0, 1, "¯aµÓ");
		vExit();
	}
	vMCOFF();
	vDarkenTextWindowTitle(cur_w);
	vSwapTextWindowArea(1, cur_w);
	for(i=0;i<ws_loc;i++) {
		if(wstack[i]==wnum) {
			memmove(wstack+i, wstack+i+1, 5-i);
			wstack[ws_loc-1]=wnum;
			break;
		}
	}
	cur_w=wnum;
	vDrawTextWindow(cur_w);
	tw[cur_w].active=ON;
	vSetCurrent();
}

void vNextWindow(void)
{
	BYTE i, j=0xff;

	if(!tw[cur_w].active) return;
	for(i=cur_w+1;i<6;i++) {
		if(tw[i].active) {
			j=i;
			break;
		}
	}
	if(j==0xff) {
		for(i=0;i<6;i++) {
			if(tw[i].active) {
				j=i;
				break;
			}
		}
		if(cur_w==i) return;
	}
	vChangeWindow(j);
}

void vKeyMoveTextWindow(BYTE n)
{
	int x, y, lx, ly;
	BYTE t;

	x=tw[n].x;
	y=tw[n].y;
	lx=tw[n].lx;
	ly=tw[n].ly;
	t=iKeyMoveWindow(&x, &y, &lx, &ly, 1);
	if(t==1) {
		tw[n].cx+=x-tw[n].x;
		top_x+=x-tw[n].x;
		tw[n].cy+=y-tw[n].y;
		tw[n].x=x;
		tw[n].y=y;
		tw[n].lx=lx;
		tw[n].ly=ly;
		vRedrawAllWindow();
		vDrawTextWindow(n);
	}
	vMCON();
}

void vXORTabLoc(int n, int choice)
{
	int x, y;

	y=tw[n].y*8+choice*8+8;
	x=tw[n].x+52-14;
	vNLineXBit(x, bmaxx, y, 2);
};

void vSetTab(int flag)
{
	INKEY c;
	int cho=0, scr, t_cho;
	int sy, ey;
	BYTE t_tab[16], t;

	if(tw[cur_w].active==0) return;
	vMCOFF();
	if(show_rod==0) {
		show_rod=1;
		vPutRod(cur_w, 0);
		show_rod=0;
	}
	scr=tw[cur_w].scr_c;
	sy=tw[cur_w].y*8+8;
	ey=tw[cur_w].ly-4;
	if(flag==0) {
		cho=tw[cur_w].ly/2;
		mousey=sy+cho*8+2;
	}
	else {
		while(lbp);
		cho=(mousey-sy)/8;
	}
	memcpy(t_tab, tw[cur_w].tab, 16);

	vXORTabLoc(cur_w, cho);
	for(;;) {
		while(!wCheckKeyHit()) {
			t_cho=(mousey-sy)/8;
			if(t_cho!=cho && t_cho<ey && t_cho>-1) {
				vXORTabLoc(cur_w, cho);
				cho=t_cho;
				vXORTabLoc(cur_w, cho);
			}
			if(lbp) {
				if(cho<ey && t_cho>-1) {
					while(lbp) {
						if(rbp) {
							vXORTabLoc(cur_w, cho);
							goto OUT;
						}
					}
					vXORTabLoc(cur_w, cho);
					goto SP;
				}
			}
			if(rbp) {
				vXORTabLoc(cur_w, cho);
				goto RN;
			}
		}
		c.i=bioskey(0);
		vXORTabLoc(cur_w, cho);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC :
OUT:			vMCON();
					if(show_rod==0) vHideRod();
					return;
				case RETURN :
RN:				memcpy(tw[cur_w].tab, t_tab, 16);
					if(show_rod==0) vHideRod();
					return;
				case SPACE  :
SP:       t=t_tab[(scr+cho)/8] & (0x80>>((scr+cho)%8));
					t_tab[(scr+cho)/8] ^= (0x80>>((scr+cho)%8));
					if(t) {
						t=0;
					}
					else t=1;
					vPutTabChar(cur_w, cho, t);
					break;
			}
		}
		else {
			switch(c.ch[1]) {
				case CTRL_LARROW :
					cho-=8;
				case LEFT :
					if(c.ch[1]!=CTRL_LARROW) cho--;
					if(cho<0) cho=0;
					mousey=sy+cho*8;
					break;
				case CTRL_RARROW :
					cho+=8;
				case RIGHT :
					if(c.ch[1]!=CTRL_RARROW) cho++;
					if(cho>ey) cho=ey;
					mousey=sy+cho*8;
					break;
				case F1 :
					break;
				case HOME :
					cho=0;
					mousey=sy;
					break;
				case END  :
					cho=ey;
					mousey=sy+cho*8;
					break;
			}
		}
		vXORTabLoc(cur_w, cho);
	}
}

int iMouseInTextWindow(void)
{
	int mx, my, x, y, ex, ey, blx, bly, i;
	BYTE wnum;

	getpos(mx, my);
	if(mx>bminx && mx<bmaxx && my>bminy && my<bmaxy) return 2;
	x=tw[cur_w].x;
	y=tw[cur_w].y*8;
	ex=x+tw[cur_w].lx*8;
	ey=y+tw[cur_w].ly*8;
	blx=tw[cur_w].lx*8;
	bly=tw[cur_w].ly*8;
	wnum=iCheckTextWindowArea(mx, my);
	if(wnum==0xff) return 0;
	if(wnum!=cur_w) {
		while(lbp);
		vDispTime(2);
		block_task=OFF;
		vChangeWindow(wnum);
		return 1;
	}
	else {
		if(mx<x+22 && mx>x+4) { // iMoveTextWindow or Maximize
			if(my>ey-20) {
				while(lbp);
				vDispTime(2);
				vMaximizeWindow();
			}
			else if(my<y+21) {
				if(block_task) goto OUT;
				while(lbp);
				vDispTime(2);
				iCloseTextWindow(1);
			}
			else {
				vDispTime(2);
				iMoveTextWindow(cur_w);
			}
			block_task=OFF;
			return 1;
		}
		else if(my<ey-16) {
			if(mx<x+36) {
			}
			else if(mx<x+50 && show_rod==1) {       // setting tap size
				vSetTab(1);
				return 1;
			}
			else if(!rbp) { 											 // set text cursor or set block
				return 2;
			}
		}
		else {
			if(mx<ex-16) {    			 							 // setting hvar
				Editing=ON;               					 // local y is bit
				i=iVscrollBarHandler(x+36, x+blx-16, y+bly-16, tw[cur_w].total-1, \
										tw[cur_w].cr, tw[cur_w].cr, 1, tw[cur_w].total);
				Editing=OFF;
				if(i!=-100) {
					key_flag=ON;
					tw[cur_w].cr=i;
					vSaveCR(tw[cur_w].total, i);
					vSetCurrent();
					iSetCurrentLine();
					return 1;
				}
			}
			else if(mx>ex-16) {     							 // iResizeTextWindow
				vDispTime(2);
				iResizeTextWindow(cur_w);
				block_task=OFF;
				return 1;
			}
		}
	}
OUT :
	return 0;
}

int iGetWindowHandle(void)
{
	BYTE i;
	for(i=1;i<MAXTWIN;i++) {
		if(!tw[i].active) return i;
	}
	return 0xff;
}


