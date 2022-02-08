#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/* 1. ¸»¡Ð·–I ¯aËaŸ±·i ¸÷·Ðe”a. */
FILE *redirect_stream;

/* Ðq® ¶¥Ñw */
void my_redirect_func(char *string);

void main(int argc, char **argv)
{
   int redirect;

   /* 2. ¡wwÐ— ·¥¸aŸi ¹¡¬aÐaµa "list9_10 to <filename>"‰Á  ˆ{·e */
   /*    Ñw¯¢·a¡ list9_10·i ¯©Ð—¯¡Åv”a¡e <filename>·i µ¡ÏeÐe”a. */
   /*    ·¡ ˜ ¸»¡Ð· ‹¡Ñ¡“e "to"·¡”a.                           */
   if(argc == 3 && !strcmp(argv[1], "to"))
   {
      if((redirect_stream = fopen(argv[2], "wt")) == NULL)
      {
         printf("Can't write to %s", argv[2]);
         exit(1);
      }
      else
         redirect = 1;
   }
   else
      redirect = 0;
   if(init_han() != OK_HAN) exit(1);

   puts_han("”a·q 5º‰ e Â‰b·¡ ¸»¡Ð·–S“¡”a.\n");
   /* 3. ¸åµb ¥e®  redirect_func·¡ ¸»¡Ð·–I ¯aËaŸ±·i ÀáŸ¡Ða“e */
   /*    my_redirect_func()·i ˆaŸ¡Ç¡•¡¢ Ðe”a. »¡‹q¦Èá Â‰b–A */
   /*    “e •A·¡Èá“e ¡¡– my_redirect_func·a¡ ¸å”i–E”a.       */
   if(redirect) redirect_func = my_redirect_func;
   /* printf_han(), puts_han(), cputs_han() ¡¡– ¸»¡Ð··¡ ˆa“w */
   /* Ðq·i ¥¡µa º…”a.                                          */
   printf_han("*****\n");
   puts_han("Â‰b ¸»¡Ð··i ·¡¶wÐa¡e Ïa¡‹aœ‘·");
   puts_han("¯©Ð— ‰i‰ÁŸi ¤a¡ ÑÁ·©·¡a ÏaŸ¥Èá¡");
   puts_han("Â‰bÐi ® ·¶¯s“¡”a.");
   cputs_han("*****\n");
   /* Â‰b–A“e •A·¡Èáˆa ´a¢ Ðq®µA•¡ ¸å”i–A»¡ ´g•¡¢ Ðe”a. */
   if(redirect) redirect_func = NULL;
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}

void my_redirect_func(char *string)
/* 4. ¸»¡Ð·–E •A·¡ÈáŸi ÀáŸ¡Ða“e Ðq®Ÿi  e—e”a. */
{
   /* ¸»¡Ð·–E •A·¡Èá string·i redirect_stream·a¡ Â‰bÐe”a. */
   fputs(string, redirect_stream);
}
