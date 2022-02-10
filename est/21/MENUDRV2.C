#define  _MENUDRV2
#include <stdio.h>
#include <dir.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include <dos.h>
#include "est.h"

	#include "21.h"
	#include "proto.h"
#include "pdmenu1.h"
#include "mouse.h"
#include "graph.h"

#define NORM_VID 0
#define REV_VID  1
#define ON      1
#define OFF     0

menu_panel panel[30];

int last_top=1;
int top_choice;     
BYTE *our_buf;
WORD our_buf_size;
BYTE rev_flag=1;
BYTE timing=0;

extern text_window tw[];
extern int MaxX,MaxY;
extern BYTE cur_w;
extern int graph_mode;
extern BYTE color_mode;
extern unsigned long Scount;
extern BYTE block_task;
extern BYTE wc[];
extern BYTE sc[];
extern BYTE disp_japan;
extern BYTE exe_path[];
extern BYTE g_dir[];
extern BYTE show_rod;
extern int bminx, bminy, bmaxx, bmaxy;
extern BYTE xms_state;
extern BYTE ems_state;
extern BYTE vms_state;

extern BYTE View_time;
extern WORD no_work;

extern int ws_loc;
extern BYTE wstack[];

void vFreeOurBuf(void)
{
	vFreeUMB(our_buf);
}

extern BYTE far MACname[];

void vExit(void)
{
	vMCOFF();
	if(!iCloseAllTextWindow()) return;
	vDeinitMouse();
	vDeinitINT8();
//	vDeinitINT66();
	vWriteKeyMacro(MACname);
	vGLSFileIO(1);
	vWriteConfig();
	vFreeOurBuf();

	vDeinitVMS();
	vFreeAllUMB();
	vDeinitMemorySystem();

	vSetCtrlC(1);
	vDeinitHangul();
	exit(0);
}

extern video_init vit[];

void vMakeOurBuf(void)
{
	if(vit[graph_mode].mx>480) {
		our_buf=pAllocUMB((WORD)1024*60);
		our_buf_size=(WORD)60000l;
	}
	else {
		our_buf=pAllocUMB((WORD)1024*40);
		our_buf_size=(WORD)38400l;
	}
	if(!our_buf) {
		vErrorMsgfDlg(1, 0, 0);
		vExit();
	}
}

int iMouseInMenu(int status,int pn)
{																			// num is retun value
	WORD x, y, sx;         						// status is CNode mode

	sx=panel[pn].x;
	if(panel[pn].head) sx+=18;
	getpos(x,y); y/=8;
	switch(status) {
		case 0 :          // '0' meaning top_choice
			if(x<16) {
				if(y<2) return 0;
				else if((y-2)/8+1 <10) return (y-2)/8+1;
			}
			else return -1;
		case 1 :          // '1' is meaning menu_select
			if(y>panel[pn].y && y<panel[pn].endy && x>(sx+2) && x<panel[pn].endx) {
				return (x-3-sx)/17+1;
			}
			else return -1;
		default : return -1;
	}
}

void vDrawTopMenu(void)
{
	BYTE attr;
	register int i;

	vBarByte(0,72,15,(MaxY+1)/8, rev_flag);
	if(rev_flag) attr=0x04;
	else attr=0;
	vWriteString(0, 0, Top_line[0]+1, attr);
	for (i=0;i<9;i++) {
		vWriteString(0,2+(i*8),Top_line[i+1]+1, attr);
	}
	vNLineYBit(16, 0, MaxY, !rev_flag);
}

void vSetMenuOpt(int n, WORD code)
{
	int i;

	for(i=0; i<panel[n].count; i++) {
		if(code & (0x01<<i)) {
			panel[n].menu[i][0] = '0';
		}
		else panel[n].menu[i][0] = '2';
	}
}

void vRevTopMenu(int n)
{
	vMCOFF();
	if(n<1) vXORBarBytex16(0,0,2);
	else vXORBarBytex16(0,2+8*(n-1),8);
	vMCON();
}



