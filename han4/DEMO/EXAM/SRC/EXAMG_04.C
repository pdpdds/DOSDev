#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   unsigned char keyflag;
   int ascii, scan;

   if(init_han() != OK_HAN) exit(1);

   puts_han("�a�� ǡ�a ���a�e �a ǡ�� �a�aǡ š�a�� �aą š�a�i "
            "�i�a ���e �a���a�������a.");
   puts_han("���e�a�� ���a�e <Left Shift> ǡ�� <End> ǡ�i �q�A ���a�A�a.");

   while(1)
   {
      scan = get_key_han(&ascii);
      keyflag = get_keyflag_han();
      if(scan == END && (keyflag & LEFT_SHIFT) != 0) break;
      printf_han("�a�aǡ š�a = %02x  �aą š�a = %02x\n", ascii, scan);
   }

   close_han();
}
