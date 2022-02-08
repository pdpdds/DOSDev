/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          draw_status_han(), status_on_han(),                 */
/*                      status_off_han()                                    */
/*   FILENAME           status.c                                            */
/*   DATE & TIME        03/11/92(WED) 18:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void draw_status_han(void)
/* ¨w»Å úa∑•µA ∏˜•°üi ŒaØ°–eîa. */
{
   int attr2, color2, bkcolor2, i;

   /* ¨w»Å úa∑•∑° êa»aêa ∑∂ª° ¥g∑a°e §aù° ü°»Â */
   if(!is_status_on_han) return;

   attr2       = attr_han;
   color2      = color_han;
   bkcolor2    = bkcolor_han;
   attr_han    = FONT_INVERSE;
   color_han   = LIGHTGRAY;
   bkcolor_han = BLACK;

   for(i = 0; i <= abs_maxx_han; i++) put_eng_font(i, abs_maxy_han, ' ');
   put_eng_font( 1, abs_maxy_han, 179);
   put_eng_font( 8, abs_maxy_han, 179);
   put_eng_font(16, abs_maxy_han, 179);
   put_eng_font(23, abs_maxy_han, 179);

   /* –e/µw ¨w»Å ŒaØ° */
   if(han_mode)
   {
      put_han_font(3, abs_maxy_han, '–', 'e');	/* "–e" */
      put_han_font(5, abs_maxy_han, 'ã', 'i');	/* "ãi" */
   }
   else
   {
      put_han_font(3, abs_maxy_han, 'µ', 'w');	/* "µw" */
      put_han_font(5, abs_maxy_han, '¢', 'Ö');  /* "¢Ö" */
   }

   /* 2 §ÈØ¢/3 §ÈØ¢ ¨w»Å ŒaØ° */
   bul3_mode ? put_eng_font(10, abs_maxy_han, '3')
	     : put_eng_font(10, abs_maxy_han, '2');
   put_han_font(11, abs_maxy_han, '§', 'È');	/* "§È" */
   put_han_font(13, abs_maxy_han, 'Ø', '¢');	/* "Ø¢" */

   /* ¨s∑≥/ÆÅ∏˜ ¨w»Å ŒaØ° */
   if(ins_mode)
   {
      put_han_font(18, abs_maxy_han, '¨', 's');	/* "¨s" */
      put_han_font(20, abs_maxy_han, '∑', '≥');	/* "∑≥" */
   }
   else
   {
      put_han_font(18, abs_maxy_han, 'Æ', 'Å');	/* "ÆÅ" */
      put_han_font(20, abs_maxy_han, '∏', '˜');	/* "∏˜" */
   }

   attr_han    = attr2;
   color_han   = color2;
   bkcolor_han = bkcolor2;
}

void status_on_han(void)
/* ¨w»Å úa∑•∑° êa»aêaâA –eîa. */
{
   /* ∑°£° ¨w»Å úa∑•∑° êa»aêa ∑∂∑a°e §aù° ü°»Â */
   if(is_status_on_han) return;

   if(cury_han == abs_maxy_han)
   {
      xscrollup_han(0, 0, abs_maxx_han, abs_maxy_han);
      cury_han--;
   }
   maxy_han--;

   is_status_on_han = 1;
   draw_status_han();
}

void status_off_han(void)
/* ¨w»Å úa∑•∑i ¨aúaª°âA –eîa. */
{
   int i;

   /* ¨w»Å úa∑•∑° êa»aêa ∑∂ª° ¥g∑a°e §aù° ü°»Â */
   if(!is_status_on_han) return;

   maxy_han++;
   for(i = 0; i <= abs_maxx_han; i++) put_eng_font(i, abs_maxy_han, ' ');
   is_status_on_han = 0;
}
