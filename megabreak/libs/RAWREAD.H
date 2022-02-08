/*
    Funksjoner for å lese og skrive linka PCX-filer,
    samt funksjoner for å lese inn font-filer.

    Knut 22/1-97
    Sist oppdatert 9/3-97
*/

/*
    Disse rutinene er alt man trenger for å lage et program som linker
    sammen PCX-filer til en stor data-fil. De er ikke spesielt effektive,
    og ikke spesielt godt laget heller, men de skulle virke greit...
*/

int HEIGHT, WIDTH, LENGTH=0, TOTAL_LENGTH=0, IMAGE_COUNTER;

short pcxRawRead(char *filename, byte *rawbuf, signed char *pal);
void pcxDecode(byte *rawdata, byte *image);
void pcxWriteData(char *filename, byte *rawbuf);
void LinkedPCXRead(char *filename, byte *image, short offset);
int pcxinfo(char *filename);
void LinkPCX(char *mainfile, char *pcxfile);
void WritePalette(char *filename, signed char *palette);
void ReadPalette(char *filename, signed char *palette);
//void ReadFont(char *filename, byte font[50]);
byte *dynamicLPCXRead(char *filename, short offset);
void LinkedFileRead(char *filename, byte *data, short offset, short len);
void LinkFile(char *destfile, char *srcfile);
int streamlength(FILE *handle);

struct PCXfile {
    byte Manufacturer;
    byte Version;
    byte Encoding;
    byte BitsPerPixel;
    word Xmin;
    word Ymin;
    word Xmax;
    word Ymax;
    word Hres;
    word Vres;
    byte EgaColors[48];
    byte Reserved;
    byte NumPlanes;
    word BytesPerLine;
    word PaletteInfo;
    word HScreenSize;
    word VScreenSize;
    byte filler[54];
};

int pcxinfo(char *filename)
{
    FILE *fp;
    PCXfile header;

    fp = fopen(filename, "rb");
    if (!fp)
        return 0;

    // Les inn header
    fread(&header, sizeof(header), 1, fp);

    // Kalkuler st¢rrelse på bilde
    WIDTH = header.Xmax - header.Xmin + 1;
    HEIGHT = header.Ymax - header.Ymin + 1;

    fclose(fp);
    return 1;
}


short pcxRawRead(char *filename, byte *rawbuf, signed char *pal)
{
  unsigned char count, col;
  short x, y;
  LENGTH = 0;

  fstream file;
  file.open(filename, ios::in | ios::binary);
  if (file.seekg(-769, ios::end) == 0) return -1;
  file.get(col);
  if (col != 12) return -1;
  if (pal)
  {
	if (file.read(pal, 768) == 0) return -1;

	for(x=0; x<768; x++)
		pal[x]/=4;
  }

  if (rawbuf) {

  file.seekg(128, ios::beg);
  count=0;
  for(y=0; y<HEIGHT; y++)
  {
    for(x=0; x<WIDTH; x++)
	{
	  if(!count)
	  {
		file.get(col);
		*rawbuf++=col;
        LENGTH++;
		if(col>=192)
		{
		  count=col&63;
		  file.get(col);
		  *rawbuf++=col;
          LENGTH++;
		}
		else count=1;
	  }
	  count--;
	}
  }
  } // if

  file.close();
  return 1;
}

void pcxDecode(byte *rawdata, byte *image)
{
  short x, y;
  short count=0;
  short col;

  for(y=0; y<HEIGHT; y++)
  {
    for(x=0; x<WIDTH; x++)
	{
	  if(!count)
	  {
		col = *rawdata++;

		if(col>=192)
		{
		  count=col&63;
		  col = *rawdata++;
		}
		else count=1;
	  }
      *image++ = col;
	  count--;
	}
  }
}

void pcxWriteData(char *filename, byte *rawbuf)
{

    FILE *handle;
    short i, result;
    short upperWord, lowerWord;

    handle = fopen(filename, "r+b");    // Pr¢v å åpne en eksisterende fil

    if (handle == NULL)
        handle = fopen(filename, "w+b"); // Hvis ikke, åpne en ny fil

    fseek(handle, 0, SEEK_END);     // Legg til data på slutten av fil

    upperWord = WIDTH/16;
    lowerWord = WIDTH%16;

    putc(upperWord, handle);
    putc(lowerWord, handle);

    upperWord = HEIGHT/16;
    lowerWord = HEIGHT%16;

    putc(upperWord, handle);
    putc(lowerWord, handle);


    for (i = 0; i < LENGTH; i++)
        putc(*rawbuf++, handle);

    fclose(handle);
}