void vSetMenu(int n,int x, int y, int count,BYTE **menu, BYTE *head)
{
	WORD width=0;
	int endx,endy, i;

	width = strlen(menu[0]);
	for(i=1; i<count; i++) {
		if(width<strlen(menu[i])) width=strlen(menu[i]);
	}
	endx = x+3+((count)*17);
	endy = y+width+1;
	if(endx>MaxX) {
		endx=MaxX-5;
		x=endx-3-count*17;
	}
	if(head) {
		x-=18;
		panel[n].head=(BYTE far *)head;
	}
	panel[n].x = x;
	panel[n].y = y;
	panel[n].endx = endx;
	panel[n].endy = endy;
	panel[n].menu = (BYTE **)menu;
	panel[n].count = count;
	panel[n].last_menu = 1;
	panel[n].active = OFF;
}

	extern int maxchan;

void vInitMenu(void)
{
	vSetMenu( PAN_0,17,0,3,Est, 0);
	vSetMenu( PAN_1,17,2,8,File,0);
	vSetMenu( PAN_2,17,10,9,Edit,0);
	vSetMenu( PAN_3,17,18,6,Input,0);
	vSetMenu( PAN_4,17,26,5,Search,0);
	vSetMenu( PAN_5,17,34,6,Word,0);
	vSetMenu( PAN_6,17,42,7,Screen,0);
	vSetMenu( PAN_7,17,50,4,Print,0);
	vSetMenu( PAN_8,17,58,6,Special,0);
	vSetMenu( PAN_9,17,66,5,Option,0);

	vSetMenu( PAN_33,36+17*2,21,4,Macpan,0);
	vSetMenu( PAN_34,36+17*3,21,3,Glspan,0);
	vSetMenu( PAN_51,36,37,5,Select_word,0);

	vSetMenu( PAN_511,55,40,2,Hangul_key,0);
	vSetMenu( PAN_521,72,40,13,English_key,0);
	vSetMenu( PAN_531,89,40,4,Japan_key,0);
	vSetMenu( PAN_541,106,40,7,Special_char,0);
	vSetMenu( PAN_551,123,40,6,Giho_dohyong,0);
	vSetMenu( LINE_PAN, (MaxX-8*17)/2,(MaxY+1)/16-5, 8, Line, " ¬å¹·ŸA");
	vSetMenu( HJPAN,  (MaxX-10*17)/2,(MaxY+1)/16-5, 10, Hjpan,"   Ðe¸a´á");
	vSetMenu( SCADULER, 100, 20, 7, Span,0);
	vSetMenu( PRANGE, 100, 20, 5, P_range,0);
	vSetMenu( EXTPAN1,100,20,maxchan+1,H_font,0);
	vSetMenu( EXTPAN2,100,20,7,E_font,0);
	vSetMenu( EXTPAN3,100,20,3,J_font,0);
	vSetMenu( HF_PAN, (MaxX-10*17)/2,(MaxY+1)/16-5, 3, HF_dat, "  áŸ¡ i");
                                                             //"¡Ÿ¡ i"
	vSetMenu( JOPAN,  10,(MaxY+1)/8-16, 3, Jopan,0);
}


void vSetMenuParaAll(int n, int x, int y, int count, int ENode)
{
	WORD width=0;
	int endx,endy;

	if(count==0xff && ENode==0xff) {
		width = strlen(panel[n].menu[1]);
		endx = x+3+((panel[n].count)*17);
		endy = y+width+1;
		if(endx>MaxX) {
			endx=MaxX-5;
			x=endx-3-panel[n].count*17;
		}
		if(panel[n].head) x-=18;
		panel[n].x=x; panel[n].y=y;
		panel[n].endx=endx; panel[n].endy=endy;
	}
	if(count<0xff) panel[n].count=count;
	if(ENode<0xff) panel[n].last_menu=ENode;
}

void vSetMenuPara(int n,int x, int y)
{
	vSetMenuParaAll(n, x, y, 0xff, 0xff);
}

