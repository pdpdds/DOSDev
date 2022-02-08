#include <graphics.h>
#include "hanio.h"

void main(void)
{
   if(init_han() != OK_HAN)
   {
      puts("Can\'t initialize the HANGUL environment.");
      exit(1);
   }

   puts_han("Ðe‹i·¡ Â‰b–A´ö¯s“¡”a.");
   circle(100, 100, 100);
   getch();
   close_han();
}
