/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          HGC detecting function                              */
/*   FILENAME           ishgc.c                                             */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_hgc(void)
/* This routine detects the presence of HGC by monitoring bit 7 of the CRT */
/* Status byte. This bit changes on Hercules adapters but does not change  */
/* on an MDA.								   */
{
   asm	mov	dx, 03bah	/* DX = status port */
   asm	in	al, dx
   asm	and	al, 80h
   asm	mov	ah, al		/* AH = bit 7 of CRT Status byte */

   asm	mov	cx, 8000h	/* wait for bit 7 changed */

   wait_bit7_changed:

   asm	in	al, dx
   asm	and 	al, 80h
   asm	cmp	ah, al
   asm	loope	wait_bit7_changed

   asm	jne	hgc_exist	/* if bit 7 changed, it's a Hercules */
   asm	xor	ax, ax
   asm	jmp	quit

   hgc_exist:

   asm	mov	ax, 1

   quit: ;
}

