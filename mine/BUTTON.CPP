#include <graphics.h>
#include <stdlib.H>
#include <dos.h>
#include <conio.h>

#include "button.h"

/*-----------------------------------------------------------------------------------*/
void button_up_s(int left,int top,int right,int bottom,int color,int yes_no)
{   unsigned size = imagesize(left+2,top+2,right-1,bottom-1);
	void *buff;

	if (yes_no == YES)
	{
		buff = malloc(size);
		getimage(left+2,top+2,right-1,bottom-1,buff);
	}

	line_x(left,right,top,15);
	line_y(top+1,bottom,left,15);

	line_x(left+1,right,bottom,8);
	line_y(top+1,bottom-1,right,8);

	if (yes_no == YES)
	  {	putimage(left+1,top+1,buff,COPY_PUT);
		free(buff);
		line_x(left+1,right-1,bottom-1,color);
		line_y(top+1,bottom-1,right-1,color);
	  } else
		box_fill(left+1,top+1,right-1,bottom-1,color);
}
/*-----------------------------------------------------------------------------------*/
void button_down_s(int left,int top,int right,int bottom,int color,int yes_no)
{   unsigned size = imagesize(left+1,top+1,right-2,bottom-2);
	void *buff;

	if (yes_no == YES)
	{	buff = malloc(size);
		getimage(left+1,top+1,right-2,bottom-2,buff);
	}

	line_x(left,right-1,top,8);
	line_y(top+1,bottom-1,left,8);

	line_x(left,right,bottom,15);
	line_y(top,bottom-1,right,15);

	if (yes_no == YES)
	 {	putimage(left+2,top+2,buff,COPY_PUT);
			free(buff);
			line_x(left+1,right-1,top+1,color);
			line_y(top+1,bottom-1,left+1,color);
	 } else
			box_fill(left+1,top+1,right-1,bottom-1,color);
}
/*-----------------------------------------------------------------------------------*/
void button_no_fill(int left,int top,int right,int bottom,int up_down)
{   if (up_down == UP)
	  {	line_x(left,right,top,15);
		line_y(top+1,bottom,left,15);

		line_x(left+1,right,bottom,8);
		line_y(top+1,bottom-1,right,8);
	  } else
	  {	line_x(left,right,top,8);
		line_y(top+1,bottom,left,8);

		line_x(left+1,right,bottom,15);
		line_y(top+1,bottom-1,right,15);
	  }
}
/*-----------------------------------------------------------------------------------*/
void panel_s(int left,int top,int right,int bottom,int color)
{	button_up_s(left,top,right,bottom,color,NO);
	button_no_fill(left+5,top+5,right-5,bottom-5,DOWN);
}
/*-----------------------------------------------------------------------------------*/
int line_x(int left,int right,int y,int color)
{   if (right-left<0 || left<0 || right >639 || y<0 || y>479) return(0);
	static unsigned char le[8] = { 0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01 },
						 ri[8] = { 0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff };
	unsigned char far *address;
	unsigned int offset_ = y*80+((unsigned)left>>3)+1, segment = 0xa000;
	int len = ((unsigned)right>>3)-((unsigned)left>>3)-1;

	address = (unsigned char far *)0xa0000000L + (long)y*80L +((unsigned long)left>>3);

	outportb(0x3ce,0x00);	outportb(0x3cf,color);
	outport(0x3ce,0x0f01);

	if (len == -1)
		{	outportb(0x3ce,0x08);	outportb(0x3cf,*(le+(left&7)) & *(ri+(right&7)));
			*address = *address;
		}
	  else
		{	outportb(0x3ce,0x08);	outportb(0x3cf,*(le+(left&7)));
			*address = *address++;

			outport(0x3ce,0xff08);

			#pragma inline
			{
				asm push 	di
				asm pushf

				asm cld
				asm mov 	es, segment
				asm mov 	di, offset_
				asm mov 	al, 0xff
				asm mov 	cx, len
				asm rep 	stosb

				asm popf
				asm pop 	di
			}
			address += len;

			outportb(0x3ce,0x08);	outportb(0x3cf,*(ri+(right&7)));
			*address = *address;
		}

	outport(0x3ce,0x0001);
	outport(0x3ce,0xff08);

	return(1);
}
/*-----------------------------------------------------------------------------------*/
int line_y(int top,int bottom,int x,int color)
{   if (bottom-top<0 || x<0 || x >639 || top<0 || bottom>479) return(0);
	static unsigned char le[8] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
	unsigned offset_ = top*80+((unsigned)x>>3), offset__ = offset_+(bottom-top+1)*80;

	outport(0x3ce,0x0205);
	outportb(0x3ce,0x08);	outportb(0x3cf,*(le+(x&7)));

	#pragma inline
	{	asm push ds

		asm mov ax, 0xa000
		asm mov ds, ax
		asm mov bx, offset_
		asm mov ah, color
		asm mov dx, offset__
	AAAA:
		asm mov al, [bx]
		asm mov [bx], ah
		asm add bx, 80
		asm cmp bx, dx
		asm jne AAAA

		asm pop ds
	}
	outport(0x3ce,0x0005);
	outport(0x3ce,0xff08);

	return(1);
}
/*-----------------------------------------------------------------------------------*/
void box(int left,int top,int right,int bottom,int color)
{	line_x(left,right,top,color);
	line_x(left,right,bottom,color);
	line_y(top,bottom,left,color);
	line_y(top,bottom,right,color);
}
/*-----------------------------------------------------------------------------------*/
int box_fill(int left,int top,int right,int bottom,int color)
{   if (right-left<0 || bottom-top<0 || left<0 || right >639 || top<0 || bottom>479) return(0);

	static unsigned char le[8] = { 0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01 },
						 ri[8] = { 0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff };
	register int i;
	unsigned char far *address;
	int left8 = (unsigned)left>>3, right8 = (unsigned)right>>3, len = right8-left8-1;
	unsigned int offset_ = top*80+left8+1, segment = 0xa000, qq = 80-len,
				 ww = qq-1, le8 = le[left & 7], ri8 = ri[right & 7];
	address = (unsigned char far *)0xa0000000L + (long)top*80L +(long)left8;

	outportb(0x3ce,0x00);	outportb(0x3cf,color);
	outport(0x3ce,0x0f01);	outport(0x3ce,0x0005);

	if (len == -1)
		{	outportb(0x3ce,0x08);	outportb(0x3cf,le8 & ri8);
			for (i=top;i<=bottom;i++, address += 80L)
				*address = *address;
		}
	  else
		{	for (i=top;i<=bottom;i++, offset_ += qq)
			{	outportb(0x3ce,0x08);	outportb(0x3cf,le8);
				*address = *address++;

				outport(0x3ce,0xff08);
				#pragma inline
				{   asm push	di
					asm pushf

					asm	cld
					asm	mov 	es, segment
					asm	mov 	di, offset_
					asm	mov 	al, 0xff
					asm	mov 	cx, len
					asm	rep 	stosb
					asm mov		offset_, di

					asm popf
					asm	pop 	di
				}
				address += len;

				outportb(0x3ce,0x08);	outportb(0x3cf,ri8);
				*address = *address;
				address += ww;
			}
		}
	outport(0x3ce,0x0001);
	outport(0x3ce,0xff08);

	return (1);
}
/*-----------------------------------------------------------------------------------*/
int box_fill_x8(int left,int top,int right,int bottom,int color)
{   if (right-left<0 || bottom-top<0 || left<1 || right >80 || top<0 || bottom>479) return(0);

	int len = right-left+1, len_y = bottom-top+1, step = 80-len;
	unsigned int offset_ = top*80+left-1;

	outportb(0x3ce,0x00);	outportb(0x3cf,color);
	outport(0x3ce,0x0f01);	outport(0x3ce,0x0005);
	outport(0x3ce,0xff08);

	#pragma inline
	{   asm push	di
		asm pushf

		asm	cld
		asm mov		ax, 0xa000
		asm	mov 	es, ax
		asm mov		di, offset_
		asm	mov 	al, 0xff
		asm mov		bx, len_y
		asm mov		dx, len
	LOOPBF8:
		asm	mov 	cx, dx
		asm	rep 	stosb
		asm add		di, step
		asm dec		bx
		asm jnz     LOOPBF8

		asm popf
		asm	pop 	di
	}

	outport(0x3ce,0x0001);
	outport(0x3ce,0xff08);

	return (1);
}
/*-----------------------------------------------------------------------------------*/

