/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          EGA detecting function                              */
/*   FILENAME           isega.c                                             */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_ega(void)
/* This routine detects the presence of EGA by making a call to an EGA BIOS */
/* function which doesn't exist in the default MDA/CGA BIOS.		    */
{
   asm	mov	bl, 10h		/* BL = return EGA info */
   asm	mov	ah, 12h		/* AH = INT 10H function number */
   asm	int 	10h		/* call EGA BIOS for info */

   asm	cmp	bl, 10h
   asm  jne	ega_exist	/* if EGA BIOS is present BL != 10h */
   asm	xor	ax, ax
   asm	jmp	quit

   ega_exist:

   asm	mov	ax, 1

   quit: ;
}

