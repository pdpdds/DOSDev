/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          replace_font(), set_cursor_han(), modify_image(),   */
/*                      putimage_8(), putimage_16(), zoomed_out_8(),        */
/*                      zoomed_out_16()                                     */
/*   FILENAME           hanbase.c                                           */
/*   DATE & TIME        03/11/92(WED) 19:17                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <alloc.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/****************************************************************************/
/*                    Implementations of the C Functions                    */
/****************************************************************************/

int replace_font(char *han_font_file, char *eng_font_file, char *spc_font_file)
/* reread font file to change current font */
{
   FILE *stream;

   if(strlen(han_font_file))    /* reread hangul font */
   {
      if((stream = fopen(han_font_file, "rb")) == NULL) return(HAN_FONT_NOT_FOUND);
      if(fread(han1_font, NO_BUL1*(NO_HAN1+1)*HAN_FONT_SIZE, 1, stream) != 1)
      {
	 fclose(stream);
	 return(HAN_FONT_READ_ERROR);
      }
      if(fread(han2_font, NO_BUL2*(NO_HAN2+1)*HAN_FONT_SIZE, 1, stream) != 1)
      {
	 fclose(stream);
	 return(HAN_FONT_READ_ERROR);
      }
      if(fread(han3_font, NO_BUL3*(NO_HAN3+1)*HAN_FONT_SIZE, 1, stream) != 1)
      {
	 fclose(stream);
	 return(HAN_FONT_READ_ERROR);
      }
      fclose(stream);
   }

   if(strlen(eng_font_file))    /* reread english font */
   {
      if((stream = fopen(eng_font_file, "rb")) == NULL) return(ENG_FONT_NOT_FOUND);
      if(fread(eng_font, NO_ENG*ENG_FONT_SIZE, 1, stream) != 1)
      {
	 fclose(stream);
	 return(ENG_FONT_READ_ERROR);
      }
      fclose(stream);
   }

   if(strlen(spc_font_file))    /* reread special character font */
   {
      if((stream = fopen(spc_font_file, "rb")) == NULL) return(SPC_FONT_NOT_FOUND);
      if(fread(spc_font, NO_ENG*HAN_FONT_SIZE, 1, stream) != 1)
      {
	 fclose(stream);
	 return(SPC_FONT_READ_ERROR);
      }
      fclose(stream);
   }

   return(OK_HAN);
}

void set_cursor_han(int han_no, char *han_shape, int eng_no, char *eng_shape)
/* change cursor shape */
{
   /* shape for Hangul box cursor */
   static const unsigned char shape32[HAN_FONT_SIZE] =
   {
      0xff, 0xff, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
      0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
      0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
      0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xff, 0xff
   };
   /* shape for English box cursor */
   static const unsigned char shape16[ENG_FONT_SIZE] =
   {
      0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
      0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff
   };
   int i;

   if(han_no == BIG_CURSOR)
      for(i = 0; i < 16*2; i++) han_cursor[i+1] = 0xff;
   else if(han_no == SMALL_CURSOR)
   {
      for(i = 0; i < 14*2; i++) han_cursor[i+1] = 0x00;
      for(; i < 16*2; i++)      han_cursor[i+1] = 0xff;
   }
   else if(han_no == BOX_CURSOR)
      for(i = 0; i < 16*2; i++) han_cursor[i+1] = shape32[i];
   else if(han_no == USER_DEFINED_CURSOR)
      for(i = 0; i < 16*2; i++) han_cursor[i+1] = han_shape[i];

   if(eng_no == BIG_CURSOR)
      for(i = 0; i < 16; i++) eng_cursor[i+1] = 0xff;
   else if(eng_no == SMALL_CURSOR)
   {
      for(i = 0; i < 14; i++) eng_cursor[i+1] = 0x00;
      for(; i < 16; i++)      eng_cursor[i+1] = 0xff;
   }
   else if(eng_no == BOX_CURSOR)
      for(i = 0; i < 16; i++) eng_cursor[i+1] = shape16[i];
   else if(eng_no == USER_DEFINED_CURSOR)
      for(i = 0; i < 16; i++) eng_cursor[i+1] = eng_shape[i];
}

