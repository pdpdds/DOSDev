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
/* ���e�i ���i�a�a �b�e�a. */
{
   char buff[LIMITX_HAN+1];
   int  index, buff_index, first_line, width, i;

   width = maxx_han - minx_han + 1;
   if(width < indent + 4)
   /* �i�a�a���A ��Ё �啡���� ͢�� �ᢁ ���a�e �a�� puts_han() �a�� �b */
   {
      puts_han(string);
      return;
   }

   index      = 0;
   first_line = 1;
   curx_han   = minx_han;
   while(string[index])
   /* string �i �e ���� ���i�a�a �b�e�a. */
   {
      buff_index = 0;

      /* �i�a�a�� �១ */
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
      /* �� ���� �a���e �E���A ���e ���e �����i ���� */
         while(string[index] == ' ' && buff_index < width) index++;

      /* �E���A ���e ���e ������ �����E ���a�i�A�� width ���� ���a�i */
      /* buff �A ���a                                                */
      while(string[index] && buff_index < width) buff[buff_index++] = string[index++];
      buff[buff_index] = 0;

      /* buff �i ���i�e ҁ, ���i�i ��Ё �s���E ���� ���e�q index �q�� */
      index -= arrange_string(buff, width);
      /* ���i�E buff �i �b */
      cputs_han(buff);
   }

   putch_han("\n");
}
