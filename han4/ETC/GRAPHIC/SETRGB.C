void set_rgb_palette(int color_num, int red, int green, int blue)
{
   static unsigned char pallette_num[] =
   {
      0, 1, 2, 3, 4, 5, 20, 7, 56, 57, 58, 59, 60, 61, 62, 63
   };

   /* select palette */
   asm	mov	si, offset pallette_num
   asm	add	si, color_num
   asm	lodsb
   asm	mov	dx, 0x03c8
   asm	out	dx, al

   /* set red */
   asm	inc	dx
   asm	mov	ax, red
   asm	out	dx, al

   /* set green */
   asm	mov	ax, green
   asm	out	dx, al

   /* set blue */
   asm	mov	ax, blue
   asm	out	dx, al
}
