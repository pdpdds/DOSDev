#include <stdio.h>
#include <alloc.h>

#include "hanio.h"
#include "pcx.h"

void show_line_vga(unsigned offset, char *bit_map, int bytes);
void decode_line_pcx(char *dest, int bytes, FILE *stream);
void set_rgb_palette(int color_num, int red, int green, int blue);

int show_pcx_vga(int x, int y, pcx_header_struct *header, FILE *stream)
{
   char *bit_map;
   unsigned off_set, max_line, cur_line;
   int i;

   if((bit_map = malloc(header->bpl << 2)) == NULL) return(MEMORY_NOT_ENOUGH_PCX);

   /* offset = (abs_maxx_han + 1) * 16 * y + x */
   asm	mov	ax, abs_maxx_han
   asm	inc	ax
   asm	mov	cl, 4
   asm	shl	ax, cl
   asm	mov	dx, y
   asm	mul	dx
   asm	add	ax, x
   asm	mov	off_set, ax

   /* max_line = maximum displayable line # in graphical coordinates */
   asm	mov	ax, abs_maxy_han
   asm	inc	ax
   asm	mov	cl, 4
   asm	shl	ax, cl
   asm	mov	max_line, ax

   /* cur_line = current displaying line # in graphical coordinates */
   asm	mov	ax, y
   asm	mov	cl, 4
   asm	shl	ax, cl
   asm	mov	cur_line, ax

   /* set RGB palette */
   for(i = 0; i < 16; i++)
      set_rgb_palette(i, header->palette[i][0] >> 2,
			 header->palette[i][1] >> 2,
			 header->palette[i][2] >> 2);

   for(i = header->y1; i <= header->y2; i++)
   {
      if(++cur_line > max_line) break;
      decode_line_pcx(bit_map, header->bpl << 2, stream);
      show_line_vga(off_set, bit_map, header->bpl);
      off_set += 80;
   }
   free(bit_map);
   return(OK_PCX);
}

void show_line_vga(unsigned off_set, char *bit_map, int bytes)
{
   asm	push	ds

   asm	lds	si, bit_map

   asm	mov	ax, 0a000h
   asm	mov	es, ax
   asm	mov	di, off_set

   asm 	mov 	dx, 03c4h		/* DX = port 0x03c4 */
   asm	mov	al, 02h			/* Seqencer Map Mask register */
   asm	mov	cx, bytes

   asm	mov	ah, 01h			/* start with plan 0 */

   draw_line_vga:

   asm	out	dx, ax
   asm	push	cx
   asm	rep     movsb
   asm	pop	cx
   asm	sub	di, bytes
   asm	shl	ah, 1
   asm  cmp	ah, 10h
   asm	jb	draw_line_vga

   asm	mov	ax, 0x0f02
   asm	out	dx, ax

   asm	pop	ds
}