void putimage_8(int x, int y, unsigned char *code)
/* draw 8x16 bit map image */
{
   asm  push    es

   asm  cmp     in_vga_system, 1
   asm  mov     es, video_seg           /* ES = segment of VRAM */
   asm  je      vga_out

   /* put 8x16 image in HGC */

   asm  mov     di, x                   /* DI = destination offset */
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

   asm  mov     bx, abs_maxx_han        /* BX = <abs_maxx_han> */
   asm  inc     bx

   asm  push    ds                      /** push DS **/

   asm  lds     si, dword ptr code      /* DS:SI = source */
   asm  mov     cx, 4                   /* CX = loop counter */

   loop1:

   asm  mov     ax, di                  /* not to use "push DI" */
   asm  movsb
   asm  add     di, 0x1fff
   asm  movsb
   asm  add     di, 0x1fff
   asm  movsb
   asm  add     di, 0x1fff
   asm  movsb
   asm  mov     di, ax
   asm  add     di, bx
   asm  loop    loop1
   asm  jmp     quit_putimage_8

   vga_out:

   /* put 8x16 image in EGA/VGA */

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

   /* prepare for outputting the font in VGA */

   asm  mov     cx, 16                  /* CX = loop counter */
   asm  mov     bx, VGA_SCR_WIDTH       /* BX = increment to adjust DI */
   asm  dec     bx

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */

   asm  xor     ax, ax                  /* set/reset = bkcolor_han */
   asm  mov     ah, bkcolor_han
   asm  out     dx, ax
   asm  mov     ax, 0x0f01              /* enable set/reset = 00001111b */
   asm  out     dx, ax
   asm  mov     byte ptr es:[di], al    /* erase one byte with bkcolor_han */
   asm  mov     al, byte ptr es:[di]
   asm  xor     ax, ax                  /* set/reset = color_han */
   asm  mov     ah, color_han
   asm  out     dx, ax
   asm  mov     al, 0x08                /* bit mask */

   asm  xor     ah, ah			/* only for ET-4000 */
   asm	out	dx, ax			/* draw font 0x00   */
   asm  mov	byte ptr es:[di], al

   asm  push    ds                      /** push DS **/

   asm  lds     si, dword ptr code      /* DS:SI = source */

   /* draw the font in EGA/VGA */

   draw_line:

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  stosb                           /* draw the font */
   asm  inc     si

   asm  add     di, bx
   asm  loop    draw_line

   asm  xor     ax, ax                  /* set/reset = 0x00 */
   asm  out     dx, ax
   asm  inc     ax                      /* enable = 0x00 */
   asm  out     dx, ax
   asm  mov     ax, 0xff08              /* bit mask = 0xff */
   asm  out     dx, ax

   quit_putimage_8:

   asm  pop     ds                      /** pop DS **/

   asm  pop     es
}

