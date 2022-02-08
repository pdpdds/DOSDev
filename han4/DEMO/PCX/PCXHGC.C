#include <stdio.h>
#include <alloc.h>
#include "hanio.h"
#include "pcx.h"

void show_line_hgc(unsigned offset, char *bit_map, int bytes);
void decode_line_pcx(char *dest, int bytes, FILE *stream);

int show_pcx_hgc(int x, int y, pcx_header_struct *header, FILE *stream)
{
   char *bit_map;
   unsigned off_set, max_line, cur_line;
   int i;

   if((bit_map = malloc(header->bpl)) == NULL) return(MEMORY_NOT_ENOUGH_PCX);

   /* offset = (abs_maxx_han + 1) * 4 * y + x */
   asm	mov	ax, abs_maxx_han
   asm	inc	ax
   asm	shl	ax, 1
   asm	shl	ax, 1
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

   for(i = header->y1; i <= header->y2; i++)
   {
      if(++cur_line > max_line) break;
      decode_line_pcx(bit_map, header->bpl, stream);
      show_line_hgc(off_set, bit_map, header->bpl);
      off_set += 0x2000;
      if(off_set >= 0x8000) off_set += (0x8000 + abs_maxx_han + 1);
   }
   free(bit_map);
   return(OK_PCX);
}

void show_line_hgc(unsigned off_set, char *bit_map, int bytes)
{
   asm	push	ds

   asm	lds	si, bit_map

   asm	mov	ax, 0b000h
   asm	mov	es, ax
   asm	mov	di, off_set
   asm	mov	cx, bytes
   asm	rep	movsb

   asm	pop	ds
}