void LinkedPCXRead(char *filename, byte *image, short offset)
{
    FILE *handle;
    short i;
    short garbage = 0;
    short upperWord, lowerWord;

    handle = fopen(filename, "rb");

    fseek(handle, offset, SEEK_SET);

    upperWord = getc(handle);
    lowerWord = getc(handle);

    WIDTH = (upperWord*16)+lowerWord;

    upperWord = getc(handle);
    lowerWord = getc(handle);

    HEIGHT = (upperWord*16)+lowerWord;

    if (WIDTH % 2)
        garbage = 1;

    short x, y;
    short count=0;
    short col;

    for(y=0; y<HEIGHT; y++)
    {
        for(x=0; x<WIDTH; x++)
        {
            if(!count)
            {
                col = getc(handle);

                if(col>=192)
                {
                    count=col&63;
                    col = getc(handle);
                }
                else count=1;
            }
            *image++ = col;
            count--;
        }
        if (garbage)
            col = getc(handle);
    }

    fclose(handle);
}



void LinkPCX(char *mainfile, char *pcxfile)
{
    static byte rawbuffer[64000];
    short result;

    TOTAL_LENGTH += LENGTH;

    pcxinfo(pcxfile);
    if ((result = pcxRawRead(pcxfile, rawbuffer, 0)) == -1) {
        cout << "Invalid PCX-file" << endl;
        exit(0);
    }

    pcxWriteData(mainfile, rawbuffer);

    LENGTH += 4;

    cout << "Succesfully linked " << pcxfile << " with " << mainfile << endl;
    cout << "Length of linked file: " << LENGTH << endl;
    cout << "Offset into linked file: " << TOTAL_LENGTH << endl;
    cout << endl;

    // Skriv offset til fil
    FILE *handle;

    handle = fopen("offsets.txt", "r+b");
    if (!handle)
        handle = fopen("offsets.txt", "w+b");
    fseek(handle, 0, SEEK_END);

    // Konverter nummer til ASCII og skriv til fil

    int NUM = TOTAL_LENGTH;

    int numlist[10];

    int i;

    for (i=0; i<10; i++)
    {
        numlist[i]=0;
    }
    
    int numindex = 0;

    if (NUM < 10)
    {
        putc(48+NUM, handle);
    }
    else {
        numlist[numindex] = NUM%10;
        NUM = NUM/10;
        numindex++;

        while (NUM > 0 || numlist[numindex] > 0)
        {
            numlist[numindex] = NUM%10;
            NUM = NUM/10;
            numindex++;
        }

        for (i=(numindex-1); i>=0; i--)
        {
            putc(48+numlist[i], handle);
        }
    } // else

    putc(32, handle);

    fclose(handle);

    getch();
}

void WritePalette(char *filename, signed char *palette)
{
    FILE *handle;
    short i;

    handle = fopen(filename, "w+b");

    for (i = 0; i < 768; i++)
        putc(*palette++, handle);

    fclose(handle);
}

void ReadPalette(char *filename, signed char *palette)
{
    FILE *handle;
    short i;

    handle = fopen(filename, "r+b");

    if (!handle) {
        cout << "Slem Palette-fil. Hade." << endl;
        exit(0);
    }

    for (i = 0; i < 768; i++)
        *palette++ = getc(handle);

    fclose(handle);
}

byte *dynamicLPCXRead(char *filename, short offset)
{
    FILE *handle;
    short i;
    short upperWord, lowerWord;
    byte *image;
    byte *imgptr;

    handle = fopen(filename, "rb");

    fseek(handle, offset, SEEK_SET);

    upperWord = getc(handle);
    lowerWord = getc(handle);

    WIDTH = (upperWord*16)+lowerWord;

    upperWord = getc(handle);
    lowerWord = getc(handle);

    HEIGHT = (upperWord*16)+lowerWord;

    LENGTH = WIDTH * HEIGHT;

    image = new byte[LENGTH];
    imgptr = image;

    short x, y;
    short count=0;
    short col;

    for(y=0; y<HEIGHT; y++)
    {
        for(x=0; x<WIDTH; x++)
        {
            if(!count)
            {
                col = getc(handle);

                if(col>=192)
                {
                    count=col&63;
                    col = getc(handle);
                }
                else count=1;
            }
            *image++ = col;
            count--;
        }
    }

    fclose(handle);
    return imgptr;
}












