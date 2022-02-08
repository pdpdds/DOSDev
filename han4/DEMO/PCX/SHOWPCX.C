#include <stdio.h>
#include "hanio.h"
#include "pcx.h"

int show_pcx_hgc(int x, int y, pcx_header_struct *header, FILE *stream);
int show_pcx_vga(int x, int y, pcx_header_struct *header, FILE *stream);

int show_pcx(int x, int y, char *file_name)
{
   pcx_header_struct header;
   FILE *stream;
   int ret;

   /* open PCX file */
   if((stream = fopen(file_name, "rb")) == NULL) return(FILE_NOT_FOUND_PCX);
   /* read PCX file header */
   if(fread(&header, sizeof(header), 1, stream) != 1)
      ret = READ_HEADER_ERROR_PCX;
   else if(header.maker != 10)
   /* check if it is a valid PCX file. */
      ret = NOT_PCX_FILE;
   else if(in_vga_system)
   /* check video mode */
   {
      if(header.plane_num != 4)
	 ret = NOT_16COLOR_PCX;
      else
	 ret = show_pcx_vga(x, y, &header, stream);
   }
   else
   {
      if(header.plane_num != 1)
	 ret = NOT_2COLOR_PCX;
      else
	 ret = show_pcx_hgc(x, y, &header, stream);
   }
   fclose(stream);
   return(ret);
}
