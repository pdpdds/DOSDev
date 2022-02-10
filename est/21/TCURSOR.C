#define TCSPEED 9
void interrupt (*old_int8)(void);
unsigned long int Scount=0;
WORD no_work=0;
BYTE saving=OFF;
int scr_saver_num=2;
WORD no_work_time=1;

BYTE cursor_hsize=1;
BYTE cursor_vsize=1;
BYTE tc_clip;
BYTE tc_h_e;

static void near vXORCursor(int x, int y, BYTE cur_h, BYTE h, BYTE v, BYTE s, BYTE h_e)
{
	char *offset;
	register int i;
	BYTE t;

	vMCOFF();
	if(line_state) {
		for(i=0; i<cur_h; i++ ) {
			if(x>0) {
				offset=vid_addr+offsetx[x++]+y;
				*offset ^= 0xff;
			}
			else x++;
		}
	}
	else {
		x+=cur_h-(h<<4);
		if(s==ON) {
			if(h_e==1 || h_e==2) t=0xf0;
			else t=0xc0;
			if(x<bminx && tc_clip) {
				i=bminx-x; x=bminx;
			}
			else i=0;
			for(; i<16*h; i++ ) {
				if(x>0) {
					offset=vid_addr+offsetx[x++]+y;
					*offset ^= t;
				}
				else x++;
			}
		}
		else {
			for(i=0;i<v*2;i++) {
				offset=vid_addr+offsetx[x+h*16-3]+y+i;
				*offset ^=0xff;
				offset=vid_addr+offsetx[x+h*16-2]+y+i;
				*offset ^=0xff;
				offset=vid_addr+offsetx[x+h*16-1]+y+i;
				*offset ^=0xff;
			}
		}
	}
	vMCON();
}
unsigned long tc_count=0;

void vTCON(void)
{
	tc_count=0;
	cursor_flag=ON;
}

void vTCOFF(void)
{
	if(on_off==ON) {
		vXORCursor(iCX, iCY, cur_hsize, cursor_hsize,
						cursor_vsize, start, tc_h_e);
		on_off=OFF;
	}
	cursor_flag=OFF;
}

void vTextCursorOperation(BYTE clip, BYTE h_e)
{
	if(cursor_flag) {
		if(Scount>tc_count+TCSPEED) {
			tc_count=Scount;
			on_off^=0x01;
			tc_clip=clip;
			tc_h_e=h_e;
			vXORCursor(iCX, iCY, cur_hsize, cursor_hsize,
							cursor_vsize, start, h_e);
		}
	}
}

void vDeinitINT8(void)
{
	setvect(8, old_int8);
}

extern BYTE clipping;
extern BYTE sb4[];
extern BYTE sb5[];
extern BYTE null_image[];

BYTE View_time=0;
BYTE T_time=0, T_flag=0;

void vWatch(int flag, int h, int m)
{
	BYTE *p, t, t2;
	int x, y;

	if(tw[cur_w].active==0) return;
	x=tw[cur_w].x+4;
	y=tw[cur_w].y+tw[cur_w].ly-2-7;
	p=gsf[0]+16*16;

	t=h/12; if(h>12) h-=12; t2=mouse_on_off;
	if(t2==1)	vMCOFF();
	if(flag != 2) {
		if(h>9) {
			vPutFreeImageByte(x, y, 16, 8, p+16, 1);
		}
		else vPutFreeImageByte(x, y, 16, 8, null_image, 1);
		vPutFreeImageByte(x, y+1, 16, 8, p+16*(h%10), 1);
		if(flag==1) {
			vPutFreeImageByte(x, y+2, 16, 8, gsf[0]+16*26, 1);
		}
		else	vPutFreeImageByte(x, y+2, 16, 8, null_image, 1);
		vPutFreeImageByte(x, y+3, 16, 8, p+16*(m/10), 1);
		vPutFreeImageByte(x, y+4, 16, 8, p+16*(m%10), 1);
		if(t) {
			vPutFreeImageByte(x, y+5, 16, 8, p+16*64, 1);
		}
		else	vPutFreeImageByte(x, y+5, 16, 8, p+16*49, 1);
	}
	else vNBarBit(x, y*8, x+15, (y+6)*8, 1);
	if(t2==1) vMCON();
}

void vDispTime(int t)
{
	struct time cur;
	static unsigned long old_sc=0;
	static BYTE flag=1;

	if(t) {
		vWatch(2, 0, 0);
	}
	if(View_time==0) return;
	if(Scount>old_sc+9) {
		disable();
		old_sc=Scount;
		gettime(&cur);
		vWatch(flag, cur.ti_hour, cur.ti_min);
		flag^=1;
		enable();
	}
}

void interrupt vNewINT8(void)
{
	(*old_int8)();
	Scount++;
	if(no_work>(unsigned long)no_work_time*30*37) {
		no_work=1;
		if(scr_saver_num) vScreenSaver(scr_saver_num);
	}
	if(!saving) {
		no_work++;
	}
}

void vInitINT8(void)
{
	old_int8=getvect(8);
	setvect(8, vNewINT8);
}

void vCheckSpeed(void)
{
	unsigned long t;
	unsigned long s=0;

	t=Scount;
	while(t==Scount);
	t++;
	while(t==Scount) s++;
	SYSPEED=(WORD)(s/130);
}

void vDelay(WORD tick)
{
	unsigned long t;
	t=Scount+tick;
	while(t>Scount);
}
