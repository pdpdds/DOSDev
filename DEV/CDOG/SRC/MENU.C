#include "grafx.h"
#include "text.h"
#include "actors.h"


void ShowControls( void )
{
  TextStringAt( 50, 190, "(use player 1 controls or arrow keys + Enter/Backspace)");
}

void DisplayMenuItem( int x, int y, const char *s, int selected )
{
  if (selected)
    TextStringWithTableAt( x, y, s, tableFlamed);
  else
    TextStringAt( x, y, s);
}

void DisplayMenu( int x, const char **table, int count, int index )
{
  int i;

  for (i = 0; i < count; i++)
    DisplayMenuItem( x, 50 + i*TextHeight(), table[i], i == index);
}