void vSaveMenuBk(int n)
{
	if (panel[n].active==OFF) {
		if(n <PAN_33) {
			vGetImage((panel[n].x),(panel[n].y),
			(panel[n].endx)+30,(panel[n].endy)+4, our_buf);
		}
		else if(n <PAN_511) {
			vGetImage((panel[n].x),(panel[n].y),
			(panel[n].endx)+30,(panel[n].endy)+4, our_buf+5000);
		}
		else if(n <JOPAN)  {
			vGetImage((panel[n].x),(panel[n].y),
			(panel[n].endx)+30,(panel[n].endy)+4, our_buf+12000);
		}
		panel[n].active = ON;
	}
}

void vRestoreMenuBk(int n)
{

	if(panel[n].active==ON) {
		vMCOFF();
		if(n <PAN_33) {
			 vPutImage((panel[n].x),(panel[n].y),
			 (panel[n].endx)+30,(panel[n].endy)+4, our_buf);
		}
		else if(n <PAN_511) {
			 vPutImage((panel[n].x),(panel[n].y),
			 (panel[n].endx)+30,(panel[n].endy)+4, our_buf+5000);
		}
		else if(n <JOPAN) {
			 vPutImage((panel[n].x),(panel[n].y),
			 (panel[n].endx)+30,(panel[n].endy)+4, our_buf+12000);
		}
		panel[n].active = OFF;
		vMCON();
	}
}

void vRevMenuItem(int n, int choice)
{
	int x=panel[n].x;
	int y=panel[n].y;
	int endy=panel[n].endy;

	vMCOFF();
	if(panel[n].head) x+=18;
	vXORBarBit(x+2+(choice-1)*17, y*8+2,x+18+(choice-1)*17, endy*8-2);
	vMCON();
}

extern BYTE sc[];
extern BYTE wc[];
extern BYTE *ext_1;
extern BYTE *ext_2;
extern int hj_len;

void vPutMenu(int n)
{
	register int i, j;
	BYTE flag=1,attr=0, t;
	int choice;
	int x = panel[n].x;
	int y = panel[n].y;
	int endx = panel[n].endx;
	int endy = panel[n].endy;

	vMCOFF();
	if((!rev_flag || n>PAN_551) && n!=JOPAN) {
		flag=0;
		attr=0x00;
	}
	else {
		flag=1;
		attr=0x04;
	}
	if(n!=JOPAN) vSaveMenuBk(n);
	vDrawPanelByte(x, y, endx, endy, flag);
	if(panel[n].head) x+=18;
	for(i=0; i<panel[n].count; i++) {
		if((!rev_flag || n>PAN_551) && n!=JOPAN) attr=0x00;
		else attr=0x04;
		t=panel[n].menu[i][0]-48;
		switch(t) {
			case 0 : break;
			case 1 :
				if(block_task) attr|=0x40;
				break;
			case 2 :
				if(block_task==OFF) attr|=0x40;
				break;
			case 3 : attr|=0x10;
		}
		if(n==LINE_PAN) {
			vClipOff();
			sc[1]=attr;
			sc[2]=0x11;
			sc[3]=0x23;
			for(j=0;j<9;j++) {
				if(j%4)	sc[0]=33+i*11+9;
				else sc[0]=33+i*11+j/4;
				vPHFfar(x+2+i*17, y+1+j, sc);
			}
			vClipOn(cur_w);
		}
		else if(n==HJPAN) {
			if(i==0) vWriteString(x+2+i*17, y+1, ext_1, attr);
			else {
				wc[2]=attr;
				wc[3]=0x11;
				for(j=0;j<hj_len;j++) {
					wc[0]=ext_1[j*2];
					wc[1]=ext_1[j*2+1];
					wc[4]=0x80 | (ext_2[(i-1)*hj_len+j]-1);
					vPFFfar(x+2+i*17, y+1+j*2, wc);
				}
			}
		}
		else {
			vWriteString(x+2+i*17, y+1, panel[n].menu[i]+1, attr);
		}
	}
	if(panel[n].head) {
		if((!rev_flag || n>PAN_551) && n!=JOPAN) {
			vNLineYBit(x+2-18+16, y*8, endy*8, 1); attr=0x00;
		}
		else {
			vNLineYBit(x+2-18+16, y*8, endy*8, 0); attr=0x04;
		}
//		i=((endy-y+1)-strlen(panel[n].head))/2;
		vWriteString(x+1-18, y+1, panel[n].head, attr);
	}
	choice=panel[n].last_menu;
	vRevMenuItem(n, choice);
	vMCON();
}