void putimage_16(int x, int y, unsigned char *code)
/* draw 16x16 bit map image */
{
   asm  push    es

   asm  cmp     in_vga_system, 1
   asm  mov     es, video_seg           /* ES = segment of VRAM */
   asm  je      vga_out

   /* put 16x16 image in HGC */

   asm  mov     di, x                   /* DI = destination offset */
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

   asm  mov     bx, abs_maxx_han        /* BX = <abs_maxx_han> */
   asm  inc     bx

   asm  push    ds                      /** push DS **/

   asm  lds     si, dword ptr code      /* DS:SI = source */
   asm  mov     cx, 4                   /* CX = loop counter */

   loop1:

   asm  mov     ax, di                  /* not to use "push DI" */
   asm  movsw
   asm  add     di, 0x1ffe
   asm  movsw
   asm  add     di, 0x1ffe
   asm  movsw
   asm  add     di, 0x1ffe
   asm  movsw
   asm  mov     di, ax
   asm  add     di, bx
   asm  loop    loop1
   asm  jmp     quit_putimage_16

   vga_out:

   /* put 16x16 image in EGA/VGA */

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

   /* prepare for outputting the font in VGA */

   asm  mov     cx, 16                  /* CX = loop counter */
   asm  mov     bx, VGA_SCR_WIDTH       /* BX = increment to adjust DI */
   asm  dec     bx
   asm  dec     bx

   asm  mov     dx, 0x03ce              /* DX = port 0x03ce */

   asm  xor     ax, ax                  /* set/reset = bkcolor_han */
   asm  mov     ah, bkcolor_han
   asm  out     dx, ax
   asm  mov     ax, 0x0f01              /* enable set/reset = 00001111b */
   asm  out     dx, ax
   asm  mov     byte ptr es:[di], al    /* erase one byte with bkcolor_han */
   asm  mov     al, byte ptr es:[di]
   asm  xor     ax, ax                  /* set/reset = color_han */
   asm  mov     ah, color_han
   asm  out     dx, ax
   asm  mov     al, 0x08                /* bit mask */

   asm  xor     ah, ah			/* only for ET-4000 */
   asm	out	dx, ax			/* draw font 0x00   */
   asm  mov	byte ptr es:[di], al

   asm  push    ds                      /** push DS **/

   asm  lds     si, dword ptr code      /* DS:SI = source */

   /* draw the font in EGA/VGA */

   draw_line:

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  stosb                           /* draw the font */
   asm  inc     si

   asm  mov     ah, byte ptr [si]
   asm  out     dx, ax
   asm  stosb                           /* draw the font */
   asm  inc si

   asm  add     di, bx
   asm  loop    draw_line

   asm  xor     ax, ax                  /* set/reset = 0x00 */
   asm  out     dx, ax
   asm  inc     ax                      /* enable = 0x00 */
   asm  out     dx, ax
   asm  mov     ax, 0xff08              /* bit mask = 0xff */
   asm  out     dx, ax

   quit_putimage_16:

   asm  pop     ds                      /** pop DS **/

   asm  pop     es
}

