/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_2byte_font()                                    */
/*   FILENAME           get2font.c                                          */
/*   DATE & TIME        05/10/92(SUN) 10:00                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <stdio.h>
#include "hanio.h"

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

#define mini            unsigned char
#define HANJA_FONT_FILE "hanja.fnt"

/****************************************************************************/
/*               Tables(to be referenced by get_2byte_font())               */
/****************************************************************************/

/*********************** Table to decide # of font **************************/

/* to decide # of chosung font by chosung code */
const mini han1_table[32] = {   0, 0, 1, 2, 3, 4, 5, 6,
				7, 8, 9,10,11,12,13,14,
			       15,16,17,18,19, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0   };
/* to decide # of jungsung font by jungsung code */
const mini han2_table[32] = {   0, 0, 0, 1, 2, 3, 4, 5,
				0, 0, 6, 7, 8, 9,10,11,
				0, 0,12,13,14,15,16,17,
				0, 0,18,19,20,21, 0, 0   };
/* to decide # of jongsung font by jongsung code */
const mini han3_table[32] = {   0, 0, 1, 2, 3, 4, 5, 6,
				7, 8, 9,10,11,12,13,14,
			       15,16, 0,17,18,19,20,21,
			       22,23,24,25,26,27, 0, 0   };

/************************ Table to decide # of series ***********************/

/* to decide # of jongsung series by jungsung */
const mini mid_table1[22] = {   0, 0, 2, 0, 2, 1, 2, 1,
				2, 3, 0, 2, 1, 3, 3, 1,
				2, 1, 3, 3, 1, 1         };
/* to decide # of chosung series by jungsung when there is no jongsung  */
const mini mid_table2[22] = {   0, 0, 0, 0, 0, 0, 0, 0,
				0, 1, 3, 3, 3, 1, 2, 4,
				4, 4, 2, 1, 3, 0             };
/* to decide # of chosung series by jungsung when there is any jongsung */
const mini mid_table3[22] = {   0, 5, 5, 5, 5, 5, 5, 5,
				5, 6, 7, 7, 7, 6, 6, 7,
				7, 7, 6, 6, 7, 5             };

/* to decide # of jungsung series by chosung when there is no jongsung  */
const mini fst_table1[20] = {   0, 0, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1,
				0, 1, 1, 1                   };
/* to decide # of jungsung series by chosung when there is any jongsung */
const mini fst_table2[20] = {   3, 2, 3, 3, 3, 3, 3, 3,
				3, 3, 3, 3, 3, 3, 3, 3,
				2, 3, 3, 3                   };

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

extern unsigned hanja_buff_ptr;

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void __get_hanja_font(mini code1, mini code2)
/* store hanja font into <or_font> */
{
   FILE *stream;
   long fp;
   int  i, j;

   /* check hanja buffer */
   for(i = 0; i < hanja_buff_size; i++)
      if(hanja_buff[i].code == ((unsigned) code1 << 8 | code2))
      {
	 for(j = 0; j < HAN_FONT_SIZE; j++) or_font[j] = hanja_buff[i].font[j];
	 return;
      }

   /* check hanja font file */
   fp = (long) (code1 & 0x1f) * 188 + (code2 < 0x91 ? code2 - 0x31 : code2 - 0x43);
   fp <<= 5;
   if((stream = fopen(HANJA_FONT_FILE, "rb")) == NULL)
   {
      for(i = 0; i < HAN_FONT_SIZE; i++) or_font[i] = 0;
      return;
   }

   fseek(stream, fp, SEEK_SET);
   fread(hanja_buff[hanja_buff_ptr].font, 32, 1, stream);
   fclose(stream);
   for(i = 0; i < HAN_FONT_SIZE; i++) or_font[i] = hanja_buff[hanja_buff_ptr].font[i];
   hanja_buff[hanja_buff_ptr].code = (unsigned) code1 << 8 | code2;
   hanja_buff_ptr = (hanja_buff_ptr + 1) % hanja_buff_size;
}

