#define SOK

#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
//#include <mem.h>

#include "sok.h"
#include "vga320.h"

#define FONT_X  12
#define FONT_Y  12
#define MESS_X  260

void Box3D( int x1, int y1, int x2, int y2, int co1, int co2, int co3 )
{
    ClrBox( x1, y1, x2, y2, co1 );
    Hline( x1, x2, y1, 1, co2 );
    Hline( x1, x2, y2, 1, co3 );
    Vline( y1, y2, x1, 1, co2 );
    Vline( y1, y2, x2, 1, co3 );
}

void Rect3D( int x1, int y1, int x2, int y2, int co2, int co3 )
{
    Hline( x1, x2, y1, 1, co2 );
    Hline( x1, x2, y2, 1, co3 );
    Vline( y1, y2, x1, 1, co3 );
    Vline( y1, y2, x2, 1, co2 );
}

void Msg3D( int x, int y, char *s, int co1, int co2 )
{
    Box3D( x, y, x+strlen( s )*8+32, y+16+16, co2, co2+2, co2-2 );
    printxy2( x+16, y+8, s, co1, co2 );
}

void Msg3D2( int x, int y, char *s, int co1, int co2 )
{
    Box3D( x, y, x+strlen( s )*8+4, y+16+4, co2, co2+2, co2-2 );
    printxy2( x+2, y+2, s, co1, co2 );
}

void PrintNos( MAPINFO *map )
{
    char buf[16];

    sprintf( buf, "%3d", map->fillno );
    printxy2( MESS_X+16, 82, buf, RED, 20 );
    Rect3D( MESS_X+16, 82, MESS_X+16+8*3, 82+16, 18, 22 );

    sprintf( buf, "%4d", map->move );
    printxy2( MESS_X+8, 122, buf, RED, 20 );
    Rect3D( MESS_X+8, 122, MESS_X+8+8*4, 122+16, 18, 22 );
}

void putfont( MAPINFO *map, int x, int y )
{
    int kind;
    int xx = x * FONT_X;
    int yy = y * FONT_Y;

//    xx += FONT_X * map->mapoffx;
//    yy += FONT_Y * map->mapoffy;

    switch ( map->map[x][y] ) {
        case MAN :
            kind = MAN + map->mandir;
            break;
        case MAN + DEST:
            kind = F_REDMAN_L + map->mandir;
            break;
        case DEST :
            kind = F_DEST;
            break;
        case DEST + BLOCK :
            kind = F_RED_BLOCK;
            break;
        default :
            kind = map->map[x][y];
    }
    if( kind > -1 && kind < FONT_NO )
        PutImage( xx, yy, font[kind], 0 );
}

void getdir( int dir, int *dx, int *dy )
{
    *dx = *dy = 0;

    switch ( dir ) {
        case D_LEFT :
            *dx = -1;
            break;
        case D_RIGHT :
            *dx = 1;
            break;
        case D_UP :
            *dy = -1;
            break;
        case D_DOWN :
            *dy = 1;
            break;
    }
}

int cango( MAPINFO *map, int dir )
{
    int dx, dy;

    getdir( dir, &dx, &dy );
    switch( map->map[ map->manx + dx ][ map->many + dy ] ) {
        case EMPTY :
        case DEST :
            return 1;
        case BLOCK :
        case BLOCK + DEST :
            switch( map->map[ map->manx + dx + dx ][ map->many + dy + dy ] ) {
                case EMPTY :
                case DEST :
                    return 1;
            }
            return 0;
    }
    return 0;
}

