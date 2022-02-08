#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char string1[] = "Ðe‹i·e ë¦ó•äÞïmA¬á ôÜóBÐa­v”a.", string2[128];
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* string1·i Ðe¸aŸi ¬a¶wÐaµa Â‰b */
   xputs_han(0, 0, string1);

   /* string1 · ¡¡—e Ðe¸aŸi Ðe‹i¡ ¥eÑÅ */
   i = 0;
   while(i < strlen(string1))
   {
      if((unsigned char) string1[i] >= 0xe0)
      /* Ðe¸a·¡¡e Ðe‹i¡ ¥eÑÅ¯¡Ç¥”a. */
      {
         hanja_to_hangul(string1 + i, string2 + i);
         i++;
      }
      else if(string1[i] & 0x80)
      /* Ðe‹i·¡¡e 2¤a·¡ËaŸi ¥¢¬aÐe”a. */
      {
         string2[i++] = string1[i];
         string2[i]   = string1[i];
      }
      else
      /* µw¢…¸a·¡¡e 1¤a·¡ËaŸi ¥¢¬aÐe”a. */
         string2[i] = string1[i];
      i++;
   }
   /* string2µA é ¢…¸aŸi ”õ¦›·¥”a. */
   string2[i] = 0;

   /* Ðe‹i¡ ¥eÑÅ–E string2Ÿi Â‰b */
   xputs_han(0, 1, string2);

   xputs_han(0, 4, "´a¢ Ç¡a ’Ÿa­A¶a.. ");
   if(!getch()) getch();
   close_han();
}
