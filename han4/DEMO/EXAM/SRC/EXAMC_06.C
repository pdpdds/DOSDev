#include <stdio.h>
#include <conio.h>
#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(int argc, char *argv[])
{
   FILE *stream;
   char string[128];
   int  source_code;
   char ch;

   if(argc < 2)         /* �����w�� �e�i š�a�a �����A�� �g�v�i �� */
   {
      puts("Usage : examc_06 <file_name> [/j/k/7]");
      puts("        /j = JOHAP");
      puts("        /k = KSC5601");
      puts("        /7 = BIT7");
      exit(1);
   }

   if(argc < 3)         /* �e�i š�a�a �����A�� �g�v�a�e */
      source_code = JOHAP;
   else if(!strcmp(argv[2], "/j"))      /* ���s�w���a�e */
      source_code = JOHAP;
   else if(!strcmp(argv[2], "/k"))      /* �Ŭ��w���a�e */
      source_code = KSC5601;
   else if(!strcmp(argv[2], "/7"))      /* ������ �e�i���a�e */
      source_code = BIT7;
   else                                 /* �i�� �����A���a�e */
      source_code = JOHAP;

   if((stream = fopen(argv[1], "rt")) == NULL)  /* �����i ���e�e�a. */
   {
      puts("file not found");
      exit(2);
   }

   if(init_han() != OK_HAN) exit(4);

   while(!feof(stream))
   {
      if(fgets(string, 127, stream) == NULL)
         break;
      else
      {
         if(source_code != JOHAP) convert_han_code(string, source_code, JOHAP);
         cputs_han(string);
      }

      if(kbhit())                               /* ǡ�a ���v�a�e */
      {
         if((ch = getch()) == 0) getch();       /* ���� ǡ�i ���e�a.         */
         if(ch == _ESC)                         /* <Esc>�a �����e �{���a.    */
            break;
         else if(ch == _SPACE)                  /* <Space>�a �����e �a�� ǡ  */
         {                                      /* �a ���i ���a�� ���a���a.  */
            if(!getch()) getch();
         }
      }
   }

   if(feof(stream))                             /* <Esc>�i ���a�� �g�� �A��  */
   {                                            /* �� �{�a�� �����i �����a�e */
      set_attr_han(FONT_SHADE);                 /* �a�� ǡ�a ���i ���a��  �� */
      puts_han("<�{>");                         /* �a���a.                   */
      set_attr_han(FONT_INVERSE);
      puts_han("\n\n�a�� ǡ�a ���a�A�a");
      if(!getch()) getch();
   }

   fclose(stream);
   close_han();
}
