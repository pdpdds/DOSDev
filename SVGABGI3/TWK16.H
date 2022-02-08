/************************************************/
/* 						*/
/*        Tweaked 16 BGI driver defines		*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

#include <dos.h>

typedef unsigned char DacPalette16[16][3];

extern int far Twk16_fdriver[];

/* These are the currently supported modes */
#define TWK704x528x16	0	/* 704x528x16 Tweaked VGA */
#define TWK720x540x16	1	/* 720x540x16 Tweaked VGA */
#define TWK736x552x16	2	/* 736x552x16 Tweaked VGA */
#define TWK752x564x16	3	/* 752x564x16 Tweaked VGA */
#define TWK784x588x16	4	/* 784x588x16 Tweaked VGA */
#define TWK800x600x16	5	/* 800x600x16 Tweaked VGA */

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