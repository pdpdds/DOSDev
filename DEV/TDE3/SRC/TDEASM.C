/*
 * The DTE 5.1 editor used a big text buffer for editing a file.  The big
 * text buffer holds an image of a file pretty much as it appears on
 * disk.  In that big buffer, '\0' was used to delimit the files.  In TDE
 * versions 2.10 and less, a big text buffer was also used, but ^Z was used
 * instead of '\0' to delimit files.  A double linked list is used in TDE 2.2
 * to hold text.
 *
 * With these functions written in assembly, this editor is fairly fast.
 * I feel the need for speed.
 *
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991, version 1.0
 * Date:             July 29, 1991, version 1.1
 * Date:             October 5, 1991, version 1.2
 * Date:             January 20, 1992, version 1.3
 * Date:             February 17, 1992, version 1.4
 * Date:             April 1, 1992, version 1.5
 * Date:             June 5, 1992, version 2.0
 * Date:             October 31, 1992, version 2.1
 * Date:             April 1, 1993, version 2.2
 * Date:             June 5, 1993, version 3.0
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 */

#include "tdestr.h"
#include "common.h"
#include "tdefunc.h"


/*
 * Name:    ptoul - pointer to unsigned long
 * Purpose: convert a far pointer to unsigned long integer
 * Date:    June 5, 1991
 * Passed:  s:  a far pointer
 * Notes:   combine the offset and segment like so:
 *                offset       0000
 *                segment   + 0000
 *                          =======
 *                            00000
 *          result is returned in dx:ax
 */
unsigned long ptoul( void far *s )
{
   ASSEMBLE {
        mov     ax, WORD PTR s          /* ax = OFFSET of s */
        mov     dx, WORD PTR s+2        /* dx = SEGMENT of s */
        mov     bx, dx          /* put copy of segment in bx */
        mov     cl, 12          /* cl = decimal 12 - shift hi word 3 hex digits */
        shr     dx, cl          /* convert to 'real segment' */
        mov     cl, 4           /* cl = 4  - shift hi word 1 hex digit left */
        shl     bx, cl          /* shift bx - to add 3 digits of seg to 4 of off */
        add     ax, bx          /* add low part of segment to offset */
        adc     dx, 0           /* if carry, bump to next 'real' segment */
   }
}


/*
 * Name:    tabout
 * Purpose: Expand tabs in display line
 * Date:    October 31, 1992
 * Passed:  s:  string pointer
 *          len:  pointer to current length of string
 * Notes:   Expand tabs in the display line according to current tab.
 *          If eol display is on, let's display tabs, too.
 */
text_ptr tabout( text_ptr s, int *len )
{
text_ptr to;
int space;
int col;
int i;
int tab_size;
int show_tab;
int tab_len;


   tab_size = mode.ptab_size;
   show_tab = mode.show_eol;
   to  = (text_ptr)g_status.tabout_buff;
   i = tab_len  = *len;

   ASSEMBLE {
        push    si
        push    di
        push    ds
        push    es

        mov     bx, WORD PTR tab_size   /* keep tab_size in bx */
        xor     cx, cx                  /* keep col in cx */

        mov     di, WORD PTR to
        mov     ax, WORD PTR to+2
        mov     es, ax                  /* es:di == to or the destination */
        mov     si, WORD PTR s
        mov     ax, WORD PTR s+2
        mov     ds, ax                  /* ds:si == s or the source */
   }
top:

   ASSEMBLE {
        cmp     cx, MAX_LINE_LENGTH     /* are at end of tabout buffer? */
        jge     get_out

        cmp     WORD PTR i, 0           /* are at end of string? */
        jle     get_out

        lodsb                           /* al == BYTE PTR ds:si */
        cmp     al, 0x09                /* is this a tab character? */
        je      expand_tab

        stosb                           /* store character in es:di inc di */
        inc     cx                      /* increment col counter */
        dec     WORD PTR i              /* decrement string counter */
        jmp     SHORT top
   }
expand_tab:

   ASSEMBLE {
        mov     ax, cx
        xor     dx, dx                  /* set up dx:ax for IDIV */
        IDIV    bx                      /* col % tab_size */
        mov     ax, bx                  /* put tab_size in bx */
        sub     ax, dx                  /* ax = tab_size - (col % tab_size) */
        mov     dx, ax                  /* put ax in dx */
        add     cx, ax                  /* col += space */
        cmp     cx, MAX_LINE_LENGTH     /* is col > MAX_LINE_LENGTH? */
        jge     get_out                 /* yes, get out */
        mov     ax, ' '                 /* save blank character in ax */
        cmp     WORD PTR show_tab, 0    /* was show_tab flag set? */
        je      do_the_tab
        mov     ax, 0x09                /* put tab character in ax */
   }
do_the_tab:

   ASSEMBLE {
        stosb                           /* store in es:di and incr di */
        dec     dx
        cmp     dx, 0                   /* any spaces left to fill? */
        jle     end_of_space            /* no, get another character */
        add     WORD PTR tab_len, dx    /* add spaces to string length */
        mov     ax, ' '                 /* save blank character in ax */
   }
space_fill:

   ASSEMBLE {
        cmp     dx, 0                   /* any spaces left to fill? */
        jle     end_of_space            /* no, get another character */
        stosb                           /* store ' ' in es:di and incr di */
        dec     dx                      /* space-- */
        jmp     SHORT space_fill        /* fill space */
  }
end_of_space:

   ASSEMBLE {
        dec     WORD PTR i
        jmp     SHORT top
   }
get_out:

   ASSEMBLE {
        pop     es
        pop     ds
        pop     di
        pop     si
   }
   if (tab_len > MAX_LINE_LENGTH)
      tab_len = MAX_LINE_LENGTH;
   *len = g_status.tabout_buff_len = tab_len;
   return( (text_ptr)g_status.tabout_buff );

/*
   tab_size = mode.ptab_size;
   show_tab = mode.show_eol;
   to  = g_status.tabout_buff;
   i = tab_len  = *len;
   for (col=0; col < (MAX_LINE_LENGTH - (tab_size+1)) &&  i > 0; s++, i--) {
      if (*s != '\t') {
         *to++ = *s;
         ++col;
      } else {
         space = tab_size - (col % tab_size);
         col += space;
         space--;
         if (space > 0)
            tab_len += space;
         if (show_tab)
            *to++ = '\t';
         else
            *to++ = ' ';
         for (; space > 0; space--)
            *to++ = ' ';
      }
   }
   if (tab_len > MAX_LINE_LENGTH)
      tab_len = MAX_LINE_LENGTH;
   *len = g_status.tabout_buff_len = tab_len;
   return( (text_ptr)g_status.tabout_buff );
*/
}
