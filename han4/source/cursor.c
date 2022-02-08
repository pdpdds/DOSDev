/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          hid_cursor(), show_cursor(), draw_cursor_han()      */
/*   FILENAME           cursor.c                                            */
/*   DATE & TIME        03/11/92(WED) 18:13                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void hide_cursor_han(void)
/* prohibit getch?_han() from drawing the cursor */
{
   draw_cursor = 0;
}

void show_cursor_han(void)
/* let getch?_han() draw the cursor */
{
   draw_cursor = 1;
}

void draw_cursor_han(int x, int y)
/* draw cursor on (x, y) */
{
   /* if cursor is disabled, just return */
   asm	cmp	draw_cursor, 0
   asm	jne 	continue_process
   return;            /* too long distance for relative jump so use "return" */

   continue_process:

   asm	push	es

   asm  xor     exist_cursor, 1         /* toggle <exist_cursor> */
   asm  mov     es, video_seg
   asm  mov     si, word ptr cursor_han

   asm  cmp     in_vga_system, 1
   asm  je      vga_out_link

   /* draw cursor in HGC */

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

   asm  mov     bx, abs_maxx_han
   asm  inc     bx

   asm  mov     cx, 4

   asm  lodsb                           /* store the x size of the cursor in AL */
   asm  cmp     al, 7
   asm  jne     draw_16_cursor

   loop1:

   asm  push    di
   asm  lodsb
   asm  xor     al, es:[di]
   asm  stosb
   asm  add     di, 0x1fff
   asm  lodsb
   asm  xor     al, es:[di]
   asm  stosb
   asm  add     di, 0x1fff
   asm  lodsb
   asm  xor     al, es:[di]
   asm  stosb
   asm  add     di, 0x1fff
   asm  lodsb
   asm  xor     al, es:[di]
   asm  stosb
   asm  pop     di
   asm  add     di, bx
   asm  loop    loop1
   asm  jmp     quit_draw_cursor

   vga_out_link:

   asm  jmp     vga_out

   draw_16_cursor: ;

   loop2:

   asm  push    di
   asm  lodsw
   asm  xor     ax, es:[di]
   asm  stosw
   asm  add     di, 0x1ffe
   asm  lodsw
   asm  xor     ax, es:[di]
   asm  stosw
   asm  add     di, 0x1ffe
   asm  lodsw
   asm  xor     ax, es:[di]
   asm  stosw
   asm  add     di, 0x1ffe
   asm  lodsw
   asm  xor     ax, es:[di]
   asm  stosw
   asm  pop     di
   asm  add     di, bx
   asm  loop    loop2
   asm  jmp     quit_draw_cursor

   vga_out:

   /* draw cursor in EGA/VGA */

   asm  mov     ax, VGA_SCR_WIDTH
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

   asm  mov     cx, 16
   asm  mov     bx, VGA_SCR_WIDTH

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */

   asm  mov     ax, 0x0205              /* write mode 2 */
   asm  out     dx, ax
   asm  mov     ax, 0x1803              /* shift/rotate = XOR */
   asm  out     dx, ax

   asm  lodsb                           /* store the x size of the cursor in AX */
   asm  cmp     al, 7
   asm  mov     al, 0x08                /* bit mask */
   asm  jne     draw_16_cursor_vga

   /* draw the font */

   draw_line_8:

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  mov     ah, byte ptr es:[di]
   asm  mov     byte ptr es:[di], 0xff
   asm  inc     si

   asm  add     di, bx
   asm  loop    draw_line_8
   asm  jmp     quit_draw_cursor_vga

   draw_16_cursor_vga:

   /* prepare for outputting the font in VGA */

   asm  dec     bx

   /* draw the font */

   draw_line_16:

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  mov     ah, byte ptr es:[di]
   asm  mov     byte ptr es:[di], 0xff
   asm  inc     si
   asm  inc     di

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  mov     ah, byte ptr es:[di]
   asm  mov     byte ptr es:[di], 0xff
   asm  inc     si

   asm  add     di, bx
   asm  loop    draw_line_16
   asm  jmp     quit_draw_cursor_vga

   quit_draw_cursor_vga:

   asm  mov     ax, 0x0005              /* restore write mode to 0 */
   asm  out     dx, ax
   asm  mov     ax, 0x0003              /* restore shift/rotate register */
   asm  out     dx, ax
   asm  mov     ax, 0xff08              /* bit mask = 0xff */
   asm  out     dx, ax

   quit_draw_cursor:

   asm  pop     es
}