void get_2byte_font(mini code1, mini code2)
/* gets font of a 2-bytes character and store that into <or_font> */
{
   unsigned char cho, jung, jong, bul1, bul2, bul3;
   int i;

   asm  cmp     byte ptr code1, SPC_ID
   asm  jne     check_hanja       /* if not special character, jump han_out */

   for(i = 0; i < HAN_FONT_SIZE; i++) or_font[i] = spc_font[code2][i];
   return;

   check_hanja :

   asm  cmp     byte ptr code1, 0xe0
   asm  jb      check_hangul

   __get_hanja_font(code1, code2);
   return;

   check_hangul:

   asm  push    es

   /* decide cho, jung, jong                                           */
   /* cho  = han1_table[(code1 & 0x7c) >> 2];                          */
   /* jung = han2_table[((code1 & 0x03) << 3) +((code2 & 0xe0) >> 5)]; */
   /* jong = han3_table[code2 & 0x1f];                                 */

   asm  mov     si, offset han1_table
   asm  mov     al, code1
   asm  and     ax, 0x007c
   asm  shr     ax, 1
   asm  shr     ax, 1
   asm  add     si, ax
   asm  lodsb
   asm  mov     cho, al

   asm  mov     si, offset han2_table
   asm  mov     al, code1
   asm  and     ax, 0x0003
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  shl     ax, 1
   asm  add     si, ax
   asm  mov     al, code2
   asm  and     ax, 0x00e0
   asm  mov     cl, 5
   asm  shr     ax, cl
   asm  add     si, ax
   asm  lodsb
   asm  mov     jung,al

   asm  mov     si, offset han3_table
   asm  mov     al, code2
   asm  and     ax, 0x001f
   asm  add     si, ax
   asm  lodsb
   asm  mov     jong, al

   /* decide bul1, bul2, bul3       */
   /* bul3 = mid_table1[jung];      */
   /* if(!jong)                     */
   /* {                             */
   /*    bul2 = fst_table1[cho];    */
   /*    bul1 = mid_table2[jung];   */
   /* }                             */
   /* else                          */
   /* {                             */
   /*    bul2 = fst_table2[cho];    */
   /*    bul1 = mid_table3[jung];   */
   /* }                             */

   asm  mov     si, offset mid_table1
   asm  xor     ah, ah
   asm  mov     al, jung
   asm  add     si, ax
   asm  lodsb   
   asm  mov     bul3, al

   asm  cmp     jong, ah
   asm  jne     exist_jong
   asm  mov     si, offset fst_table1
   asm  mov     al, cho
   asm  add     si, ax
   asm  lodsb
   asm  mov     bul2, al
   asm  mov     si, offset mid_table2
   asm  mov     al, jung
   asm  add     si, ax
   asm  lodsb
   asm  mov     bul1, al
   asm  jmp     exit_chk_alpha

   exist_jong:

   asm  mov     si, offset fst_table2
   asm  mov     al, cho
   asm  add     si, ax
   asm  lodsb
   asm  mov     bul2, al
   asm  mov     si, offset mid_table3
   asm  mov     al, jung
   asm  add     si, ax
   asm  lodsb
   asm  mov     bul1, al

   /* after checking code1 and code2 to get all data to display hangul */

   exit_chk_alpha:

   /* load chosung font to or_font */

   asm  mov     al, bul1
   asm  mov     bl, NO_HAN1+1
   asm  mul     bl
   asm  add     al, cho
   asm  mov     cl, 5
   asm  mov     bx, ds                  /* save DS to BX, so never touch BX! */
   asm  lds     si, dword ptr han1_font
   asm  shl     ax, cl
   asm  add     si, ax
   asm  mov     es, bx                  /* ES = DS */
   asm  mov     di, offset es:or_font
   asm  mov     cx, 32
   asm  rep     movsb

   /* load jungsung font and mix that image with or_font */
   /* at this time ES contains original DS */

   asm  mov     ds, bx
   asm  lds     si, dword ptr han2_font
   asm  mov     al, bul2
   asm  mov     cl, NO_HAN2+1
   asm  mul     cl
   asm  add     al, jung
   asm  mov     cl, 5
   asm  shl     ax, cl
   asm  add     si, ax
   asm  mov     di, offset es:or_font
   asm  mov     cx, 32

   loop2:

   asm  lodsb
   asm  or      es:[di], al
   asm  inc     di
   asm  loop    loop2

   /* load jongsung font and mix that image with or_font */

   asm  mov     ds, bx
   asm  lds     si, dword ptr han3_font
   asm  mov     al, bul3
   asm  mov     cl, NO_HAN3+1
   asm  mul     cl
   asm  add     al, jong
   asm  mov     cl, 5
   asm  shl     ax, cl
   asm  add     si, ax
   asm  mov     di, offset es:or_font
   asm  mov     cx, 32

   loop3:

   asm  lodsb
   asm  or      es:[di], al
   asm  inc     di
   asm  loop    loop3

   asm  mov     ds, bx                  /* restore DS with BX */
   asm  pop     es
}
