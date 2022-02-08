/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          xscrolldn_han()                                     */
/*   FILENAME           xscroldp.c                                          */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void xscrolldn_han(int x1, int y1, int x2, int y2)
/* scrolls down one line */
{
   unsigned scr_width;                  /* equivalent to one register */

   asm  push    es

   /* scroll down screen buffer */

   asm  les     di, dword ptr scr_buff  /* DS = destination segment */
   asm  mov     ax, y2                  /* DI = destination offset  */
   asm  mov     bx, LIMITX_HAN
   asm  mul     bx
   asm  add     ax, x1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     di, ax

   asm  mov     si, di                  /* SI = source offset */
   asm  mov     ax, LIMITX_HAN
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  sub     si, ax

   asm  mov     cx, x2                  /* CX = # of bytes to be moved */
   asm  sub     cx, x1                  /* in one line                 */
   asm  inc     cx  
   asm  shl     cx, 1
   asm  shl     cx, 1
   asm  mov     dx, cx                  /* backup CX to DX */

   asm  mov     bx, LIMITX_HAN          /* BX = skip bytes */   
   asm  shl     bx, 1
   asm  shl     bx, 1

   asm  push    ds                      /** push DS */

   asm  mov     ax, es                  /* set DS with ES */
   asm  mov     ds, ax
   asm  mov     ax, y2                  /* AX = # of line to be moved */
   asm  sub     ax, y1

   scroll_scr_buff:

   asm  cmp     ax, 0
   asm  jbe     exit_scroll_scr_buff
   asm  push    si
   asm  push    di
   asm  rep     movsb
   asm  mov     cx, dx
   asm  pop     di
   asm  pop     si
   asm  sub     si, bx
   asm  sub     di, bx
   asm  dec     ax
   asm  jmp     scroll_scr_buff

   exit_scroll_scr_buff:

   /* erase the first line of the screen buffer */

   asm  pop     ds                      /* pop DS */

   asm  mov     cx, 4                   /* BH = color info */
   asm  mov     bh, bkcolor_han
   asm  shl     bh, cl
   asm  or      bh, color_han
   asm  mov     bl, FONT_NORMAL         /* BL = attr info */

   asm  mov     cx, x2                  /* CX = # of bytes to be cleared */
   asm  sub     cx, x1                  /* in one line                   */   
   asm  inc     cx

   asm  mov     ah, ENG
   asm  mov     al, ' '

   erase_first_scr_buff:

   asm  cmp     cx, 0
   asm  jbe     exit_erase_first_scr_buff
   asm  stosw
   asm  push    ax
   asm  mov     ax, bx
   asm  stosw
   asm  dec     cx
   asm  pop     ax
   asm  jmp     erase_first_scr_buff

   exit_erase_first_scr_buff:

   /* scroll down visual screen */

   asm  cmp     in_vga_system, 1
   asm  jne     hgc_scroll
   asm  jmp     vga_scroll

   hgc_scroll:

   /* scroll down in HGC */

   asm  mov     di, x1                  /* DI = destination offset   */
   asm  mov     ax, y2
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  inc     ax
   asm  inc     ax
   asm  inc     ax                      /* for reverse scrolling */
   asm  mov     dx, abs_maxx_han        
   asm  inc     dx
   asm  mov     scr_width, dx           /* <scr_width> = hangul screen width */
   asm  mul     dl
   asm  add     di, ax

   asm  mov     ax, abs_maxx_han        /* SI = source offset */
   asm  inc     ax
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     si, di
   asm  sub     si, ax

   asm  mov     ax, y2                  /* AX = screen height to be scrolled */
   asm  sub     ax, y1
   asm  shl     ax, 1                   /* to justify 4 times scanning by */
   asm  shl     ax, 1
   asm  mov     cx, x2                  /* CX = screen width to be scrolled */
   asm  sub     cx, x1
   asm  inc     cx
   asm  mov     dx, cx                  /* backup CX to DX */

   asm  push    ds                      /** push DS **/
        
   asm  mov     es, video_seg           /* ES = segment of VRAM */
   asm  mov     ds, video_seg           /* DS = segment of VRAM */

   asm  mov     bx, 0x2000              /* BX = decrement to justify SI & DI */
   asm  sub     bx, cx                  /* when MOVSB, STOSB occurred         */

   scroll_line_hgc:

   asm  cmp     ax, 0
   asm  jbe     exit_scroll_line_hgc

   asm  push    si
   asm  push    di

   asm  rep     movsb                   /* we don't have to push & pop CX */
   asm  mov     cx, dx                  /* so much faster                 */

   asm  add     di, bx
   asm  add     si, bx
   asm  rep     movsb
   asm  mov     cx, dx

   asm  add     di, bx
   asm  add     si, bx
   asm  rep     movsb
   asm  mov     cx, dx

   asm  add     di, bx
   asm  add     si, bx
   asm  rep     movsb
   asm  mov     cx, dx

   asm  pop     di
   asm  pop     si
   asm  sub     si, scr_width
   asm  sub     di, scr_width
   asm  dec     ax
   asm  jmp     scroll_line_hgc

   exit_scroll_line_hgc:

   asm  pop     ds                      /** pop DS **/

   /* erase the first text line in HGC */

   asm  xor     ax, ax                  /* AX = fill char */
   asm  xor     si, si                  /* SI = loop counter */

   erase_line_hgc:

   asm  push    di
   asm  rep     stosb
   asm  mov     cx, dx

   asm  add     di, bx
   asm  rep     stosb
   asm  mov     cx, dx

   asm  add     di, bx
   asm  rep     stosb
   asm  mov     cx, dx

   asm  add     di, bx
   asm  rep     stosb
   asm  mov     cx, dx

   asm  pop     di
   asm  sub     di, scr_width
   asm  inc     si
   asm  cmp     si, 4

   asm  jb      erase_line_hgc
   asm  jmp     quit_xscrolldn_han

   vga_scroll:
   /* scroll down in EGA/VGA */

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */
   asm  mov     ax, 0x0105              /* write mode 1 */
   asm  out     dx, ax

   asm  mov     ax, y2                  /* DI = destination offset */
   asm  shl     ax, 1                   /*    = (y2*16+15)*80      */
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     ax, 15
   asm  mov     cx, VGA_SCR_WIDTH
   asm  mul     cx
   asm  add     ax, x1
   asm  mov     di, ax

   asm  shl     cx, 1                   /* SI = source offset */
   asm  shl     cx, 1                   /*    = DI - 80*16    */
   asm  shl     cx, 1
   asm  shl     cx, 1
   asm  sub     ax, cx
   asm  mov     si, ax

   asm  mov     ax, y2                  /* AX = screen height to be scrolled */
   asm  sub     ax, y1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  mov     cx, x2                  /* CX = screen width to be scrolled */
   asm  sub     cx, x1
   asm  inc     cx

   asm  push    ds                      /** push DS **/

   asm  mov     es, video_seg           /* ES = segment of VRAM */
   asm  mov     ds, video_seg           /* DS = segment of VRAM */

   asm  mov     bx, VGA_SCR_WIDTH       /* BX = decrement to justify SI & DI */
                                        /* when MOVSB, STOSB occurred        */
   scroll_line_vga:

   asm  cmp     ax, 0
   asm  jbe     exit_scroll_line_vga

   asm  push    si
   asm  push    di
   asm  push    cx
   asm  rep     movsb
   asm  pop     cx
   asm  pop     di
   asm  pop     si

   asm  sub     si, bx
   asm  sub     di, bx
   asm  dec     ax
   asm  jmp     scroll_line_vga

   exit_scroll_line_vga:

   asm  pop     ds                      /** pop DS **/

   /* erase the first text line in EGA/VGA */

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */
   asm  mov     ax, 0x0205              /* write mode 2 */
   asm  out     dx, ax

   /* erase the last text line in EGA/VGA */

   asm  mov     al, bkcolor_han         /* AL = bkcolor_han */
   asm  xor     si, si                  /* SI = loop counter */

   erase_line_vga:

   asm  push    di
   asm  push    cx
   asm  rep     stosb
   asm  pop     cx
   asm  pop     di

   asm  sub     di, bx
   asm  inc     si
   asm  cmp     si, 16
   asm  jb      erase_line_vga

   asm  mov     ax, 0x0005              /* restore write mode to 0 */
   asm  out     dx, ax

   quit_xscrolldn_han:

   asm  pop     es
}
