/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          ATI VGA Wonder Rev. 2 dtecting function            */
/*   FILENAME           iswond2.c                                           */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_wonder2(void)
/* This routine detects the presence of ATI VGA Wonder Rev. 2 by looking for */
/* the ID code in c000h:0043h 				     	             */
{
   asm	mov	ax, 0c000h
   asm  mov	es, ax
   asm	mov	di, 0043h
   asm	cmp	word ptr es:[di], 1
   asm  ja	exist
   asm	xor	ax, ax
   asm	jmp	quit

   exist:

   asm	mov	ax, 1

   quit: ;
}
