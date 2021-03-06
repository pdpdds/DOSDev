#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "image.h"
#include "types.h"
#include "memory.h"
#include "vga.h"

#ifdef __GNUC__
#include <iostream>

using namespace std;
#endif

/* mode, width, height, bytes_per_line, base_addr, bpp, Bpp, colors, vsync_reg, scale */
const adapter::video_mode vga::video_modes[] = {
 {TEXT,80,25,80,0xb800,1,8,0xF,0x3da,0},
 {VGAMONO,640,480,80,0xa000,1,1,0x1,0x3da,0},
 {VGAHI,640,480,80,0xa000,4,1,0xF,0x3da,0},
 {VGALO,320,200,320,0xa000,1,8,0xFF,0x3da,2},
 {X16,160,100,160,0xb800,1,8,0xF,0x3da,4}
};

vga::vga(void)
{
	_savedvmode = getmode();
}

vga::~vga(void)
{
	setmode(_savedvmode);
	if (_cur_palette == palette::CUSTOM && _palette) delete _palette;
}

bool vga::setup(void)
{
	for (unsigned long int i=0;i<sizeof(video_modes);i++) {
		if (video_modes[i].mode == _vmode) {
			_row_bytes = video_modes[i].bytes;
			_width = video_modes[i].x;
			_height = video_modes[i].y;
			bpp = video_modes[i].bpp;
			Bpp = 8/bpp;
			planes = video_modes[i].planes;
			colors = video_modes[i].colors;
			SR = video_modes[i].sr;
			buf_size = _row_bytes*_height;

#ifdef __BORLANDC__
			_buffer = (unsigned char far *) MK_FP(video_modes[i].fp,0);
			screen.size(_width,_height);
#endif
			return true;
		}
	}
	return false;
}

void vga::setmode(Mode mode)
{
#ifdef __BORLANDC__
	_AL=(unsigned char) mode;
	_AH=0;
	geninterrupt(0x10);
#endif
}

bool vga::graphmode(Mode mode)
{
	switch (mode) {
		case TEXT:
			return textmode();
		case X16:
			setpalette(palette::TEXT_PAL);
			return x16mode();
		case VGALO:
			setpalette(palette::VGA_PAL);
			break;
		case UNDEF:
		case CGALO1:
		case CGALO2:
		case CGAHI:
		case COMPAQ:
		case HERC:
		case BWTEXT:
		case PCJRLO:
		case PCJRMED:
		case PCJRHI:
		case EGALO:
		case EGAMED:
		case EGAMONO:
		case EGAHI:
		case VGAMONO:
		case VGAHI:
		case SVGA:
		case XGA:
		case HD:
		case FHD:
			return false;
	}

	_vmode=mode;

	setmode(_vmode);
	return setup();
}

bool vga::textmode(void)
{
#ifdef __BORLANDC__
	_AL=0x03;
	_AH=0;
	geninterrupt(0x10);
	return setup();
#else
	return true;
#endif
}


bool vga::x16mode(void)
{
#ifdef __BORLANDC__
	unsigned char status;

	_AL=(unsigned char) TEXT;
	_AH=0;
	geninterrupt(0x10);


	//disable VGA blink
	status=inportb(0x3da);
	outportb(0x3c0,0x30);
	status=inportb(0x3c1);

	if (status != 0xFF) {  // VGA presumed, turn 80x50 into 80x100

		status&=0xf7;
		outportb(0x3c0,status);
		write_crtc(CRTCb,0x09,0x03);

	}

	_vmode=X16;

	return setup();
#else
	return true;
#endif
}

void vga::write_crtc(unsigned int port, unsigned char reg, unsigned char val)
{
#ifdef __BORLANDC__
	outportb(port, reg);
	outportb(port+1,val);
#endif
}

adapter::Mode vga::getmode(void)
{
#ifdef __BORLANDC__
	_AX=0x0f00;
	geninterrupt(0x10);
	_vmode=(Mode)_AL;
	return (_vmode);
#else
	return _vmode;
#endif
}