int iIsIn(BYTE n, BYTE code)
{
	BYTE i, t;
	for(i=0; i<panel[n].count; i++) {
		t=strlen(panel[n].menu[i]);
		if(panel[n].menu[i][t-1]==code) {
			if(panel[n].menu[i][t-2]==' ') return i+1;
		}
	}
	return -1;
}

int iGetUserResponse(int n)
{
	int t_choice,flag=0, t, t2;
	INKEY c;
	int key=0;
	int choice=panel[n].last_menu;   

	for(;;) {
		if(rbp) {
			while(rbp);
			return -1;
		}
		for(;lbp;) {
			vResetKeyBuffer();
			t_choice=iMouseInMenu(0,-1);
			if(t_choice != -1) {
				if(t_choice !=top_choice) {
					top_choice=t_choice;
					return -2;
				}
			}
			else {
				t_choice=iMouseInMenu(1,n);
				if(t_choice!=choice) {
					if(t_choice!=-1) {
						if(flag==0) vRevMenuItem(n, choice);
						choice=t_choice;
						vRevMenuItem(n, choice);
						flag=0;
					}
					else if(flag==0) {
						vRevMenuItem(n, choice);
						flag=1;
						choice=t_choice;
					}
				}
				if(lbp==0) {
					if(iMouseInMenu(1, n)==-1) return -1;
					t=panel[n].menu[choice-1][0]-48;
					if((block_task==OFF && t<2) || (block_task && t==2 || t==0)) {
						if(choice==-1) choice=1;
						panel[n].last_menu=choice;
						if(t_choice==-1) return -1;
						else if(panel[n].menu[choice-1][0]!='3') {
							return choice;
						}
					}
				}
			}
		}
		if(wCheckKeyHit()) {
			c.i=bioskey(0);
			if(n<PAN_551+1) {
				key=iCheckHotKey(c);
				if(key) return key;
			}
			vRevMenuItem(n, choice);
			if(47<c.ch[0] && c.ch[0]<58 && n<PAN_9+1) {
				if(key==47) {
					key=10;
				}
				else key=c.ch[0]-48;
				if(key<11) {
					top_choice=key;
					return -2;
				}
			}
			if(c.ch[0]) {
				switch(c.ch[0]) {
					case '\r' :
						t=panel[n].menu[choice-1][0]-48;
						if(block_task) {
							if(t==1 || t==3) break;
						}
						else {
							if(t==2 || t==3) break;
						}
						vRevMenuItem(n, choice);
						panel[n].last_menu = choice;
						return choice;
					case SPACE :
//						if(n>EXTPAN1-1) break;
						panel[n].last_menu = choice;
						choice++;
						if(choice>panel[n].count) choice=1;
						break;
					case ESC :
						panel[n].last_menu = choice;
						return -1;
					default :
						if((c.ch[0]>64 && c.ch[0]< 91) || (c.ch[0]>96 && c.ch[0]<123) || (c.ch[0]>46 && c.ch[1]<57)) {
							if(c.ch[0]>96 && c.ch[0]<123) c.ch[0]-=32;
							t=iIsIn(n, c.ch[0]);
							t2=panel[n].menu[t-1][0]-48;
							if(block_task) {
								if(t2==1 || t2==3) break;
							}
							else {
								if(t2==2 || t2==3) break;
							}
							if(t!=-1) {
								choice=t;
								panel[n].last_menu=choice;
								return choice;
							}
						}
						break;
				}
			}
			else {
				switch(c.ch[1]) {
					case DOWN :
						panel[n].last_menu = choice;
						choice++;
						if(choice>panel[n].count) choice=1;
						break;
					case UP :
						panel[n].last_menu = choice;
						choice--;
						if(choice<1) choice=panel[n].count;
						break;
					case RIGHT :
							if(n>9) break;
							panel[n].last_menu=choice;
							top_choice++;
							if(top_choice==10) top_choice=0;
							return -2;
					case LEFT :
							if(n>9) break;
							panel[n].last_menu=choice;
							top_choice--;
							if(top_choice<0) top_choice=9;
							return -2;
					case HOME :  case  PGUP:
						choice=1; break;
					case END : case  PGDN :
						choice=panel[n].count;
						break;
					case DELETE :
						if(n!=HJPAN || choice==1) break;
						panel[n].last_menu=choice;
						return -3;
					case F1 :
						vHelpDlg(top_choice+1);
						break;
				}
			}
			vRevMenuItem(n, choice);
		}
	}
}

