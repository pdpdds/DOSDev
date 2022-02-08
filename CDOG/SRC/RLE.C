#define SCREEN_WIDTH  320


int RLESprite( void *sprite, unsigned char *rle )
{
	int x, y, w, h;
	unsigned char *p;
	unsigned short *s;
	int size = 0;
  int transparent;
  int length;
  int first = 1;
	
	s = sprite;
	w = *s++;
	h = *s++;
	p = (unsigned char *)s;

  if (!*p)
  {
    if (rle)
      *rle++ = 0;
    size++;
  }
	for (y = 0; y < h; y++)
	{
		x = 0;
		while (x < w)
		{
      if (*p)
      {
      }
      else
      {
        if (transparent
      }
		}
	}
	size++;
	return size;
}
