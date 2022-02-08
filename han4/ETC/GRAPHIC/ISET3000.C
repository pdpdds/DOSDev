/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          ET 3000 detecting function                          */
/*   FILENAME           iset3000.c                                          */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_et3000(void)
/* This routine detects the presence of ET 3000 by looking for page */
/* selection register(03cdh). 					    */
{
   asm	mov	dx, 03cdh	/* page selection register */
   asm	in	al, dx
   asm	mov	ah, al		/* preserve original value in AH */

   asm	mov     al, bl		/* AL = arbitrary value(BL) */
   asm	out	dx, al
   asm	in	al, dx
   asm	xchg	ah, al		/* AL = orginal value, AH = new value */
   asm	out	dx, al

   asm	cmp	ah, bl
   asm	je	exist
   asm	xor	ax, ax		/* if not changed, it's not an ET 3000 */
   asm	jmp	quit

   exist:

   asm	mov	ax, 1 		/* if changed, it's an ET 3000 */

   quit: ;
}

