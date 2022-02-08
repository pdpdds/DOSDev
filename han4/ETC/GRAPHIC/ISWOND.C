/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          ATI VGA Wonder dtecting function                   */
/*   FILENAME           iswond.c                                            */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_wonder(void)
/* This routine detects the presence of ATI VGA Wonder by looking for */
/* the ID code in c000h:0030h 					      */
{
   char id_code[10] = { 0x20, 0x37, 0x36, 0x31, 0x32,
			0x39, 0x35, 0x35, 0x32, 0x30 };

   asm	push	ds

   asm	lds	si, id_code
   asm	mov	ax, 0c000h
   asm  mov	es, ax
   asm	mov	di, 0030h

   asm	mov	cx, 5

   compare_id_code:

   asm	lodsw
   asm	cmp	ax, word ptr es:[di]
   asm  jne	not_exist
   asm	loop	compare_id_code

   asm	mov	ax, 1
   asm  jmp	quit

   not_exist:

   asm	xor	ax, ax

   quit:

   asm	pop	ds
}

