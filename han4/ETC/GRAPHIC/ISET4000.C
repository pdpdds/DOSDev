/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          ET 4000 detecting function                          */
/*   FILENAME           iset4000.c                                          */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_et4000(void)
/* This routine detects the presence of ET 4000 by looking for an   */
/* ET 4000-only register(33h). I/O port of 33h is 03d4h(color mode) */
/* or 03b4h(mono mode).						    */
{
   asm	mov	dx, 03cch
   asm	in	al, dx
   asm	and	al, 01h
   asm  jz	mono_mode
   asm	mov	dx, 03d4h	/* if bit 0 is 1, it's in color mode */
   asm	jmp	check_for_et4000

   mono_mode:

   asm	mov	dx, 03b4h	/* if bit 0 is not 1, it's in mono mode */

   check_for_et4000:

   asm	mov	al, 33h		/* ET 4000-only register */
   asm	out	dx, al
   asm	inc	dx
   asm	in	al, dx		/* AL = current value */
   asm	mov	ah, al		/* preserve original value in AH */

   asm	xor	al, 0fh		/* inverse lower 4 bit of AL for test */
   asm	out	dx, al		/* send AL to output port */
   asm	in	al, dx		/* AL = returned value */
   asm	xchg	ah, al		/* AH = returned value, AL = original value */
   asm  out	dx, al		/* restore original value */

   asm	xor	al, 0fh
   asm	cmp	ah, al
   asm	je	exist		/* if changed, it's a ET 4000 */
   asm  xor	ax, ax
   asm	jmp	quit

   exist:

   asm	mov	ax, 1

   quit: ;
}