int iPullDownMenu(int n)
{
	INKEY t;
	BYTE  c;
//	vTest();
	if(wCheckKeyHit()) {
		t.i=bioskey(1);
		if(t.ch[1]==RIGHT) {
			if(n<10) {
				bioskey(0);
				top_choice++;
				if(top_choice==10) top_choice=0;
				return -2;
			}
		}
		else if(t.ch[1]==LEFT) {
			if(n<10) {
				bioskey(0);
				top_choice--;
				if(top_choice<0) top_choice=9;
				return -2;
			}
		}
	}
	if(n==PAN_6) {
		if(ws_loc<2) {
			if(block_task) c='1';
			else c='2';
		}
		else c='1';
		panel[PAN_6].menu[1][0]=c;
	}
	if(panel[n].active==0) {
		vPutMenu(n);
	}
	return iGetUserResponse(n);
}

int iGeneralMenuDrv(int pan_n,int add_val,int mul_val)
{
	int select1,n;
	select1=iPullDownMenu(pan_n);
	if(select1>50) n=select1;
	else {
		switch(select1) {
			case -2 : case -1 :
				n=select1; break;
			default :
				n=add_val+select1*mul_val;
		}
	}
	vRestoreMenuBk(pan_n);
	return n;
}

int iEditMenuDrv(void)
{
	int choice,n;
	for(;;) {
		choice=iPullDownMenu(PAN_2);
		if(choice<0) {
			vRestoreMenuBk(PAN_2);
			return choice;
		}
		else {
			if(choice>50) n=choice;
			else n=(choice)*10+200;
		}
		if(n>0 || n==-2) {
			vRestoreMenuBk(PAN_2);
			return n;
		}
	}
}

int iInputMenuDrv(void)
{
	int select1=0, n=-2;
	for(;;) {
		select1=iPullDownMenu(PAN_3);
		if(select1>50) n=select1;
		else {
			switch (select1) {
				case -2 : case -1 :
					vRestoreMenuBk(PAN_3);
					return select1;
				case  3 :	n=iGeneralMenuDrv(PAN_33, 330, 1); break;
				case  4 : n=iGeneralMenuDrv(PAN_34, 340, 1); break;
				default :
					n=select1*10+300; 
					break;
			}
		}
		if(n>0 || n==-2) {
			vRestoreMenuBk(PAN_3);
			return n;
		}
	}
}

BYTE key_table[]={
	1, 2, 0,
	ENGLOC, ENGLOC+1, ENGLOC+2, ENGLOC+3, ENGLOC+4, ENGLOC+5,	ENGLOC+6,
	ENGLOC+7, ENGLOC+8, ENGLOC+9, ENGLOC+10, ENGLOC+11,
	JAPANLOC, JAPANLOC+1, JAPANLOC+2,  JAPANLOC+3,
	HALFLOC, HALFLOC+1, FULLLOC, FULLLOC+1, FULLLOC+2, FULLLOC+3, FULLLOC+4,
	HALFLOC+2, HALFLOC+3, FULLLOC+5, FULLLOC+6, FULLLOC+7, FULLLOC+8
};

