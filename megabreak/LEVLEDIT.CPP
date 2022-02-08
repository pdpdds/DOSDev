/*
    MEGABREAK LEVEL EDITOR

    COPYRIGHT (C) 1997 Knut A. Ruud
*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "libs\w_mlib.h"    // Mouse-library
#include "libs\gfxlib.h"   // Gfx-header
#include "libs\imageio.h"
#include "libs\stdtext.h"
#include "brkfunc.h"




void mouse_to_brick(short &bx, short &by);
void mouse_to_grid(short &gx, short &gy);
void add_brick(short x, short y, short gx, short gy, short type);
void del_brick(short x, short y, short gx, short gy);
void draw_rectangle(short x, short y, short xlen, short ylen, short col);
void feedback(short x, short y);
void handle_buttons();
void draw_bricks();


const short TRUE = 1;
const short FALSE = 0;


short bricktype = 0;
short buttonX, buttonY;
short quit;
static byte bricks[15][96];     // Bilde-data til hver brikke
static byte brickmap[286];
char filename[13];


void main(int argc, char *argv[])
{
    // ** Variabler

    short brickX=0, brickY=0;
    short gridX=0, gridY=0;
    short key, i=0;
    static signed char palette[768];
    static byte mainscreen[64000];

    // Kopier filnavn uten å bruke strcpy (sparer plass)

    while (argv[1][i] != 0) {
        filename[i] = argv[1][i];
        i++;
    }

    cout << "MegaBreak Level-Editor V 1.0\n\n";

    if (argc != 2) {
        cout << "\nYou must specify the name of the file you want to \n";
        cout << "create or edit.\n\n";
        cout << "Usage:     levledit <filename.map>\n\n";
        cout << "Example:   levledit newlevel.map\n";

        exit(0);
    }

        // ** Initialiser



    LinkedPCXRead("gfx.dat", bricks[0], 60);        // ** BRICKS        **
    LinkedPCXRead("gfx.dat", bricks[1], 86);
    LinkedPCXRead("gfx.dat", bricks[2], 112);
    LinkedPCXRead("gfx.dat", bricks[3], 138);
    LinkedPCXRead("gfx.dat", bricks[4], 164);
    LinkedPCXRead("gfx.dat", bricks[5], 190);
    LinkedPCXRead("gfx.dat", bricks[6], 290);
    LinkedPCXRead("gfx.dat", bricks[7], 390);
    LinkedPCXRead("gfx.dat", bricks[8], 490);
    LinkedPCXRead("gfx.dat", bricks[9], 590);
    LinkedPCXRead("gfx.dat", bricks[13], 711);
    LinkedPCXRead("gfx.dat", bricks[14], 761);


    LinkedPCXRead("gfx.dat", mainscreen, 20055);      // ** MAINSCREEN    **

    ReadPalette("megbreak.pal", palette);


    // Reset brickmap
    memset(brickmap, 11, 286);

    brick_load(filename, brickmap);
    
    mode_13h();
    set_pal(palette);
    put_block(0, 0, 320, 200, mainscreen);
    draw_bricks();
    buttonX = 238;
    buttonY = 101;
    draw_rectangle(238, 101, 17, 7, 1);
    put_block(294, 77, 16, 6, bricks[bricktype]);
    show_mouse();

    while (quit == FALSE) {

        get_mouse_status();

        if ((MOUSE_Y <= 151) && (MOUSE_Y >= 12)
                    && (MOUSE_X/2 > 4) && (MOUSE_X/2 < 224)) {

            mouse_to_brick(brickX, brickY);
            mouse_to_grid(gridX, gridY);
    
            if (BUTTON_STATE > 0) {
                if (BUTTON_STATE == 1)      // Legg til brikke
                    add_brick(brickX, brickY, gridX, gridY, bricktype);

                if (BUTTON_STATE == 2)      // Slett brikke
                    del_brick(brickX, brickY, gridX, gridY);
            }
        }

        if (kbhit()) {

            key = getch();

            if (key == 27)      // ESC  - Quit
                quit = TRUE;
            if (key == 83 || key == 115)    // S/s  - Save
                brick_save(filename, brickmap);
        }

        handle_buttons();

    }

    text_mode();
    reset_mouse();
}

void mouse_to_brick(short &bx, short &by)
{
    bx = (((MOUSE_X/2)-4)/17*17)+4;
    by = ((MOUSE_Y-12)/7*7)+12;
}

void mouse_to_grid(short &gx, short &gy)
{
    gx = (((MOUSE_X/2)-4)/17+1);
    gy = ((MOUSE_Y-12)/7);
}

void add_brick(short x, short y, short gx, short gy, short type)
{
    hide_mouse();
    put_block(x, y, 16, 6, bricks[type]);
    brickmap[gy*13+(gx-1)] = type;
    show_mouse();
}

void del_brick(short x, short y, short gx, short gy)
{
    hide_mouse();
    draw_box(x, y, 16, 6, 0);
    brickmap[gy*13+(gx-1)] = 11;
    show_mouse();
}

void draw_rectangle(short x, short y, short xlen, short ylen, short col)
{
    short i;

    for (i = x; i <= (x+xlen); i++)
        plot(i, y, col);

    for (i = x; i <= (x+xlen); i++)
        plot(i, (y+ylen), col);

    for (i = y; i <= (y+ylen); i++)
        plot(x, i, col);

    for (i = y; i <= (y+ylen); i++)
        plot((x+xlen), i, col);
}

void feedback(short x, short y)
{
    hide_mouse();

    draw_rectangle(buttonX, buttonY, 17, 7, 0);

    buttonX = x;
    buttonY = y;

    draw_rectangle(buttonX, buttonY, 17, 7, 1);

    put_block(294, 77, 16, 6, bricks[bricktype]);

    show_mouse();

    while (BUTTON_STATE > 0)
        get_mouse_status();
}

void handle_buttons()
{
    if (MOUSE_X/2 > 238 && MOUSE_X/2 < 254 && MOUSE_Y > 101 && MOUSE_Y < 107
        && BUTTON_STATE > 0) {
        bricktype = 0;
        feedback(238, 101);
    }

    if (MOUSE_X/2 > 263 && MOUSE_X/2 < 279 && MOUSE_Y > 101 && MOUSE_Y < 107
        && BUTTON_STATE > 0) {
        bricktype = 1;
        feedback(263, 101);
    }

    if (MOUSE_X/2 > 288 && MOUSE_X/2 < 304 && MOUSE_Y > 101 && MOUSE_Y < 107
        && BUTTON_STATE > 0) {
        bricktype = 2;
        feedback(288, 101);
    }

    if (MOUSE_X/2 > 238 && MOUSE_X/2 < 254 && MOUSE_Y > 113 && MOUSE_Y < 119
        && BUTTON_STATE > 0) {
        bricktype = 3;
        feedback(238, 113);
    }

    if (MOUSE_X/2 > 263 && MOUSE_X/2 < 279 && MOUSE_Y > 113 && MOUSE_Y < 119
        && BUTTON_STATE > 0) {
        bricktype = 4;
        feedback(263, 113);
    }

    if (MOUSE_X/2 > 288 && MOUSE_X/2 < 304 && MOUSE_Y > 113 && MOUSE_Y < 119
        && BUTTON_STATE > 0) {
        bricktype = 5;
        feedback(288, 113);
    }

    if (MOUSE_X/2 > 238 && MOUSE_X/2 < 254 && MOUSE_Y > 125 && MOUSE_Y < 131
        && BUTTON_STATE > 0) {
        bricktype = 6;
        feedback(238, 125);
    }

    if (MOUSE_X/2 > 263 && MOUSE_X/2 < 279 && MOUSE_Y > 125 && MOUSE_Y < 131
        && BUTTON_STATE > 0) {
        bricktype = 7;
        feedback(263, 125);
    }

    if (MOUSE_X/2 > 288 && MOUSE_X/2 < 304 && MOUSE_Y > 125 && MOUSE_Y < 131
        && BUTTON_STATE > 0) {
        bricktype = 8;
        feedback(288, 125);
    }

    if (MOUSE_X/2 > 238 && MOUSE_X/2 < 254 && MOUSE_Y > 137 && MOUSE_Y < 143
        && BUTTON_STATE > 0) {
        bricktype = 9;
        feedback(238, 137);
    }

    if (MOUSE_X/2 > 263 && MOUSE_X/2 < 279 && MOUSE_Y > 137 && MOUSE_Y < 143
        && BUTTON_STATE > 0) {
        bricktype = 13;
        feedback(263, 137);
    }

    if (MOUSE_X/2 > 288 && MOUSE_X/2 < 304 && MOUSE_Y > 137 && MOUSE_Y < 143
        && BUTTON_STATE > 0) {
        bricktype = 14;
        feedback(288, 137);
    }

    // SAVE

    if (MOUSE_X/2 > 237 && MOUSE_X/2 < 266 && MOUSE_Y > 171 && MOUSE_Y < 182
        && BUTTON_STATE > 0) {
        hide_mouse();
        draw_rectangle(237, 266, 20, 9, 1);

        while (BUTTON_STATE > 0)
            get_mouse_status();

        brick_save(filename, brickmap);

        draw_rectangle(237, 266, 20, 9, 4);
        show_mouse();
    }

    if (MOUSE_X/2 > 278 && MOUSE_X/2 < 306 && MOUSE_Y > 171 && MOUSE_Y < 182
        && BUTTON_STATE > 0) {
        hide_mouse();
        draw_rectangle(237, 266, 20, 9, 1);

        while (BUTTON_STATE > 0)
            get_mouse_status();

        quit = TRUE;

        draw_rectangle(237, 266, 20, 9, 4);
        show_mouse();
    }

}

void draw_bricks()
{
    int x,y,i=0;

    for (y=0; y<20; y++) {
        for (x=0; x<13; x++) {
            if (brickmap[i] != 11)
                put_block((x*17)+4, (y*7)+12, 16, 6, bricks[brickmap[i]]);
            i++;
        }
    }
}





