#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#include <i86.h>
#include "grafx.h"
#include "sprcomp.h"


//static int oldMode;


void Mode13h( void )
{
  union REGS regs;

  regs.w.ax = 0x13;
  int386( 0x10, &regs, &regs);
}

void TextMode( void )
{
  union REGS regs;

  regs.w.ax = 3;
  int386( 0x10, &regs, &regs);
}

int ReadPics( const char *filename, void **pics, int maxPics, color *palette )
{
  int f;
  unsigned short int size;
  int i = 0;

  f = open( filename, O_RDONLY | O_BINARY);
  if (f >= 0)
  {
    if (palette)
      read( f, palette, sizeof( TPalette));
    else
      lseek( f, sizeof( TPalette), SEEK_CUR);
    while (!eof( f) && i < maxPics)
    {
      read( f, &size, sizeof( size));
      if (size)
      {
        pics[i] = malloc( size);
        read( f, pics[i], size);
      }
      else
        pics[i] = NULL;
      i++;
    }
    close( f);
  }
  return i;
}

int AppendPics( const char *filename, void **pics, int startIndex, int maxPics )
{
  int f;
  unsigned short int size;
  int i = startIndex;

  f = open( filename, O_RDONLY | O_BINARY);
  if (f >= 0)
  {
    lseek( f, sizeof( TPalette), SEEK_CUR);
    while (!eof( f) && i < maxPics)
    {
      read( f, &size, sizeof( size));
      if (size)
      {
        pics[i] = malloc( size);
        read( f, pics[i], size);
      }
      else
        pics[i] = NULL;
      i++;
    }
    close( f);
  }
  return i - startIndex;
}

int CompilePics( int picCount, void **pics, void **compiledPics )
{
  int i, size, total = 0;
  int skipped = 0;

  for (i = 0; i < picCount; i++)
  {
    if (pics[i])
    {
      size = compileSprite( pics[i], NULL);
      total += size;
      if (size)
      {
        compiledPics[i] = malloc( size);
        compileSprite( pics[i], compiledPics[i]);
      }
      else
      {
        compiledPics[i] = NULL;
        skipped++;
      }
    }
    else
      compiledPics[i] = NULL;
  }
  if (skipped)
    printf( "%d solid pics not compiled\n", skipped);
  return total;
}

int RLEncodePics( int picCount, void **pics, void **rlePics )
{
  int i, size, total = 0;
  int skipped = 0;

  for (i = 0; i < picCount; i++)
  {
    if (pics[i])
    {
      size = RLEncodeSprite( pics[i], NULL);
      total += size;
      if (size)
      {
        rlePics[i] = malloc( size);
        RLEncodeSprite( pics[i], rlePics[i]);
      }
      else
      {
        rlePics[i] = NULL;
        skipped++;
      }
    }
    else
      rlePics[i] = NULL;
  }
  if (skipped)
    printf( "%d solid pics not RLE'd\n", skipped);
  return total;
}

void vsync( void )
{
  while ((inp( 0x03da) & 8) != 0);
  while ((inp( 0x03da) & 8) == 0);
}

int PicWidth( void *pic )
{
  if (!pic)
    return 0;
  return ((short *)pic)[0];
}

int PicHeight( void *pic )
{
  if (!pic)
    return 0;
  return ((short *)pic)[1];
}
