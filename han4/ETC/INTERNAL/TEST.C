#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   int gr_mode;

   set_internal_font();
   if((gr_mode = choose_gr_mode()) == ESC) exit(1);
   if(_init_han2(gr_mode) != OK_HAN) exit(1);

   puts_han("ͥ�a ���w ���� �������a.");
   puts_han("�e�i�� english�a �i �a���a?");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   _close_han2(gr_mode);
}
