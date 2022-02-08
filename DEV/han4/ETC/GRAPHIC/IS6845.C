/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          6845 detecting function                             */
/*   FILENAME           is6845.c                                            */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int is_6845(int port_addr)
/* This routine detects the presence of the CRTC on an MDA, CGA, or HGC. */
/* The technique is to write and read register 0x0f of the chip(Cursor   */
/* Location Low). If the same value is read as written, assume the chip  */
/* is present at the specified port address 				 */
{
   asm	mov	dx, port_addr	/* DX = port addr */
   asm 	mov	al, 0fh
   asm	out	dx, al		/* select 6845 register 0x0f */
   asm 	inc	dx

   asm 	in	al, dx		/* AL = current Cursor Low value */
   asm	mov	ah, al		/* preserve AL in AH */
   asm	mov	al, 66h		/* AL = arbitrary value */
   asm	out	dx, al		/* try to write to 6845 */

   asm	mov	cx, 100h	/* wait for 6845 to respond */

   wait_to_respond:

   asm  loop	wait_to_respond

   asm 	in	al, dx		/* AL = returned value */
   asm	xchg	ah, al		/* AH = returned value, AL = orginal value */
   asm  out	dx, al		/* restore original value */

   asm	cmp	ah, 66h
   asm  je	exist
   asm  xor	ax, ax          /* if not changed, there is not an 6845 */
   asm	jmp	quit

   exist:

   asm  mov	ax, 1           /* if changed, there is an 6845 */

   quit: ;
}
