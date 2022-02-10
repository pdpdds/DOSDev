#define  _PRNDRV
#include <stdio.h>
#include <mem.h>
#include <stdarg.h>
#include <dos.h>
#include <alloc.h>
#include <conio.h>
#include <stdlib.h>
#include "est.h"
#include "21.h"
#include "proto.h"

#define u_gap    PD[0]   // up free space by 'mm' unit
#define d_gap    PD[1]   // down  ~
#define l_gap    PD[2]   // vLeftKey  ~
#define r_gap    PD[3]   // vRightKey ~
#define w_gap    PD[4]   // WORD gap by 'dot' unit
#define repeat   PD[5]   // printing repeat
#define range_s  PD[6]   // range_s =0xffff (print all)
#define range_e  PD[7]   // range vExit
#define zoom     PD[8]   // printing Zoom rate (ex: 50%-200%)
#define pport    PD[9]   // LPT1=0, LPT2=1;
#define p_width  PD[10]  // paper width  by 'Dot(bit)' unit
#define p_len    PD[11]  // paper length by 'Dot(bit)' unit
#define supply   PD[12]  // paper supplying way (0=continous, 1=NATJANG)
#define head_len PD[13]  // Header length in 'mm' unit
#define foot_len PD[14]  // Footer ~
#define lgap_mul PD[15]  // line gap mutipled quantity 1~5

void huge (*PRNfunc[10])();
#define PRNopt   (*PRNfunc[0])  // setting option
#define PRNlinef (*PRNfunc[1])  // line feed
#define PRNreset (*PRNfunc[2])  // printer reset
#define PRNsetP  (*PRNfunc[3])	// set page
#define PRNtbuf  (*PRNfunc[4])  // print buf
#define PRNformf (*PRNfunc[5])	// form feed
#define PRNunit  (*PRNfunc[6])  // get units
#define is_ESC ((bioskey(0) & 0xff)==0x001b)

int START_PAGE_NUM=1;
int CUR_PAGE_NUM;

int TDEN=300;    // dot per inch
int CWIDE=24;    // default Char wide
int CLEN=24;     // default Char length
int BWID=610;    // buffer width   610 400
int BLEN=60;     // buffer length   60  85
int IMGLEN=48;   // image length    48  80
int CW=12;			 // single char width(english)
int CL=24;       // single char length
int WG=1;        // WORD gap unit setting factor  'CW/10'
int PD[20];
int div_sx, div_ex;
int PRN_bit=0;   // CNode printed dot

BYTE is_head=0;  // header exist or not
BYTE is_foot=0;  // footer exist or not
BYTE PRN_err=0;  // PRINTING error flag (if 'ESC' pressed!!)
BYTE *PRNaddr=0; // printing driver loaded address(use when 'free')

extern BYTE *our_buf;
extern BYTE *STR;
extern WORD work_pagex;





/* PRNDRV2.c */
void vFreeDriver ( void );
int PRNm2d ( int i , int dpi );
int iInitPrint ( int *arg );
int iPrintFile ( NODE *vHomeKey );
int iInitPrinterDriver ( BYTE *path );

static void PRN_setp ( int dpi );
static BYTE bIsInRange ( int pn );
static int iMakeLineGap ( BYTE *str , int flag );
static void vPutLineGap ( BYTE *str , int lc0 , int vLeftKey , int width );
static int near iPTP ( int flag , BYTE *Pstr );
static int iPrintLine ( int n , int Hsize , BYTE *str2 , BYTE *lc , BYTE attr );
static void vPrintHeader ( int pn );
static void vPrintFooter ( int pn );
static int iNewPaper ( int pn );

int SCR2PRN(const int n)
{
	return (int)((unsigned long)n*CL/16);
}

int PRNm2d(const int i, const int dpi)
{
	return (int)((unsigned long)i*dpi*393/10000);
}

static void PRN_setp(const int dpi)
{
	u_gap=PRNm2d(u_gap, dpi);
	d_gap=PRNm2d(d_gap, dpi);
	l_gap=PRNm2d(l_gap, dpi);
	head_len=PRNm2d(head_len, dpi);
	foot_len=PRNm2d(foot_len, dpi);
	p_width=PRNm2d(p_width, dpi);
}