int iKeyMapMenuDrv(void)
{
	int select1,n=-2;

	for(;;) {
		select1=iPullDownMenu(PAN_51);
		if(select1>50) n=select1;
		else {
			switch (select1) {
				case -1 : case -2 :
					vRestoreMenuBk(PAN_51);
					return select1;
				case  1 :
					n=iGeneralMenuDrv(PAN_511,0,1); break;
				case  2 :
					n=iGeneralMenuDrv(PAN_521,2,1); break;
				case  3 :
					n=iGeneralMenuDrv(PAN_531,JAPANLOC,1); break;
				case  4 :
					n=iGeneralMenuDrv(PAN_541,HALFLOC,1); break;
				case  5 :
					n=iGeneralMenuDrv(PAN_551,HALFLOC+7,1); break;
				case  6 : n=26; break;
				case  7 : n=27; break;
			}
		}
		if (n!=-1 && n<50) {
			tw[cur_w].h_e=key_table[n-1];
			if(tw[cur_w].h_e>1) {
				if(tw[cur_w].h_e<JAPANLOC) {  // etc
				}
				else if(tw[cur_w].h_e<HALFLOC) {  // japan input automata
				}
				else if(tw[cur_w].h_e<FULLLOC) { // half input automata
					sc[3]=0x20 | (tw[cur_w].h_e-HALFLOC);
				}
				else {  // full input automata
					wc[4]=0xc0 | (tw[cur_w].h_e-FULLLOC);
				}
			}
			vRestoreMenuBk(PAN_51);
			if(disp_japan) vDrawKeyMap();
			return 1;
		}
	}
}


int iWordMenuDrv(void)
{
	int select1=0,  n=-2;
	for (;;) {
		select1=iPullDownMenu(PAN_5);
		if(select1>50) n=select1;
		else {
			switch (select1) {
				case -2 : case -1 :
					vRestoreMenuBk(PAN_5);
					return select1;
				case  1 : n=iKeyMapMenuDrv(); break;
				case  2 : n=599; break; 
				default :
					n=500+select1*10;
					break;
			}
		}
		if(n>0 || n==-2) {
			vRestoreMenuBk(PAN_5);
			return n;
		}
	}
}


int iPrintDrv(void)
{
	int select1=0,  n=-2;
	for (;;) {
		select1=iPullDownMenu(PAN_7);
		if(select1>50) n=select1;
		else {
			if(select1<0) {
				vRestoreMenuBk(PAN_7);
				return select1;
			}
			else {
				n=700+select1*10;
			}
		}
		if(n>0 || n==-2) {
			vRestoreMenuBk(PAN_7);
			return n;
		}
	}
}

int iMenuDrvier(void)
{
	int n=1;
	static int block_top=2;
	top_choice=last_top;

	vMCOFF();
	vGetImage(0, 0, 17, (MaxY+1)/8, our_buf+25000);
	vDrawTopMenu();
	if(block_task) {
		last_top=block_top;
		top_choice=block_top;
	}
	vRevTopMenu(last_top);
	for(;;) {
		switch(top_choice) {
			case -1 :
				vRevTopMenu(top_choice);
				vMCOFF();
				vPutImage(0, 0, 17, (MaxY+1)/8, our_buf+25000);
				return -1;
			case 0  : n = iGeneralMenuDrv(PAN_0,0,10); break;
			case 1  : n = iGeneralMenuDrv(PAN_1,100,10);break;
			case 2  : n = iEditMenuDrv(); break;
			case 3  : n = iInputMenuDrv(); break;
			case 4  : n = iGeneralMenuDrv(PAN_4,400,10);break;
			case 5  : n = iWordMenuDrv(); break;
			case 6  : n = iGeneralMenuDrv(PAN_6,600,10);break;
			case 7  : n = iPrintDrv(); break;
			case 8  : n = iGeneralMenuDrv(PAN_8,800,10);break;
			case 9  : n = iGeneralMenuDrv(PAN_9,900,10);break;
		}
		if(n==-2) {
			vRevTopMenu(last_top);
			vRevTopMenu(top_choice);
			last_top=top_choice;
		}
		else {
			vRevTopMenu(last_top);
			if(block_task) block_top=top_choice;
			vMCOFF();
			vPutImage(0, 0, 17, (MaxY+1)/8, our_buf+25000);
			return n;
		}
	}
}