/*


    Funksjonen leser inn en font-fil.

    Den forutsetter at font-array'en er stor nok, og at alle bokstaver er
    like store.
*/
/*
void ReadFont(char *filename, byte font[50])
{
    FILE *handle;
    short i=0, ii;
    short upperWord, lowerWord;

    handle = fopen(filename, "rb");

    fseek(handle, 0, SEEK_SET);

    upperWord = getc(handle);
    lowerWord = getc(handle);

    WIDTH = (upperWord*16)+lowerWord;

    upperWord = getc(handle);
    lowerWord = getc(handle);

    HEIGHT = (upperWord*16)+lowerWord;

    short x, y;
    short count=0;
    short col;

    for (ii=0; ii<50; ii++) {   // Dekod 50 bokstaver...

        for(y=0; y<HEIGHT; y++)
        {
            for(x=0; x<WIDTH; x++)
            {
                if(!count)
                {
                    col = getc(handle);

                    if(col>=192)
                    {
                        count=col&63;
                        col = getc(handle);
                    }
                    else count=1;
                }
                *font[ii]++ = col;

                count--;
            }
        }
    }
    fclose(handle);
}
  */

void LinkedFileRead(char *filename, byte *data, int offset, int len)
{
    FILE *handle;

    handle = fopen(filename, "rb");

    fseek(handle, offset, SEEK_SET);

    for (int i=0; i<len; i++) {
        *data++ = getc(handle);
    }

    fclose(handle);
}

void LinkFile(char *destfile, char *srcfile)
{
    int src, dest;
    int destlen = 0, srclen = 0;
    char garbage;
    byte *buffer;

    // Åpne source + destination filer
    src = open(srcfile, O_RDONLY | O_BINARY);

    dest = open(destfile, O_WRONLY | O_BINARY);
    if (dest == -1)
        dest = open(destfile, O_CREAT | S_IWRITE);

    // Finn lengde på destination fil
    destlen = filelength(dest);

    // Finn lengde på source fil
    srclen = filelength(src);

    buffer = new byte[srclen];

    lseek(src, 0, SEEK_SET);
    lseek(dest, 0, SEEK_END);

    read(src, buffer, srclen);
    write(dest, buffer, srclen);

    cout << "Successfully linked " << srcfile << " with " << destfile << endl;
    cout << "Length of linked file: " << srclen << endl;
    cout << "Offset into linked file: " << destlen << endl << endl;

    close(src);
    close(dest);
}

int streamlength(FILE *handle)
{
    int i=0;
    char garbage;

    while (!feof(handle)) {
        i++;
        handle++;
    }
    return i;
}

short _linkedpcx_read(char *filename, signed int state, image new_image)
{
    FILE *handle;
    short i;
    short upperWord, lowerWord;
    byte *image;
    unsigned int image_length;


    handle = fopen(filename, "rb");
    if (!handle)
        return 0;

    if (!state)                                 // Start på begynnelsen av fil
        fseek(handle, 0, SEEK_SET);

    if (state == -1)                            // Fortsett med påbegynt fil
        fseek(handle, IMAGE_COUNTER, SEEK_SET);

    if (state > 0)
        fseek(handle, state, SEEK_SET);

    upperWord = getc(handle);
    lowerWord = getc(handle);

    new_image.width = (upperWord*16)+lowerWord;

    upperWord = getc(handle);
    lowerWord = getc(handle);

    new_image.height = (upperWord*16)+lowerWord;

    image_length = new_image.width * new_image.height;

    new_image.pointer = new byte[image_length];
    image = new_image.pointer;

    short x, y;
    short count=0;
    short col;

    for(y=0; y<HEIGHT; y++)
    {
        for(x=0; x<WIDTH; x++)
        {
            if(!count)
            {
                col = getc(handle);

                if(col>=192)
                {
                    count=col&63;
                    col = getc(handle);
                }
                else count=1;
            }
            *image++ = col;
            count--;
        }
    }

    fclose(handle);
    return 1;
}

