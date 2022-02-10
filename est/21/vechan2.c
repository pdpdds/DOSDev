#define MAX_POLY_POINTS 2000
#define main_arr_SIZE 55*37
#define MAXHEIGHT 96
#define MAXCLOSS	15
#define absol(a) {if(a&0x8000) a=(a^0xffff)+1;}

#define BUFPOINT(x, y) {if(x>=0 && x<xlength)\
												*(main_arr+x*bufwid+(y>>3)) |= bit_tbl[y&7];}

#define WRITEDOT(x, y) {if(x>=0 && x<xlength) {g_map[x][g_closs[x]++]=y;}}


#define move_to(x, y) {off_x=x;off_y=y;}
#define line_to(x, y) {lisx=x;lisy=y;vLineInImageBuf();}


int arrayPtr;

static int off_y=0, off_x =0;
static int prev_dx, prev_prev_dx,
					 firstY, firstX,
					 secondY, secondX,
					 thirdY, thirdX,
					 polyY, polyX;

//static BYTE near g_pointArray[MAX_POLY_POINTS*2];
static int near g_map[MAXHEIGHT][MAXCLOSS];
static BYTE near g_closs[MAXHEIGHT];

BYTE near main_arr[main_arr_SIZE];
static BYTE near tarr[main_arr_SIZE];

