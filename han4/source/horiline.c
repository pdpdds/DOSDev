/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          draw_horiz_line()                                   */
/*   FILENAME           horiline.c                                          */
/*   DATE & TIME        05/13/92(WED) 21:20                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_horiz_line(int x, int y, int width, int color)
/* draw line horizontally */
{
   asm	push	es;

   asm	cmp	in_vga_system, 1
   asm	je	vga_out
   asm	jmp     hgc_out

   vga_out:

   asm	mov	ax, video_seg
   asm	mov	es, ax

   asm  mov     dx, 03ceh
   asm	mov	ax, color
   asm	xchg    ah, al
   asm	out	dx, ax
   asm	mov	ax, 0f01h
   asm	out	dx, ax

   asm	mov	cl, 4
   asm  mov     ax, y
   asm	shl	ax, cl
   asm	mov	bx, ax
   asm	shl	ax, 1
   asm	shl 	ax, 1
   asm	add	ax, bx
   asm	mov	bx, x
   asm	mov	cl, 3
   asm	shr	bx, cl
   asm	add	ax, bx
   asm	mov	di, ax

   asm	mov	cx, width
   asm	mov	ax, x
   asm	and	ax, 7
   asm	jz	full_bytes_vga
   asm	mov	bx, 0ffh
   asm	push	cx
   asm	mov	cx, ax
   asm	shr	bx, cl
   asm	pop	cx
   asm	add	cx, ax
   asm	sub	cx, 08h
   asm	jge	set_mask_vga
   asm	neg	cx
   asm	shr	bx, cl
   asm	shl	bx, cl
   asm	xor	cx, cx

   set_mask_vga:

   asm	mov	al, 8
   asm  mov	ah, bl
   asm	out	dx, ax
   asm	mov	al, es:[di]
   asm	mov	es:[di], al
   asm	inc	di

   full_bytes_vga:

   asm	mov	bx, cx
   asm	cmp	cx, 8
   asm	jl	trail_bytes_vga
   asm	shr	cx, 1
   asm	shr	cx, 1
   asm	shr	cx, 1
   asm	mov	ax, 0ff08h
   asm	out	dx, ax

   full_bytes_vga_loop:

   asm	mov	al, es:[di]
   asm	mov	es:[di], al
   asm	inc	di
   asm	loop	full_bytes_vga_loop

   trail_bytes_vga:

   asm	and	bx, 7
   asm	jz      end_bytes_vga
   asm	mov	cl, bl
   asm	mov	ah, 0ffh
   asm	shr	ah, cl
   asm	xor	ah, 0ffh
   asm	mov     al, 8
   asm	out	dx, ax
   asm	mov	al, es:[di]
   asm	mov	es:[di], al

   end_bytes_vga:

   asm	mov	dx, 03ceh
   asm	mov	ax, 0ff08h
   asm	out	dx, ax
   asm	mov	ax, 0001h
   asm	out	dx, ax

   asm  jmp     quit_draw_horiz_line

   hgc_out:

   asm	mov	ax, video_seg
   asm	mov	es, ax

   asm  mov     bx, y
   asm	mov	cx, bx
   asm	and	bx, 3
   asm	shr	cx, 1
   asm	shr	cx, 1
   asm	mov	al, 20h
   asm	mul	bl
   asm	xchg	ah, al
   asm	mov	di, ax
   asm	mov	ax, abs_maxx_han
   asm	inc	ax
   asm	mul	cl
   asm	add	di, ax
   asm  mov	ax, x
   asm	mov	cl, 3
   asm	shr	ax, cl
   asm	add	di, ax

   asm	mov	cx, width
   asm	mov	ax, x
   asm	and	ax, 7
   asm	jz	full_bytes_hgc
   asm	mov	bx, 0ffh
   asm	push	cx
   asm	mov	cx, ax
   asm	shr	bx, cl
   asm	pop	cx
   asm	add	cx, ax
   asm	sub	cx, 8
   asm	jge	set_mask_hgc
   asm	neg	cx
   asm	shr	bx, cl
   asm	shl	bx, cl
   asm	xor	cx, cx

   set_mask_hgc:

   asm	mov	al, es:[di]
   asm	or	al, bl
   asm	mov	es:[di], al
   asm	inc	di

   full_bytes_hgc:

   asm	mov	bx, cx
   asm	cmp	cx, 8
   asm	jl	trail_bytes_hgc
   asm	shr	cx, 1
   asm	shr	cx, 1
   asm	shr	cx, 1

   asm	mov	al, 0ffh
   asm	rep	stosb

   trail_bytes_hgc:

   asm	and	bx, 7
   asm	jz      quit_draw_horiz_line
   asm	mov	cl, bl
   asm	mov	bl, 0ffh
   asm	shr	bl, cl
   asm	xor	bl, 0ffh
   asm	mov	al, es:[di]
   asm	or	al, bl
   asm	mov	es:[di], al

   quit_draw_horiz_line:

   asm	pop	es
}