int iInitPrint(int *arg)
{
	int Data[10], i;
	int Opt[5];

	for(i=0; i<20; i++)
		PD[i]=arg[i];
	PRNunit((int far *)Opt);               // get drivers ability
	if(Opt[0]!=10) {
		vErrorMsgfDlg(0, 5, "ÏaŸ¥Èá —aœa·¡¤á·");
		return 0;
	}
	CLEN=Opt[2]*zoom/100;
	CWIDE=Opt[3]*zoom/100;
	TDEN=Opt[4];

	vSetUnit(CLEN, CWIDE);
	vSetImageHeight(IMGLEN);
	CW=CWIDE/2;	CL=CLEN;	WG=CW/10;

	PRN_setp(TDEN);	// This must be here
	Data[0]=PD[9];	   Data[1]=BLEN;
	Data[2]=BWID;  Data[3]=l_gap;
	Data[4]=FP_SEG(our_buf);
	Data[5]=FP_OFF(our_buf);
	Data[6]=FP_SEG(&PRN_err);
	Data[7]=FP_OFF(&PRN_err);
	Data[8]=p_width;
	PRNopt((int far *)Data);
	PRNreset();
	PRNsetP();
	return 1;
}

static BYTE bIsInRange(int pn)
{
	BYTE flag=0;

	if((WORD)range_s==0xffff) {
		switch(range_e) {
			case 0 : flag=1; break;
			case 1 : flag=1; break;
			case 2 :
				if((pn % 2)==0) flag=1;
				else flag=0;
				break;
			case 3 :
				if(pn % 2) flag=1;
				else flag=0;
				break;
		}
	}
	else {
		if(pn >= range_s) {
			if(pn<= range_e) flag=1;
			else {
				return 0xff;
			}
		}
		else flag=0;
	}
	return flag;
}



//###################### From here line analizing ##################

BYTE PRNlc[11]={
	0x0c, 0x0d, 0x09, 0x0e,
	0x0f, 0x0b, 0x06, 0x07,
	0x03, 0x05, 0x0a
};

static int iMakeLineGap(BYTE *str, int flag)
{
	int v;
	int y=0, add;
	int line_flag=0;
	BYTE lnc[4]={0, 0, 0x11, 0x23};

	while(1) {
		if(*str>127) {
			if(*(str+2)>127 && y) break;
			v=((*(str+3) & 0x0f)<<1);
			y+=(w_gap*WG+CW)*v;
			str+=5;
		}
		else {
			if(*str==0) {
				y+=CW+w_gap*WG;
				str++;
				continue;
			}
			if(*str==13) break;
			if(*(str+1)>127 && y) break;
			v=(*(str+2) & 0x0f);
			add=v*w_gap;
			v*=CW;
			if(*(str+3)==0x23) {
				if(PRNlc[(*str-33)%11] & 8) {
					lnc[0]=((*str-33)/11)*11+43;
					lnc[1]=*(str+1);
					lnc[2]=*(str+2);
					vPHFfar(0, y, lnc);
					vPutImageToPrintBuffer(0, y, flag, v);
					line_flag=1;
				}
			}
			y+=v+add*WG;
			str+=4;
		}
	}
	return line_flag;
}

static void vPutLineGap(BYTE *str, int lc0, int vLeftKey, int width)
{
	int i, ccx;

	if(width<2 || PRN_err) return;
	vSetImageStartLocation(0);
	for(i=0;i<lc0;) {
		ccx=lc0-i;
		if(ccx>IMGLEN) ccx=IMGLEN;
		setmem(our_buf, BWID*BLEN, 0);              // clear buffer
		if(iMakeLineGap(str, ccx))
			PRNtbuf(vLeftKey, ccx, width);
		else PRNlinef(ccx);
		if(PRN_err) return;
		i+=IMGLEN;
	}
}