void vga::cls(void)
{
	screen.clear();
	update();
}

void vga::update(void)
{
	vsync();
	translate(screen._buffer);
}

void vga::translate(ptr_t src)
{
#ifdef __BORLANDC__
	switch (_vmode) {
		case TEXT:
			_CX=buf_size>>1;
			_DI=FP_OFF(_buffer);
			_SI=FP_OFF(src);
			_BX=FP_SEG(src);
			_DS=_BX;
			_AX=FP_SEG(_buffer);
			_ES=_AX;

		xlate:
			asm {
				lodsw				// 5
				mov	bx, ax
				shl ax,8
				mov al,0xDB
				stosw
				mov	ax, bx
				mov al,0xDB
				stosw
				loop	xlate       // 15
			}                       // 44

			break;
		case X16:
#ifdef i86
			_CX=buf_size>>1;
			_DI=FP_OFF(_buffer);
			_SI=FP_OFF(src);
			_BX=FP_SEG(src);
			_DS=_BX;
			_AX=FP_SEG(_buffer);
			_ES=_AX;

		xlate16:
			asm {
				lodsw				// 5
				shl		al, 4       // 3
				or		ah, al      // 2
				mov		al, 0xde    // 2
				stosw               // 4
				loop	xlate16       // 15
			}                       // 44
#else
			_CX=buf_size>>2;
			_DI=FP_OFF(_buffer);
			_SI=FP_OFF(src);
			_BX=FP_SEG(src);
			_DS=_BX;
			_AX=FP_SEG(_buffer);
			_ES=_AX;

		xlate32:
			asm {
				lodsd				// 5
				//and		eax, 0x0F0F0F0F // 2
				shl		al, 4       // 3
				or		ah, al      // 2
				mov		al, 0xde    // 2
				ror		eax, 16     // 3
				shl		al, 4       // 3
				or		ah, al      // 2
				mov		al, 0xde    // 2
				rol		eax, 16     // 3
				stosd               // 4
				loop	xlate32       // 15
			}                       // 44
#endif
			break;

		case VGALO:
			memory::blit(_buffer,src,buf_size);
			break;
	}
#endif
}

void vga::vsync(void)
{
#ifdef __BORLANDC__
	unsigned char p;

		do {
			p=inportb(SR);
		} while (p & 8);

		do {
			p=inportb(SR);
		} while (!(p & 8));
#endif
}

bool vga::setpalette(palette::pal_type pal)
{
	if (!adapter::setpalette(pal)) return false;

	setpalentries((palette::pal_t *)palette::palettes[pal].pal,palette::palettes[pal].palette_entries);
	return true;
}

bool vga::setpalette(palette::pal_t *pal, int palette_entries)
{
	if (_cur_palette == palette::CUSTOM && _palette) delete _palette;
	_palette_size=palette_entries;
	_cur_palette=palette::CUSTOM;
	_palette=new palette::pal_t[_palette_size];

	if (_palette == NULL) return false;

	memcpy(_palette,pal,sizeof(palette::pal_t)*_palette_size);

	setpalentries(pal,palette_entries);

	return true;
}

void vga::setpalentries(palette::pal_t *pal, int palette_entries)
{
#ifdef __BORLANDC__
	outportb(VGA_PAL_MASK,0xff);
	outportb(VGA_PAL_REG,0);
	for(int i=0;i<_palette_size;i++) {
		outportb(VGA_PAL_DATA,pal[i].r>>2);
		outportb(VGA_PAL_DATA,pal[i].g>>2);
		outportb(VGA_PAL_DATA,pal[i].b>>2);
	}
#endif
}

void vga::setpalentry(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
#ifdef __BORLANDC__
	  asm {
	    mov dx, VGA_PAL_REG
	    mov al, index
	    out dx, al

	    inc dx
	    mov al,r
		shr al,2
	    out dx, al
	    mov al, g
		shr al,2
	    out dx, al
	    mov al, b
		shr al,2
	    out dx, al
	  }
#endif
}
