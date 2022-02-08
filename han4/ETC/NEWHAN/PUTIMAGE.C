/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          putimage_han()                                      */
/*   FILENAME           putimage.c                                          */
/*   DATE & TIME        12/18/92(FRI) 14:15                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void putimage_han(int left, int top, char *bitmap)
/* restores an image saved in bitmap */
{
   int width, height;

   asm	push	ds

   asm	cmp	in_vga_system, 1
   asm	je	vga_putimage

   asm	mov	es, video_seg   	/* ES:DI = VRAM address */
   asm	mov	di, left
   asm	mov	ax, abs_maxx_han
   asm	inc	ax
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	mov	cx, top
   asm  mul	cx
   asm	add	di, ax

   asm	mov	bx, ds			/* save DS to BX */
   asm  lds     si, bitmap      	/* DS:SI = buffer address */
   asm	mov	dx, ds			/* save new DS to DX */
   asm	lodsw				/* get width */
   asm	mov	width, ax
   asm	lodsw				/* get height */
   asm	mov	height, ax
   asm	mov	ds, bx			/* recover DS by BX */

   asm	mov	ax, abs_maxx_han        /* AX = adjust DI */
   asm	inc	ax
   asm	mov	ds, dx			/* recover DS by DX */
   asm	mov	cx, width		/* CX = width */
   asm	mov	dx, height      	/* DX = height */
   asm	mov	bx, 0x2000		/* BX = skip bytes */
   asm	sub	bx, cx

   hgc_out:

   asm	push	di

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx
   asm	add	di, bx

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx
   asm	add	di, bx

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx
   asm	add	di, bx

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx

   asm	pop	di
   asm	add	di, ax
   asm	dec	dx
   asm	jg	hgc_out
   asm	jmp	quit_putimage

   vga_putimage:

   asm 	mov 	es, video_seg		/* ES:DI = VRAM address */
   asm	mov	di, left
   asm  mov     ax, VGA_SCR_WIDTH
   asm  mov     bx, top
   asm  mul     bx
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm  add     di, ax

   asm  lds	si, bitmap		/* DS:SI = buffer address */
   asm  lodsw				/* get width */
   asm	mov	width, ax
   asm	lodsw				/* get height */
   asm	mov	height, ax

   asm  mov  	bx, height
   asm 	mov 	dx, 0x03c4		/* DX = port 0x03c4 */
   asm	mov	al, 0x02		/* Seqencer Map Mask register */
   asm	mov	cx, width

   vga_out:

   asm	mov	ah, 0x08		/* start with plan 3 */

   draw_line_vga:

   asm	out	dx, ax
   asm	push	cx
   asm	rep     movsb
   asm	pop	cx
   asm	sub	di, width
   asm	shr	ah, 1
   asm	jnc	draw_line_vga

   asm	add	di, VGA_SCR_WIDTH
   asm	dec	bx
   asm	jg	vga_out

   asm	mov	ax, 0x0f02
   asm	out	dx, ax

   quit_putimage:

   asm 	pop 	ds
}