static int near iPTP(int flag, BYTE *Pstr)
{
	int h=0, v;
	int y=0, add;
	int tx;
	int ltx;
	int i;
	BYTE lnc[4]={0, 0, 0x11, 0x23};
	BYTE nb[5];
	BYTE numc[4]={0, 0, 0x11, 0x42};

	while(1) {
		if(*Pstr>127) {
			if(*(Pstr+2)>127 && h) break;
			v=((*(Pstr+3) & 0x0f)<<1);
			add=v*w_gap;
			v*=CW;
			h=SCR2PRN((*(Pstr+3) & 0xf0));
			tx=flag-h;
			if(tx<div_ex) {
				if(tx<div_sx) {
					ltx=0;
					vSetImageStartLocation(div_sx-tx);
					if(div_ex>=flag) tx=h-div_sx+tx+1;
					else tx=0;
				}
				else {
					ltx=tx-div_sx;
					vSetImageStartLocation(0);
					if(div_ex>=flag) tx=h;
					else tx=0;
				}
				if(*Pstr==0xff && *(Pstr+1)==0xff) {
					sprintf(nb, "%4d", CUR_PAGE_NUM+START_PAGE_NUM-1);
					numc[1]=Pstr[2];
					numc[2]=Pstr[3];
					for(i=0;nb[i];i++) {
						if(nb[i]!=32) {
							numc[0]=nb[i]-48;
							vPHFfar(tx, y, numc);
							vCutImage(h, v/2, v/2);
							vPutImageToPrintBuffer(ltx, y, h, v/2);
						}
						y+=(v/2+add/2*WG);
					}
					Pstr+=5;
					continue;
				}
				else {
					vPFFfar(tx, y, Pstr);
					vCutImage(h, v, v);
					vPutImageToPrintBuffer(ltx, y, h, v);
				}
			}
			y+=v+add*WG;
			Pstr+=5;
		}
		else {
			if(*Pstr==0) {
				y+=CW+w_gap*WG;
				Pstr++;
				continue;
			}
			if(*Pstr==13) break;
			if(*(Pstr+1)>127 && h) break;
			if(((*(Pstr+3)) & 0x0f)==0x0f) {
				y+=CW+w_gap*WG;
				Pstr+=4;
				continue;
			}
			v=(*(Pstr+2) & 0x0f);
			add=v*w_gap;
			v*=CW;
			h=SCR2PRN((*(Pstr+2) & 0xf0));
			tx=flag-h;
			if(tx<div_ex) {
				if(tx<div_sx) {
					ltx=0;
					vSetImageStartLocation(div_sx-tx);
					if(div_ex>=flag) tx=h-div_sx+tx+1;
					else tx=0;
				}
				else {
					ltx=tx-div_sx;
					vSetImageStartLocation(0);
					if(div_ex>=flag) tx=h;
					else tx=0;
				}
				vPHFfar(tx, y, Pstr);
				vCutImage(h, v, v);
				vPutImageToPrintBuffer(ltx, y, h, v);
				if(*(Pstr+3)==0x23) {               // Word gap processing
					if(PRNlc[(*Pstr-33)%11] & 4) {
						lnc[0]=((*Pstr-33)/11)*11+42;
						lnc[1]=*(Pstr+1);
						lnc[2]=(*(Pstr+2) & 0xf0 ) | ((add+9)/10);
						vPHFfar(tx, y, lnc);
						vCutImage(h, v, add);
						vPutImageToPrintBuffer(ltx, y+v, h, v);
					}
				}
			}
			y+=v+add*WG;
			Pstr+=4;
		}
	}
	return y;
}

static int iPrintLine(int n, int Hsize, BYTE *str2, BYTE *lc, BYTE attr)
{
	int st, ccx, Cn, tmp;
	WORD Wid;

	if(attr==0) PRN_bit+=(Hsize/CLEN)*(TDEN*10/75);
	for(Cn=0;Cn<Hsize;) {
		if(wCheckKeyHit()) {
			if(is_ESC) return 0;
		}
		div_sx=Cn;
		div_ex=Cn+IMGLEN;
		setmem(our_buf, BWID*BLEN,  0);
		Wid=iPTP(Hsize, str2)+7; Wid=(Wid>>3)+1;
		if(Wid<2) {
			tmp=TDEN*10/75+lc[0]*lgap_mul;
			PRNlinef(tmp);
			break;
		}
		if(n==0) {
			st=(int)((char)lc[1])+lc[2]-1;
			if(attr) st-=2;
		}
		else st=lc[2]-1;
		st =st*(CW+w_gap*WG);
		ccx=Hsize-Cn;
		if(ccx>IMGLEN) ccx=IMGLEN;
		PRNtbuf(st, ccx, Wid);
		if(PRN_err) return 0;
		Cn+=IMGLEN;
	}
	vPutLineGap(str2, lc[0]*lgap_mul, st, Wid);
	if(attr==0) PRN_bit+=lc[0]*lgap_mul;
	vShowPrintStatus();
	return 1;
}

extern NODE *head_node[];
extern NODE *foot_node[];
	extern int ex_err;

static void vPrintHeader(int pn)
{
	int Hsize, i, len=0, tmp;
	BYTE *hs, *lc, *str, flag=0;
	NODE *H[2];

	PRN_bit=0;
	for(i=0; i<4; i++) if(head_node[i]) flag=1;
	if(!flag) {
		PRNlinef(head_len);
		return;
	}
	if(pn%2) {
		H[0]=head_node[0];
		H[1]=head_node[1];
	}
	else {
		H[0]=head_node[2];
		H[1]=head_node[3];
	}
	for(i=0; i<2; i++) {
		if(H[i]) {
			_CHECKNODE(H[i]);
			hs=H[i]->hs;
			lc=H[i]->lc;
			str=H[i]->line;
			Hsize=SCR2PRN((hs[0] & 0x70));
			tmp=(Hsize+lc[0]*lgap_mul);
			len+=tmp;
			if(len<head_len) {
				iPrintLine(0, Hsize, str, lc, 1);
			}
			else {
				PRNlinef(head_len-len+tmp);
				return;
			}
		}
	}
	PRNlinef(head_len-len);
}

