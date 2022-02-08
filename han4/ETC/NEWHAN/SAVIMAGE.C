/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          save_image_han()                                    */
/*   FILENAME           savimage.c                                          */
/*   DATE & TIME        01/07/93(THU) 19:15                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include <stdio.h>

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void save_image_han(int left, int top, int right, int bottom, char *file_name)
/* saves an image */
{
   FILE *stream;
   unsigned char buff;
   int width, height;

   if((stream = fopen(file_name, "wt")) == NULL) return;

   asm	cmp	in_vga_system, 1
   asm  je  vga_save_image_link
   asm  jmp hgc_save_image

   vga_save_image_link:
   asm  jmp vga_save_image

   hgc_save_image:

   asm  mov     ax, right       	/* width = right - left + 1 */
   asm  sub     ax, left
   asm  inc     ax
   asm  mov     width, ax
   putc(width, stream);			/* header of image file */
   asm  mov     ax, bottom      	/* height = (bottom - top + 1) * 4 */
   asm  sub     ax, top
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     height, ax
   putc(height, stream);                /* header of image file */

   asm	mov	si, left		/* ES:SI = VRAM address */
   asm	mov	ax, abs_maxx_han
   asm	inc	ax
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	mov	cx, top
   asm  mul	cx
   asm	add	si, ax

   asm	mov	cx, width		/* CX = width */
   asm	mov	dx, height      	/* DX = height */
   asm	mov	bx, 0x2000		/* BX = skip bytes */
   asm	sub	bx, cx
   asm	mov	ax, abs_maxx_han        /* AX = adjust SI */
   asm	inc	ax

   asm  mov     es, video_seg

   hgc_read:

   asm	push	si
   asm  push    cx

   get_byte1:

   asm	cmp	cx, 0
   asm	jbe	exit_get_byte1
   asm	dec	cx
   asm	mov	al, es:[si]
   asm	mov	buff, al
   asm	inc	si
   asm	push	bx
   putc(buff, stream);
   asm	pop	bx
   asm	jmp	get_byte1

   exit_get_byte1:

   asm	pop	cx
   asm	add	si, bx

   get_byte2:

   asm	cmp	cx, 0
   asm	jbe	exit_get_byte2
   asm	dec	cx
   asm	mov	al, es:[si]
   asm	mov	buff, al
   asm	inc	si
   asm	push	bx
   putc(buff, stream);
   asm	pop	bx
   asm	jmp	get_byte2

   exit_get_byte2:

   asm	pop	cx
   asm	add	si, bx

   get_byte3:

   asm	cmp	cx, 0
   asm	jbe	exit_get_byte3
   asm	dec	cx
   asm	mov	al, es:[si]
   asm	mov	buff, al
   asm	inc	si
   asm	push	bx
   putc(buff, stream);
   asm	pop	bx
   asm	jmp	get_byte3

   exit_get_byte3:

   asm	pop	cx
   asm	add	si, bx

   get_byte4:

   asm	cmp	cx, 0
   asm	jbe	exit_get_byte4
   asm	dec	cx
   asm	mov	al, es:[si]
   asm	mov	buff, al
   asm	inc	si
   asm	push	bx
   putc(buff, stream);
   asm	pop	bx
   asm	jmp	get_byte4

   exit_get_byte4:

   asm	pop	cx

   asm	pop	si
   asm	add	si, ax
   asm	dec	dx
   asm  jbe quit_save_image_link
   asm  jmp hgc_read

   quit_save_image_link:

   asm  jmp quit_save_image

   vga_save_image:

   asm  mov     ax, right       	/* width = right - left + 1 */
   asm  sub     ax, left
   asm  inc     ax
   asm  mov     width, ax
   putc(width, stream);			/* header of image file */
   asm  mov     ax, bottom      	/* height = (bottom - top + 1) * 16 */
   asm  sub     ax, top
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     height, ax		/* header of bitmap */
   putc(height, stream);		/* header of image file */

   asm  mov     es, video_seg           /* ES:SI = VRAM address */
   asm  mov     si, left
   asm  mov     ax, VGA_SCR_WIDTH
   asm  mov     bx, top
   asm  mul     bx
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     si, ax

   asm  mov     bx, height
   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */
   asm  mov     al, 0x04		/* AL = Read Map Select register */

   vga_read:

   asm  mov     ah, 0x03		/* start with plan 3 */

   read_each_plan:

   asm 	out 	dx, ax
   asm 	mov 	cx, width
   asm	push	ax

   get_byte:

   asm	cmp	cx, 0
   asm	jbe	exit_get_byte
   asm	dec	cx
   asm	mov	al, es:[si]
   asm	mov	buff, al
   asm	inc	si
   asm	push	bx
   asm	push	cx
   asm	push	si
   putc(buff, stream);
   asm	pop	si
   asm	pop	cx
   asm	pop	bx
   asm	jmp	get_byte

   exit_get_byte:

   asm	pop	ax
   asm	sub	si, width
   asm 	dec 	ah
   asm 	jge 	read_each_plan		/* end with plan 0 */

   asm 	add 	si, VGA_SCR_WIDTH
   asm	dec	bx
   asm	jg	vga_read

   asm  mov	ax, 0x0004
   asm	out	dx, ax

   quit_save_image:

   fclose(stream);
}
