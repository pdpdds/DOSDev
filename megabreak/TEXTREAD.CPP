#include <dos.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "..\mylibs\stdtext.h"

static char text[390][82];
FILE *handle;
int num_lines;

const int SINGLE = 1;
const int PAGE = 22;

void open_file(char *filename);
void view_text(int starty, int endy);
void colour_line(int y, char *startup_text);


void main()
{
    char *top_bar    = " [MegaBreak Manual]    -        KnutReader v 1.0 Copyright ¸ 1998 Knut A. Ruud  \0";
    char *bottom_bar = " Keys: [] [] [Page up] [Page down] [Home] [End]          Press [Esc] to quit  \0";

    int starty, endy;
    signed int tinc = 0;
    char key;

    open_file("manual.txt");

    // init

    starty = 1;
    endy = 23;

    system("mode co80");
    set_cursor(0,25);
    view_text(0, 23);
    cls();
    colour_line(0, top_bar);
    colour_line(24, bottom_bar);
    view_text(starty, endy);

    // Mainloop

    while (key != 27) {
        if (kbhit()) {
            key = getch();

            switch (key) {
                case 72:                // up
                    if (starty > 0) {
                        starty -= 1;
                        endy -= 1;
                    }
                    break;
                case 80:                // down
                    if (endy < num_lines) {
                        starty += 1;
                        endy += 1;
                    }
                    break;
                case 73:                // page up
                    if ((starty - PAGE) > 0) {
                        starty -= PAGE;
                        endy -= PAGE;
                    }
                    else {
                        starty = 0;
                        endy = 23;
                    }
                    break;
                case 81:                // page down
                    if ((endy + PAGE) < num_lines) {
                        starty += PAGE;
                        endy += PAGE;
                    }
                    else {
                        endy = num_lines;
                        starty = endy - 23;
                    }
                    break;
                case 71:                // home
                    starty = 0;
                    endy = 23;
                    break;
                case 79:                // end
                    endy = num_lines;
                    starty = endy - 23;
            } // switch

            view_text(starty, endy);
        } // if kbhit
    }
    system("cls");
}

void open_file(char *filename)
{
    handle = fopen(filename, "r");

    if (!handle) {
        cout << "Error opening file" << endl;
        exit(0);
    }

    int x = 0, y = 0, still_read;
    char letter;
    
    for (y = 0; y < 500; y++) {

        x=0;
        still_read = 1;

        while (still_read) {
            letter = getc(handle);

            if (letter == 10) {
                still_read = 0;
                text[y][x] = 0;
            }
            else if (letter == 255) {
                num_lines = --y;
                y = 10000;
                still_read = 0;
            }
            else {
                text[y][x] = letter;
                ++x;
            }
        } // while
    } // for

    fclose(handle);
} // function

void view_text(int starty, int endy)
{
    cls();
    int yy = 1;

    for (int y = starty; y < endy; y++) {
        print_line(yy, text[y], 1, 15);
        yy++;
    }
}

void colour_line(int y, char *startup_text)
{
    int i = 0, x = 0;
    byte bg_color, fg_color;

    x = 0;
    i = 0;
    while ( *(startup_text + i) != '\0') {
        if ( *(startup_text + i) < 20 || *(startup_text + i) == 143) {
            fg_color = *(startup_text + i);
            i++;
            bg_color = *(startup_text + i);
            i++;
        }
    print_character(x, y, *(startup_text + i), bg_color, fg_color);
    x++;
    i++;
    }
}

