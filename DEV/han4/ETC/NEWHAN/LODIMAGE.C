/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          load_image_han()                                    */
/*   FILENAME           lodimage.c                                          */
/*   DATE & TIME        01/07/93(THU) 19:15                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void load_image_han(int left, int top, char *file_name)
/* restores an image saved in the <file_name> */
{
   FILE *stream;
   int width, height;

   if((stream = fopen(file_name, "rt")) == NULL) return;

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

   getc(stream);			/* get width */
   asm	mov	width, ax
   getc(stream);
   asm	mov	height, ax              /* get height */

   asm	mov	ax, abs_maxx_han        /* AX = adjust DI */
   asm	inc	ax
   asm	mov	cx, width		/* CX = width */
   asm	mov	dx, height      	/* DX = height */
   asm	mov	bx, 0x2000		/* BX = skip bytes */
   asm	sub	bx, cx

   hgc_out:

   asm	push	di

   asm	push	cx

   put_byte1:

   asm	cmp	cx, 0
   asm	jbe	exit_put_byte1
   asm	dec	cx
   getc(stream);
   asm  movsb
   asm	jmp	put_byte1

   exit_put_byte1:

   asm	pop	cx
   asm	add	di, bx

   asm	push	cx

   put_byte2:

   asm	cmp	cx, 0
   asm	jbe	exit_put_byte2
   asm	dec	cx
   getc(stream);
   asm  movsb
   asm	jmp	put_byte2

   exit_put_byte2:

   asm	pop	cx
   asm	add	di, bx

   asm	push	cx

   put_byte3:

   asm	cmp	cx, 0
   asm	jbe	exit_put_byte3
   asm	dec	cx
   getc(stream);
   asm  movsb
   asm	jmp	put_byte3

   exit_put_byte3:

   asm	pop	cx
   asm	add	di, bx

   asm	push	cx

   put_byte4:

   asm	cmp	cx, 0
   asm	jbe	exit_put_byte4
   asm	dec	cx
   getc(stream);
   asm  movsb
   asm	jmp	put_byte4

   exit_put_byte4:

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

   getc(stream);			/* get width */
   asm	mov	width, ax
   getc(stream);
   asm	mov	height, ax              /* get height */

   asm  mov  	bx, height
   asm 	mov 	dx, 0x03c4		/* DX = port 0x03c4 */
   asm	mov	al, 0x02		/* Seqencer Map Mask register */
   asm	mov	cx, width

   vga_out:

   asm	mov	ah, 0x08		/* start with plan 3 */

   draw_line_vga:

   asm	out	dx, ax
   asm	push	ax
   asm	push	cx

   put_byte:

   asm	cmp	cx, 0
   asm	jbe	exit_put_byte
   asm	dec	cx
   getc(stream);
   asm  movsb
   asm	jmp	put_byte

   exit_put_byte:

   asm	pop	cx
   asm	pop	ax
   asm	sub	di, width
   asm	shr	ah, 1
   asm	jnc	draw_line_vga

   asm	add	di, VGA_SCR_WIDTH
   asm	dec	bx
   asm	jg	vga_out

   asm	mov	ax, 0x0f02
   asm	out	dx, ax

   quit_putimage:

   fclose(stream);
}