void vHideRod(void)
{
	vNBarBit(tw[cur_w].x+51-14, tw[cur_w].y*8+3, tw[cur_w].x+51, tw[cur_w].y*8+8, 0);
	vNLineYBit(tw[cur_w].x+51-14, tw[cur_w].y*8+3, (tw[cur_w].y+tw[cur_w].ly)*8-4, 0);
}



void vSetRod(void)
{
	if(tw[cur_w].active==0) return;
	if(show_rod) {
		vPutRod(cur_w, 0);
		bminx=tw[cur_w].x+51;
		vBLineYBit(bminx, bminy, bmaxy);
	}
	else {
		vHideRod();
		bminx=tw[cur_w].x+51-14;
	}
}

void vChangeActiveWindow(void)
{
	unsigned long i;
	INKEY ch;

	i=Scount;
	while(Scount<i+9) {
		if(wCheckKeyHit()) {
			ch.i=bioskey(0);
			if(ch.ch[1]<=ALT_0 && ch.ch[1]>=ALT_1) vChangeWindow(ch.ch[1]-119);
			else if(ch.ch[0]>='0' && ch.ch[0]<'6') vChangeWindow(ch.ch[0]-48);
			else break;
			return;
		}
	}
	vChangeTextWindowDlg();
}

//#include "\est\21\stamp.h"

void vLogo(void)
{
	FILE *fp;
	int x,y,ex,ey;
	WORD mh=0;
	WORD i;
	BYTE t;

	x=MaxX/2-200/2;
	y=MaxY/16-50/2;
	ex=x+199; ey=y+49;

	sprintf(g_dir, "%s\\21.LOG", exe_path);
	if((fp=fopen(g_dir,"rb"))==NULL) {
		vErrorMsgfDlg(0, 1, "21.LOG");
		vExit();
	}
	vMCOFF();
	vGetImage(x, y, ex+2, ey+2, our_buf);
	if(!iSwapMem(MSWRITE, 202*60, &mh, our_buf)) vErrorMsgfDlg(1, 2, 0);
	fread(our_buf, 10000, 4, fp);
//	fread(g_dir, 80, 1, fp);
	fclose(fp);

	vBarByte(x, y, ex, ey, rev_flag);
	if(!color_mode) {
		for(i=0;i<10000;i++) {
			our_buf[i]=our_buf[i] & our_buf[i+10000] & our_buf[i+20000] & our_buf[i+30000];
		}
	}
	if(!rev_flag) {
		for(i=0;i<10000;i++) {
			t=our_buf[i] & our_buf[i+10000] & our_buf[i+20000] & our_buf[i+30000];
			t^=0xff;
			our_buf[i]&=t;
			our_buf[i+10000]&=t;
			our_buf[i+20000]&=t;
			our_buf[i+30000]&=t;
		}
	}
	if(!color_mode) vPutImage(x, y, ex, ey, our_buf);
	else vPutColorImageFarEByte(x, y, ex, ey, our_buf);
//	vWriteString(x+106, y+32, "»¡Ñw·¡Ÿi ¶áÐ!!", rev_flag<<2);


//	vPutColorImageLByte(x+125, y+41, 40, 5, 4, stamp);
	vMCON();
	for(;;) {
		no_work=0;
		if(wCheckKeyHit()) {
			bioskey(0);
			break;
		}
		if(lbp || rbp) break;
	}
	vMCOFF();
	if(!iSwapMem(MSREAD, 202*60, &mh, our_buf))
		vErrorMsgfDlg(1, 2, 0);
	vFreeMem(&mh);
	vPutImage(x, y, ex+2, ey+2, our_buf);
	vMCON();
}


void vBeep(void)
{

	printf("\7");
}

extern WORD no_work;
extern BYTE cur_lnum;

