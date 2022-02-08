/************************************************/
/* 						*/
/*       SuperVGA 256 BGI driver defines	*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

#include <dos.h>

typedef unsigned char DacPalette256[256][3];

extern int far _Cdecl Svga256_fdriver[];

/* These are the currently supported modes */
#define	SVGA320x200x256		0	/* 320x200x256 Standard VGA */
#define	SVGA640x400x256		1	/* 640x400x256 Svga/VESA */
#define	SVGA640x480x256		2	/* 640x480x256 Svga/VESA */
#define	SVGA800x600x256		3	/* 800x600x256 Svga/VESA */
#define SVGA1024x768x256	4	/* 1024x768x256 Svga/VESA */

#ifndef XNOR_PUT
#define XNOR_PUT	5
#define NAND_PUT	6
#define NOR_PUT		7
#endif
#define TRANS_COPY_PUT	8

/* Setvgapalette256 sets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* setvgapalette256(&dac256);			      */
void setvgapalette256(DacPalette256 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1012;
  reg.r_bx = 0;
  reg.r_cx = 256;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}