int box_fill_pattern(int left,int top,int right,int bottom,int color,int bcolor,int pattern)
{   if (right-left<0 || bottom-top<0 || left<0 || right >639 || top<0 || bottom>479) return(0);
	if (bcolor!=-1) box_fill(left,top,right,bottom,bcolor);

	static unsigned char le[8] = { 0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01 },
						 ri[8] = { 0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff };
	register int i;
	unsigned char far *address;
	int left8 = left >> 3, right8 = right >> 3, len = right8-left8-1;
	unsigned int offset_ = top*80+left8+1, segment = 0xa000, qq = 80-len, ww = qq-1;
	address =(unsigned char far *)MK_FP(0xa000,offset_-1);

	outport(0x3ce,0x0005);	outportb(0x3ce,0x00);
	outportb(0x3cf,color);  outport(0x3ce,0x0f01);

	pattern = ~pattern;
	if (len == NO)
		{	outportb(0x3ce,0x08);
			outportb(0x3cf,le[left & 7] & ri[right & 7] & pattern);
			for (i=top;i<=bottom;i++, address += 80L)
				*address = *address;
		}
	  else
		{	for (i=top;i<=bottom;i++)
			{   pattern = ~pattern;

				outportb(0x3ce,0x08);	outportb(0x3cf,le[left & 7] & pattern);
				*address = *address++;
				outportb(0x3ce,0x08);	outportb(0x3cf,pattern);

//				#pragma inline
				{ 	asm push ds
					asm push di
					asm push si
					asm pushf

					asm	mov	es, segment
					asm mov	ds, segment
					asm	mov	di, offset_
					asm mov	si, offset_
					asm cld
					asm mov cx, len
					asm rep movsb

					asm add di, qq
					asm mov	offset_, di

					asm popf
					asm pop	si
					asm pop	di
					asm pop	ds
				}
				address += len;

				outportb(0x3ce,0x08);	outportb(0x3cf,ri[right & 7] & pattern);
				*address = *address;
				address += ww;
			}
		}
	outport(0x3ce,0x0001);
	outport(0x3ce,0xff08);

	return (1);
}
/*-----------------------------------------------------------------------------------*/
void get_image_x8(int left,int top,int right,int bottom,char far *buff)
{   unsigned _offset = top*80+left-1, _segment, len_x = right-left+1,
			 len_y = bottom-top+1;

	((int *)buff)[0] = (len_x<<3)-1;
	((int *)(buff+2))[0] = len_y-1;
	buff +=4;

	_segment = FP_SEG(buff);

	outport(0x3ce,0x0005);
	outport(0x3ce,0xff08);
	outportb(0x3ce,4);

	#pragma inline
	{	asm push di
		asm push si
		asm push ds
		asm pushf

		asm mov ax, 0xa000
		asm mov ds, ax
		asm mov si, _offset
		asm mov es, _segment
		asm mov di, OFFSET buff
		asm cld
		asm mov ah, len_x
		asm mov bx, len_y
		asm mov dx, 0x3cf

	LOOPGIX1:
		asm mov al, 3

		LOOPGIX2:
			asm out dx, al
			asm xor ch, ch
			asm mov cl, ah
			asm rep movsb
			asm sub si, len_x
			asm dec al
			asm jns LOOPGIX2

		asm add si, 80
		asm dec bx
		asm jnz LOOPGIX1

		asm popf
		asm pop ds
		asm pop si
		asm pop di
	}

	outportb(0x3cf,0);
 }
