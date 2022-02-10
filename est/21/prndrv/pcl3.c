#define MK_FP( seg,ofs )( (void _seg * )( seg ) +( void near * )( ofs ))
#define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
#define FP_OFF( fp )( (unsigned )( fp ))
typedef unsigned char byte;

int Curx=0, Cury=0;
int near PRN_port;
int near PRN_blen;
int near PRN_bwid;
int near PRN_left;
int near PRN_papw;
char far *PRN_buf;
unsigned char *PRN_errs;
unsigned long int *PRN_tm;

int  far (*select)(byte far *str);
void far (*err)(byte far *str);

#pragma warn -cln
byte pstatus(void)
{
	byte cod;
	asm mov ah, 0x02;
	asm mov dx, PRN_port;
	asm int 0x17;
	asm mov cod, ah;
	return cod;
}

void print(byte dat)
{
	unsigned long tm;
	byte cod;

	tm=(*PRN_tm);
	if((*PRN_errs)) return;
	while(1) {
		cod=pstatus();
		if(cod & 0x20) {  // Paper OUT
			if(!(*select)("¹··¡¦¹¢.. ‰­¢?")) {
				tm=(*PRN_tm); continue;
			}
			else {
				(*PRN_errs)=2; return;
			}
		}
		if(cod & 0x08) {  // I/O error
			if(!(*select)("I/O µAœá.. ‰­¢?")) {
				tm=(*PRN_tm); continue;
			}
			else {
				(*PRN_errs)=3; return;
			}
		}
		if(cod & 0x01) {  // Time out
			if(!(*select)("¯¡ˆeÁ¡‰Á.. ‰­¢?")) {
				tm=(*PRN_tm); continue;
			}
			else {
				(*PRN_errs)=4; return;
			}
		}
		if(!(cod & 0x80)) {  // Printer NOT BUSY
			if((*PRN_tm)<tm+18*30) continue;
			else {
				if(!(*select)("ÏaŸ¥ÈáµAœá.. ‰­¢?")) {
					tm=(*PRN_tm); continue;
				}
				else {
					(*PRN_errs)=1; return;
				}
			}
		}
    if((cod & 0x80) /*&& (cod & 0x10)*/) {
			asm mov ah, 0;
			asm mov al, dat;
			asm mov dx, PRN_port;
			asm int 0x17;
			return;
		}
	}
}
#pragma warn +cln


void huge PRN_opt(int *data)
{
	int sg, of;

	PRN_port=data[0];
	PRN_blen=data[1];
	PRN_bwid=data[2];
	PRN_left=data[3];
	PRN_buf=MK_FP(data[4], data[5]);
	PRN_errs=MK_FP(data[6], data[7]);
	PRN_papw=data[8]*2;

	asm mov ax, 1;
	asm int 0x60;
	asm mov sg, cx;
	asm mov of, dx;
	err=MK_FP(sg, of);

	asm mov ax, 2;
	asm int 0x60;
	asm mov sg, cx;
	asm mov of, dx;
	select=MK_FP(sg, of);

	asm mov ax, 3;
	asm int 0x60;
	asm mov sg, cx;
	asm mov of, dx;
	PRN_tm=MK_FP(sg, of);
}

void printN(int n)
{
	byte str[5], i=0;

	str[0]=n/1000;  n%=1000;
	str[1]=n/100;   n%=100;
	str[2]=n/10;    n%=10;
	str[3]=n;
	str[4]=0;
	while(!str[i] && i<5) i++;
	for(; i<4; i++) {
		print(str[i]+'0');
	}
}

void goto_xy(int x, int y)  // twisted x, y
{
	print(27); print('*'); print('p');
	printN(y); print('x'); printN(x); print('Y');
}

void huge linefeed(int n)
{
	Curx+=(n*2);
}

void huge reset_prn(void)
{
  print(27); print('E');
}

void huge set_page(void)
{
}

void near set_pmode(void)
{
	print(27); print('*'); print('t');
	printN(300); print('R');

// image start code
	print(27);  print('*'); print('r');
	print('1'); print('A');
}

void near set_img_size(int size)
{
	print(27); 		print('*'); print('b');
	printN(size); print('W');
}

void huge print_buf(int st, int x, int y)
{
	int i, j, t;
	byte *b_addr;

  t=st+PRN_left*2;
  if(y*8+t>PRN_papw-150) y=(PRN_papw-150-t)/8;
  b_addr=PRN_buf;
  goto_xy(Curx, t);
	set_pmode();
	for(i=0; i<x; i++) {
		set_img_size(y);
		for(j=0; j<y; j++) print(b_addr[PRN_bwid*i+j]);
	}
	print(27); print('*');
	print('r'); print('B');
	Curx+=x;
}

void huge formfeed()
{
	print(12);
  reset_prn();
	Curx=Cury=0;
}

void huge send_opt(int far *buf)
{
	int i;

	for(i=0; i<5; i++) buf[i]=0;
	buf[0]=10;
	buf[2]=40;
	buf[3]=40;
  buf[4]=150;
}

void huge (*PRN_funcs[10])()=
{
	PRN_opt, linefeed, reset_prn, set_page,
	print_buf, formfeed, send_opt, 0, 0, 0
};

int get_faddr(char c)
{
	if(c==0) {
		return (int)FP_SEG(PRN_funcs);
	}
	else {
		return (int)FP_OFF(PRN_funcs);
	}
}
