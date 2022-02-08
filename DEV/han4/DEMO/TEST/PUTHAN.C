/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          put_han_font()                                      */
/*   FILENAME           puthan.c                                            */
/*   DATE & TIME        03/11/92(WED) 12:00                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void get_2byte_font(unsigned char code1, unsigned char code2);

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void put_han_font(int x, int y, unsigned char code1, unsigned char code2)
{
   unsigned char color, bkcolor;

   asm  push    es

   asm  mov     al, color_han           /* save static variable in stack */
   asm  mov     color, al
   asm  mov     al, bkcolor_han
   asm  mov     bkcolor, al

   /* update screen buffer                   */
   /* scr_buff[y][x][0]   = code1;           */
   /* scr_buff[y][x][1]   = HAN1;            */
   /* scr_buff[y][x][2]   = attr_han;        */
   /* scr_buff[y][x][3]   = color;           */
   /* scr_buff[y][x+1][0] = code2;           */
   /* scr_buff[y][x+1][1] = HAN2;            */
   /* scr_buff[y][x+1][2] = attr_han;        */
   /* scr_buff[y][x+1][3] = color;           */
   asm  les     di, dword ptr scr_buff
   asm  mov     ax, y
   asm  mov     bx, LIMITX_HAN
   asm  mul     bx
   asm  add     ax, x
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     di, ax

   asm  mov     cx, 4                   /* BH = color info */
   asm  mov     bh, bkcolor
   asm  shl     bh, cl
   asm  or      bh, color
   asm  mov     bl, attr_han            /* BL = attr info */

   asm  mov     al, code1
   asm  mov     ah, HAN1
   asm  stosw
   asm  mov     ax, bx
   asm  stosw
   asm  mov     al, code2
   asm  mov     ah, HAN2
   asm  stosw
   asm  mov     ax, bx
   asm  stosw

   get_2byte_font(code1, code2);

   asm  mov     al, attr_han
   asm  test    al, 11000000b
   asm  jz      modify_han                 /* if not necessary to zoom, jump */

   zoomed_out_16(x, y, or_font, attr_han);
   asm  jmp     quit_put_han

   modify_han:

   modify_image(or_font, or_font, 32, attr_han);

   set_register:

   /* at this time DS is recovered to original value */

   asm  mov     es, video_seg
   asm  mov     si, offset or_font

   asm  cmp     in_vga_system, 1
   asm  je      vga_out

   /* display hangul in HGC */

   asm  mov     di, x
   asm  mov     ax, abs_maxx_han
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     bx, y
   asm  mul     bx
   asm  add     di, ax

   asm  mov     cx, output_offset_han
   asm  cmp     cx, 4
   asm  jb      continue1
   asm  shr     cx, 1
   asm  shr     cx, 1
   asm  mov     ax, abs_maxx_han
   asm  inc     ax
   asm  mul     cx
   asm  add     di, ax

   continue1:

   asm  mov     cx, 4

   /* draw the font in HGC */

   draw_font_hgc:

   asm  push    di
   asm	push	ax
   asm  mov	ax, word ptr [si]
   asm	or	word ptr es:[di], ax
   asm	pop	ax
   asm	inc	si
   asm  add     di, 0x2000
   asm	push	ax
   asm  mov	ax, word ptr [si]
   asm	or	word ptr es:[di], ax
   asm	pop	ax
   asm	inc	si
   asm  add     di, 0x2000
   asm	push	ax
   asm  mov	ax, word ptr [si]
   asm	or	word ptr es:[di], ax
   asm	pop	ax
   asm	inc	si
   asm  add     di, 0x2000
   asm	push	ax
   asm  mov	ax, word ptr [si]
   asm	or	word ptr es:[di], ax
   asm	pop	ax
   asm	inc	si
   asm  pop     di
   asm  add     di, abs_maxx_han
   asm  inc     di
   asm  loop    draw_font_hgc
   asm  jmp     quit_put_han

   vga_out:

   /* display hangul in EGA/VGA */

   asm  mov     ax, VGA_SCR_WIDTH       /* DI = destination offset */
   asm  mov     cx, y
   asm  mul     cx
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     di, ax
   asm  add     di, x

   asm  mov     cx, output_offset_han
   asm  cmp     cx, 0
   asm  je      continue2
   asm  mov     ax, VGA_SCR_WIDTH
   asm  mul     cx
   asm  add     di, ax

   continue2:

   asm  mov     cx, 16                  /* CX = 16 line per char in EGA/VGA */
   asm  mov     bx, VGA_SCR_WIDTH       /* BX = increment to adjust DI */
   asm  dec     bx

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */

   asm  xor     ax, ax                  /* set/reset = bkcolor_han */
   asm  mov     ah, bkcolor
   asm  out     dx, ax
   asm  mov     ax, 0x0f01              /* enable set/reset = 00001111b */
   asm  out     dx, ax
   asm  mov     byte ptr es:[di], al    /* erase one byte with bkcolor_han */
   asm  mov     al, byte ptr es:[di]
   asm  xor     ax, ax                  /* set/reset = color_han */
   asm  mov     ah, color
   asm  out     dx, ax
   asm  mov     al, 0x08                /* bit mask */

   asm  xor     ah, ah			/* only for ET-4000 */
   asm	out	dx, ax			/* draw font 0x00   */
   asm  mov	byte ptr es:[di], al

   /* draw the font in EGA/VGA */

   draw_line_vga:

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  mov     byte ptr es:[di], al
   asm  inc     si
   asm  inc     di
   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  mov     byte ptr es:[di], al
   asm  inc     si
   asm  add     di, bx
   asm  loop    draw_line_vga

   asm  xor     ax, ax                  /* set/reset = 0x00 */
   asm  out     dx, ax
   asm  inc     ax                      /* enable = 0x00 */
   asm  out     dx, ax
   asm  mov     ax, 0xff08              /* bit mask = 0xff */
   asm  out     dx, ax

   quit_put_han:

   asm  pop     es
}
