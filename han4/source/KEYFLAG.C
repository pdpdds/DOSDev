/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_keyflag_han(), is_keyflag_han(),                */
/*                      set_keyflag_han(), toggle_keyflag_han()             */
/*   FILENAME           keyflag.c                                           */
/*   DATE & TIME        03/11/92(WED) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "misc.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

unsigned char get_keyflag_han(void)
/* returns the value of key flag byte */
{
   asm	push	ds

   asm  xor     ax, ax
   asm	mov     ds, ax
   asm	mov     si, 0417h
   asm  lodsb

   asm  pop	ds
   return _AX;
}

int is_keyflag_han(unsigned char flag)
/* returns 1 if flag is on */
{
   asm	push	ds

   asm  xor     ax, ax
   asm	mov     ds, ax
   asm	mov     si, 0417h
   asm  lodsb
   asm  and	al, flag
   asm  cmp	al, flag
   asm	jne	not_equal
   asm	mov	ax, 1
   asm	jmp	quit

   not_equal:
   asm  xor	ax, ax

   quit:
   asm  pop	ds
   return _AX;
}

void set_keyflag_han(unsigned char flag)
/* sets key flag to <flag> */
{
   asm	push	es

   asm	cli
   asm  xor     ax, ax
   asm	mov     es, ax
   asm	mov     di, 0417h
   asm  mov     al, flag
   asm  stosb
   asm  mov     ah, 01h
   asm  int     16h
   asm	sti

   asm  pop	es
}

void toggle_keyflag_han(unsigned char flag)
/* toggle some bits of key flag byte */
{
   asm	push	es

   asm	cli
   asm  xor     ax, ax
   asm	mov     es, ax
   asm	mov     di, 0417h
   asm	mov	al, es:[di]
   asm  xor     al, flag
   asm  stosb
   asm  mov     ah, 01h
   asm  int     16h
   asm	sti

   asm  pop	es
}
