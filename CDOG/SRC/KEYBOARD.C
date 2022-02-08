#include <string.h>
#include <conio.h>
#include <dos.h>
#include "keyboard.h"


char *keyNames[256] =
{ /* $00 */ NULL, "Esc", "1", "2", "3", "4", "5", "6",
  /* $08 */ "7", "8", "9", "0", "+", "Apostrophe", "Backspace", "Tab",
  /* $10 */ "Q", "W", "E", "R", "T", "Y", "U", "I",
  /* $18 */ "O", "P", "è", "^", "Enter", "Left Ctrl", "A", "S",
  /* $20 */ "D", "F", "G", "H", "J", "K", "L", "ô",
  /* $28 */ "é", "Paragraph", "Left shift", "'", "Z", "X", "C", "V",
  /* $30 */ "B", "N", "M", ",", ".", "-", "Right shift", "* (pad)",
  /* $38 */ "Alt", "Space", "Caps Lock", "F1", "F2", "F3", "F4", "F5",
  /* $40 */ "F6", "F7", "F8", "F9", "F10", "Num Lock", "Scroll Lock", "7 (pad)",
  /* $48 */ "8 (pad)", "9 (pad)", "- (pad)", "4 (pad)", "5 (pad)", "6 (pad)", "+ (pad)", "1 (pad)",
  /* $50 */ "2 (pad)", "3 (pad)", "0 (pad)", ", (pad)", "SysRq", NULL, "<", "F11", "F12",
  /* $59 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $60 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $70 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $80 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $90 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Enter (pad)", "Right Ctrl", NULL, NULL,
  /* $A0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $B0 */ NULL, NULL, NULL, NULL, NULL, "/ (pad)", NULL, "PrtScr", "Alt Gr", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $C0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Home",
  /* $C8 */ "Up arrow", "Page Up", NULL, "Left arrow", NULL, "Right arrow", NULL, "End",
  /* $D0 */ "Down arrow", "Page Down", "Insert", "Delete", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $E0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $F0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


static void __far *oldInterrupt9 = NULL;
static volatile unsigned char uKeys[256];
static unsigned char xFlag;
static unsigned char code;


void __interrupt __far Interrupt9( void )
{
  _disable();
  code = inp( 0x60);
  if (code == 0xE0)
    xFlag = 128;
  else
  {
    uKeys[ (code & 127) + xFlag] = ((code & 128) != 0 ? 0 : 1);
    xFlag = 0;
  }
  outp( 0x20, 0x20);
  _enable();
}

void InstallKbdHandler( void )
{
  oldInterrupt9 = _dos_getvect( 0x09);
  _dos_setvect( 0x09, Interrupt9);
  ClearKeys();
}

void RemoveKbdHandler( void )
{
  if (oldInterrupt9)
    _dos_setvect( 0x09, oldInterrupt9);
  oldInterrupt9 = NULL;
}

int KeyDown( int key )
{
  return uKeys[ key];
}

int GetKeyDown( void )
{
  int i;

  for (i = 0; i < 256; i++)
    if (uKeys[i] && keyNames[i])
      return i;
  return 0;
}

int AnyKeyDown( void )
{
  return (GetKeyDown() > 0);
}

void ClearKeys( void )
{
  memset( &uKeys, 0, sizeof( uKeys));
}
