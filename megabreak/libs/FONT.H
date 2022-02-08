/*
    Knut's Font-library

    12/3-97
*/

#include "standard.h"
#include <stdio.h>

// For å gjøre om til KSCII-format


char conversion_table[91] = {   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0,51, 0, 1, 0, 0, 2, 0, 3, 4,
                                5, 0, 6, 7, 8, 9,10,11,12,13,
                                14,15,16,17,18,19,20,21, 0, 0,
                                0, 0,22, 0,23,24,25,26,27,28,
                                29,30,31,32,33,34,35,36,37,38,
                                39,40,41,42,43,44,45,46,47,48   };

int FONT_WIDTH = 0;
int FONT_HEIGHT = 0;

// Prototyper

short load_font(char *filename, byte *fontdata[50]);
void set_font_dimensions(int width, int height);
void write_char(char item, int x, int y, byte *fontdata[50]);
void write(char *text, int x, int y, byte *fontdata[50]);
void center_write(char *text, int x, int y, int xlen, byte *fontdata[50]);
void num_fwrite(int number, int x, int y, int total_digits, byte *fontdata[50]);


/*
    Allokerer minne for, lader inn og dekoder en font fil.
*/

short load_font(char *filename, byte *fontdata[50])
{
    FILE *handle;
    short i, ii, c;
    short size;
    short x, y;
    short count=0;
    short col;
    short width, height;
    short upperWord, lowerWord;

    handle = fopen(filename, "rb");

    fseek(handle, 0, SEEK_SET);

    upperWord = getc(handle);
    lowerWord = getc(handle);
    width = (upperWord*16)+lowerWord;

    upperWord = getc(handle);
    lowerWord = getc(handle);
    height = (upperWord*16)+lowerWord;

    size = width * height;

    for (i=0; i<50; i++) {
        fontdata[i] = new byte[size];
        if (!fontdata[i])
            return 0;
    }

    for (ii=0; ii<50; ii++) {           // Dekod 50 bokstaver...

        c=0;
        for(y=0; y<height; y++) {
            for(x=0; x<width; x++) {
                if(!count) {
                    col = getc(handle);

                    if(col>=192) {
                        count=col&63;
                        col = getc(handle);
                    }
                    else count=1;
                }
                *(fontdata[ii] + c++) = col;

                count--;
            }
        }

        // Vi vet allerede størrelsen, så dette kastes bort
        upperWord = getc(handle);
        upperWord = getc(handle);
        upperWord = getc(handle);
        upperWord = getc(handle);
    }
    fclose(handle);

    return 1;
}

void set_font_dimensions(int width, int height)
{
    FONT_WIDTH = width;
    FONT_HEIGHT = height;
}

void write_char(char item, int x, int y, byte *fontdata[50])
{
    put_block(x, y, FONT_WIDTH, FONT_HEIGHT, fontdata[item]);
}


void write(char *text, int x, int y, byte *fontdata[50])
{
    int stringlength, i;
    char *newstring;

    stringlength = strlen(text);

    newstring = new char[stringlength];

    // Konverter fra ASCII -> KSCII
    for (i=0; i<stringlength; i++)
        *(newstring + i) = conversion_table[text[i]];

    // Blit text til skjermen
    for (i=0; i<stringlength; i++) {
        if (*(newstring + i) < 51) {
            put_block(x+(i*FONT_WIDTH), y, FONT_WIDTH, FONT_HEIGHT, fontdata[*(newstring + i)]);
            x++;    // En pixel mellomrom mellom bokstavene
        }
    }

    delete newstring;
}

void center_write(char *text, int x, int y, int xlen, byte *fontdata[50])
{
    int stringlength, grlength;

    stringlength = strlen(text);
    grlength = (FONT_WIDTH * stringlength) + (1 * stringlength);

    if (grlength < xlen) {
        x += (xlen/2) - (grlength/2);
        write(text, x, y, fontdata);
    }
}

void num_fwrite(int number, int x, int y, int total_digits, byte *fontdata[50])
{
    int i;

    short *digits;

    digits = new short[total_digits];

    // Formater tall
    for (i=(total_digits - 1); i>=0; i--) {
        *(digits + i) = number % 10;
        number /= 10;
    }

    // ASCII -> KSCII
    for (i=0; i<total_digits; i++)
        *(digits + i) = (conversion_table[(*(digits + i)+48)]);

    // Skriv ut tall
    for (i=0; i<total_digits; i++) {
        put_block(x+(i*FONT_WIDTH), y, FONT_WIDTH, FONT_HEIGHT, fontdata[*(digits + i)]);
        x++;
    }

    delete digits;
}