static void vPrintFooter(int pn)
{
	int Hsize, i=2, j, l1, l2;
	BYTE *hs, *lc, *str, flag=0;
	NODE *F[2];

	for(i=0; i<4; i++) if(foot_node[i]) flag=1;
	if(!flag) goto OUT;
	if(PRN_bit<work_pagex) {
		PRNlinef(work_pagex-PRN_bit);
	}
//	else printf("PRN_bit is big %d ", PRN_bit-work_pagex);
	if(pn%2) {
		F[0]=foot_node[0];
		F[1]=foot_node[1];
	}
	else {
		F[0]=foot_node[2];
		F[1]=foot_node[3];
	}
	if(F[0]) {
		Hsize=SCR2PRN((F[0]->hs[0] & 0x70));
		l1=(Hsize+F[0]->lc[0]*lgap_mul);
	}
	if(F[1]) {
		Hsize=SCR2PRN((F[1]->hs[0] & 0x70));
		l2=(Hsize+F[1]->lc[0]*lgap_mul);
	}
	if(F[0]==0 && F[1]==0) goto OUT;
	if(l1>foot_len) goto OUT;
	if(l1+l2>foot_len) {
		j=1; PRNlinef(foot_len-l2);
	}
	else {
		j=2; PRNlinef(foot_len-l2-l1);
	}
	for(i=0; i<j; i++) {
		_CHECKNODE(F[i]);
		hs=F[i]->hs;
		lc=F[i]->lc;
		str=F[i]->line;
		Hsize=SCR2PRN((hs[0] & 0x70));
		iPrintLine(0, Hsize, str, lc, 1);
	}
OUT:
	PRN_bit=0; return;
}

static int iNewPaper(int pn)
{
	BYTE i;

	START_PAGE_NUM--;
	vPrintFooter(pn-1);
	START_PAGE_NUM++;
	PRNformf();
	if(bIsInRange(pn)!=0xff) {
		if(supply) {
			i=ihUserChoiceDlg("¶w»¡ˆa º…§¡–A´ö¯s“¡Œa?");
			if(i==1) return 0;
		}
		PRNlinef(u_gap);
		vPrintHeader(pn);
	}
	return 1;
}

extern NODE *s_node;

int iPrintFile(NODE *vHomeKey)
{
	WORD pn=1;
	BYTE Pflag=0, Old=0;
	BYTE *lc, *hs, *str;
	NODE *cur;
	WORD loc;
	int tmp, Hsize, n, ret=1;

	PRN_err=0;
	CUR_PAGE_NUM=pn;
	for(n=0; n<4; n++) {
		head_node[n]=0;	foot_node[n]=0;
	}
	s_node=vHomeKey;
	vFindHeader();
	if(bIsInRange(pn)) {
		PRNlinef(u_gap);
		vPrintHeader(pn);
	}
	cur=vHomeKey->next;
	for(;;) {
		if(cur==0) goto FO;
		while(cur->kd!=0) {
			if(iSetHeaderNodeFar(cur)==2) {
				cur=cur->next->next;
			}
			else cur=cur->next;
		}
		_CHECKNODE(cur);
		str=cur->line;  lc=cur->lc;
		hs=cur->hs;
		for(n=0; n<lc[5]; n++) {
			if(hs[n]>127) {
				pn++;
				CUR_PAGE_NUM=pn;
				if(Pflag) {if(!iNewPaper(pn)) {ret=0; goto OUT2;}}
			}
			Old=Pflag;
			Pflag=bIsInRange(pn);
			if(Pflag==0xff) goto OUT2;
			if(Old==0 && Pflag==1 && pn>1) {
				PRNlinef(u_gap); vPrintHeader(pn);
			}
			if(Pflag) {
				loc=wLine2Loc(n, cur);
				Hsize=SCR2PRN((hs[n] & 0x70));
//				PRN_bit+=((int)(hs[n]>>4) & 0x07)*(TDEN*10/75);
				tmp=iPrintLine(n, Hsize, str+loc, lc, 0);
				if(tmp==0) {ret=0; goto OUT;}
			}
		}
NEXT:
		cur=cur->next;
	}
FO:
	vPrintFooter(pn);
OUT:
	if(!PRN_err) PRNformf();
	if(!supply && !PRN_err) PRNreset();
OUT2:
	return ret;
}

//###### From here printing driver managing ######
//###### 1.exist check    2.load driver     ######
//###### 3.set subfunc    4.free it         ######
void far *drvaddr=0;



int iInitPrinterDriver(BYTE *path)
{
	int i;
	void huge **addr;

	if(drvaddr) return 1;
	if((drvaddr=(void far *)pInitOverlay(path))!=NULL) {
		addr=(void *)pGetModuleAddr();
		for(i=0; i<10; i++) {
			PRNfunc[i]=addr[i];
		}
		return 1;
	}
	else return 0;
}

void vFreeDriver(void)
{
	if(drvaddr==0) return;
	free(drvaddr);
	drvaddr=(BYTE *)0;
}
