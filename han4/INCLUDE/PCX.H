/* return value of show_pcx() */
enum
{
   OK_PCX,
   FILE_NOT_FOUND_PCX,
   READ_HEADER_ERROR_PCX,
   NOT_PCX_FILE,
   NOT_16COLOR_PCX,
   NOT_2COLOR_PCX,
   MEMORY_NOT_ENOUGH_PCX
};

/* structure of PCX file header(128 bytes) */
typedef struct
{
   char maker;			  	/* always 10 for PCX file */
   char version;			/* version info */
   char rle;				/* compressed by RLE? */
   char bpp;				/* bits per pixel */
   int x1, y1, x2, y2;			/* position of the picture */
   int hres, vres;			/* resolution */
   unsigned char palette[16][3];	/* palette info for 16 colors */
   char video_mode;			/* video mode */
   char plane_num;			/* number of plane(1,4,8) */
   int bpl;				/* bytes per line */
   int pallette_info;			/* pallette info(0->gray, 1->color) */
   int hres_scan, scan_vres_scan;	/* resolution of the scanner */
   char unused[54];			/* unused bytes */
} pcx_header_struct;

int show_pcx(int x, int y, char *file_name);