int move( MAPINFO *map, int dir )
{
    int dx, dy;
    int k;

    if( !cango( map, dir ) )
        return 1;

    map->move++;
    switch ( map->map[ map->manx ][ map->many ] ) {
        case DEST + MAN :
            k = DEST;
            break;
        case MAN :
            k = EMPTY;
            break;
    }

    map->map[ map->manx ][ map->many ] = k;
    putfont( map, map->manx, map->many );

    getdir( dir, &dx, &dy );
    map->manx += dx;
    map->many += dy;
    map->mandir = dir;

    switch( map->map[ map->manx ][ map->many ] ) {
        case EMPTY :
        case DEST :
            map->map[ map->manx ][ map->many ] += MAN;
            putfont( map, map->manx, map->many );
            break;
        case BLOCK + DEST :
            map->fillno--;
        case BLOCK :
            map->map[ map->manx ][ map->many ] -= BLOCK;
            map->map[ map->manx ][ map->many ] += MAN;
            putfont( map, map->manx, map->many );
            switch ( map->map[ map->manx + dx ][ map->many + dy ] ) {
                case DEST :
                    map->fillno++;
                case EMPTY :
                    map->map[ map->manx + dx ][ map->many + dy ] += BLOCK;
                    putfont( map, map->manx + dx, map->many + dy );
            }
            break;
    }

    PrintNos( map );

    if( map->fillno == map->blockno )
        return 0;
    return 1;
}

void backmove( MAPINFO *map, ACTION *hist )
{
    int dx, dy;
    char buf[40];

    map->hist--;
    if( map->hist < 0 )
        map->hist = MAX_HISTORY -1;
    if( hist [ map->hist ].dir == 0xFF )
        return;

    map->move--;
    getdir( hist[ map->hist ].dir, &dx, &dy );

    map->fillno = hist[ map->hist ].fillno;
    map->manx -= dx;
    map->many -= dy;
    map->map[ map->manx ][ map->many ] = hist[ map->hist ].cur;
    map->map[ map->manx + dx ][ map->many + dy ] = hist[ map->hist ].plus1;
    map->map[ map->manx + dx + dx ][ map->many + dy + dy ] = hist[ map->hist ].plus2;

    putfont( map, map->manx, map->many );
    putfont( map, map->manx + dx, map->many + dy );
    putfont( map, map->manx + dx + dx, map->many + dy + dy );

    hist[ map->hist ].dir = 0xFF;

    PrintNos( map );
}

void saveaction( MAPINFO *map, ACTION *hist, int dir )
{
    int dx, dy;


    getdir( dir, &dx, &dy );
    hist[ map->hist ].dir = dir;
    hist[ map->hist ].cur = map->map[ map->manx ][ map->many ];
    hist[ map->hist ].plus1 = map->map[ map->manx + dx ][ map->many + dy ];
    hist[ map->hist ].plus2 = map->map[ map->manx + dx + dx ][ map->many + dy + dy ];
    hist[ map->hist ].fillno = map->fillno;
    map->hist++;
    if( map->hist >= MAX_HISTORY )
        map->hist = 0;
}

void clearhist( ACTION *hist )
{
    int i;
    for( i=0; i< MAX_HISTORY; i++ )
        hist[i].dir = 0xFF;

}

int game( MAPINFO *map , ACTION *hist )
{
    int cont = 1;

    clearhist( hist );

    while( cont ) {
        switch ( getkey() ) {
            case LEFT :
                if( cango( map, D_LEFT ) ) {
                    saveaction( map, hist, D_LEFT );
                    cont = move( map, D_LEFT );
                }
                break;
            case RIGHT :
                if( cango( map, D_RIGHT ) ) {
                    saveaction( map, hist, D_RIGHT );
                    cont = move( map, D_RIGHT );
                }
                break;
            case UP :
                if( cango( map, D_UP ) ) {
                    saveaction( map, hist, D_UP );
                    cont = move( map, D_UP );
                }
                break;
            case DOWN :
                if( cango( map, D_DOWN ) ) {
                    saveaction( map, hist, D_DOWN );
                    cont = move( map, D_DOWN );
                }
                break;
            case 'U' : case 'u' :
                backmove( map,  hist );
                break;
            case 'R' : case 'r' :
                return RETRY;
            case ESC :
                return QUIT_SOK ;
            case 'N' : case 'n' :
                return NEXT_ROUND;
            case 'Q' : case 'q' :
                return QUIT_GAME;
        }
    }

    Msg3D( 0, 100, "¬÷‰·Ða­v¯s“¡”a. ”a·q Àw‰¡µA •¡¸å·i." , CYAN, 55 );
    getkey();
    return NEXT_ROUND;
}

void readfonts( void )
{
    int i=0;
    for( i=0; i<FONT_NO; i++ )
        font[i] = loadMFEfont( "SOK.MFE", i );
}

