#ifdef SOK
    #define GLOBAL
#else
    #define GLOBAL extern
#endif

#define MAX_MAP_X        40
#define MAX_MAP_Y        20
#define FONT_NO          13
#define MAX_HISTORY    1024
typedef struct {
    int level;
    int maxx, maxy;
    int manx, many, mandir;
    int blockno, fillno;
    char map[MAX_MAP_X][MAX_MAP_Y];
    int mapoffx, mapoffy;
    int hist;
    int move;
} MAPINFO;

typedef struct {
    char dir;    // moving direction.
    char cur;    // manx, many
    char plus1;  // manx+dx, manx+dy
    char plus2;  // manx+dx+dx, manx+dy+dy
    char fillno; // old fillno.
} ACTION;

enum MapIncludeTypes {
    EMPTY, WALL, BLOCK, MAN,
    DEST = 0x80  // Mask for Destination position.
};

enum MapFontTypes {
    F_EMPTY, F_WALL, F_BLOCK,
    F_MAN_L, F_MAN_R, F_MAN_U, F_MAN_D,
    F_DEST,
    F_RED_BLOCK,
    F_REDMAN_L, F_REDMAN_R, F_REDMAN_U, F_REDMAN_D,
};

enum ManDirection { D_LEFT, D_RIGHT, D_UP, D_DOWN };
enum { NEXT_ROUND, QUIT_GAME, QUIT_SOK, RETRY };
/*
    global variables.
*/
GLOBAL MAPINFO lsh;
GLOBAL int contgame;
GLOBAL int gameover;
GLOBAL char *font[FONT_NO];
GLOBAL ACTION hist[MAX_HISTORY];

/*
    function prototypes
*/
int readscreen( MAPINFO *map);
