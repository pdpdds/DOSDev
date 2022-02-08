/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          adv_puts_han()                                      */
/*   FILENAME           advputs.c                                           */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "misc.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void adv_puts_han(char *string, int indent)
/* ¢…”e·i ¸÷iÐaµa Â‰bÐe”a. */
{
   char buff[LIMITX_HAN+1];
   int  index, buff_index, first_line, width, i;

   width = maxx_han - minx_han + 1;
   if(width < indent + 4)
   /* —iµa³a‹¡µA §¡Ð ¶å•¡¶· Í¢·¡ á¢ ¹³”a¡e ‹a· puts_han() ·a¡ Â‰b */
   {
      puts_han(string);
      return;
   }

   index      = 0;
   first_line = 1;
   curx_han   = minx_han;
   while(string[index])
   /* string ·i Ðe º‰³¢ ¸÷iÐaµa Â‰bÐe”a. */
   {
      buff_index = 0;

      /* —iµa³a‹¡ ÀáŸ¡ */
      if(first_line && indent > 0)
      {
         for(i = 0; i < indent; i++) buff[i] = ' ';
         buff_index += indent;
      }
      else if(!first_line && indent < 0)
      {
         for(i = indent; i < 0; i++) buff[i] = ' ';
         buff_index -= indent;
      }

      if(first_line)
         first_line = 0;
      else
      /* Àõ º‰·¡ ´a“¡¡e ¶E½¢µA ·¶“e ¡¡—e ‰·¤‚·i ¢¯¡ */
         while(string[index] == ' ' && buff_index < width) index++;

      /* ¶E½¢µA ·¶“e ¡¡—e ‰·¤‚·¡ ¢¯¡–E ¢…¸aµiµA¬á width ˆ· ¢…¸aŸi */
      /* buff µA ¥¢¬a                                                */
      while(string[index] && buff_index < width) buff[buff_index++] = string[index++];
      buff[buff_index] = 0;

      /* buff Ÿi ¸÷iÐe Ò, ¸÷i·i ¶áÐ ¬s·³–E ‰·¤‚ ® eÇq index ˆq­¡ */
      index -= arrange_string(buff, width);
      /* ¸÷i–E buff Ÿi Â‰b */
      cputs_han(buff);
   }

   putch_han("\n");
}
