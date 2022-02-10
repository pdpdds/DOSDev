#include <dos.h>

#define TCSPEED 8
void interrupt (*old_int8)(void);
unsigned long int Scount=0;

BYTE cursor_hsize=1;
BYTE cursor_vsize=1;
#define INDEXREGS 0x3CE
#define VALREGS 	0x3CF
#define OUTPUT(index, value) { outportb(INDEXREGS, index);\
															 outportb(VALREGS, value); }

void vXORCursor(int x, int y)
{
	char far *offset;
	register int i;

	x+=cur_hsize-(cursor_hsize<<4);
	for(i=0; i<16*cursor_hsize; i++ ) {
		offset=vid_addr+offsetx[x++]+y;
		*offset ^= 0xC0;
	}
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
		vXORCursor(iCX, iCY);
		on_off=OFF;
	}
	cursor_flag=OFF;
}

void vTextCursorOperation(void)
{
	if(cursor_flag) {
		if(Scount>tc_count+TCSPEED) {
			tc_count=Scount;
			on_off^=0x01;
			vXORCursor(iCX, iCY);
		}
	}
}

void interrupt tick(void)
{
	(*old_int8)();
	Scount++;
}

void init_tick(void)
{
	old_int8=getvect(8);
	setvect(8, tick);
}

void deinit_tick(void)
{
	setvect(8, old_int8);
}
