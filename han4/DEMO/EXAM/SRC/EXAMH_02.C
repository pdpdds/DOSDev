#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   puts_han("���a�i�i ���b�a�A�a. ���e �a�a�a�� �g�s���a.");
   hide_cursor_han();
   gets_han(string);

   puts_han("���a�i�i ���b�a�A�a. ���a �a�a�s���a.");
   show_cursor_han();
   gets_han(string);

   close_han();
}
