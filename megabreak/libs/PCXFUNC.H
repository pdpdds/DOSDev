/*
  PCX-functions  V 1.1

  Nytt: Lader nå inn filer med odde bredder i tillegg.

  Last update: 17/4-97
*/


int pcxRead(char *filename, byte *buf, signed char *pal, int width, int height);
short dynamicPCXRead(char *filename, image *new_image);

int pcxRead(char *filename, byte *buf, signed char *pal, int width, int height)
{
  unsigned char count, col;
  int x, y;
  short garbage = 0;

  if (width % 2)
	garbage = 1;

  fstream file;
  file.open(filename, ios::in | ios::binary);
  if (file.seekg(-769, ios::end) == 0) return 0;
  file.get(col);
  if (col != 12) return 0;
  if (pal)
  {
	if (file.read(pal, 768) == 0) return 0;

	for(x=0; x<768; x++)
		pal[x] = (byte)pal[x]/4;
  }

  file.seekg(128, ios::beg);
  count=0;
  if (buf) {
  for(y=0; y<height; y++)
  {
	for(x=0; x<width; x++)
	{
	  if(!count)
	  {
		file.get(col);
		if(col>=192)
		{
		  count=col&63;
		  file.get(col);
		}
		else count=1;
	  }
	  *buf++=col;
	  count--;
	}
	if (garbage)
		file.get(col);
  }
  }
  return 1;
}

short dynamicPCXRead(char *filename, image *new_image)
{
	int result;

	pcxinfo(filename);

	LENGTH = WIDTH * HEIGHT;

    new_image -> height = HEIGHT;
    new_image -> width = WIDTH;

    new_image -> pointer = new byte[LENGTH];
    if (!new_image -> pointer)
		return 0;

    result = pcxRead(filename, new_image -> pointer, 0, WIDTH, HEIGHT);
	if (!result)
		return 0;

	return 1;
}

