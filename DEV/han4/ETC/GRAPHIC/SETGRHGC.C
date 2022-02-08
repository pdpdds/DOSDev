/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          HGC graphics mode setting function                  */
/*   FILENAME           sethgc.c                                            */
/*   DATE & TIME        12/21/92 (MON) 13:12                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/


void set_gr_mode_hgc(int mode)
/* This routine sets graphics mode of HGC. */
{
   static unsigned char reg_data[] =
   {
      0x31, 0x28, 0x29, 0x08, 0x68, 0x02,	/* 640x400 mode */
      0x64, 0x65, 0x02, 0x03, 0x02, 0x01,
      0x35, 0x2d, 0x2e, 0x07, 0x5b, 0x02,	/* 720x480 mode */
      0x57, 0x57, 0x02, 0x03, 0x00, 0x00
   };
   static unsigned char flag[] =
   {
      0x1e, 0x9e, 0x0a, 0x8a
   };

   /* prepare 6845 CRTC */

   asm	mov	dx, 03bfh
   asm	mov	al, 03h
   asm	out	dx, al
   asm	mov	dl, 0b8h
   asm	xor	ax, ax
   asm	out	dx, al

   asm  mov	si, offset reg_data
   asm	mov	ax, mode
   asm	shr	ax, 1
   asm  mov	bl, 12
   asm	mul	bl
   asm	add	si, ax

   /* send data to register */

   asm	xor	cx, cx

   loop1:

   asm	mov	dl, 0b4h
   asm	mov	ax, cx
   asm	out	dx, al
   asm	inc	dx
   asm	lodsb
   asm	out	dx, al
   asm	inc	cx
   asm	cmp	cx, 12
   asm	jb	loop1

   /* set mode */
   asm	mov	dl, 0b8h
   asm	mov	si, offset flag
   asm	mov	ax, mode
   asm  sub	ax, HGC_640x400x2_0
   asm	add	si, ax
   asm  lodsb
   asm	out	dx, al
}
