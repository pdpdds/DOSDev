/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_to_gr_mode(), set_to_text_mode(),               */
/*                      set_gr_card_han(), set_hgc_mode_han()               */
/*   FILENAME           setgr.c                                             */
/*   DATE & TIME        05/09/92(SAT) 11:45                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <dos.h>
#include "hanio.h"
#include "misc.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

static void set_gr_card_han(unsigned char flag, int mode)
/* set registers of graphics card except for HGC */
{
   static char old;
   union REGS inr, outr;

   if(mode)	/* set mode */
   {
      inr.h.ah = 0x0f;
      int86(0x10, &inr, &outr);
      old      = outr.h.al;
      inr.h.ah = 0x00;
      inr.h.al = flag;
      int86(0x10, &inr, &outr);
   }
   else		/* recover */
   {
      inr.h.ah = 0;
      inr.h.al = old;
      int86( 0x10, &inr, &outr );
   }
}

static void set_hgc_mode_han(int mode)
/* set mode of HGC(set to text mode, if mode == -1) */
{
   const static char reg_data[3][12] =
   {
      { 0x31, 0x28, 0x29, 0x08, 0x68, 0x02,	/* 640x400 mode */
	0x64, 0x65, 0x02, 0x03, 0x02, 0x01 },
      { 0x35, 0x2d, 0x2e, 0x07, 0x5b, 0x02,	/* 720x480 mode */
	0x57, 0x57, 0x02, 0x03, 0x00, 0x00 },
      { 0x61, 0x50, 0x52, 0x0f, 0x19, 0x06,	/* text mode */
	0x19, 0x19, 0x02, 0x0d, 0x0b, 0x0c }
   };
   unsigned char flag;
   int index, i;

   /* send data to register */
   outportb(0x03bf, 0x03);
   outportb(0x03b8, 0x00);
   index = (mode == -1) ? 2 : mode / 2;
   for(i = 0; i < 12; i++)
   {
      outportb(0x03b4, i);
      outportb(0x03b5, reg_data[index][i]);
   }

   /* set mode */
   switch(mode)
   {
      case INIT_HGC640_0 : flag = 0x1e; break;
      case INIT_HGC640_1 : flag = 0x9e; break;
      case INIT_HGC720_0 : flag = 0x0a; break;
      case INIT_HGC720_1 : flag = 0x8a; break;
      case -1            : flag = 0x28; break;
   }
   outportb(0x03b8, flag);
}

void set_to_gr_mode(int mode)
/* set graphics mode(all kinds of graphics card, set to text mode if mode == -1) */
{
   char flag;

   switch(mode)
   {
      case INIT_HGC640_0 :
      case INIT_HGC640_1 :
      case INIT_HGC720_0 :
      case INIT_HGC720_1 : set_hgc_mode_han(mode);
			   break;
      case INIT_VGA      : flag = 0x12;
			   break;
      case INIT_EGA      : flag = 0x10;
			   break;
      case INIT_EGA_MONO : flag = 0x0f;
			   break;
      case INIT_PC24     :
      case INIT_ATT      : flag = 0x69;
			   break;
      case INIT_CORONA   : flag = 0x61;
			   break;
      case INIT_TOSHIBA  : flag = 0x74;
			   break;
      case INIT_SPC      : flag = 0x16;
			   break;
      case INIT_TV       : flag = 0x9;
			   break;
   }
   if(mode != INIT_HGC640_0 && mode != INIT_HGC640_1 &&
      mode != INIT_HGC720_0 && mode != INIT_HGC720_1)
      set_gr_card_han(flag, 1);
   if(mode == INIT_SPC) outportb(0x3df, 0xbe);
}

void set_to_text_mode(int mode)
{
   if(mode == INIT_HGC640_0 || mode == INIT_HGC640_1 ||
      mode == INIT_HGC720_0 || mode == INIT_HGC720_1)
      set_hgc_mode_han(-1);
   else
      set_gr_card_han(0, 0);
}
