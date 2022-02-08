/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          getimage_han()                                      */
/*   FILENAME           getimage.c                                          */
/*   DATE & TIME        12/18/92(FRI) 14:15                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void getimage_han(int left, int top, int right, int bottom, char *bitmap)
/* saves an image */
{
   int width, height;

   asm	push	ds

   asm	cmp	in_vga_system, 1
   asm	je	vga_getimage

   asm  les     di, bitmap      	/* ES:DI = buffer address */
   asm  mov     ax, right       	/* width = right - left + 1 */
   asm  sub     ax, left
   asm  inc     ax
   asm  mov     width, ax
   asm	stosw                   	/* header of bitmap */
   asm  mov     ax, bottom      	/* height = (bottom - top + 1) * 4 */
   asm  sub     ax, top
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     height, ax		/* header of bitmap */
   asm	stosw

   asm	mov	si, left		/* DS:SI = VRAM address */
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

   asm  mov     ds, video_seg

   hgc_read:

   asm	push	si

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx
   asm	add	si, bx

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx
   asm	add	si, bx

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx
   asm	add	si, bx

   asm	push	cx
   asm	rep	movsb
   asm	pop	cx

   asm	pop	si
   asm	add	si, ax
   asm	dec	dx
   asm	jg	hgc_read
   asm  jmp	quit_getimage

   vga_getimage:

   asm  les     di, bitmap      	/* ES:DI = buffer address */
   asm  mov     ax, right       	/* width = right - left + 1 */
   asm  sub     ax, left
   asm  inc     ax
   asm  mov     width, ax
   asm	stosw                   	/* header of bitmap */
   asm  mov     ax, bottom      	/* height = (bottom - top + 1) * 16 */
   asm  sub     ax, top
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     height, ax		/* header of bitmap */
   asm	stosw

   asm  mov     ds, video_seg           /* DS:SI = VRAM address */
   asm	mov	si, left
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
   asm 	rep 	movsb
   asm	sub	si, width
   asm 	dec 	ah
   asm 	jge 	read_each_plan		/* end with plan 0 */

   asm 	add 	si, VGA_SCR_WIDTH
   asm	dec	bx
   asm	jg	vga_read

   asm  mov	ax, 0x0004
   asm	out	dx, ax

   quit_getimage:

   asm 	pop 	ds
}