void modify_image(unsigned char *dest, unsigned char *src, unsigned char bytes, unsigned char attr)
/* modify image */
{
   unsigned char temp_buff[32];
   unsigned char *temp = temp_buff;          /* <temp> points to the buffer */

   asm  push    ds
   asm  push    es

   asm  lds     si, src                 /* DS:SI = src  */
   asm  les     di, dest                /* ES:DI = dest */
   asm  xor     cx, cx
   asm  mov     cl, bytes
   asm  rep     movsb

   asm  mov     dl, attr                /* save <attr> to DL, don't touch DL! */

   asm  cmp     byte ptr bytes, 16      /* check for image size(16 or 32) */
   asm  jne     modify_image_32_link

   /***************** modify image for 8x16 = 16 bytes ********************/

   asm  shr     dl, 1                   /* check for bold */
   asm  jnc     chk_outline_16

   asm  lds     si, dest
   asm  mov     di, si
   asm  mov     cl, 16

   loop_bold_16:
   asm  lodsb
   asm  shr     al, 1
   asm  or      [di], al
   asm  inc     di
   asm  loop    loop_bold_16

   chk_outline_16:

   asm  shr     dl, 1                   /* check for outline */
   asm  jnc     chk_shadow_16
   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16
   asm  push    si
   asm  push    di
   asm  rep     movsb
   asm  pop     di
   asm  pop     si

   asm  mov     cl, 16
   asm  push    si
   asm  push    di

   loop_outline_ltrt_16:
   asm  lodsb
   asm  push    ax
   asm  shr     al, 1
   asm  or      es:[di], al
   asm  pop     ax
   asm  shl     al, 1
   asm  or      es:[di], al
   asm  inc     di
   asm  loop    loop_outline_ltrt_16

   asm  pop     di
   asm  pop     si

   asm  mov     cl, 15

   loop_outline_updn_16:
   asm  lodsb
   asm  mov     bl, [si]
   asm  or      es:[di+1], al
   asm  or      es:[di], bl
   asm  inc     di
   asm  loop    loop_outline_updn_16

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16

   loop_outline_set_temp_16:
   asm  lodsb
   asm  not     al
   asm  and     es:[di], al
   asm  inc     di
   asm  loop    loop_outline_set_temp_16

   asm  lds     si, temp
   asm  les     di, dest
   asm  mov     cl, 16
   asm  rep     movsb
   asm  jmp     chk_shadow_16

   modify_image_32_link: 
   asm  jmp     modify_image_32

   chk_shadow_16:

   asm  shr     dl, 1                   /* check for shadow */
   asm  jnc     chk_shade_16
   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16
   asm  rep     movsb

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16

   loop_shadow_rt_16:
   asm  lodsb
   asm  shr     al, 1
   asm  or      es:[di], al
   asm  inc     di
   asm  loop    loop_shadow_rt_16

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 15

   loop_shadow_dn_16:
   asm  lodsb
   asm  or      es:[di+1], al
   asm  inc     di
   asm  loop    loop_shadow_dn_16

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16

   loop_shadow_set_temp_16:
   asm  lodsb
   asm  not     al
   asm  and     es:[di], al
   asm  inc     di
   asm  loop    loop_shadow_set_temp_16

   asm  lds     si, temp
   asm  les     di, dest
   asm  mov     cl, 16
   asm  rep     movsb

   chk_shade_16:        

   asm  shr     dl, 1                   /* check for shade */
   asm  jnc     chk_underline_16

   asm  lds     di, dest
   asm  mov     cl, 8

   loop_shade_16:
   asm  or      byte ptr[di], 0aah
   asm  or      byte ptr[di+1], 055h
   asm  inc     di
   asm  inc     di
   asm  loop    loop_shade_16

   chk_underline_16:

   asm  shr     dl, 1                   /* check for underline */
   asm  jnc     chk_inverse_16

   asm  lds     di, dest
   asm  or      byte ptr[di+15], 0ffh

   chk_inverse_16:

   asm  shr     dl, 1                   /* check for inverse */
   asm  jnc     quit_modify_image_link

   asm  lds     di, dest
   asm  mov     cl, bytes

   loop_inverse_16:
   asm  not     byte ptr[di]
   asm  inc     di
   asm  loop    loop_inverse_16

   asm  jmp     quit_modify_image_link

   modify_image_32:

   /***************** modify image for 16x16 = 32 bytes *******************/

   asm  shr     dl, 1                   /* check for bold */
   asm  jnc     chk_outline_32

   asm  lds     si, dest
   asm  mov     di, si
   asm  mov     cl, 16

   loop_bold_32:
   asm  lodsw
   asm  xchg    ah, al
   asm  shr     ax, 1
   asm  xchg    ah, al
   asm  or      [di], ax
   asm  inc     di
   asm  inc     di
   asm  loop    loop_bold_32

   chk_outline_32:

   asm  shr     dl, 1                   /* check for outline */
   asm  jnc     chk_shadow_32
   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16
   asm  push    si
   asm  push    di
   asm  rep     movsw
   asm  pop     di
   asm  pop     si

   asm  mov     cl, 16
   asm  push    si
   asm  push    di

   loop_outline_ltrt_32:
   asm  lodsw
   asm  xchg    ah, al
   asm  push    ax
   asm  shr     ax, 1
   asm  xchg    ah, al
   asm  or      es:[di], ax
   asm  pop     ax
   asm  shl     ax, 1
   asm  xchg    ah, al
   asm  or      es:[di], ax
   asm  inc     di
   asm  inc     di
   asm  loop    loop_outline_ltrt_32

   asm  pop     di
   asm  pop     si

   asm  mov     cl, 15

   loop_outline_updn_32:
   asm  lodsw
   asm  mov     bx, [si]
   asm  or      es:[di+2], ax
   asm  or      es:[di], bx
   asm  inc     di
   asm  inc     di
   asm  loop    loop_outline_updn_32

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16
   asm  jmp     loop_outline_set_temp_32

   quit_modify_image_link:
   asm  jmp     quit_modify_image

   loop_outline_set_temp_32:
   asm  lodsw
   asm  not     ax
   asm  and     es:[di], ax
   asm  inc     di
   asm  inc     di
   asm  loop    loop_outline_set_temp_32

   asm  lds     si, temp
   asm  les     di, dest
   asm  mov     cl, 16
   asm  rep     movsw

   chk_shadow_32:

   asm  shr     dl, 1                   /* check for shadow */
   asm  jnc     chk_shade_32
   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16
   asm  rep     movsw

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16

   loop_shadow_rt_32:
   asm  lodsw
   asm  xchg    ah, al
   asm  shr     ax, 1
   asm  xchg    ah, al
   asm  or      es:[di], ax
   asm  inc     di
   asm  inc     di
   asm  loop    loop_shadow_rt_32

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 15

   loop_shadow_dn_32:
   asm  lodsw
   asm  or      es:[di+2], ax
   asm  inc     di
   asm  inc     di
   asm  loop    loop_shadow_dn_32

   asm  lds     si, dest
   asm  les     di, temp
   asm  mov     cl, 16

   loop_shadow_set_temp_32:
   asm  lodsw
   asm  not     ax
   asm  and     es:[di], ax
   asm  inc     di
   asm  inc     di
   asm  loop    loop_shadow_set_temp_32

   asm  lds     si, temp
   asm  les     di, dest
   asm  mov     cl, 16
   asm  rep     movsw

   chk_shade_32:

   asm  shr     dl, 1                   /* check for shade */
   asm  jnc     chk_underline_32

   asm  lds     di, dest
   asm  mov     cl, 8

   loop_shade_32:
   asm  or      word ptr[di], 0aaaah
   asm  or      word ptr[di+2], 05555h
   asm  add     di, 4
   asm  loop    loop_shade_32

   chk_underline_32:

   asm  shr     dl, 1                   /* check for underline */
   asm  jnc     chk_inverse_32

   asm  lds     di, dest
   asm  or      word ptr[di+30], 0ffffh

   chk_inverse_32:

   asm  shr     dl, 1                   /* check for inverse */
   asm  jnc     quit_modify_image

   asm  lds     di, dest
   asm  mov     cl, 16

   loop_inverse_32:
   asm  not     word ptr[di]
   asm  inc     di
   asm  inc     di
   asm  loop    loop_inverse_32

   quit_modify_image:

   asm  pop     es
   asm  pop     ds
}

