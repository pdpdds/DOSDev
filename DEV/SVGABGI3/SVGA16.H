/************************************************/
/* 						*/
/*        SuperVGA 16 BGI driver defines	*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

#include <dos.h>

typedef unsigned char DacPalette16[16][3];

extern int far Svga16_fdriver[];

/* These are the currently supported modes */
#define SVGA320x200x16	0	/* 320x200x16 Standard EGA/VGA */
#define SVGA640x200x16	1	/* 640x200x16 Standard EGA/VGA */
#define SVGA640x350x16	2	/* 640x350x16 Standard EGA/VGA */
#define SVGA640x480x16	3	/* 640x480x16 Standard VGA */
#define SVGA800x600x16	4	/* 800x600x16 SuperVGA/VESA */
#define SVGA1024x768x16	5	/* 1024x768x16 SuperVGA/VESA */

#ifndef XNOR_PUT
#define	XNOR_PUT	5
#define NAND_PUT	6
#define NOR_PUT		7
#endif

/* Setvgapalette16 sets the entire 16 color palette */
/* PalBuf contains RGB values for all 16 colors     */
/* R,G,B values range from 0 to 63	            */
/* Usage: 					    */ 
/*  DacPalette16 dac16;                             */
/*						    */
/*  setvgapalette(&dac16);			    */
void setvgapalette16(DacPalette16 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1012;
  reg.r_bx = 0;
  reg.r_cx = 16;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}

