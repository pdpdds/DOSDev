/*
	Yet another fortune cookie type program.
	I wrote this because I wanted one that didn't need a pointer
	file, didn't require ANSI.SYS loaded, didn't write files to
	disk and didn't crash '386 machines, and I couldn't find one.
	The data file should be a standard text file with each fortune
	starting with a # character.

	The fortunes have been collected from a variety of sources.

	Peter Summers - 24/3/91
*/


#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <limits.h>
// For Borland C
#include <io.h>
#include "KorHdr.h"

int main(const int argc, const char *argv[])
{
  int handle, numread;  long offset;
  char	*extension, *buffer, *outstring;

  char* datafile=(char*) calloc(128, sizeof(char));
  if (argc>1) strcpy(datafile,argv[1]);
  else
  {
    strcpy(datafile, argv[0]);
    extension =strrchr(datafile,'.');
    if (extension==NULL)
    {
      cprintf("%s","No '.' found in argc[0] in %s", argv[0]); exit(1);
    } else
      { strcpy(extension,".DAT"); }
  }

  handle=open(datafile,O_RDONLY|O_BINARY,S_IREAD);

  if (handle==-1)
  {
   cprintf("Can't open data file %s in WISDOM", datafile); exit(1);
  }

  randomize();
  offset=((long)random(INT_MAX)*INT_MAX+random(INT_MAX))%filelength(handle);
  buffer=(char*)calloc(4097, sizeof(char));
  lseek(handle, offset, SEEK_SET);
  numread=read(handle, buffer, 4096);

  if (numread==-1)
  {
   cprintf("Can't read from data file %s in WISDOM", datafile); exit(1);
  }

  if (numread < 4096)
  {
    lseek(handle, 0, SEEK_SET);
    read(handle, &buffer[numread], 4096 - numread);
  }

  outstring =strchr(buffer, '#') + 1;
  *strchr(outstring,'#') =0;
#if TEXT_MODE
  textcolor(3);
  cprintf("%s", outstring);
#else
  graph_init(-1);
  gColor(0, 3);
  gputs(3, 12, 1, outstring);
  (void)getchar();
  graph_close();
#endif

  return(0);
}