void zoomed_out_8(int x, int y, unsigned char *font, unsigned char attr)
/* zoom 8x16 font and draw it */
{
   unsigned char out_font[ENG_FONT_SIZE*2];
   unsigned char mask, mask2;
   int i;

   if((attr & (FONT_HORIZ_DBL | FONT_VERT_DBL)) == (FONT_HORIZ_DBL | FONT_VERT_DBL))
   {
      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
	 out_font[i*2]   = 0;
	 out_font[i*2+1] = 0;
         mask2 = 0x80;
         for(mask = 0x80; mask > 0x08; mask >>= 1)
	 {
            if(font[i] & mask)
            {
	       out_font[i*2] |= mask2;
               out_font[i*2] |= mask2 >> 1;
	    }
	    mask2 >>= 2;
         }
         out_font[i*2+1] = out_font[i*2];
      }
      if(attr & FONT_UNDERLINE)
         modify_image(out_font, out_font, 16, attr & (!FONT_UNDERLINE));
      else
	 modify_image(out_font, out_font, 16, attr);
      putimage_8(x, y,  out_font);
      modify_image(out_font+ENG_FONT_SIZE, out_font+ENG_FONT_SIZE, 16, attr);
      putimage_8(x, y+1, out_font+ENG_FONT_SIZE);

      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
	 out_font[i*2]   = 0;
         out_font[i*2+1] = 0;
         mask2           = 0x80;
         for(mask = 0x08; mask > 0x00; mask >>= 1)
	 {
            if(font[i] & mask)
	    {
               out_font[i*2] |= mask2;
               out_font[i*2] |= mask2 >> 1;
	    }
            mask2 >>= 2;
         }
         out_font[i*2+1] = out_font[i*2];
      }
      if(attr & FONT_UNDERLINE)
         modify_image(out_font, out_font, 16, attr & (!FONT_UNDERLINE));
      else
	 modify_image(out_font, out_font, 16, attr);
      putimage_8(x+1, y,  out_font);
      modify_image(out_font+ENG_FONT_SIZE, out_font+ENG_FONT_SIZE, 16, attr);
      putimage_8(x+1, y+1, out_font+ENG_FONT_SIZE);
      return;
   }

   if(attr & FONT_VERT_DBL)
   {
      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
         out_font[i*2]   = font[i];
         out_font[i*2+1] = font[i];
      }
      if(attr & FONT_UNDERLINE)
	 modify_image(out_font, out_font, 16, attr & (!FONT_UNDERLINE));
      else
	 modify_image(out_font, out_font, 16, attr);
      putimage_8(x, y, out_font);
      modify_image(out_font+ENG_FONT_SIZE, out_font+ENG_FONT_SIZE, 16, attr);
      putimage_8(x, y+1, out_font+ENG_FONT_SIZE);
      return;
   }

   if(attr & FONT_HORIZ_DBL)
   {
      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
	 out_font[i] = 0;
	 mask2       = 0x80;
         for(mask = 0x80; mask > 0x08; mask >>= 1)
	 {
            if(font[i] & mask)
            {
               out_font[i] |= mask2;
               out_font[i] |= mask2 >> 1;
            }
	    mask2 >>= 2;
         }
      }
      modify_image(out_font, out_font, 16, attr);
      putimage_8(x, y, out_font);

      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
         out_font[i] = 0;
         mask2       = 0x80;
         for(mask = 0x08; mask > 0x00; mask >>= 1)
         {
	    if(font[i] & mask)
            {
               out_font[i] |= mask2;
               out_font[i] |= mask2 >> 1;
            }
	    mask2 >>= 2;
         }
      }
      modify_image(out_font, out_font, 16, attr);
      putimage_8(x+1, y, out_font);
      return;
   }
}

