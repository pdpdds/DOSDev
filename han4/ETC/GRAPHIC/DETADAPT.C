/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          video adapter detecting function                    */
/*   FILENAME           detadapt.c                                          */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int detect_adapter(void)
/* This routine detects video adapter automatically. */
{
   static char ps2_video_bios[] =
   {
      NONE, MDA, CGA, NONE, EGA, EGA, NONE,
      VGA, VGA, NONE, MCGA, MCGA, MCGA
   };
					/* adapter = video adapter number   */
   unsigned char adapter;

   asm	mov	ax, 1a00h		/* call the PS/2 BIOS */
   asm	int	10h

   asm	mov	byte ptr adapter, NONE  /* assume there is no video adapter */

   asm	cmp 	al, 1ah
   asm	jne	no_ps2		/* if AL is not 1ah, it is not a PS/2 BIOS */

   asm  cmp	bl, 0ffh
   asm	je	quit            /* if BL is ffh, it is an unknown adapter */

   asm	mov	ax, ds
   asm	mov	es, ax
   asm  mov 	di, offset ps2_video_bios
   asm  xor	bh, bh
   asm	add	di, bx
   asm	mov	al, byte ptr es:[di]
   asm	mov	byte ptr adapter, al
   asm	jmp	quit

   no_ps2:				/* direct detection */

   if(is_vga())
      adapter = VGA;
   else if(is_ega())
      adapter = EGA;
   else if(is_6845(0x03d4))
      adapter = CGA;
   else if(is_6845(0x03b4))
      adapter = (is_hgc() ? HGC : MDA);

   quit:

   asm	xor	ax, ax
   asm  mov 	al, adapter		/* return video adapter number */
}
