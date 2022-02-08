/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          VGA detecting function                              */
/*   FILENAME           isvga.c                                             */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_vga(void)
/* This routine detects the presence of VGA by making a call to an PS/2 BIOS */
/* function. Every VGA has PS/2 BIOS.					     */
{
   asm	mov	ax, 1a00h
   asm	int	10h
   asm	cmp	bl, 07h
   asm	je	vga_exist
   asm	cmp	bl, 08h
   asm	je	vga_exist
   asm	xor	ax, ax
   asm	jmp	quit

   vga_exist:

   asm	mov	ax, 1

   quit: ;
}