int iAction(int n)
{
	if(!tw[cur_w].active) {
		if((n>200 && n<800) || n>1000) {
//		|| (n<100 && n>0) || n==130 || n==140) {
			vErrorMsgfDlg(1, 3, 0);
			return -1;
		}
	}
	if(n<400 || n==900 || n==901) {
		switch(n) {
			case   -2 : return 1;
			case   -1 : break;           
			case   10 : vLogo(); break;        
			case 901 : case 20 : vHelpDlg(0); break;
			case 900 : case 30 : vResetDesktop(); break;
//			case   40 : vExcuteSubEXE(4); break;
//			case   50 : vExcuteSubEXE(2); break;
			case  110 :	vNewText(); break;
			case	120	:	vOpenTextDlg(1); break;
			case  130 :	iSaveTextWindow(); break;
			case  140 :	vNewTextWindow();	break;
			case  150 :	iSaveAsDlg(); break;
			case  160 :	vExcuteSubEXE(3); break;
			case  170 : vDosShell(); break;
			case  180 : vExit();  break;
			case  210 : vCtrlKKey(ON); break;
			case  220 : vClearBlock(); break;
			case  230 : iWriteBlockText(1); break;
			case  240 : vPasteBlock(); break;
			case  250 : vReadBlock(1); break;
			case  260 : vReadBlock(2); break;
			case  270 :	vReadBlock(0);	break;
			case  280 : iWriteBlockText(0); break;
			case  290 : vClipBoard(); break;
			case  310 : vParagraphDlg();break;
			case  320 : vLineDraw(); break;
			case  331 : vSetMacro(); break;
			case  332 : vEtcFileIODlg(0x80, 4); break;
			case  333 : vEtcFileIODlg(0x81, 5); break;
			case  334 : vUseMacroRepeat(); break;
			case  341 : vFreqTextDlg(); break;
			case  342 : vEtcFileIODlg(0, 6); break;
			case  343 : vEtcFileIODlg(1, 7); break;
			case  350 : vInsertHeaderNode(0); break;
			case  360 : vInsertSpecialCode(0); break;
		}
	}
	else {
		switch(n) {
			case  410 : vSearchStringDlg(); break;
			case  420 : vChangeStringDlg(); break;
			case  430 : vSearchAgain(); break;
			case  440 : vGotoPageDlg(0); break;
			case  450 :	vGotoLine(); break;
			case  530 : vCircleKeyboardDlg(); break;
			case  540 : vF9Key(); break;
			case  550 : vHanja2Hangul(); break;
			case  560 : vUpdateHanjaWordDlg(); break;
			case  599 : vSetFontEffect(); break;
			case  610 : vMaximizeWindow(); break;
			case  620 : vNextWindow(); break;
			case  630 : vKeyMoveTextWindow(cur_w); break;
			case  640 : vChangeTextWindowDlg(); break;
			case  650 : iCloseTextWindow(1); break;
			case	660	: iCloseAllTextWindow(); break;
			case  670 : vShowKeyMap(); break;
			case  710 : vPrintStart(); break;
			case  720 :	vPreview(); break;
			case  730 : vPrintOptDlg(cur_w); break;
			case  740 : vPrinterSetupDlg(); break;
			case  810 : vSortNodeDlg(); break;
			case  820 : vBlockSumDlg(); break;
			case  830 :	vDelLineChar(0); break;
			case  840 :	vDelLineChar(1); break;
			case  850 : vExcuteSubEXE(2); break;
			case  860 : vExcuteSubEXE(4); break;

//#else
//			case  850 : calendar(); break;
//			case  860 : calculator(); break;
			case  910 : vSetTab(0); break;
			case  920 : vDispOptDlg(); break;
			case  930 : vScreenSaverDlg(); break;
			case  940 : vMouseOptDlg(); break;
			case  950 : vEtcDlg(); break;
			case 1009 : show_rod^=1; vSetRod(); break;
			case 1010 : case 1011 : case 1012 : case 1013 :
				vSetMargin(n-1010); break;
			case 1014 : case 1015 :
				vAdjParagraph(n-1014); break;
			case 1016 : case 1017 :
				vSetMargin(n-1016+8); break;
			case 1020 : vUserDividePage(); break;
		}
	}
	return -1;
}

void vLetsGo(void)
{
	vMCON();
	for(;;) {
		if(iAction(iEditor())!=1);
		else iAction(iMenuDrvier());
	}
}

