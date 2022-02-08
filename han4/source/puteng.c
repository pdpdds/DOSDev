/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          put_eng_font()                                      */
/*   FILENAME           puteng.c                                            */
/*   DATE & TIME        03/11/92(WED) 12:00                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void put_eng_font(int x, int y, unsigned char code)
{
   unsigned char color, bkcolor;

   asm  push    es

   asm  mov     al, color_han           /* save static variable in stack */
   asm  mov     color, al
   asm  mov     al, bkcolor_han
   asm  mov     bkcolor, al

   /* update screen buffer              */
   /* scr_buff[y][x][0] = code;         */
   /* scr_buff[y][x][1] = ENG;          */
   /* scr_buff[y][x][2] = attr_han;     */
   /* scr_buff[y][x][2] = color_han;    */
   asm  les     di, dword ptr scr_buff
   asm  mov     ax, y
   asm  mov     bx, LIMITX_HAN
   asm  mul     bx
   asm  add     ax, x
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     di, ax

   asm  mov     al, code
   asm  mov     ah, ENG
   asm  stosw
   asm  mov     cx, 4                   /* AH = color info */
   asm  mov     ah, bkcolor
   asm  shl     ah, cl
   asm  or      ah, color
   asm  mov     al, attr_han            /* AL = attr info */
   asm  stosw

   asm  cmp     al, 0
   asm  jne     chk_zoom                /* if attr != FONT_NORMAL, jmp chk_zoom */

   asm  les     si, dword ptr eng_font
   asm  xor     ah, ah
   asm  mov     al, code
   asm  mov     cl, 4
   asm  shl     ax, cl
   asm  add     si, ax
   asm  jmp     set_register

   chk_zoom:

   asm  test    al, 11000000b
   asm  jz      modify                  /* if not necessary to zoom, jmp modify */

   zoomed_out_8(x, y, eng_font[code], attr_han);

   asm  jmp     quit_put_eng

   modify:

   modify_image(or_font, eng_font[code], 16, attr_han);

   asm  mov     ax, ds
   asm  mov     es, ax
   asm  mov     si, offset or_font

   set_register:
   /* at this point, ES contains segment of font and SI contains offset */

   asm  mov     bx, ds                  /* save DS to BX */
   asm  cmp     in_vga_system, 1
   asm  je      vga_out

   /* Hercules output routine */
   asm  mov     di, x
   asm  mov     ax, abs_maxx_han
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     cx, y
   asm  mul     cx
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

   asm  mov     ax, abs_maxx_han
   asm  mov     dx, es
   asm  mov     es, video_seg
   asm  mov     ds, dx
   asm  mov     cx, 4

   /* draw the font in HGC */

   draw_font_hgc:

   asm  push    di
   asm  movsb
   asm  add     di, 0x1fff
   asm  movsb
   asm  add     di, 0x1fff
   asm  movsb
   asm  add     di, 0x1fff
   asm  movsb
   asm  pop     di
   asm  add     di, ax
   asm  inc     di
   asm  loop    draw_font_hgc

   asm  mov     ds, bx                  /* restore DS with BX */
   asm  jmp     quit_put_eng

   vga_out:

   /* EGA/VGA output routine */

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

   asm  mov     ax, es
   asm  mov     es, video_seg           /* ES = segment of VRAM */
   asm  push    ds                      /** push DS **/
   asm  mov     ds, ax                  /* DS = source segment  */

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
   asm  mov     byte ptr es:[di], al    /* draw the font */

   asm  inc     si
   asm  add     di, bx
   asm  loop    draw_line_vga

   asm  xor     ax, ax                  /* set/reset = 0x00 */
   asm  out     dx, ax
   asm  inc     ax                      /* enable = 0x00 */
   asm  out     dx, ax
   asm  mov     ax, 0xff08              /* bit mask = 0xff */
   asm  out     dx, ax

   asm  pop     ds                      /** pop DS **/

   quit_put_eng:

   asm  pop     es
}
