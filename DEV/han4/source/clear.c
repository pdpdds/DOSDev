/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          clear_text_han()                                    */
/*   FILENAME           clear.c                                             */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void clear_text_han(int x1, int y1, int x2, int y2)
/* clears (left, top) - (right, bottom) area */
{
   unsigned scr_width;

   asm  push    es

   /* clear screen buffer */

   asm  les     di, dword ptr scr_buff  /* ES:DI = destination */
   asm  mov     ax, y1
   asm  mov     bx, LIMITX_HAN
   asm  mul     bx
   asm  add     ax, x1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     di, ax

   asm  mov     si, y2                  /* SI = loop counter */
   asm  sub     si, y1
   asm  inc     si

   asm  mov     ah, ENG                 /* AH = ENG */
   asm  mov     al, ' '                 /* AL = ' ' */
   asm  mov     cx, 4                   /* BH = color info */
   asm  mov     bh, bkcolor_han
   asm  shl     bh, cl
   asm  or      bh, color_han
   asm  mov     bl, FONT_NORMAL         /* BL = attr info */

   asm  mov     cx, x2                  /* CX = screen width */
   asm  sub     cx, x1
   asm  inc     cx

   asm  mov     dx, LIMITX_HAN          /* DX = skip byte */
   asm  sub     dx, cx
   asm  shl     dx, 1
   asm  shl     dx, 1

   clear_scr_buff:

   asm  cmp     si, 0
   asm  jbe     exit_clear_scr_buff
   asm  push    cx

   clear_element:
   asm  cmp     cx, 0
   asm  jbe     exit_clear_element
   asm  stosw
   asm  push    ax
   asm  mov     ax, bx
   asm  stosw
   asm  pop     ax
   asm  dec     cx
   asm  jmp     clear_element

   exit_clear_element:

   asm  pop     cx
   asm  add     di, dx
   asm  dec     si
   asm  jmp     clear_scr_buff

   exit_clear_scr_buff:

   asm  cmp     in_vga_system, 1
   asm  jne     hgc_clear
   asm  jmp     vga_clear

   hgc_clear:

   /* clear screen in HGC */

   asm  mov     di, x1                  /* DI = destination offset   */
   asm  mov     ax, y1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     dx, abs_maxx_han        /* DX = hangul screen width  */
   asm  inc     dx
   asm  mul     dl                      /* not to touch DX */
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

   asm  mov     si, y2                  /* SI = clearing screen height */
   asm  sub     si, y1
   asm  inc     si
   asm  shl     si, 1                   /* to justify 4 times scanning by */
   asm  shl     si, 1
   asm  mov     cx, x2                  /* CX = clearing screen width */
   asm  sub     cx, x1
   asm  inc     cx
   asm  mov     scr_width, cx           /* backup CX to <scr_width> */

   asm  mov     es, video_seg           /* ES = segment of VRAM */

   asm  mov     bx, 0x2000              /* BX = increment to justify SI & DI */
   asm  sub     bx, cx                  /* when MOVSB,STOSB occurred         */

   asm  xor     ax, ax                  /* AX = fill char */

   clear_line_hgc:

   asm  cmp     si, 0
   asm  jbe     quit_clear_text_han_link
   asm  push    di

   asm  rep     stosb
   asm  mov     cx, scr_width

   asm  add     di, bx
   asm  rep     stosb
   asm  mov     cx, scr_width

   asm  add     di, bx
   asm  rep     stosb
   asm  mov     cx, scr_width

   asm  add     di, bx
   asm  rep     stosb
   asm  mov     cx, scr_width

   asm  pop     di
   asm  add     di, dx
   asm  dec     si
   asm  jmp     clear_line_hgc

   quit_clear_text_han_link:

   asm  jmp     quit_clear_text_han

   vga_clear:

   /* clear screen in EGA/VGA */

   asm  mov     ax, VGA_SCR_WIDTH       /* DI = destination offset   */
   asm  mov     cx, y1
   asm  mul     cx
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     ax, x1
   asm  mov     di, ax

   asm  mov     cx, output_offset_han
   asm  cmp     cx, 0
   asm  je      continue2
   asm  mov     ax, VGA_SCR_WIDTH
   asm  mul     cx
   asm  add     di, ax

   continue2:

   asm  mov     ax, y2                  /* SI = screen height to be cleared */
   asm  sub     ax, y1
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     si, ax
   asm  mov     cx, x2                  /* CX = clearing screen width */
   asm  sub     cx, x1
   asm  inc     cx

   asm  mov     es, video_seg           /* ES = segment of VRAM */

   asm  mov     bx, VGA_SCR_WIDTH       /* BX = increment to justify SI & DI */
   asm  sub     bx, cx                  /* when MOVSB, STOSB occurred        */

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */
   asm  mov     ax, 0x0205              /* write mode 2 */
   asm  out     dx, ax

   /* erase the last text line in EGA/VGA */

   asm  mov     al, bkcolor_han         /* AL = bkcolor_han */

   clear_line_vga:

   asm  cmp     si, 0
   asm  jbe     quit_clear_text_han_vga

   asm  push    cx
   asm  rep     stosb
   asm  pop     cx

   asm  add     di, bx
   asm  dec     si
   asm  jmp     clear_line_vga

   quit_clear_text_han_vga:

   asm  mov     ax, 0x0005              /* restore write mode to 0 */
   asm  out     dx, ax

   quit_clear_text_han:

   asm  pop     es
}
