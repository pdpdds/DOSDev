/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          imagesize_han()                                     */
/*   FILENAME           imagesiz.c                                          */
/*   DATE & TIME        12/18/92(FRI) 14:15                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

unsigned imagesize_han(int left, int top, int right, int bottom)
/* returns memory size needed to save an image */
{
   asm  cmp	in_vga_system, 1
   asm  je	vga_imagesize

   /* size = (right - left + 1) * (bottom - top + 1) * 16 + 4 */
   asm	mov 	ax, right
   asm	sub	ax, left
   asm	inc	ax
   asm	mov	bx, bottom
   asm	sub	bx, top
   asm	inc	bx
   asm  mul	bx
   asm	add	ax, 4
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	jmp	quit_imagesize

   vga_imagesize:

   /* size = (right - left + 1) * (bottom - top + 1) * 16 * 4 + 4 */
   asm	mov 	ax, right
   asm	sub	ax, left
   asm	inc	ax
   asm	mov	bx, bottom
   asm	sub	bx, top
   asm	inc	bx
   asm  mul	bx
   asm  mov     cl, 0x06
   asm	shl	ax, cl
   asm	add	ax, 4

   quit_imagesize:;
}