void zoomed_out_16(int x, int y, unsigned char *font, unsigned char attr)
/* zoom 16x16 font and draw it */
{
   unsigned char out_font[HAN_FONT_SIZE*2];
   unsigned char mask, mask2;
   int i;

   if((attr & (FONT_HORIZ_DBL | FONT_VERT_DBL)) == (FONT_HORIZ_DBL | FONT_VERT_DBL))
   {
      for(i = 0; i < ENG_FONT_SIZE; i++) /* do not change to HAN_FONT_SIZE */
      {
         out_font[i*4]   = 0;
         out_font[i*4+1] = 0;
         out_font[i*4+2] = 0;
	 out_font[i*4+3] = 0;
         mask2           = 0x80;
         for(mask = 0x80; mask > 0x08; mask >>= 1)
         {
	    if(font[i*2] & mask)
	    {
	       out_font[i*4] |= mask2;
	       out_font[i*4] |= mask2 >> 1;
	    }
            mask2 >>= 2;
         }
         mask2 = 0x80;
	 for(mask = 0x08; mask > 0x00; mask >>= 1)
         {
            if(font[i*2] & mask)
            {
               out_font[i*4+1] |= mask2;
               out_font[i*4+1] |= mask2 >> 1;
            }
            mask2 >>= 2;
	 }
         out_font[i*4+2] = out_font[i*4];
         out_font[i*4+3] = out_font[i*4+1];
      }
      if(attr & FONT_UNDERLINE)
	 modify_image(out_font, out_font, 32, attr & (!FONT_UNDERLINE));
      else
         modify_image(out_font, out_font, 32, attr);
      putimage_16(x, y, out_font);
      modify_image(out_font+HAN_FONT_SIZE, out_font+HAN_FONT_SIZE, 32, attr);
      putimage_16(x, y+1, out_font+HAN_FONT_SIZE);

      for(i = 0; i < ENG_FONT_SIZE; i++) /* do not change to HAN_FONT_SIZE */
      {
         out_font[i*4]   = 0;
         out_font[i*4+1] = 0;
         out_font[i*4+2] = 0;
         out_font[i*4+3] = 0;
         mask2           = 0x80;
         for(mask = 0x80; mask > 0x08; mask >>= 1)
	 {
            if(font[i*2+1] & mask)
            {
               out_font[i*4] |= mask2;
	       out_font[i*4] |= mask2 >> 1;
            }
	    mask2 >>= 2;
         }
	 mask2 = 0x80;
         for(mask = 0x08; mask > 0x00; mask >>= 1)
         {
            if(font[i*2+1] & mask)
	    {
               out_font[i*4+1] |= mask2;
               out_font[i*4+1] |= mask2 >> 1;
            }
            mask2 >>= 2;
         }
         out_font[i*4+2] = out_font[i*4];
	 out_font[i*4+3] = out_font[i*4+1];
      }
      if(attr & FONT_UNDERLINE)
         modify_image(out_font, out_font, 32, attr & (!FONT_UNDERLINE));
      else
         modify_image(out_font, out_font, 32, attr);
      putimage_16(x+2, y, out_font);
      modify_image(out_font+HAN_FONT_SIZE,out_font+HAN_FONT_SIZE,32,attr);
      putimage_16(x+2, y+1, out_font+HAN_FONT_SIZE);
      return;
   }

   if(attr & FONT_VERT_DBL)
   {
      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
         out_font[i*4]   = font[i*2];
         out_font[i*4+1] = font[i*2+1];
         out_font[i*4+2] = font[i*2];
         out_font[i*4+3] = font[i*2+1];
      }
      if(attr & FONT_UNDERLINE)
         modify_image(out_font, out_font, 32, attr & (!FONT_UNDERLINE));
      else
         modify_image(out_font, out_font, 32, attr);
      putimage_16(x, y, out_font);
      modify_image(out_font+HAN_FONT_SIZE, out_font+HAN_FONT_SIZE, 32, attr);
      putimage_16(x, y+1, out_font+HAN_FONT_SIZE);
      return;
   }

   if(attr & FONT_HORIZ_DBL)
   {
      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
         out_font[i*2]   = 0;
         out_font[i*2+1] = 0;
         mask2           = 0x80;
         for(mask = 0x80; mask > 0x08; mask >>= 1)
	 {
            if(font[i*2] & mask)
	    {
	       out_font[i*2] |= mask2;
               out_font[i*2] |= mask2 >> 1;
            }
            mask2 >>= 2;
         }
	 mask2 = 0x80;
         for(mask = 0x08; mask > 0x00; mask >>= 1)
	 {
            if(font[i*2] & mask)
            {
	       out_font[i*2+1] |= mask2;
	       out_font[i*2+1] |= mask2 >> 1;
            }
            mask2 >>= 2;
         }
      }
      modify_image(out_font, out_font, 32, attr);
      putimage_16(x, y, out_font);

      for(i = 0; i < ENG_FONT_SIZE; i++)
      {
         out_font[i*2]   = 0;
         out_font[i*2+1] = 0;
	 mask2           = 0x80;
         for(mask = 0x80; mask > 0x08; mask >>= 1)
	 {
            if(font[i*2+1] & mask)
	    {
               out_font[i*2] |= mask2;
               out_font[i*2] |= mask2 >> 1;
            }
	    mask2 >>= 2;
         }
         mask2 = 0x80;
         for(mask = 0x08; mask > 0x00; mask >>= 1)
         {
	    if(font[i*2+1] & mask)
            {
	       out_font[i*2+1] |= mask2;
	       out_font[i*2+1] |= mask2 >> 1;
            }
            mask2 >>= 2;
         }
      }
      modify_image(out_font, out_font, 32, attr);
      putimage_16(x+2, y, out_font);
      return;
   }
}
