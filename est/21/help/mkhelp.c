#include <stdio.h>
#include <dos.h>
#include <est.h>

unsigned long loctbl[100];
const byte the21[]= {"Word Processor 21st Century Help File V1.0\x1a\x5\x4\x3\x2\x1" };

int main(int argc, char* argv[])
{
  FILE *in, *out;
  byte ib=0;
  int num;
  unsigned long loc=0;

  if((in=fopen("21.DAT", "rb"))==NULL) {
    printf("Cannot open 21.DAT\n");
    exit(-1);
  }
  if((out=fopen("21.$$$", "wb+"))==NULL) {
    printf("Cannot make 21.$$$\n");
    exit(-1);
  }

  loc=448;
  loctbl[0]=loc;
  num=1;
  while(!feof(in)) {
    ib=fgetc(in);
    if(ib=='`') {
      loctbl[num]=loc;
      num++;
      continue;
    }
    if(ib=='\r') ib=0;
    else if(ib=='\n') continue;
    loc++;
    fputc(ib, out);
  }
  fclose(in);
  fclose(out);
  if((in=fopen("21.$$$", "rb"))==NULL) {
    printf("Cannot open 21.$$$\n");
    exit(-1);
  }
  if((out=fopen("21.HLP", "wb+"))==NULL) {
    printf("Cannot make 21.HLP\n");
    exit(-1);
  }
  fwrite(the21, 48, 1, out);
  fwrite(loctbl, 100, 4, out);
  while(1) {
    ib=fgetc(in);
    if(feof(in)) break;
		fputc(ib, out);
  }
  fclose(in);
  fclose(out);
}