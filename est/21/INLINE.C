#define _INLINE
#include <stdio.h>
#include <mem.h>
#include <dir.h>
#include <dos.h>
#include "est.h"

	#include "21.h"
	#include "proto.h"

#include "graph.h"

extern BYTE *vid_addr;
extern int graph_mode;
extern int MaxX, MaxY;
extern unsigned char *our_buf;
extern unsigned work_area_size;
extern unsigned far no_work;
extern BYTE aflag;

unsigned key_macro[10][101];
unsigned char key_macro_state[10][101];
unsigned char cur_macro=0;
unsigned char mac_loc=0;
unsigned char mac_use=0;
int MACcnt=1;
WORD k_buf[16];
int k_buf_loc=0;
int k_buf_loc2=0;


int wGetKey(char func)
{
	union REGS r;
	int rtn_num;

	if(mac_use==2) {
		if(func==0) return key_macro[cur_macro][mac_loc++];
		else if(func==2) return key_macro_state[cur_macro][mac_loc-1];
		else return key_macro[cur_macro][mac_loc];
	}
	if(k_buf_loc) {
		rtn_num=k_buf[k_buf_loc];
		if(k_buf_loc==k_buf_loc2) k_buf_loc=k_buf_loc2=0;
	}
	else {
		func+=0x10;
		r.h.ah=func;
		int86(0x16, &r, &r);
		if(func==0x10) {
			if(r.h.al==0xe0) r.h.al=0;
		}
		rtn_num=r.x.ax;
	}
	if(mac_use==1 && func==0x10) {
		key_macro[cur_macro][mac_loc]=rtn_num;
		r.h.al=0;
		r.h.ah=0x12;
		int86(0x16, &r, &r);
		key_macro_state[cur_macro][mac_loc++]=r.x.ax;
		if(mac_loc>99) {
			key_macro[cur_macro][100]=0;
			mac_use=0;
			vDispKeyInputStatus();
		}
//		iPrintf(MaxX-15, 0, 0, "%d", mac_loc);
	}
	return rtn_num;
}

void vPutKey(char scan_code, char ascii_code)
{
	int *p;

	disable();
	p=(int *)0x0040001aL;

	pokeb(0x0040, *(p+1), ascii_code);
	pokeb(0x0040, *(p+1)+1, scan_code);

	if(*(p+1)==0x3c) *(p+1)=0x1e;
	else (*(p+1))+=2;
	enable();
}



unsigned wCheckKeyHit(void)
{

	unsigned rtn=0;
//	int *kbs = (int *)0x0040001al;
//	int *kbe = (int *)0x0040001cl;

	if(mac_use==2) {
		if(key_macro[cur_macro][mac_loc]==0) {
			if(MACcnt>1) {
				MACcnt--;
				mac_loc=0;
				return 1;
			}
			mac_use=0;
			vResetKeyBuffer();
			return 0;
		}
		return 1;
	}

	if(k_buf_loc==0) {
		asm MOV AH, 0x11;
		asm INT 0x16;
		asm JZ NOKEY;
		asm MOV rtn, AX;
NOKEY:
		if(rtn==0) return 0;
	}

//	if(*kbs==*kbe) return 0;
	aflag=no_work=0;
	return 1;
}

void vResetKeyBuffer(void)
{
	k_buf_loc=0;
	while(wCheckKeyHit()) bioskey(0);
	return;
}

int iGetMacroNumber(void)
{
	INKEY ch;

	vDispExtraStatus(4);
	while(!wCheckKeyHit());
	ch.i=bioskey(0);
	vDispKeyInputStatus();
	if((ch.ch[1]<=ALT_0 && ch.ch[1]>=ALT_1)||	(ch.ch[0]<='9' && ch.ch[0]>='0')) {
		if(ch.ch[1]<=ALT_0 && ch.ch[1]>=ALT_1) return ch.ch[1]-ALT_1;
		else if(ch.ch[0]<='9' && ch.ch[0]>='1') return ch.ch[0]-'1';
		else return 9;
	}
	else return -1;
}

void vSetMacro(void)
{
	int i;

	if(mac_use!=0) {
		if(mac_use==2&&MACcnt>1) {
			MACcnt--;
			mac_loc=0;
			return;
		}
		mac_use=mac_loc=0;
		vResetKeyBuffer();
		return;
	}
	i=iGetMacroNumber();
	if(i!=-1) {
		cur_macro=i;
		setmem(key_macro[cur_macro], 101*2, 0);
		setmem(key_macro_state[cur_macro], 101, 0);
		mac_use=1;
		mac_loc=0;
	}
	else vBeep();
}

void vUseMacro(int num)
{
	if(mac_use==1) {
		if(num!=cur_macro) {
			vBeep();
			if(mac_loc>0) mac_loc--;
			return;
		}
		mac_use=0;
		mac_loc=0;
	}
	else if(mac_use==0) {
		cur_macro=num;
		mac_use=2;
		mac_loc=0;
	}
}

void vUseMacroRepeat(void)
{
	int i, cnt=1;

	i=iGetMacroNumber();
	if(i>=0) {
		MACcnt=wGetNumber(cnt, 1, 100, "¤å", "¡y ¤å ¯©Ð—ÐiŒa¶a?");
    vUseMacro(i);
		vDispKeyInputStatus();
	}
}