/*-----------------------------------------------------------------------------------*/
void put_image_x8(int left,int top,char far *buff,int mode)
{   unsigned _offset = top*80+left-1, _segment, len_x, len_y;

	if (mode == COPY_PUT) mode = 0x0003;
		else if (mode == AND_PUT) mode = 0x0803;	// unequal  putimage(,,,,,AND_PUT)
			else if (mode == OR_PUT) mode = 0x1003;
				else if (mode == XOR_PUT) mode = 0x1803;
					else return;

	len_x = ((*(int *)buff++)>>3)+1;
	buff++;
	len_y = *(int *)buff++ +1;
	buff++;

	_segment = FP_SEG(buff);

	outport(0x3ce,0xff08);
	outport(0x3ce,0x0005);
	outport(0x3ce,mode);
	outportb(0x3c4,2);

	#pragma inline
	{	asm push di
		asm push si
		asm push ds
		asm pushf

		asm mov ax, 0xa000
		asm mov ds, _segment
		asm mov si, OFFSET buff
		asm mov es, ax
		asm mov di, _offset

		asm cld
		asm mov ah, len_x
		asm mov bx, len_y
		asm mov dx, 0x3c5

	LOOPGIX1:
		asm mov al, 8

	LOOPGIX2:
			asm out dx, al
			asm xor ch, ch
			asm mov cl, ah
			asm rep movsb
			asm sub di, len_x
			asm shr al, 1
			asm jnc LOOPGIX2

		asm add di, 80
		asm dec bx
		asm jnz LOOPGIX1

		asm popf
		asm pop ds
		asm pop si
		asm pop di
	}

	outport(0x3ec,0x0003);
	outportb(0x3c5,0x0f);
 }
/*-----------------------------------------------------------------------------------*/
