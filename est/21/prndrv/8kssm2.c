#define MK_FP( seg,ofs )( (void _seg * )( seg ) +( void near * )( ofs ))
#define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
#define FP_OFF( fp )( (unsigned )( fp ))
typedef unsigned char byte;

int near PRN_port;
int near PRN_blen;
int near PRN_bwid;
int near PRN_left;
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

void huge linefeed(int n)
{
	int i;

	if(n==0) return;
	if(n < 0xff) {
		print(27); print(51); print(n); print(10);
	}
	else {
		for(i=0; i< n>>8; i++) {
			print(27); 	print(51);	print(0xff); print(10);
		}
		print(27);	print(51);	print(n & 0xff); print(10);
	}
  print(13);
}

void huge reset_prn(void)
{
	print(27); print(64);
}

void set_pmode(int n)
{
	print(27);
	print(42);
  print(3);
	print(n & 0xff);
	print(n >> 8);
}

void set_left(int x)
{
	int i;

  x=x*2/3;
  set_pmode(x);
  for(i=0; i<x; i++) print(0);
}

void huge set_page(void)
{
	print(27); print(81);	  print(136);   // right free space setting
	print(27); print(0x55); print(1);     // single direction setting (<-> 0)
}

void near twist_byte(byte *sr, byte *dest)
{
	int i,j;

	for(i=0; i<8; i++) dest[i]=0;
	for(i=0;i<8;i++) {
		for(j=0;j<8;j++) dest[i] |= ((sr[j] & (0x80>>i))<<i) >>j;
	}
}

void huge print_buf(int st, int x, int y)
{
	int i=0,j,k,l;
	byte far *b_addr;
	byte b[8], t[8];

	b_addr=PRN_buf;
	print(13);
	while(1) {
		for(j=0; j<3; j++) {
			set_left(PRN_left+st);
			set_pmode(8*y);
			for(k=0; k<y; k++) {
				for(l=0; l<8; l++) b[l]=*(b_addr+(unsigned)PRN_bwid*(i+l*3+j)+k);
				twist_byte(b, t);
				for(l=0; l<8; l++) print(t[l]);
				if(*PRN_errs) return;
			}
			i++;
			linefeed(1);
			if(i>=x) return;
		}
		i+=21;
		if(i<x) linefeed(21);
		else {
			i-=21;
			linefeed(x-i);
			break;
		}
	}
}

void huge formfeed()
{
	print(12);
}

void huge send_opt(int far *buf)
{
	int i;

	for(i=0; i<5; i++) buf[i]=0;
  buf[0]=10;
  buf[2]=24; buf[3]=32;
  buf[4]=180;
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