BYTE near SB[8] = {0xff, 0x7f, 0x3f, 0x1f, 0xf, 7, 3, 1};
BYTE near EB[8] = {0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};
BYTE near bit_tbl[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

static void near vDrawLineInPolyArray(int, int);

void vClipPutImage(int x, int y, int height, int width,
										 BYTE flag, BYTE * buf)
{
	WORD offsetd, vseg, bseg, boff;
	int t, len;

	vseg=FP_SEG(vid_addr);
	bseg=FP_SEG(buf);
	boff=FP_OFF(buf);
	len=width;

	height+=x;
	if(flag) {
		len--;
		if(x<bminx+1) {
			boff+=(bminx+1-x)*width;
			x=bminx+1;
		}
		else if(height>bmaxx) height=bmaxx;
		t=(bminy>>3)+1;
		if(y<t) {
			len-=t-y;
			boff+=t-y;
			y=t;
		}
		else if(y+len>bmaxy>>3) len=(bmaxy>>3)-y;
	}

	for(;x<height;x++) {
		offsetd=offsetx[x]+y;
		asm PUSH DS;
		asm PUSH SI;
		asm MOV  AX, bseg;
		asm MOV  DS, AX;
		asm MOV  SI, boff;
		asm MOV  AX, vseg;
		asm MOV  ES, AX;
		asm MOV  DI, offsetd;
		asm MOV  CX, len;
		asm CLD;
		asm REP  MOVSB;
		asm POP  SI;
		asm POP  DS;
		boff+=width;
	}
}

void vPutImage(int x, int y, int ex, int ey, unsigned char *buf)
{
	vClipPutImage(x, y, ex-x+1, ey-y+1, 0, buf);
}

void vGetImage(int x, int y, int ex, int ey, unsigned char *buf)
{
	unsigned offsetd, vseg, bseg, boff;
	unsigned len;

	vseg=FP_SEG(vid_addr);
	bseg=FP_SEG(buf);
	boff=FP_OFF(buf);
	len=ey-y+1;

	for(;x<ex+1;x++) {
		offsetd=offsetx[x]+y;
		asm PUSH DS;
		asm PUSH SI;
		asm MOV  AX, vseg;
		asm MOV  DS, AX;
		asm MOV  SI, offsetd;
		asm MOV  AX, bseg;
		asm MOV  ES, AX;
		asm MOV  DI, boff;
		asm MOV  CX, len;
		asm CLD;
		asm REP MOVSB;
		asm POP  SI;
		asm POP  DS;
		boff+=len;
	}
}

#pragma warn -rvl

static near WORD wShiftRightWord(WORD i)
{
	asm MOV ax, i;
	asm ROR ax, 8;
	asm SHR ax, 1;
	asm ROR ax, 8;
}

static near WORD wShiftLeftWord(WORD i)
{
	asm MOV ax, i;
	asm ROR ax, 8;
	asm SHL ax, 1;
	asm ROR ax, 8;
}

#pragma warn +rvl

void vEffectWordImage(WORD *image_16x16, WORD *timage, BYTE mode)
{
	register int i;
	WORD image[16];


	for(i=0 ; i<16; i++) timage[i]=image[i]=image_16x16[i];
	if(mode & BIT_8)  
		for(i=0 ; i<16 ; i++) {
			timage[i] |= wShiftRightWord(image[i]);
			image[i]=timage[i];
		}
  if(mode & BIT_5) 
		for(i=0 ; i<16 ;) {
			timage[i] = timage[i++] | 0xaaaa;
			timage[i] = timage[i++] | 0x5555;
    }
  else if(mode & BIT_2)
    for(i=0 ; i<16 ;) {
			timage[i] = timage[i++] & 0x5555;
			timage[i] = timage[i++] & 0xaaaa;
		}
	if(mode & BIT_3) { 
		for(i=1 ; i<15 ; i++) {
			timage[i]|=wShiftRightWord(image[i]);
			timage[i]|=wShiftLeftWord(image[i]);
			timage[i-1]|=(image[i]);
			timage[i+1]|=(image[i]);
		}
		for(i=0 ; i<16 ; i++) timage[i] ^= image[i];
	}
	else if (mode & BIT_4) { 
		for(i=1 ; i<15 ; i++) {
			timage[i+1] |= image[i];
			timage[i+1] |= wShiftRightWord(image[i]);
			timage[i] ^= image[i];
		}
	}
	if(mode & BIT_7) timage[15] = 0xffff;
	if(mode & BIT_6)  
		for(i=0 ; i<16 ; i++) timage[i] = timage[i] ^ 0xffff;
}

void vEffectByteImage(BYTE *image_16x8, BYTE *timage, BYTE mode)
{
	register int i;
	BYTE image[16];

  for(i=0 ; i<16; i++) timage[i]=image[i]=image_16x8[i];

  if(mode & BIT_8)  
    for(i=0 ; i<16 ; i++) image[i] = timage[i] |= (image[i]>>1);
  if(mode & BIT_5) 
		for(i=0 ; i<16 ;) {
			timage[i] = timage[i++] | 0xaa;
			timage[i] = timage[i++] | 0x55;
    }
	else if(mode & BIT_2) 
    for(i=0 ; i<16 ;) {
			timage[i] = timage[i++] & 0x55;
			timage[i] = timage[i++] & 0xaa;
		}
  if(mode & BIT_3) { 
		for(i=1 ; i<15 ; i++) {
			timage[i]|=(image[i]>>1);
			timage[i]|=(image[i]<<1);
			timage[i-1]|=(image[i]);
      timage[i+1]|=(image[i]);
    }
    for(i=1 ; i<15 ; i++) timage[i] ^= image[i];
  }
  else if (mode & BIT_4) { 
    for(i=1 ; i<15 ; i++) {
			timage[i+1] |= image[i];
			timage[i+1] |= (image[i]>>1);
			timage[i] ^= image[i];
		}
	}
	if(mode & BIT_7) timage[15] = 0xff;
	if(mode & BIT_6)  
		for(i=0 ; i<16 ; i++) timage[i] = timage[i] ^ 0xff;
}

static void near vPutImage16x16(int x, int y, WORD *image_16x16, BYTE mode)
{
	register int i;
	WORD *offset;
	BYTE *offsetb;
	BYTE *v;
	BYTE s=0, e=16;
	WORD timage[16];


	if(bminx>x) s=bminx-x+1;
	if(bmaxx<x+16) e=bmaxx-x;

	v=vid_addr;
	if(!mode) {
		if(bminy>>3>y-1) {
			for(i=s ; i<e ; i++) {
				offsetb=(BYTE *)(v+offsetx[x+i]+y+1);
				*offsetb = (BYTE)(image_16x16[i]>>8);
			}
		}
		else if(bmaxy>>3<y+2) {
			for(i=s ; i<e ; i++) {
				offsetb=(BYTE *)(v+offsetx[x+i]+y);
				*offsetb = (BYTE)image_16x16[i];
			}
		}
		else {
			for(i=s ; i<e ; i++) {
				offset=(WORD *)(v+offsetx[x+i]+y);
				*offset = image_16x16[i];
			}
		}
		return;
	}
	vEffectWordImage(image_16x16, timage, mode);
	if(bminy>>3>y-1) {
		for(i=s ; i<e ; i++) {
			offsetb=(BYTE *)(v+offsetx[x+i]+y+1);
			*offsetb = (BYTE)(timage[i]>>8);
		}
	}
	else if(bmaxy>>3<y+2) {
		for(i=s ; i<e ; i++) {
			offsetb=(BYTE *)(v+offsetx[x+i]+y);
			*offsetb = (BYTE)timage[i];
		}
	}
	else {
		for(i=s ; i<e ; i++) {
			offset=(WORD *)(v+offsetx[x+i]+y);
			*offset = timage[i];
		}
	}
}

static void near vPutImage16x8(int x, int y, BYTE *image_16x8, BYTE mode)
{
	register int i;
	BYTE *offset;
	char s=0, e=16;
	BYTE timage[16];
	BYTE *v;

	if(bminx>x) s=bminx-x+1;
	if(bmaxx<x+16) e=bmaxx-x;

	v=vid_addr;
	if(mode==0) {
		for(i=s ; i<e ; i++) {
			offset=v+offsetx[x+i]+y;
			*offset = image_16x8[i];
		}
		return;
	}
	vEffectByteImage(image_16x8, timage, mode);
	for(i=s ; i<e ; i++) {
		offset=v+offsetx[x+i]+y;
		*offset = timage[i];
	}
}

static void near vHLineInImageBuf(int x, int y_start, int y_end)
{
	int i;
	BYTE sByte, eByte;

	if( y_start > y_end) {
		i = y_start;
		y_start = y_end;
		y_end = i;
	}
	sByte = y_start>>3;
	eByte = y_end>>3;

	if(sByte == eByte) {
		main_arr[bufwid*x + sByte] |= (SB[y_start & 7] & EB[ y_end & 7]);
	}
	else {
		main_arr[bufwid*x + sByte ] |= (SB[y_start & 7]);
		main_arr[bufwid*x + eByte ] |= (EB[y_end & 7]);
		for(i= sByte+1 ; i<eByte ; ++i) {
			main_arr[bufwid*x + i]=0xff;
		}
	}
}


static void near vQSort(unsigned loff, unsigned roff)
{
	unsigned x;

	asm MOV SI, loff;
	asm MOV DI, roff;
LOOP:
	asm MOV BX, roff;
	asm SUB BX, loff;
	asm SHR BX, 1;
	asm AND BX, 0xFFFE;
	asm ADD BX, loff; // x in AX
	asm MOV AX, [BX];
	asm MOV x, AX;
LOOP1:
LOOP1_1:
	asm CMP [SI], AX;  // [BX] means *p,  BX means p
	asm JAE ENDLOOP1_1;
	asm INC SI;
	asm INC SI;
	asm JMP  LOOP1_1;  //if(*p < x)
ENDLOOP1_1:

LOOP1_2:
	asm CMP [DI], AX;
	asm JBE ENDLOOP1_2;
	asm DEC DI;
	asm DEC DI;
	asm JMP  LOOP1_2;  //if(*q > x)
ENDLOOP1_2:

	asm CMP SI, DI;
	asm JA OUT1;
	asm MOV CX, [SI];
	asm MOV DX, [DI]
	asm MOV [SI], DX;
	asm MOV [DI], CX;
	asm INC SI;
	asm INC SI;
	asm DEC DI;
	asm DEC DI;
	asm CMP SI, DI;  //if(p<=q)
	asm JBE LOOP1;
OUT1:
	asm CMP loff, DI;
	asm JAE SKIP;
	vQSort(loff, _DI);
	asm MOV AX, x;
SKIP:
	asm MOV loff, SI;
	asm MOV DI, roff;
	asm MOV CX, loff;
	asm CMP CX, roff;
	asm JB  LOOP;
}

void vFillPoly(void)
{
	int i, j;
//	(void) qsort(g_pointArray, arrayPtr>>1, 2, komp);
//	eqsort(g_pointArray, g_pointArray+arrayPtr-2);
	for(i=0;i<MAXHEIGHT;i++) {
		if(g_closs[i]) {
			vQSort(FP_OFF(g_map[i]), FP_OFF(g_map[i]+g_closs[i]-1));
			for(j=0;j<g_closs[i];j+=2) {
				vHLineInImageBuf(i, g_map[i][j], g_map[i][j+1]);
			}
			g_closs[i]=0;
		}
	}
}



void vOpenPoly(void)
{
	prev_prev_dx = prev_dx= INT_MAX;
	arrayPtr = 0;
}

void vClosePoly(BYTE flag)
{
	if(prev_dx !=INT_MAX)	{
		if(polyY != firstY || polyX != firstX) {
			vDrawLineInPolyArray(polyX, polyY);
			vDrawLineInPolyArray(firstX, firstY);
		}
		vDrawLineInPolyArray(firstX, firstY);
		vDrawLineInPolyArray(secondX, secondY);
		vDrawLineInPolyArray(thirdX, thirdY);
		if(flag) vFillPoly();
	}
}


static void near vLineInPolyArray(int x2, int y2)
{
	int i, y, x, endX, startX;
	long int y2my1;
	int x2mx1, hx2mx1, x1=off_x, y1=off_y;

	if(x2 ==x1) return;
	if(x2 < x1) {
		i = y1;
		y1 = y2;
		y2 =i;
		i = x1;
		x1 = x2;
		x2 =i;
	}
	startX= x1+1;
	endX =  x2-1;

	x2mx1 = x2 -x1;
	hx2mx1 = (y2 < y1 ) ?  -x2mx1 /2 : x2mx1/2;
	y2my1 =  y2 -y1;

	for(x=startX; x<=endX; ++x) {
		y = (int)(((x-x1)*(y2my1)+hx2mx1) / x2mx1 +y1);
		WRITEDOT(x, y);
	}
}


int near lisx;
int near lisy;

static void near vLineInImageBuf(void)
{
	register int t,distance;
	int xerr=0,yerr=0,delta_x=0,delta_y=0;
	int incx, incy;

	delta_x= off_x - lisx;
	delta_y= off_y - lisy;

	if(delta_x>0) incx = 1;
	else if( delta_x == 0) incx =0;
	else incx = -1;

	if(delta_y>0) incy = 1;
	else if( delta_y == 0) incy =0;
	else incy = -1;

	absol(delta_x);
	absol(delta_y);

	if( delta_x > delta_y) distance = delta_x;
	else distance = delta_y;

	for( t=0; t<distance+1; t++) {
		BUFPOINT(lisx,lisy);
		xerr += delta_x;
		yerr += delta_y;
		if( xerr<<1 > distance) {
			xerr -=distance;
			lisx +=incx;
		}
		if( yerr<<1 > distance) {
			yerr -=distance;
			lisy +=incy;
		}
	}
}



static void near vDrawLineInPolyArray(int x2, int y2)
{
	int dx;

	dx=( x2 > off_x  ? -1 : (x2 < off_x   ? 1 : 0));
	if(!dx && x2>0 && x2<xlength) vHLineInImageBuf(x2, off_y, y2);
	polyY = y2;
	polyX = x2;
	if(prev_dx == INT_MAX) {
		firstY = off_y;
		firstX = off_x ;
		secondY = y2;
		secondX = x2;
	}
	else if(prev_prev_dx == INT_MAX) {
		if(prev_dx ==0 && dx== 0) {
			secondY = y2;
			secondX = x2;
			goto lineret;
		}
		else {
			thirdY = y2;
			thirdX = x2;
		}
	}
	else  if(dx !=0) {
		vLineInPolyArray(x2, y2);
		if(!(prev_dx == 0 && dx == prev_prev_dx)) WRITEDOT(off_x, off_y);
		if(dx+prev_dx == 0) WRITEDOT(off_x, off_y);
	}
	else {
		if(prev_dx !=0) {
			WRITEDOT(off_x, off_y);
		}
		else goto lineret;
	}
	prev_prev_dx=prev_dx;
	prev_dx=dx;
lineret :
	off_y =y2;
	off_x =x2;
	return;
}

static void near vRasterize(BYTE *vf, WORD count)
{
	WORD i;
	int tx, ty, mx, my;

	if(full_vector) vOpenPoly();
	move_to((vf[0] & 0x7f)*xunit*xs/96-sx, (vf[1] & 0x7f)*yunit*ys/96);
	for(i=2;i<count;) {
		tx=vf[i++];
		ty=vf[i++];
		mx=(tx & 0x7f)*xunit*xs/96-sx;
		my=(ty & 0x7f)*yunit*ys/96;
		if(tx<128) {
			if(ty<128) {
				line_to(mx, my);
				if(full_vector) vDrawLineInPolyArray(mx, my);
				else move_to(mx, my);
			}
			else { 
				line_to(mx, my);
				if(full_vector) vDrawLineInPolyArray(mx, my);
				else move_to(mx, my);
			}
		}
		else if(ty<128) {
			if(full_vector) vDrawLineInPolyArray(mx, my);
			else move_to(mx, my);
		}
		else {
			if(full_vector) {
				vClosePoly(1);
				vOpenPoly();
			}
			move_to(mx, my);
		}
	}
	if(full_vector) vClosePoly(1);
}

static void near vDrawCurveInPolyArray(int deep, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
	int mx0, my0, mx1, my1, mx2, my2, mx3, my3;
	int mx, my, mmx, mmy;

	if(deep==0) {
		if(full_vector)	{
			vDrawLineInPolyArray(x1/10, y1/10);
			vDrawLineInPolyArray(x2/10, y2/10);
			vDrawLineInPolyArray(x3/10, y3/10);
		}
		else {
			x1/=10;	y1/=10;
			x2/=10;	y2/=10;
			x3/=10;	y3/=10;
			line_to(x1, y1);
			move_to(x1, y1);
			line_to(x2, y2);
			move_to(x2, y2);
			line_to(x3, y3);
			move_to(x3, y3);
		}
		return;
	}
	mx=(x1+x2)>>1;
	my=(y1+y2)>>1;

	mx0=(x0+x1)>>1;
	my0=(y0+y1)>>1;
	mx3=(x3+x2)>>1;
	my3=(y3+y2)>>1;

	mx1=(mx0+mx)>>1;
	my1=(my0+my)>>1;
	mx2=(mx3+mx)>>1;
	my2=(my3+my)>>1;

	mmx=(mx1+mx2)>>1;
	mmy=(my1+my2)>>1;

	vDrawCurveInPolyArray(deep-1, x0, y0, mx0, my0, mx1, my1, mmx, mmy);
	vDrawCurveInPolyArray(deep-1, mmx, mmy, mx2, my2, mx3, my3, x3, y3);
}

typedef unsigned long UL;
static void near vNewRasterizeLarge(BYTE *ef, int count)
{
	int i;
	int tx, ty, t;
	int szx, szy;

	if(count==0) return;
	szx=xunit*xs;
	szy=yunit*ys;
	vOpenPoly();
	off_x=(int)((UL)ef[0]*szx/240)-sx;
	off_y=(int)((UL)ef[1]*szy/240);
	for(i=2;i<count;) {
		if(ef[i]>239) {
			t=ef[i++];
			tx=ef[i++];
			ty=ef[i++];
			tx=(int)((UL)tx*szx/240)-sx;
			ty=(int)((UL)ty*szy/240);
			switch(t) {
				case 255: //New start;
					vClosePoly(1);
					vOpenPoly();
					off_x=tx; off_y=ty;
					break;
				case 254: //Set Semi start;
					vClosePoly(0);
					prev_prev_dx = prev_dx= INT_MAX;
					off_x=tx; off_y=ty;
					break;
				case 253: //Set Curve;
					vDrawCurveInPolyArray(3,
										 (int)((UL)ef[i-5]*szx/24)-sx*10,
										 (int)((UL)ef[i-4]*szy/24),
										 tx*10, ty*10,
										 (int)((UL)ef[i]*szx/24)-sx*10,
										 (int)((UL)ef[i+1]*szy/24),
										 (int)((UL)ef[i+2]*szx/24)-sx*10,
										 (int)((UL)ef[i+3]*szy/24));
					i+=4;
					break;
			}
			continue;
		}
		tx=ef[i++];
		ty=ef[i++];
		tx=(int)((UL)tx*szx/240)-sx;
		ty=(int)((UL)ty*szy/240);
		vDrawLineInPolyArray(tx, ty);
	}
	vClosePoly(1);
}

static void near vNewRasterize(BYTE *ef, int count)
{
	int i;
	int tx, ty, t;
	int szx, szy;

	if(count==0) return;
	szy=yunit*ys;
	szx=xunit*xs;
	if(szy>250 || szx>250) {
		vNewRasterizeLarge(ef, count);
		return;
	}
	if(full_vector) vOpenPoly();
	off_x=(int)((WORD)ef[0]*szx/240)-sx;
	off_y=(int)((WORD)ef[1]*szy/240);
	for(i=2;i<count;) {
		if(ef[i]>239) {
			t=ef[i++];
			tx=ef[i++];
			ty=ef[i++];
			tx=(int)((WORD)tx*szx/240)-sx;
			ty=(int)((WORD)ty*szy/240);
			switch(t) {
				case 255: //New start;
					if(full_vector) {
						vClosePoly(1);
						vOpenPoly();
					}
					off_x=tx; off_y=ty;
					break;
				case 254: //Set Semi start;
					if(full_vector) {
						vClosePoly(0);
						prev_prev_dx = prev_dx= INT_MAX;
					}
					off_x=tx; off_y=ty;
					break;
				case 253: //Set Curve;
					vDrawCurveInPolyArray(3,
										 (int)((WORD)ef[i-5]*szx/24)-sx*10,
										 (int)((WORD)ef[i-4]*szy/24),
										 tx*10, ty*10,
										 (int)((WORD)ef[i]*szx/24)-sx*10,
										 (int)((WORD)ef[i+1]*szy/24),
										 (int)((WORD)ef[i+2]*szx/24)-sx*10,
										 (int)((WORD)ef[i+3]*szy/24));
					i+=4;
					break;
			}
			continue;
		}
		tx=ef[i++];
		ty=ef[i++];
		tx=(int)((WORD)tx*szx/240)-sx;
		ty=(int)((WORD)ty*szy/240);
		if(full_vector)	vDrawLineInPolyArray(tx, ty);
		else {
			line_to(tx, ty);
			move_to(tx, ty);
		}
	}
	if(full_vector) vClosePoly(1);
}

static void near vShiftRightBuf(WORD soff, WORD doff, BYTE count, WORD len)
{
	asm PUSH BX;
	asm MOV  DI, doff;
	asm MOV  BX, soff;
	asm MOV  AX, len;
	asm DEC  AX;
	asm MOV  CL, count;
	asm MOV  DH, [BX];
	asm INC  BX;
	asm MOV  DL, [BX];
	asm SHR  DX, CL;

	asm OR  [DI], DH;
	asm INC  DI;

LOOP2:
	asm XOR  CL, 7;
	asm INC  CL;
	asm SHR  DX, CL;
	asm DEC  CL;
	asm XOR  CL, 7;
	asm MOV  DH, [BX];
	asm ROR  DX, CL;
	asm OR  [DI], DH;
	asm INC  BX;
	asm INC  DI;

	asm DEC  AX;
	asm CMP  AX, 0;
	asm JNE  LOOP2;
	asm POP  BX;
}

unsigned fseg, foff, toff;

static void near vCopyBuffer(WORD soff, WORD doff, WORD count)
{
	asm PUSH DS;
	asm PUSH SI;
	asm MOV AX, fseg;
	asm MOV ES, AX; 
	asm MOV DS, AX; 
	asm MOV DI, doff;
	asm MOV SI, soff;
	asm MOV CX, count; 
	asm CLD;
	asm REP MOVSB;
	asm POP SI;
	asm POP DS;
}


static void near vGrayBuffer(int bufsize)
{
	register int i, j;

	if(sx!=0) i=bufwid;
	else i=0;
	while(i<bufsize) {
		for(j=0;j<bufwid;j++) tarr[i++] |= 0x88;
		i+=bufwid;
		for(j=0;j<bufwid;j++) tarr[i++] |= 0x22;
		i+=bufwid;
	}
}

static void near vPutRastorImage(int x, int y, BYTE eff)
{
	WORD j;
//	BYTE t;
	WORD i;
	WORD bufsize;

	fseg=FP_SEG(main_arr);
	foff=FP_OFF(main_arr);
	if(bufhigh<xlength) bufsize=bufwid*bufhigh;
	else bufsize=bufwid*xlength;
	eff&=0x7f;
	if(hfc_found==0 && (hfcnum & 0xf0)) {
		if(hfc) {
			while(hfc_loc+bufsize+10>HGEFTCACHESIZE) {
				j=*((int *)(hfc+3))+5;
				movmem(hfc+j, hfc, HGEFTCACHESIZE-2-j);
				hfc_loc-=j;
				hfc_max--;
			}
			hfc[hfc_loc]=hfcnum;
			hfc[hfc_loc+1]=hfcode & 0x00ff;
			hfc[hfc_loc+2]=hfcode >> 8;
			hfc[hfc_loc+3]=bufsize & 0x00ff;
			hfc[hfc_loc+4]=bufsize >> 8;
			memcpy(hfc+hfc_loc+5, main_arr, bufsize);
			hfc_loc+=bufsize+5;
			hfc_max++;
		}
	}
	if(eff==0) {
		if(!printing) vClipPutImage(x, y, bufhigh, bufwid, 1, main_arr);
		else memcpy(tarr, main_arr, bufsize);
	}
	else {
		toff=FP_OFF(tarr);
		vCopyBuffer(foff, toff, bufsize);
		if(eff & BIT_8) { 
			vShiftRightBuf(foff, toff, 1, bufsize);
			vCopyBuffer(toff, foff, bufsize);
		}
		if(eff & BIT_5) {	
			vGrayBuffer(bufsize);

		}
		else if(eff & BIT_2) { 
			if(sx!=0) i=bufwid;
			else i=0;
//			t=0x55;
			while(i<bufsize) {
				for(j=0;j<bufwid;j++) tarr[i++] &=0x88;
				for(j=0;j<bufwid;j++) tarr[i++] =0;
				for(j=0;j<bufwid;j++) tarr[i++] &=0x22;
				for(j=0;j<bufwid;j++) tarr[i++] =0;
			}
		}
		if(eff & BIT_3) { 
			setmem(tarr, bufsize, 0);
			vShiftRightBuf(foff, toff, 7, bufsize);
			vShiftRightBuf(foff, toff+1, 1, bufsize);
			vCopyBuffer(toff+1, toff, bufsize);
			for(i=bufwid;i<bufsize;i++) tarr[i-bufwid]|=main_arr[i];
			for(i=0;i<bufsize-bufwid;i++) tarr[i+bufwid]|=main_arr[i];
			if(eff & BIT_5) vGrayBuffer(bufsize);
			for(i=0;i<bufsize;i++) tarr[i]^=main_arr[i];
		}
		else if (eff & BIT_4) { 
			vShiftRightBuf(foff, toff, 1, bufsize);
			for(i=0;i<bufsize-bufwid;i++) tarr[i+bufwid]|=main_arr[i];
			for(i=0;i<bufsize;i++) tarr[i]^=main_arr[i];
		}
		if(eff & BIT_7) {
			if(printing) {
				if(x!=0) {
					for(i=(x-1)*bufwid;i<x*bufwid;i++) tarr[i]=0xff;
				}
			}
			else {
				for(i=bufsize-bufwid;i<bufsize;i++) tarr[i]=0xff;
			}
		}
		if(eff & BIT_6) {
			for(i=0; i<bufsize; i++) {
				tarr[i]=tarr[i] ^ 0xff;
			}
		}
		if(!printing) vClipPutImage(x, y, bufhigh, bufwid, 1, tarr);
	}
//	save_buf(main_arr, bufsize);
	asm MOV AX, fseg;
	asm MOV ES, AX; 
	asm MOV AX, foff;
	asm MOV DI, AX;
	asm XOR AX, AX;
	asm MOV CX, bufsize; 
	asm CLD;
	asm REP STOSB;
}

static BYTE near b2to1(BYTE src1, BYTE src2)
{
	BYTE key=0x80;
	BYTE rtn_byte=0;
	register int i;

	for(i=0;i<8;i+=2) {
		rtn_byte|=((src2 & (key>>i)) | (src2 & (key>>(i+1))))<<(i>>1);
	}
	rtn_byte>>=4;
	for(i=0;i<8;i+=2) {
		rtn_byte|=((src1 & (key>>i)) | (src1 & (key>>(i+1))))<<(i>>1);
	}
	return rtn_byte;
}

static void near v2to3(BYTE *src, BYTE *dest)
{
	dest[0]=dest[1]=dest[2]=0;

  dest[0]|=src[0] & 0xc0;       //2
  dest[0]|=(src[0] & 0x40)>>1;  //1
	dest[0]|=(src[0] & 0x30)>>1;  //2
  dest[0]|=(src[0] & 0x10)>>2;  //1
	dest[0]|=(src[0] & 0x0c)>>2;  //2

	dest[1]|=(src[0] & 0x07)<<5;  //3
  dest[1]|=(src[0] & 0x01)<<4;  //1
  dest[1]|=(src[1] & 0xc0)>>4;  //2
  dest[1]|=(src[1] & 0x40)>>5;  //1
  dest[1]|=(src[1] & 0x20)>>5;  //1

	dest[2]|=(src[1] & 0x10)<<3;  //1
	dest[2]|=(src[1] & 0x10)<<2;  //1
  dest[2]|=(src[1] & 0x0c)<<2;  //2
	dest[2]|=(src[1] & 0x04)<<1;  //1
  dest[2]|=(src[1] & 0x03)<<1;  //2
	dest[2]|=src[1] & 0x01;       //1
}

static void near v2to4(BYTE *src, BYTE *dest)
{
	BYTE key[16]={0, 0x03, 0x0c, 0x0f, 0x30, 0x33, 0x3c, 0x3f,
								0xc0, 0xc3, 0xcc, 0xcf, 0xf0, 0xf3, 0xfc, 0xff};

	dest[0]=key[src[0]>>4];
	dest[1]=key[src[0] & 0x0f];
  dest[2]=key[src[1]>>4];
  dest[3]=key[src[1] & 0x0f];
}

static void near v2to5(BYTE *src, BYTE *dest)
{
	BYTE key[4]={0, 0x07, 0x18, 0x1f};

	dest[0]=dest[1]=dest[2]=dest[3]=dest[4]=0;

	dest[0]|=key[src[0]>>6]; //5
	dest[0]|=key[(src[0] & 0x30)>>4]>>2; //3
	dest[1]|=key[(src[0] & 0x30)>>4]<<6; //2
	dest[1]|=key[(src[0] & 0x0c)>>2]<<1; //5
	dest[1]|=key[src[0] & 0x03]>>4; //1
	dest[2]|=key[src[0] & 0x03]<<4; //4
	dest[2]|=key[src[1]>>6]>>1; //4
	dest[3]|=key[src[1]>>6]<<7; //1
	dest[3]|=key[(src[1] & 0x30)>>4]<<2; //5
	dest[3]|=key[(src[1] & 0x0c)>>2]>>3; //2
	dest[4]|=key[(src[1] & 0x0c)>>2]<<5; //3
	dest[4]|=key[src[1] & 0x03]; //5
}

static void near vPutSmallImage16x16(int x, int y, WORD *image_16x16, BYTE mode)
{
	register int i;
  BYTE *offset;
	BYTE out;
  BYTE image[32];

  if(mode) vEffectWordImage(image_16x16, (WORD *)image, mode);
	else memcpy(image, (BYTE *)image_16x16, 32);
  if(y&1) {
		y>>=1;
		x>>=1;
		for(i=0;i<8;i++) {
			out=b2to1(image[i<<2] | image[(i<<2)+2],
               image[(i<<2)+1] | image[(i<<2)+3]);
			offset=vid_addr+offsetx[x+i]+y;
      *(offset++)&=~(out>>4);
      *offset&=~(out<<4);
    }
	}
	else {
		y>>=1;
		x>>=1;
		for(i=0;i<8;i++) {
      out=b2to1(image[i<<2] | image[(i<<2)+2],
               image[(i<<2)+1] | image[(i<<2)+3]);
			offset=vid_addr+offsetx[x+i]+y;
      *offset&=~out;
    }
	}
}

static void near vPutSmallImage16x8(int x, int y, BYTE *image, BYTE mode)
{
  register int i;
	BYTE *offset;
  BYTE b[16];

  if(mode) vEffectByteImage(image, b, mode);
	else memcpy(b, image, 16);
	if(y&1) {
		y>>=1;
		x>>=1;
    for(i=0;i<8;i++) {
			offset=vid_addr+offsetx[x+i]+y;
			*offset&=~(b2to1(b[i<<1] | b[(i<<1)+1], 0)>>4);
		}
	}
	else {
    y>>=1;
		x>>=1;
		for(i=0;i<8;i++) {
      offset=vid_addr+offsetx[x+i]+y;
			*offset&=~(b2to1(b[i<<1] | b[(i<<1)+1], 0));
    }
	}
}

static void near vPutSmallRastorImage(int x, int y, BYTE eff)
{
	int i, j;
	BYTE buf[4];
	BYTE timage[32];
	BYTE image[32];
	BYTE *offset;
	int xsis;

	for(i=0;i<16;i++) {
		image[i*2]=main_arr[i*bufwid];
		image[i*2+1]=main_arr[i*bufwid+1];
	}
	if(eff) {
		vEffectWordImage((WORD *)image, (WORD *)timage, eff);
		memcpy(image, timage, 32);
	}
	xsis=(xs*xunit)>>1;          //xsis
	if(ys!=2) setmem(main_arr, bufwid*16, 0);
	switch(ys) {
		case 1 :
			for(i=0;i<32;i+=2) main_arr[i/2*bufwid]=b2to1(image[i], image[i+1]);
			break;
		case 2 :
			for(i=0;i<32;i+=2) {
				main_arr[i/2*bufwid]=image[i];
				main_arr[i/2*bufwid+1]=image[i+1];
			}
			break;
		case 3 :
			for(i=0;i<16;i++) v2to3(image+i*2, main_arr+bufwid*i);
			break;
		case 4 :
			for(i=0;i<16;i++) v2to4(image+i*2, main_arr+bufwid*i);
			break;
		case 5 :
			for(i=0;i<16;i++) v2to5(image+i*2, main_arr+bufwid*i);
			break;
		case 6 :
			for(i=0;i<16;i++) {
				v2to4(image+i*2, buf);
				v2to3(buf, main_arr+bufwid*i);
				v2to3(buf+2, main_arr+bufwid*i+3);
			}
			break;
	}
	if(y&1) {
		i=bufwid*bufhigh;
		vCopyBuffer(FP_OFF(main_arr), FP_OFF(tarr), i);
		setmem(main_arr, i, 0);
		vShiftRightBuf(FP_OFF(tarr), FP_OFF(main_arr), 4, i);
	}
	x>>=1;
	y>>=1;
	if(xsis==8) {
		for(i=0; i<8; i++) {
			offset=vid_addr+offsetx[x+i]+y;
			for(j=0; j<ys+1;j++)
				*(offset++)&=~(main_arr[i*bufwid*2+j] | main_arr[i*bufwid*2+bufwid+j]);
		}
	}
	else {
		switch(xsis) {
			case 16: case 32: case 48:
				for(i=0; i<16; i++) {
          offset=vid_addr+offsetx[x+i]+y;
					for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					if(xsis>=32) {
						x++;
						offset=vid_addr+offsetx[x+i]+y;
						for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					}
					if(xsis==48) {
						x++;
						offset=vid_addr+offsetx[x+i]+y;
						for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					}
				}
				break;
			case 24 :
				for(i=0; i<16; i++) {
					offset=vid_addr+offsetx[x+i]+y;
					for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					if(i&1) {
						x++;
						offset=vid_addr+offsetx[x+i]+y;
						for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					}
				}
				break;
			case 40 :
				for(i=0; i<16; i++) {
					offset=vid_addr+offsetx[x+i]+y;
					for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					x++;
					offset=vid_addr+offsetx[x+i]+y;
					for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					if(i&1) {
						x++;
						offset=vid_addr+offsetx[x+i]+y;
						for(j=0; j<ys+1;j++) *(offset++)&=~main_arr[i*bufwid+j];
					}
				}
				break;
		}
	}
	setmem(main_arr, bufwid*16, 0);
}

void vPutSmallFont(int x, int y, BYTE n, BYTE flag)
{
  BYTE *offset;
	int i, j=5;

	if(y<(bminy>>3)+1 || y>(bmaxy>>3)-1) return;
	if(flag==0) {
    for(i=0; i<5; i++) {
			offset=vid_addr+offsetx[x+i]+y;
			*offset=small[n][i];
    }
	}
	else {
		if(n==0 || n==5) j=4;
		for(i=0; i<j; i++) {
			offset=vid_addr+offsetx[x+i]+y;
			if(n==3) *offset|=rod_icon[n][i];
			else *offset=rod_icon[n][i];
		}
	}
}

extern BYTE *our_buf;
extern int BWID;
extern int BLEN;

BYTE rest_tbl[8]={0xff, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};

void vCutImage(int xlen, int bwid, int wid)
{
	int i, j, rest;
	BYTE *p;

	if(xlen>xlength) xlen=xlength;
	rest=wid&7;
	wid=(wid+7)>>3;
	bwid=((bwid+7)>>3)+1;
	p=tarr;
	for(i=0; i<xlen; i++) {
		p[wid-1] &= rest_tbl[rest];
		for(j=wid;j<bwid;j++) p[j]=0;
		p+=bwid;
	}
}

void vPutImageToPrintBuffer(int x, int y, int h, int v)
{
	WORD doff, soff;
	int i, j, wid, remain;
	BYTE *buf;
	BYTE *p;

	if(x<0) return;

	wid=((v+7)>>3)+1;
	buf=our_buf+x*BWID+(y>>3);
	if(x+h>xlength) h=xlength-x;

	doff=FP_OFF(main_arr);
	soff=FP_OFF(tarr);
	remain=(y%8);
	if(remain) {
		vShiftRightBuf(soff, doff, (BYTE)remain, (WORD)wid*h);
		p=main_arr;
		if(sx!=0) p+=wid;
		for(i=0; i<h; i++) {
			for(j=0; j<wid; j++) {
				*(buf+j)|=*p++;
			}
			buf+=BWID;
		}
		setmem(main_arr, wid*h, 0x00);
	}
	else {
		p=tarr;
		if(sx!=0) p+=wid;
		for(i=0; i<h; i++) {
			for(j=0; j<wid; j++) {
				*(buf+j)|=*p++;
			}
			buf+=BWID;
		}
	}
}
