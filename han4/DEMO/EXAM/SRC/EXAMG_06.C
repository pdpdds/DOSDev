#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   unsigned char keyflag, org_keyflag;

   if(init_han() != OK_HAN) exit(1);

   /* �e���� ǡ���a �i���a�i ��w�e�a. */
   org_keyflag = get_keyflag_han();

   /* 1 �e�� : get_keyflag_han(), is_keyflag_han() */

   puts_han("�e�� ǡ���a �i���a�� �wȁ�e �a�q�� �{�s���a.");
   puts_han("�a�q �e���� ���a�a�e �E�� ���a�a ǡ�� ���e�� "
            "���a�a ǡ�i �q�A ���a�A�a..");
   puts_han("\n<Ins mode> <CapsLock> <NumLock> <ScrlLock> "
             "<Alt> <Ctrl> <LeftShift> <RightShift>");
   while(1)
   {
      keyflag = get_keyflag_han();
      /* <LeftShift> ǡ�� <RightShift> ǡ�a �����A ���a���e */
      /* ���a�i �a�a �a�e�a.                                */
      if(is_keyflag_han(LEFT_SHIFT | RIGHT_SHIFT)) break;
      gotoxy_han(0, 4);
      /* ǡ���a �i���a�� �b ���a�i �a���e�a. */
      /* <INS> ǡ�a ���v�e�� ��a�a�a�e INS_MODE�� ���a AND  */
      /* Ё�� �i �� ���a. INS�� ���a AND�a�e �e �E�a.        */
      printf_han("%5d %10d %9d %10d %8d %6d %8d %12d",
                 (keyflag & INS_MODE)    ? 1 : 0,
                 (keyflag & CAPS_LOCK)   ? 1 : 0,
                 (keyflag & NUM_LOCK)    ? 1 : 0,
                 (keyflag & SCRL_LOCK)   ? 1 : 0,
                 (keyflag & ALT)         ? 1 : 0,
                 (keyflag & CTRL)        ? 1 : 0,
                 (keyflag & LEFT_SHIFT)  ? 1 : 0,
                 (keyflag & RIGHT_SHIFT) ? 1 : 0);
   }

   /* 2 �e�� : set_keyflag_han() */

   puts_han("\n\n<NumLock>, <CapsLock>, <ScrlLock> ǡ�a ���� "
            "���a�� ����� �e�i�� ���V�s���a.");
   set_keyflag_han(NUM_LOCK | CAPS_LOCK | SCRL_LOCK);
   puts_han("�a�q �e���� ���a�a�e �E�� ���a�a ǡ�� ���e�� "
            "���a�a ǡ�i �q�A ���a�A�a..");
   /* ǡ���a �i���a�i �������e�a. */
   while(!is_keyflag_han(LEFT_SHIFT || RIGHT_SHIFT)) ;

   /* 3�e�� : toggle_keyflag_han(); */

   puts_han("\nǡ���a�A ���e ���a�i �q�b��a ���V�s���a.");
   puts_han("�a�q �e���� ���a�a�e �E�� ���a�a ǡ�� ���e�� "
            "���a�a ǡ�i �q�A ���a�A�a..");
   /* ǡ���a �i���a�i �������e�a. */
   set_keyflag_han(0);
   /* ���a�a ���e ���a�i ���� ɡ�i�e�a. */
   while(!is_keyflag_han(LEFT_SHIFT || RIGHT_SHIFT))
   {
      toggle_keyflag_han(NUM_LOCK);
      delay(300);
      toggle_keyflag_han(CAPS_LOCK);
      delay(300);
      toggle_keyflag_han(SCRL_LOCK);
      delay(300);
   }

   /* ǡ���a �i���a�i �����e�a. */
   set_keyflag_han(org_keyflag);
   close_han();
}