void logo( void )
{
    loadPCX256( "soklogo.DAT", VGA_VRAM );
/*
    ClearScreen( 0 );
    printxy2( 20, 0, "* Ða“i­¡ & Çe¬aœq ­QËi ‘©·¡ ˆ¤i¦ *", BLUE, CYAN );
    printxy( 20, 70, "!!! Àw‰¡ »¡‹¡ 256 ¬‚¬w¶w !!!", YELLOW );
    printxy2( 10, 180, "´a¢ ‹i®Aa ’Ÿa¡e ‘©·¡ˆa ¯¡¸b –S“¡”a.", BLUE, DARKGRAY );
*/
    getkey();
}

void drawboard( MAPINFO *map )
{
    int x, y;
    char buf[40];

    ClearScreen( BLACK );
    for( x=0; x < map->maxx; x++ )
        for( y=0; y < map->maxy; y++ )
            putfont( map, x, y );

    ClrBox( MESS_X, 0, 319, 199, DARKGRAY );
    Msg3D2( MESS_X+2, 0, "!Àw‰¡!", YELLOW, 28 );

    Box3D( MESS_X+2, 24, MESS_X+54, 58, 28, 30, 26 );
    sprintf( buf, "%3d", map->level );
    printxy2( MESS_X+16, 40, buf, 55, 20 );
    Rect3D( MESS_X+16, 40, MESS_X+16+8*3, 40+16, 18, 22 );
    printxy2( MESS_X+16, 25, "Ìe®", WHITE, 28 );

    Box3D( MESS_X+2, 60, MESS_X+54, 99, 28, 30, 26 );
    printxy2( MESS_X+4, 62, "µ«‹¥ˆõ" , 55, 28 );
    Box3D( MESS_X+2, 102, MESS_X+54, 140, 28, 30, 26 );
    printxy2( MESS_X+4, 104, "¶‘»¢·±" , 55, 28 );
    PrintNos( map );

    vline( 0, 199, MESS_X-1, 1, LIGHTGRAY );
    printxy2( MESS_X, 148, "Çe¬aœq", 17, DARKGRAY );
    printxy2( MESS_X, 164, "  &   ", 15, DARKGRAY );
    printxy2( MESS_X, 180, "Ða“i­¡", 17, DARKGRAY );
}

int main( int argno, char **s )
{
    init320();

    readfonts();

    lsh.level = 1;
    if( argno == 2 ) {
        lsh.level = atoi( s[1] );
        if( lsh.level <=0 )
            lsh.level = 1;
    }

    logo();
    contgame = 1;
    while ( contgame ) {
        gameover = 0;
        while( !gameover ) {
            if( -1 == readscreen( &lsh ) ) {
                ClearScreen( LIGHTGRAY );
                Box3D( 5, 80, 15, 116, 55, 55, WHITE );
                printxy2( 20, 80, "”á·¡¬w· Ìe ¡¡´··¡ ´ô¯s“¡”a.", BLUE, LIGHTGRAY );
                printxy2( 20, 100, "  e—i´á ý´á º­A¶a..", BLUE, LIGHTGRAY );
                contgame = 0;
                gameover = 0;
                getkey();
                gameover = 1;
                contgame = 0;
                break;
            }

            lsh.move = 0;
            lsh.hist = 0;
            lsh.fillno = 0;
            drawboard( &lsh );
            switch ( game( &lsh, hist ) ) {
                case NEXT_ROUND :
                    lsh.level++;
                    break;
                case QUIT_GAME :
                    gameover = 1;
                    break;
                case QUIT_SOK :
                    gameover = 1;
                    contgame = 0;
                    break;
                case RETRY :
                    break;
            }
        }
    }

    close320();
    puts( "HANULSO's Game The SOKOBAN 256\n"
          "------------------------------\n"
          "    This game's data comes from Linux sokoban, but the codes totally\n"
          "    rewritten. Its algorithm rearranged, reassembled.\n"
          "\n"
          "    Hanulso's entertainment group has planed many games for several years.\n"
          "    Its goal is korean children can play original korean game.\n"
          "    If you enjoy our games or have a good idea, contact to HANULSO\n"
          "    for Our future, korean children.\n"
          "\n"
          "Thanks for playing SOK advanced version. Message from HANULSO of KPNU, KOREA"
        );
    return 0;
}
