/*
    Diverse funksjoner til MegaBreak og Level Editor

    Q 3/1-95
*/

void brick_load(char *Filename, byte *Brickmap);
void brick_save(char *Filename, byte *Brickmap);
//void draw_bricks();

const short FILE_SIZE = 260;    // Et brett tar 260 bytes (13*20)

void brick_load(char *Filename, byte *Brickmap)
{
    FILE *handle;
    int i;

    handle = fopen(Filename, "r+b");

    if (handle==NULL)
        handle = fopen(Filename, "w+b");
    else {
        for (i=0; i<FILE_SIZE; i++)
        {
            Brickmap[i] = fgetc(handle);
        }
    }
    fclose(handle);
}

void brick_save(char *Filename, byte *Brickmap)
{
    FILE *handle;
    int i;

    handle = fopen(Filename, "wb");

    for (i=0; i<FILE_SIZE; i++)
    {
        fputc(Brickmap[i], handle);
    }

    fclose(handle);
}
