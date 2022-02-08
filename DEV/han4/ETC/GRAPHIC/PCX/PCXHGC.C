#include <stdio.h>
#include <alloc.h>
#include "graph.h"
#include "pcx.h"

void show_line_hgc(unsigned offset, char far *bit_map, int bytes);
void decode_line_pcx(char far *dest, int bytes, FILE *stream);

int show_pcx_hgc(int x, int y, pcx_header_struct *header, FILE *stream)
{
   char *bit_map;
   unsigned offset, cur_line;
   int i;

   if((bit_map = malloc(header->bpl)) == NULL) return(MEMORY_NOT_ENOUGH_PCX);

   /* offset = byte_per_line * 4 * y + x */
   asm	mov	ax, byte_per_line
   asm	shl	ax, 1
   asm	shl	ax, 1
   asm	mov	dx, y
   asm	mul	dx
   asm	add	ax, x
   asm	mov	offset, ax

   /* cur_line = current displaying line # in graphical coordinates */
   asm	mov	ax, y
   asm	mov	cl, 4
   asm	shl	ax, cl
   asm	mov	cur_line, ax

   for(i = header->y1; i <= header->y2; i++)
   {
      if(++cur_line > height_gr) break;
      decode_line_pcx(bit_map, header->bpl, stream);
      show_line_hgc(offset, bit_map, header->bpl);
      offset += 0x2000;
      if(offset >= 0x8000) offset += (0x8000 + byte_per_line);
   }
   free(bit_map);
   return(OK_PCX);
}

void show_line_hgc(unsigned offset, char far *bit_map, int bytes)
{
   asm	push	ds

   asm	lds	si, bit_map

   asm	mov	ax, 0b000h
   asm	mov	es, ax
   asm	mov	di, offset
   asm	mov	cx, bytes
   asm	rep	movsb

   asm	pop	ds
}
