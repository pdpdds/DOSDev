/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          draw_vert_line()                                    */
/*   FILENAME           vertline.c                                          */
/*   DATE & TIME        05/13/92(WED) 21:20                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_vert_line(int x, int y, int height, int color)
/* draw line vertically */
{
   asm	push	es

   asm  cmp     in_vga_system, 1
   asm  je      vga_out

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
   asm  inc     ax
   asm	mul	cl
   asm	add	di, ax
   asm  mov	ax, x
   asm	mov	cl, 3
   asm	shr	ax, cl
   asm	add	di, ax

   asm	mov	cx, x
   asm	and	cl, 7
   asm	mov	bl, 80h
   asm	shr	bl, cl

   asm	mov	dx, height
   asm  inc     dx
   asm  mov     si, 8000h
   asm  sub     si, abs_maxx_han
   asm  dec     si

   draw_vert_line_hgc:

   asm  dec     dx
   asm  jz      quit_draw_vert_line
   asm	mov	al, es:[di]
   asm	or	al, bl
   asm	mov	es:[di], al
   asm	add	di, 2000h
   asm	jns	draw_vert_line_hgc
   asm	sub	di, si
   asm	jmp	draw_vert_line_hgc

   vga_out:

   asm	mov	ax, video_seg
   asm	mov	es, ax

   asm  mov     dx, 03ceh
   asm	mov  	ax, color
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

   asm	mov	cx, x
   asm	and	cl, 7
   asm	mov	bx, 80h
   asm	shr	bx, cl
   asm	mov	ah, bl
   asm	mov	al, 8
   asm	out	dx, ax

   asm	mov	bx, 80
   asm	mov	cx, height
   asm  inc     cx

   draw_vert_line_vga:

   asm  dec     cx
   asm  jz      quit_vga_loop
   asm	mov	al, es:[di]
   asm	mov	es:[di], al
   asm	add	di, bx
   asm	jmp     draw_vert_line_vga

   quit_vga_loop:

   asm	mov	dx, 03ceh
   asm	mov	ax, 0ff08h
   asm	out	dx, ax
   asm	mov	ax, 0001h
   asm	out	dx, ax

   quit_draw_vert_line:

   asm	pop	es
}
