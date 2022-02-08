/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          save_text_han(), restore_text_han()                 */
/*   FILENAME           savetext.c                                          */
/*   DATE & TIME        03/11/92(WED) 19:06                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdlib.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

char *save_text_han(int x1, int y1, int x2, int y2)
/* save text and return the pointer to the allocated memory */
{
   char *scr_data;
   unsigned size;

   asm  push    es

   asm  mov     ax, x2
   asm  sub     ax, x1
   asm  inc     ax
   asm  mov     bx, y2
   asm  sub     bx, y1
   asm  inc     bx
   asm  mul     bx
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  inc     ax
   asm  inc     ax                      /* AX = size of momory to be allocated */
   asm  mov     size, ax

   if((scr_data = (char *) malloc(size)) == NULL) return(NULL);

   asm  les     di, dword ptr scr_data  /* ES:DI = destination */
   asm  mov     ax, x2                  /* size of x */
   asm  sub     ax, x1
   asm  stosb
   asm  mov     ax, y2                  /* size of y */
   asm  sub     ax, y1
   asm  stosb

   asm  push    ds                      /** push DS **/

   asm  lds     si, dword ptr scr_buff  /* DS:SI = source */
   asm  mov     ax, y1
   asm  mov     bx, LIMITX_HAN
   asm  mul     bx
   asm  add     ax, x1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     si, ax

   asm  mov     cx, x2                  /* CX = # of moving byte */
   asm  sub     cx, x1
   asm  inc     cx
   asm  shl     cx, 1
   asm  shl     cx, 1
   asm  mov     dx, cx                  /* backup CX to DX */

   asm  mov     bx, LIMITX_HAN          /* BX = increment to adjust SI */
   asm  shl     bx, 1
   asm  shl     bx, 1
   asm  sub     bx, cx

   asm  mov     ax, y2                  /* AX = screen height */
   asm  sub     ax, y1
   asm  inc     ax

   /* save the contents of the screen buffer to <scr_data> */

   loop1:

   asm  cmp     ax, 0
   asm  jbe     exit_loop1
   asm  rep     movsb
   asm  mov     cx, dx
   asm  add     si, bx
   asm  dec     ax
   asm  jmp     loop1

   exit_loop1:

   asm  pop     ds                      /** pop DS **/

   asm  pop     es

   return(scr_data);
}

void restore_text_han(int x1, int y1, char *scr_data)
/* recover screen and free the allocated memory */
{
   unsigned x, y, column, row;
   unsigned char attr2, color2, bkcolor2;

   asm  push    es

   asm  les     di, dword ptr scr_buff  /* ES:DI = destination */
   asm  mov     ax, y1
   asm  mov     bx, LIMITX_HAN
   asm  mul     bx
   asm  add     ax, x1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     di, ax

   asm  push    ds                      /** push DS **/

   asm  lds     si, dword ptr scr_data  /* DS:SI = source */

   asm  xor     ax, ax
   asm  lodsb                                    
   asm  mov     column, ax
   asm  inc     ax
   asm  mov     cx, ax
   asm  shl     cx, 1
   asm  shl     cx, 1                   /* CX = # of moving bytes */
   asm  mov     dx, cx                  /* backup CX to DX */

   asm  lodsb                           /* AX = # of y */
   asm  mov     row, ax
   asm  inc     ax

   asm  mov     bx, LIMITX_HAN          /* increment to adjust DI */
   asm  shl     bx, 1
   asm  shl     bx, 1
   asm  sub     bx, cx

   loop1:

   asm  cmp     ax, 0
   asm  jbe     exit_loop1
   asm  rep     movsb
   asm  mov     cx, dx
   asm  add     di, bx
   asm  dec     ax
   asm  jmp     loop1

   exit_loop1:

   asm  pop     ds                      /** pop DS **/

   asm  pop     es

   free(scr_data);

   attr2    = attr_han;
   color2   = color_han;
   bkcolor2 = bkcolor_han;

   for(y = y1; y <= y1 + row; y++)
      for(x = x1; x <= x1 + column; x++)
      {
         attr_han    = scr_buff[y][x][2];
	 color_han   = scr_buff[y][x][3] & 0x0f;
         bkcolor_han = (scr_buff[y][x][3] >> 4) & 0x0f;
         if(scr_buff[y][x][1] == HAN1)
         {
            put_han_font(x, y, scr_buff[y][x][0], scr_buff[y][x+1][0]);
            x++;
         }
         else if(scr_buff[y][x][1] == HAN2)
	    put_han_font(x - 1, y, scr_buff[y][x-1][0], scr_buff[y][x][0]);
         else
            put_eng_font(x,     y, scr_buff[y][x][0]);
      }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}
