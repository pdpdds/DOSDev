/*
    MegaBreak v 1.0

    COPYRIGHT (C) 1995/96/97 Knut A. Ruud
*/

#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <i86.h>
#include <ctype.h>
#include <dos.h>
#include <graph.h>

#include "libs\standard.h"
#include "libs\w_mlib.h"    // Mouse-library
#include "libs\gfxlib.h"   // Gfx-header
#include "libs\imageio.h"
#include "libs\stdtext.h"
#include "libs\font.h"
#include "libs\random.h"
#include "brkfunc.h"



// ******************** Funksjons-prototyper **
void draw_bricks();
void bdraw_bricks();
void find_westhotspots();
void find_easthotspots();
void calc_brickcoords();
void calc_gridcoords();
void find_actuabricks();
void handle_doublebricks(short brick1, short brick2, short dim);
void destroy_brick(short bricknum);
void shot_destroy_brick(short gridx, short gridy, short x, short y);
void erase_brick(short bricknum);
void check_pad();
void check_walls();
void dump_coordinates();
void init_list();       // Initialiserer flash-lista
void load_pcx_graphics();       // Bare for versjoner som ikke gis ut
void load_dat_graphics();
void clear_main_screen();
void draw_tiles();
void appear_bat();
void disappear_bat();
void load_levels();
void load_demos();
void reset();
void init_shot_list();
void delete_shot_list();
void init_hiscores();
void update_hiscores();
void new_hiscore();
void startup();
void read_config();
void write_config();
int exp(int,int);
void show_end_screen();
void init_end_screen();
int alloc_memory();
void free_memory();

// ******************** Globale variabler/arrays/konstanter **
const short AVAILABLE = 2;
const short TRUE = 1;
const short FALSE = 0;
const short X = 0;
const short Y = 1;

const short BALLWIDTH = 4;
const short BALLHEIGHT = 4;
const short PADWIDTH = 26;
const short PAD_Y   = 190;
const short YLIMIT = 195;   /* Hvis ballen gÜr under dette Y-koordinat
                               mister man et liv */

const short ICON_COORDS[4] = { 234, 254, 274, 294 };
const short ICON_YCOORD = 100;
const short LETTER_COORDS[5] = { 237, 252, 267, 282, 297 };
const short LETTER_YCOORD = 170;
const short PANEL_COORDS[4] = { 127, 135, 143, 151 };
const short PANEL_XCOORD = 250;
const short DISPLAY_XCOORD = 233;
const short SCORE_Y = 7;
const short SPACING = 10;
const short LIVES_Y = 29;
const short BRICKS_Y = 51;
const short TIME_Y = 73;
const short DISPLAY_WIDTH = 82;

const short SECS_PR_LEVEL = 300;
const short SHUT_DOWN = 3;

const short ENHANCE__COUNT = 20;
const short FLASHDELAY = 5; // Hvor mange spill-cycles skal en flash vare?
const short BULLET_SPEED = 5;
short SHOT__DELAY = 20;  // Ikke const fordi den skal kunne forandres for ?
                        // lage auto-fire.
short NUM_LIVES = 3;

const short LEFT_SELECTOR = 40;

short menu_active = FALSE;
short selector_offset = 0;
short selector_updated = FALSE;
short option_selected = FALSE;

// BONUS-MELDINGER
const short STRONG = 0;
const short SHOOTING = 1;
const short MAGNET = 2;
const short LASER = 3;
const short _3xBRICKS = 4;
const short BONUS = 5;
const short LIFE = 6;
const short ENHANCE = 7;
const short BOMB = 8;
const short PAUSE = 9;


// BONUS-TYPER
short STRONGBALL = FALSE;
short SHOOTBAT = FALSE;
short MAGNETBAT = FALSE;
short LASERRESCUE = FALSE;
short EXTRALIFE = FALSE;
short MEGABOMB = FALSE;
short BONUSLETTER = FALSE;
short ENHANCER = FALSE;


// MENYER

const short MAIN_MENU = 1;
const short OPTIONS = 2;
const short HISCORES = 3;


short sel_offsets[3] = {0, 40, 10};

// Hvor mye tid skal man ha p?hvert brett?
short level_times[42] = {50, 45, 55, 60, 60, 70, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 90, 85, 90, 80, 90, 90, 90, 90, 90, 90, 90, 95, 100, 100, 105, 105, 110, 115, 120, 0, 0};

short GAMEOVER = FALSE;

byte *bricks[15];               // Bilde-data til hver brikke
byte *flashbricks[7];           // Animasjons data for flertreffs-brikker
byte *balls[11];                // Forskjellige ball-typer
byte *bonus_letters[2][5];      // Bonus-bokstaver
byte *panel_displays[2][4];     // Bonuser: X2 X3 X4 JACPOT
byte *powerup_icons[2][4];
byte *rescue_line;
byte *lightbrick;
byte *select;

struct find_t fileinfo;

short BrickCoords[2][4];    // Array over X og Y koords til brikk. rundt ball
short BrickTable[4];        // Holder styr p?hvilke brikker som eksisterer
short BrickCount = 0;       // Antall brikker som er igjen
short BallX[4], BallY[4];   // Holder koordinatene til ball-hotspots
short GridX[4], GridY[4];   // Holder grid-koordinater til ball-hotspots
double padX;
short x, y, i;                    // Telle-variabel
short key;                  // Til ?ta imot fra keyboard
short pause;                // Variabel som sier om hver spill-cycle
                            // skal pauses eller ikke
double speed = 2.0;               // Forteller ballens fart

long score = 0;             // Holder spillerens score
long oldscore = 0;
short next_level = FALSE;   // Forteller om neste brett er nÜdd
short level = 0;            // Holder styr p?hvilken level som er aktiv
short oldlev = -1;
short ball_launched;        // Forteller om ballen er startet
short brickflash = FALSE;   // Forteller om destroy_brick() har truffet
                            // en brikke som skal flashes
short last_level = 39;       // Spesifiserer hvilket brett som er siste
short autobat = FALSE;
short new_message = FALSE;
short message = 0;
short msgcounter = 0;
short balltype = 0;
short bonus = 0;
double step;
short xdistance, ydistance;
short ball_offset = 11;
short calculated = FALSE;
short laser_active = FALSE;
short magnet_active = FALSE;
short shoot_active = FALSE;
short strongball_active = FALSE;
short rekyl_active = FALSE;
short rekyl_count = 0;
short bat_type = 0;
short strong_state = 0;
short strong_counter = 0;
short bonusl_counter = 0;
short panelbonus_counter = 0;
short tile;
short new_game = FALSE;
short demo_mode = FALSE;
short bonus_mul = 0;
short brickscore = 0;
short shot_delay = 0;
short sensitivity = 20;
short lives = 0;
short laser_count = 1;
long lowest_score;
short secret_level_count = 0;
short bonus_mode = FALSE;
short prevlevel = 0;
short user_levels;
short level_jump = FALSE;

short enhance_ani = FALSE;
short enhance_count = 0;

short page = 1;
short new_page = FALSE;
short page_type = 0;

// Variabler for tid

short oldtime;
short gametime;
short basetime;


// Variabler for skytende bat (animasjon)
short anim_frame = 2;
const short anim_delay = 6;
const short rekyl_delay = 5;
const short strong_delay = 15;
short anim_active = FALSE;
short anim_counter = 0;

byte *vrscreen;
byte *bgscreen;

byte *bigfont[50];
byte *smallfont[50];

byte *batshoot;         // NÜr bat skyter
byte *bullet;            // Kule
byte *messages[10];      // Beskjeder p?mini-panelet
byte *bats[9];
byte *mainscreen;
signed char palette[768];
byte *tiles[21];


char *end_text[24];
char *startup_text;

struct hiscore_entry {
    char name[10];
    short level;
    long score;
};

hiscore_entry hiscore_table[10];
hiscore_entry new_entry;

struct ball {
    double X;
    double Y;
    double xinc;
    double yinc;
};  // Struktur for ball. xinc og yinc forteller ballens direksjon (-1/1)

ball Ball; // Deklarer ball

struct shot {
    short X;
    short Y;
    short gridX;
    short brickX;
    short gridY;
    short brickY;
    shot *next;
    shot *prev;
};

struct Flash {
    short screenX;
    short screenY;     // Skjerm-koords
    short gridX;
    short gridY;         // Grid-koords
    short delay;                    // Hvor lenge skal brikken flashe?
    short flashtype;
    Flash *next;
    Flash *prev;
};  // Struktur for flashing av brikker

// Legg opp strukturer til en linka liste

Flash   *flash = NULL,
        *list_h = NULL,
        *list_t = NULL,
        *prevnode = NULL,
        *reader = NULL;

shot    *Shot = NULL,
        *slist_h = NULL,
        *slist_t = NULL,
        *sprevnode = NULL,
        *snextnode = NULL,
        *sreader = NULL;

    
// ******************

byte brickmap[42][260];


int main(int argc, char *argv[])
{
	/*
		** Variabler og strukturer **
	*/
    time_t rawtime;
    auto struct tm newtime;

    short i2;               // extra telle-variabel
    short checked=0;      // variabel til bruk nÜr det er mange if's etter
			// hverandre, og man skal "si fra" om noe er testet
			// fra fõr.
    short brick;

    /*
	** Initialiser **
    */

    // Sjekk om mylevels parameter er definert

    if (!strcmp(argv[1], "-mylevels"))
        user_levels = TRUE;
    else
        user_levels = FALSE;

    for (i=0; i<260; i++)
        brickmap[last_level+1][i] = random(0, 5);

    randomize();
    short alive;

    init_end_screen();
    startup();

    // ** Lad inn data

    load_dat_graphics();
    read_config();

    load_font("small.fnt", smallfont);
    load_font("big.fnt", bigfont);

    /*
	** Sett opp **
	*/

    set_font_dimensions(8, 8);
    init_list();                // Sett opp flash-liste
    init_shot_list();
    mode_13h();
    set_pal(palette);           // Setter hoved-palettet
    set_mouse_sens(sensitivity,16);      // Mindre sensitivitet p?musa
    limit_mouse(6,190,398,190); // Bat'en kan bare g?p?liten del av skjermen

    put_block(0,0,320,200,mainscreen);     // tegn hoved-skjerm


    DEMO_MODE:

    demo_mode = TRUE;
    autobat = TRUE;
    menu_active = TRUE;
    level = last_level + 1;

    NEW_GAME:

    reset();

    if (!demo_mode && new_game) {
        load_levels();
        new_game = FALSE;
        autobat = FALSE;
        level = 0;
        oldlev = -1;
    }

    while (lives > 0) {

    alive = TRUE;

    NEXT_LEVEL:

    if (level != oldlev) {
        draw_tiles();
        bput_sprite(0, 0, 320, 200, mainscreen, bgscreen);
        oldtime = 200;
        if (user_levels)
            gametime = 0;
        else
            gametime = level_times[level];
        oldlev = level;
        delete_shot_list();
    }

    center_write("SCORE", DISPLAY_XCOORD, SCORE_Y, DISPLAY_WIDTH, smallfont);
    center_write("LIVES", DISPLAY_XCOORD, LIVES_Y, DISPLAY_WIDTH, smallfont);
    center_write("BRICKS", DISPLAY_XCOORD, BRICKS_Y, DISPLAY_WIDTH, smallfont);
    center_write("TIME", DISPLAY_XCOORD, TIME_Y, DISPLAY_WIDTH, smallfont);

    num_fwrite(score, DISPLAY_XCOORD, SCORE_Y+SPACING, 9, smallfont);
    num_fwrite(lives, DISPLAY_XCOORD+35, LIVES_Y+SPACING, 2, smallfont);
    num_fwrite(BrickCount, DISPLAY_XCOORD+35, BRICKS_Y+SPACING, 3, smallfont);
    
    clear_main_screen();

    if (!autobat) {
        center_write("GET READY!", 3, 157, 222, smallfont);
        center_write("LEVEL   ", 3, 169, 222, smallfont);
        num_fwrite((level+1), 132, 169, 2, smallfont);
    }

    draw_bricks();
    delay(500);

    ball_launched = FALSE;

    get_mouse_status();     // Oppdater X og Y koordinater og sjekk knapper
    padX = MOUSE_X/2;

    if (autobat)
        padX = random(3, 195);

    Ball.X = padX+11;

    appear_bat();

    block_copy(40, 156, 130, 24, bgscreen, 40, 156); // Slett "GET READY"

    laser_active = FALSE;
    magnet_active = FALSE;
    shoot_active = FALSE;
    strongball_active = FALSE;

    MEGABOMB = FALSE;
    EXTRALIFE = FALSE;
    ENHANCER = FALSE;
    BONUSLETTER = FALSE;
    enhance_ani = FALSE;
    anim_active = FALSE;
    laser_count = 1;
    SHOT__DELAY = 20;
    rekyl_active = FALSE;

    bat_type = 0;
    Ball.xinc = 0;  // Ballen starter stillestÜende
    Ball.yinc = 0;
    ball_offset = 11;
    bat_type = 0;

	/*
		** HOVED-LOOP **
	*/

	while (alive == TRUE) // Hopp ut nÜr man misser
	{

	// ** Putt sprites p?skjermen

    if ((!autobat) && (!pause)) {
        get_mouse_status();     // Oppdater X og Y koordinater og sjekk knapper
        padX = MOUSE_X/2;
    }
    else {
        if (Ball.Y > 155 && Ball.Y < 185) {
                ydistance = PAD_Y - (Ball.Y+3);
                xdistance = (Ball.X+1.5) - (padX+13.5);
                step = (double)xdistance / (double)(ydistance/2);
                step += Ball.xinc;
            padX += step;

            if (padX < 4)
                padX = 4;
            if ((padX+26) > 224)
                padX = 199;
        }
    }

    // G?gjennom liste av flash'es

    reader = list_h;            // G?til begynnelsen av lista..
    reader = reader -> next;    // Hopp over fõrste node
    while (reader) {            // Hvis denne noden er noe..

        if (reader -> delay == FLASHDELAY)  // Flash er ikke tegnet hvis delay er maks
            put_block(reader -> screenX, reader -> screenY, 16, 6,
                      flashbricks[reader -> flashtype]);

        if (reader -> delay > 0)   // Delay skal minkes hver spill-cycle...
            reader -> delay--;

        if (reader -> delay == 0) { // Delay er 0 og skal derfor slettes...

            if (reader -> next == NULL) { // er dette siste element?
                list_t = list_t -> prev; // G?tilbake
                flash = reader;
                reader = list_t;    // leser er p?slutten
                reader -> next = NULL;  // Ingen noder etter sluttpunkt

                brick = brickmap[level][(flash->gridY-1)*13+(flash->gridX-1)];

                put_block(flash -> screenX, flash -> screenY, 16, 6,
                              bricks[brick]);    // Ta flash bort fra skjermen
                delete flash;            // Slett slutt-noden
            }
            else {
                prevnode = reader -> prev;    // prevnode settes lik forrige node
                prevnode -> next = reader -> next;    /* prevnode peker
                                                         p?node etter reader */
                flash = reader;
                reader = reader -> next;    // reader hopper videre
                reader -> prev = prevnode;  /* og peker tilbake p?sitt nye
                                               bakelement */

                brick = brickmap[level][(flash->gridY-1)*13+(flash->gridX-1)];
                put_block(flash -> screenX, flash -> screenY, 16, 6,
                          bricks[brick]);    // Ta flash bort fra skjermen
                delete flash;    // node slettes
            }
        } // IF node skal slettes

        reader = reader -> next;    // G?til neste node

    } // WHILE

	if (ball_launched == FALSE) {

        Ball.X = padX + ball_offset;
        Ball.Y = 186;

        if (autobat)
            BUTTON_STATE = random(1,2);
        
        if (BUTTON_STATE == 1 || BUTTON_STATE == 2)
	    {
		switch (BUTTON_STATE) {
		    case 1:                 // VENSTRE MUSEKNAPP
			Ball.xinc = -speed;
			break;
		    case 2:                 // HùYRE MUSEKNAPP
			Ball.xinc = speed;
		    }
		Ball.yinc = -speed;

		ball_launched = TRUE;
	    }
	}

    if (anim_active) {
        bat_type = anim_frame;
        if (anim_counter++ == anim_delay) {
            anim_counter = 0;
            anim_frame++;
            if (anim_frame == 7) {
                anim_active = FALSE;
                bat_type = 1;
                shoot_active = TRUE;
            }
        }
    }

    if (rekyl_active)
        put_sprite(padX,(PAD_Y+1),26,5,batshoot);
    else
        put_sprite(padX,PAD_Y,26,5,bats[bat_type]);  // Putt BAT p?skjermen

    if ((!balltype) && strongball_active)
        put_sprite(Ball.X, Ball.Y, 4, 4, balls[8+strong_state]); // Putt BALL p?skjermen
    else
        put_sprite(Ball.X, Ball.Y, 4, 4, balls[balltype]); // Putt BALL p?skjermen

    // *************************************************************
	// ******************* COLLISION DETECTION *********************
	// *************************************************************

	// Ball m?sjekkes for hver direksjon den gÜr i.
	// Det den fõrste rutinen gjõr er ?finne ut hvilken retning ballen
	// gÜr i, og deretter lage en passende for-lõkke. Innholdet
	// i hver av disse for-lõkkene vil vëre litt forskjellige. Det
	// er mulig at jeg lager dem som fire funksjoner, slik at hoved-
	// koden blir mer oversiktilig.

	// *** Nordvestlig retning ***

	if (Ball.xinc < 0 && Ball.yinc < 0)
	{
        i = (Ball.X + Ball.xinc) - (int)Ball.X;

		find_westhotspots();    // Legger hotspots til ball i BallX & BallY

        if ((BallY[3] <= 151) && (BallY[0] >= 12)
            && (BallX[0] > 4) && (BallX[3] < 223))
		{
		    // Finner koordinater til kandidatbrikker (grid og vanlig)
		    calc_brickcoords();
		    calc_gridcoords();
		    find_actuabricks();

		    // Finn ut hvilken vei ballen skal g? ved ?se p?hvilke
		    // brikker som er aktive i BrickTable, og hvor

		    // Kant-tilfelle
		    if (BrickTable[0] == 1 && BrickTable[1] == 0
			&& BrickTable[2] == 0 && BrickTable[3] == 0) {
			// Sjekk om ballen treffer akkurat p?kanten av
			// brikken
			    Ball.xinc = Ball.xinc * -1;
			    Ball.yinc = Ball.yinc * -1;
			    destroy_brick(0);
			    goto endcheck;
		    }

		    // Hjõrne-tilfelle
		    if (BrickTable[0] == 1 && BrickTable[1] == 1 &&
			BrickTable[2] == 1 && BrickTable[3] == 0) {

			Ball.xinc = Ball.xinc * -1;
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(0);
			destroy_brick(1);
			destroy_brick(2);
            goto endcheck;
		    }

		    // Sjekk horisontale brikker

		    if (BrickTable[0] == 1 && BrickTable[1] == 1) {
			handle_doublebricks(0, 1, X);
			goto endcheck;
		    }

		    // Sjekk vertikale brikker
		    if (BrickTable[0] == 1 && BrickTable[2] == 1) {
			handle_doublebricks(0, 2, Y);
			goto endcheck;
		    }

		    // Sjekk "sm?-tilfeller
		    if (BrickTable[2] == 1) {
			Ball.xinc = Ball.xinc * -1;
			destroy_brick(2);
			goto endcheck;
		    }
		    if (BrickTable[1] == 1) {
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(1);
			goto endcheck;
		    }

		    endcheck:
		    checked=1; //imidlertidig

		} // IF - Er ballen innenfor omrÜdet?

	} // IF - Nordvestlig retning

	// *** Nordõstlig retning ***

	if ((Ball.xinc > 0) && (Ball.yinc < 0))
	{
        i = (Ball.X + Ball.xinc) - (int)Ball.X;

		find_easthotspots();    // Legger hotspots til ball i BallX & BallY

        if ((BallY[3] <= 151) && (BallY[0] >= 12)
		    && (BallX[0] > 4) && (BallX[3] < 224))
		{
		    // Finner koordinater til kandidatbrikker (grid og vanlig)
		    calc_brickcoords();
		    calc_gridcoords();
		    find_actuabricks();

		    // Finn ut hvilken vei ballen skal g? ved ?se p?hvilke
		    // brikker som er aktive i BrickTable, og hvor

		    // Kant-tilfelle
		    if (BrickTable[0] == 0 && BrickTable[1] == 1
			&& BrickTable[2] == 0 && BrickTable[3] == 0) {
			// Sjekk om ballen treffer akkurat p?kanten av
			// brikken
			    Ball.xinc = Ball.xinc * -1;
			    Ball.yinc = Ball.yinc * -1;
			    destroy_brick(1);
			    goto endcheck2;
		    }

		    // Hjõrne-tilfelle
		    if (BrickTable[0] == 1 && BrickTable[1] == 1 &&
			BrickTable[2] == 0 && BrickTable[3] == 1) {

			Ball.xinc = Ball.xinc * -1;
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(0);
			destroy_brick(1);
			destroy_brick(3);
            goto endcheck2;
		    }

		    // Sjekk horisontale brikker

		    if (BrickTable[0] == 1 && BrickTable[1] == 1) {
			handle_doublebricks(0, 1, X);
			goto endcheck2;
		    }

		    // Sjekk vertikale brikker
		    if (BrickTable[1] == 1 && BrickTable[3] == 1) {
			handle_doublebricks(1, 3, Y);
			goto endcheck2;
		    }

		    // Sjekk "sm?-tilfeller
		    if (BrickTable[3] == 1) {
			Ball.xinc = Ball.xinc * -1;
			destroy_brick(3);
			goto endcheck2;
		    }
		    if (BrickTable[0] == 1) {
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(0);
			goto endcheck2;
		    }

		    endcheck2:
		    checked=1; //imidlertidig

		} // IF - Er ballen innenfor omrÜdet?

	} // IF - Nordõstlig retning

	// *** Sõrvestlig retning ***

	if (Ball.xinc < 0 && Ball.yinc > 0)
	{
        i = (Ball.X + Ball.xinc) - (int)Ball.X;

		find_easthotspots();    // Bruker motsatt proc. nÜr ballen
					// gÜr sõrover

        if ((BallY[3] <= 151) && (BallY[0] >= 12)
		    && (BallX[0] > 4) && (BallX[3] < 224))
		{
		    // Finner koordinater til kandidatbrikker (grid og vanlig)
		    calc_brickcoords();
		    calc_gridcoords();
		    find_actuabricks();

		    // Finn ut hvilken vei ballen skal g? ved ?se p?hvilke
		    // brikker som er aktive i BrickTable, og hvor

		    // Kant-tilfelle
		    if (BrickTable[0] == 0 && BrickTable[1] == 0
			&& BrickTable[2] == 1 && BrickTable[3] == 0) {
			// Sjekk om ballen treffer akkurat p?kanten av
			// brikken
			    Ball.xinc = Ball.xinc * -1;
			    Ball.yinc = Ball.yinc * -1;
			    destroy_brick(2);
			    goto endcheck3;
		    }

		    // Hjõrne-tilfelle
		    if (BrickTable[0] == 1 && BrickTable[1] == 0 &&
			BrickTable[2] == 1 && BrickTable[3] == 1) {

			Ball.xinc = Ball.xinc * -1;
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(0);
			destroy_brick(2);
			destroy_brick(3);
            goto endcheck3;
		    }

		    // Sjekk horisontale brikker

		    if (BrickTable[2] == 1 && BrickTable[3] == 1) {
			handle_doublebricks(2, 3, X);
			goto endcheck3;
		    }

		    // Sjekk vertikale brikker
		    if (BrickTable[0] == 1 && BrickTable[2] == 1) {
			handle_doublebricks(0, 2, Y);
			goto endcheck3;
		    }

		    // Sjekk "sm?-tilfeller
		    if (BrickTable[0] == 1) {
			Ball.xinc = Ball.xinc * -1;
			destroy_brick(0);
			goto endcheck3;
		    }
		    if (BrickTable[3] == 1) {
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(3);
			goto endcheck3;
		    }
		    
		    endcheck3:
		    checked=1; //imidlertidig

		} // IF - Er ballen innenfor omrÜdet?

	} // IF - Sõrvestlig retning

	// *** Sõrõstlig retning ***

	if ((Ball.xinc > 0) && (Ball.yinc > 0))
	{
        i = (Ball.X + Ball.xinc) - (int)Ball.X;

		find_westhotspots();    // Motsatt proc. nÜr ball gÜr sõrover

        if ((BallY[3] <= 151) && (BallY[0] >= 12)
		    && (BallX[0] > 4) && (BallX[3] < 224))
		{
		    // Finner koordinater til kandidatbrikker (grid og vanlig)
		    calc_brickcoords();
		    calc_gridcoords();
		    find_actuabricks();

		    // Finn ut hvilken vei ballen skal g? ved ?se p?hvilke
		    // brikker som er aktive i BrickTable, og hvor

		    // Kant-tilfelle
		    if (BrickTable[0] == 0 && BrickTable[1] == 0
			&& BrickTable[2] == 0 && BrickTable[3] == 1) {
			// Sjekk om ballen treffer akkurat p?kanten av
			// brikken
			    Ball.xinc = Ball.xinc * -1;
			    Ball.yinc = Ball.yinc * -1;
			    destroy_brick(3);
			    goto endcheck4;
		    }

		    // Hjõrne-tilfelle
		    if (BrickTable[0] == 0 && BrickTable[1] == 1 &&
			BrickTable[2] == 1 && BrickTable[3] == 1) {

			Ball.xinc = Ball.xinc * -1;
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(1);
			destroy_brick(2);
			destroy_brick(3);
            goto endcheck4;
		    }

		    // Sjekk horisontale brikker

		    if (BrickTable[2] == 1 && BrickTable[3] == 1) {
			handle_doublebricks(2, 3, X);
			goto endcheck4;
		    }

		    // Sjekk vertikale brikker
		    if (BrickTable[1] == 1 && BrickTable[3] == 1) {
			handle_doublebricks(1, 3, Y);
			goto endcheck4;
		    }

		    // Sjekk "sm?-tilfeller
		    if (BrickTable[1] == 1) {
			Ball.xinc = Ball.xinc * -1;
			destroy_brick(1);
			goto endcheck4;
		    }
		    if (BrickTable[2] == 1) {
			Ball.yinc = Ball.yinc * -1;
			destroy_brick(2);
			goto endcheck4;
		    }
		    
		    endcheck4:
		    checked=1; //imidlertidig

		} // IF - Er ballen innenfor omrÜdet?

	} // IF - Sõrõstlig retning

	check_pad();
	check_walls();

	if (kbhit()) {
	    key = getch();
        switch (key) {
            case 27:            // ESC      -   Bring up menu
                if (menu_active)
                    if (page > 1) {
                        new_page = TRUE;
                        page_type = MAIN_MENU;
                    }
                    else {
                        menu_active = SHUT_DOWN;
                        pause = FALSE;
                    }
                else {
                    menu_active = TRUE;
                    pause = TRUE;
                }
                break;
            case 72:
                if (selector_offset > 0) {
                    selector_offset--;
                    selector_updated = TRUE;
                }
                break;
            case 80:
                if (selector_offset < 3) {
                    selector_offset++;
                    selector_updated = TRUE;
                }
                break;
            case 13:            // RETURN   -   Select Option
                option_selected = TRUE;
                break;
            case 112:           // p        -   Pause
                if (pause)
                    pause = FALSE;
                else {
                    pause = TRUE;
                    new_message = TRUE;
                    message = PAUSE;
                }
                break;
            case 49:
                if (STRONGBALL) {
                    STRONGBALL = FALSE;
                    strongball_active = TRUE;
                    put_block(ICON_COORDS[0], ICON_YCOORD, 18, 19, powerup_icons[STRONGBALL][0]);
                }
                break;
            case 50:
                if (SHOOTBAT) {
                    SHOOTBAT = FALSE;
                    shoot_active = TRUE;
                    magnet_active = FALSE;
                    enhance_ani = FALSE;
                    anim_active = TRUE; // start animasjon og reset variabler
                    anim_frame = 2;
                    anim_counter = 0;
                    put_block(ICON_COORDS[1], ICON_YCOORD, 18, 19, powerup_icons[SHOOTBAT][1]);
                }
                break;
            case 51:
                if (MAGNETBAT) {
                    MAGNETBAT = FALSE;
                    shoot_active = FALSE;
                    magnet_active = TRUE;
                    bat_type = 7;
                    put_block(ICON_COORDS[2], ICON_YCOORD, 18, 19, powerup_icons[MAGNETBAT][2]);
                }
                break;
            case 52:
                if (LASERRESCUE) {
                    LASERRESCUE = FALSE;
                    laser_active = TRUE;
                    put_block(4, 197, 220, 2, rescue_line);
                    put_block(ICON_COORDS[3], ICON_YCOORD, 18, 19, powerup_icons[LASERRESCUE][3]);
                }
                break;
            case 32:
                if (level_jump) {       // CHEAT 1: Level-skipping
                    next_level = TRUE;
                }
        }
    }

    if (menu_active) {
        switch (page) {
            case 1:     // Hovedmeny
                // Begrens selector
                if (selector_offset < 0)
                    selector_offset = 0;
                if (selector_offset > 4)
                    selector_offset = 4;

                set_font_dimensions(16, 15);
                center_write("MEGABREAK", 3, 65, 222, bigfont);
                set_font_dimensions(8, 8);
                center_write("PLAY GAME", 3, 87, 222, smallfont);
                center_write("OPTIONS", 3, 97, 222, smallfont);
                center_write("VIEW HISCORES", 3, 107, 222, smallfont);
                center_write("EXIT TO DOS", 3, 117, 222, smallfont);
                break;
            case 2:     // Options meny
                // Begrens selector
                if (selector_offset < 0)
                    selector_offset = 0;
                if (selector_offset > 2)
                    selector_offset = 2;

                // Konstruer et bilde av hvordan skjermen ser ut 'under' menyen
                block_copy_b(3,3,222,196,bgscreen,3,3,vrscreen);
                bdraw_bricks();
                bput_block(Ball.X, Ball.Y, 4, 4, balls[balltype], vrscreen);
                                set_font_dimensions(16, 15);
                center_write("OPTIONS", 3, 65, 222, bigfont);
                set_font_dimensions(8, 8);
                center_write("LIVES                ", 3, 87, 222, smallfont);
                num_fwrite(NUM_LIVES, 198, 87, 1, smallfont);

                block_copy(145,97,58,8,vrscreen,145,97);
                if (speed == 1)
                    center_write("BALL SPEED       SLOW", 3, 97, 222, smallfont);
                if (speed == 1.5)
                    center_write("BALL SPEED     MEDIUM", 3, 97, 222, smallfont);
                if (speed == 2.0)
                    center_write("BALL SPEED       FAST", 3, 97, 222, smallfont);

                block_copy(135,107,70,8,vrscreen,135,107);
                if (sensitivity == 32)
                    center_write("BAT SPEED      LOWEST", 3, 107, 222, smallfont);
                if (sensitivity == 26)
                    center_write("BAT SPEED         LOW", 3, 107, 222, smallfont);
                if (sensitivity == 20)
                    center_write("BAT SPEED      MEDIUM", 3, 107, 222, smallfont);
                if (sensitivity == 14)
                    center_write("BAT SPEED        HIGH", 3, 107, 222, smallfont);
                if (sensitivity ==  8)
                    center_write("BAT SPEED     HIGHEST", 3, 107, 222, smallfont);
                break;
            case 3:
                set_font_dimensions(16, 15);
                center_write("HI-SCORES", 3, 35, 222, bigfont);
                set_font_dimensions(8, 8);
                center_write(" NAME       L  SCORE     ", 3, 57, 222, smallfont);
                for (x=0; x<10; x++) {
                    write(hiscore_table[x].name, 10, 77+(10*x), smallfont);
                    num_fwrite(hiscore_table[x].level, 100, 77+(10*x), 2, smallfont);
                    num_fwrite(hiscore_table[x].score, 130, 77+(10*x), 9, smallfont);
                }
        }


        if (option_selected) {
            switch (page) {
                case 1:     // Hoved-meny
                    switch (selector_offset) {
                        case 0:
                            demo_mode = FALSE;
                            new_game = TRUE;
                            menu_active = SHUT_DOWN;
                            pause = FALSE;
                            break;
                        case 1:
                            new_page = TRUE;
                            page_type = OPTIONS;
                            break;
                        case 2:
                            new_page = TRUE;
                            page_type = HISCORES;
                            break;
                        case 3:
                            alive = FALSE;
                            lives = 0;
                            break;
                    }
                    break;
                case 2:     // Options
                    switch (selector_offset) {
                        case 0:
                            if (NUM_LIVES < 5)
                                NUM_LIVES++;
                            else
                                NUM_LIVES = 1;
                            break;
                        case 1:
                            if (speed == 1.0)
                                speed = 1.5;
                            else if (speed == 1.5)
                                speed = 2.0;
                            else if (speed = 2.0)
                                speed = 1.0;
                            break;
                        case 2:
                            if (sensitivity > 8)
                                sensitivity -= 6;
                            else
                                sensitivity = 32;
                            set_mouse_sens(sensitivity, 16);
                    }
            }
            option_selected = FALSE;
        }


        if (selector_updated && page != 3) {
            // Konstruer et bilde av hvordan skjermen ser ut 'under' menyen
            block_copy_b(3,3,222,196,bgscreen,3,3,vrscreen);
            bdraw_bricks();
            bput_block(Ball.X, Ball.Y, 4, 4, balls[balltype], vrscreen);
            // Kopier til skjermen
            block_copy(sel_offsets[page],85,8,60,vrscreen,sel_offsets[page],85);
        }
        // Oppdater selector
        if (page != 3)
            put_block(sel_offsets[page], 87+(10*selector_offset), 8, 7, select);

        if (menu_active == SHUT_DOWN) {
            // Konstruer et bilde av hvordan skjermen ser ut 'under' menyen
            block_copy_b(3,3,222,196,bgscreen,3,3,vrscreen);
            bdraw_bricks();
            bput_block(Ball.X, Ball.Y, 4, 4, balls[balltype], vrscreen);
            // Kopier til skjermen
            block_copy(3,3,222,196,vrscreen,3,3);
            menu_active = FALSE;
        }

        if (new_page) {
            page = page_type;

            // Konstruer et bilde av hvordan skjermen ser ut 'under' menyen
            block_copy_b(3,3,222,196,bgscreen,3,3,vrscreen);
            bdraw_bricks();
            bput_block(Ball.X, Ball.Y, 4, 4, balls[balltype], vrscreen);
            // Kopier til skjermen
            block_copy(3,3,222,196,vrscreen,3,3);


            new_page = FALSE;
        }
    }

    if (new_game)
        goto NEW_GAME;

    if (new_message) {
        new_message = FALSE;
        put_block(233, 189, 78, 4, messages[message]);
        msgcounter = 200;
    }

    if (magnet_active && shoot_active)
        magnet_active = FALSE;


    if (shot_delay)
        shot_delay--;

    // Sjekk etter nye avfyringer
    if (shoot_active && shot_delay == 0) {
        if (BUTTON_STATE == 1) { // Venstre museknapp for ?skyte
            // Legg til et nytt skudd p?shot-lista
            Shot = new shot;
            sprevnode = slist_t -> prev;
            slist_t -> prev = Shot;      // Ny node bak list_t
            Shot -> next = slist_t;      // node peker til list_t
            Shot -> prev = sprevnode;    // node peker tilbake p?forrige node
            sprevnode -> next = Shot;    // forrige node peker p?nye

            Shot -> X = padX+12;
            Shot -> Y = PAD_Y - speed;

            rekyl_active = TRUE;
            rekyl_count = rekyl_delay;

            shot_delay = SHOT__DELAY;
        }
    }

    if (rekyl_active) {
        rekyl_count--;
        if (!rekyl_count)
            rekyl_active = FALSE;
    }

    // G?gjennom liste av shots
    sreader = slist_h -> next;

    while (sreader -> next) {
        Shot = sreader;

        block_copy(Shot -> X, Shot -> Y, 2, 5, bgscreen, Shot -> X, Shot -> Y);
        Shot -> Y -= BULLET_SPEED;
        put_block(Shot -> X, Shot -> Y, 2, 5, bullet);

        if (Shot -> Y <= 151) {
            Shot -> gridX = ((Shot -> X-4)/17)+1;
            Shot -> gridY = ((Shot -> Y-12)/7)+1;
            Shot -> brickX = ((Shot -> X-4)/17*17)+4;
            Shot -> brickY = ((Shot -> Y-12)/7*7)+12;
            if (brickmap[level][13*(Shot -> gridY-1)+(Shot -> gridX-1)] != 11) {
                shot_destroy_brick(Shot -> gridX, Shot -> gridY, Shot -> brickX, Shot -> brickY);
                block_copy(Shot -> X, Shot -> Y, 2, 5, bgscreen, Shot -> X, Shot -> Y);
                // Slett dette skuddet
                sprevnode = Shot -> prev;
                sprevnode -> next = Shot -> next;
                snextnode = Shot -> next;
                snextnode -> prev = sprevnode;
                delete Shot;
            }
            else {
                Shot -> X += 1; // Sjekk andre halvpart av kulen ox?.

                Shot -> gridX = ((Shot -> X-4)/17)+1;
                Shot -> gridY = ((Shot -> Y-12)/7)+1;
                Shot -> brickX = ((Shot -> X-4)/17*17)+4;
                Shot -> brickY = ((Shot -> Y-12)/7*7)+12;
                if (brickmap[level][13*(Shot -> gridY-1)+(Shot -> gridX-1)] != 11) {
                    shot_destroy_brick(Shot -> gridX, Shot -> gridY, Shot -> brickX, Shot -> brickY);
                    block_copy((Shot -> X-1), Shot -> Y, 2, 5, bgscreen, (Shot -> X-1), Shot -> Y);
                    // Slett dette skuddet
                    sprevnode = Shot -> prev;
                    sprevnode -> next = Shot -> next;
                    snextnode = Shot -> next;
                    snextnode -> prev = sprevnode;
                    delete Shot;
                }
                else
                    Shot -> X -= 1;
            }
        }
        if (Shot -> Y < 12) {
            block_copy(Shot -> X, Shot -> Y, 2, 5, bgscreen, Shot -> X, Shot -> Y);
                // Slett dette skuddet
                sprevnode = Shot -> prev;
                sprevnode -> next = Shot -> next;
                snextnode = Shot -> next;
                snextnode -> prev = sprevnode;
                delete Shot;
        }
        sreader = sreader -> next;
    }

    if (strongball_active) {
        if (strong_counter++ >= strong_delay) {
            strong_counter = 0;
            if (strong_state == 0) {
                strong_state = 1;
            }
            else {
                strong_state = 0;
            }
        }
    }

    if (msgcounter) {
        msgcounter--;
        if (!msgcounter)
            draw_box(230, 189, 78, 4, 0);
    }

    // Oppdater tid
    rawtime = time(NULL);
    _gmtime(&rawtime, &newtime);

    if ((newtime.tm_sec != oldtime) && (!pause)) {
        oldtime = newtime.tm_sec;
        if (gametime > 0)
            gametime--;
   }

    if (BONUSLETTER == AVAILABLE) {
        BONUSLETTER = FALSE;
        put_block(LETTER_COORDS[bonusl_counter], LETTER_YCOORD, 12, 11, bonus_letters[1][bonusl_counter]);
        bonusl_counter++;
        if (bonusl_counter == 5) {
            put_block(PANEL_XCOORD, PANEL_COORDS[0], 46, 7, panel_displays[0][0]);
            put_block(PANEL_XCOORD, PANEL_COORDS[1], 46, 7, panel_displays[0][1]);
            put_block(PANEL_XCOORD, PANEL_COORDS[2], 46, 7, panel_displays[0][2]);
            put_block(PANEL_XCOORD, PANEL_COORDS[3], 46, 7, panel_displays[0][3]);

            if (panelbonus_counter < 3)
                bonus_mul = 2 + panelbonus_counter;
            else {
                score += score;
                bonus_mul = 1;
            }

            put_block(PANEL_XCOORD, PANEL_COORDS[panelbonus_counter], 46, 7, panel_displays[1][panelbonus_counter]);
            panelbonus_counter++;
            if (panelbonus_counter > 3)
                panelbonus_counter = 0;
            bonusl_counter = 0;
            put_block(LETTER_COORDS[0], LETTER_YCOORD, 12, 11, bonus_letters[0][0]);
            put_block(LETTER_COORDS[1], LETTER_YCOORD, 12, 11, bonus_letters[0][1]);
            put_block(LETTER_COORDS[2], LETTER_YCOORD, 12, 11, bonus_letters[0][2]);
            put_block(LETTER_COORDS[3], LETTER_YCOORD, 12, 11, bonus_letters[0][3]);
            put_block(LETTER_COORDS[4], LETTER_YCOORD, 12, 11, bonus_letters[0][4]);
        }
    }

    if (EXTRALIFE == AVAILABLE) {
        EXTRALIFE = FALSE;
        lives++;
        num_fwrite(lives, DISPLAY_XCOORD+35, LIVES_Y+SPACING, 2, smallfont);
    }

    if (MEGABOMB == AVAILABLE) {
        for (x=0; x<16; x++) {
            draw_box(3,3,222,196, 64+x);
            delay(8);
            wait_vbl();
        }
        draw_box(3,3,222,196, 0);
        next_level = TRUE;
        MEGABOMB = FALSE;
    }

    if (ENHANCER == AVAILABLE) {
        if (!shoot_active)
            enhance_ani = TRUE;
        enhance_count = ENHANCE__COUNT;
        SHOT__DELAY = 5;
        laser_count = 2;
        ENHANCER = 5;
        MAGNETBAT = FALSE;
    }

    if (enhance_ani) {
        enhance_count--;
        if (!enhance_count) {
            enhance_count = ENHANCE__COUNT;
            if (!bat_type)
                bat_type = 8;
            else
                bat_type = 0;
        }
    }

    if (next_level && !autobat) {
        if (level == last_level) {
            if (user_levels) {
                set_font_dimensions(8, 8);
                center_write("CONGRATULATIONS!", 3, 60, 222, smallfont);
                center_write("YOU HAVE COMPLETED", 3, 80, 222, smallfont);
                center_write("YOUR CUSTOM LEVELS", 3, 90, 222, smallfont);
                center_write("GO AND MAKE SOME MORE..", 3, 100, 222, smallfont);
            }
            else {
                set_font_dimensions(8, 8);
                center_write("CONGRATULATIONS!", 3, 6, 222, smallfont);
                center_write("YOU HAVE COMPLETED", 3, 80, 222, smallfont);
                center_write("MEGABREAK", 3, 90, 222, smallfont);
                center_write("GET READY FOR BATTLE..", 3, 100, 222, smallfont);
            }
            getch();

            goto end;
	    }
        else if (bonus_mode) {
            bonus_mode = FALSE;
            level = prevlevel;
        }

	    next_level = FALSE;
	    level++;

        if (gametime > 0) {
            secret_level_count++;
            set_font_dimensions(8, 8);
            center_write("TIME BONUS!", 3, 70, 222, smallfont);

            if (secret_level_count == 3) {
                center_write("ENTERING BONUS LEVEL", 2, 90, 222, smallfont);
                bonus_mode = TRUE;
                prevlevel = level;
                level = last_level + 2;
                secret_level_count = 0;
                LinkedFileRead("gfx.dat", brickmap[41], 43847, 260);
            }
            else {
                num_fwrite((3-secret_level_count), 20, 90, 1, smallfont);
                write("MORE FOR BONUS LEVEL", 38, 90, smallfont);
            }
        }
        if (gametime > 0)
            delay(1700);
        else
            delay(500);
        goto NEXT_LEVEL;
	}
    if (next_level && autobat) {
        level = last_level + 1;
        for (i=0; i<260; i++)
            brickmap[last_level+1][i] = random(0, 5);
        next_level = FALSE;
        goto NEXT_LEVEL;
    }

    num_fwrite(score, DISPLAY_XCOORD, SCORE_Y+SPACING, 9, smallfont);
    num_fwrite(BrickCount, DISPLAY_XCOORD+35, BRICKS_Y+SPACING, 3, smallfont);
    num_fwrite(gametime, DISPLAY_XCOORD+35, TIME_Y + SPACING, 3, smallfont);

	// ** Slett sprites **

    wait_vbl();                                 // Vent p?VBL

    block_copy(padX,PAD_Y,26,(5+shoot_active),bgscreen, padX, PAD_Y);           // Slett BAT
    block_copy(Ball.X, Ball.Y, 4, 4, bgscreen, Ball.X, Ball.Y);       // Slett BALL

	//  ** Inkrementer Ball-koordinater **

    if (!pause) {
        Ball.X += Ball.xinc;
        Ball.Y += Ball.yinc;
    }

	// Sjekk om ballen fortsatt er i spill
    if (Ball.Y > YLIMIT) {
        if (laser_active && laser_count > 0) {
            Ball.yinc = -Ball.yinc;
            laser_count--;
            put_block(4, 197, 220, 2, rescue_line);
            if (!laser_count)
                block_copy(4, 197, 220, 2, bgscreen, 4, 197);
        }
        else {
            if (bonus_mode)
                next_level = TRUE;
            else {
                alive = FALSE;
                laser_count--;
                magnet_active = FALSE;
                ball_offset = 11;
                disappear_bat();
            }
        }
    }

    } // while - alive

    lives--;

    if (lives == 0)
        GAMEOVER = TRUE;

    } // while - lives > 0

    end:

    if (GAMEOVER) {
        GAMEOVER = FALSE;
        center_write("GAME OVER", 3, 80, 222, smallfont);
        delay(1000);
        if (score > lowest_score) {
            block_copy_b(3,3,222,196,bgscreen,3,3,vrscreen);
            bdraw_bricks();
            bput_block(Ball.X, Ball.Y, 4, 4, balls[balltype], vrscreen);
            new_hiscore();
            update_hiscores();
        }
        goto DEMO_MODE;
    }
    
    write_config();
    reset_mouse();          // Avslutt...
    text_mode();
    free_memory();
    show_end_screen();
} // main

/*
	************************** Funksjoner *************************
*/

void draw_bricks()
{
    int x,y,i=0,dummy, dummy2;

    BrickCount = 0; // Slik at den nye brickcount ikke skal legges til den
            // gamle..

    for (y=0; y<20; y++)
    {
      for (x=0; x<13; x++)
      {
    if (brickmap[level][i] != 11)
    {
        draw_box((x*17)+5, ((y*7)+13), 16, 6, 0);
        put_block((x*17)+4, (y*7)+12, 16, 6, bricks[brickmap[level][i]]);
        BrickCount++;
        if (((y+1) < 20) && (brickmap[level][(i+13)] != 11))
            put_block((x*17)+4, ((y+1)*7)+12, 16, 6, lightbrick);
    }
    i++;
      }
      delay(15);
      wait_vbl();
    }
}

void bdraw_bricks()
{
    int x,y,i=0;

    BrickCount = 0; // Slik at den nye brickcount ikke skal legges til den
            // gamle..

    for (y=0; y<20; y++)
    {
      for (x=0; x<13; x++)
      {
    if (brickmap[level][i] != 11)
    {
        bdraw_box((x*17)+5, (y*7)+13, 16, 6, 0, vrscreen);
        bput_block((x*17)+4, (y*7)+12, 16, 6, bricks[brickmap[level][i]], vrscreen);
        BrickCount++;
    }
    i++;
      }
    }
}

// Finner hotspots nÜr ballen gÜr mot vest

void find_westhotspots()
{
    BallX[0] = (Ball.X-1)+i;
    BallX[1] = (Ball.X+BALLWIDTH)+i;
    BallX[2] = BallX[0];
    BallX[3] = BallX[1];

    BallY[0] = (Ball.Y-1)+i;
    BallY[1] = BallY[0];
    BallY[2] = (Ball.Y+BALLHEIGHT)+i;
    BallY[3] = BallY[2];
}

// Finner hotspots nÜr ballen gÜr mot õst

void find_easthotspots()
{
    BallX[0] = (Ball.X-1)+i;
    BallX[1] = (Ball.X+BALLWIDTH)+i;
    BallX[2] = BallX[0];
    BallX[3] = BallX[1];

    BallY[0] = (Ball.Y-1)-i;
    BallY[1] = BallY[0];
    BallY[2] = (Ball.Y+BALLHEIGHT)-i;
    BallY[3] = BallY[2];
}

void calc_brickcoords()
{
    BrickCoords[X][0] = ((BallX[0]-4)/17*17)+4;
    BrickCoords[X][1] = ((BallX[1]-4)/17*17)+4;
    BrickCoords[X][2] = ((BallX[2]-4)/17*17)+4;
    BrickCoords[X][3] = ((BallX[3]-4)/17*17)+4;

    BrickCoords[Y][0] = ((BallY[0]-12)/7*7)+12;
    BrickCoords[Y][1] = ((BallY[1]-12)/7*7)+12;
    BrickCoords[Y][2] = ((BallY[2]-12)/7*7)+12;
    BrickCoords[Y][3] = ((BallY[3]-12)/7*7)+12;
}

void calc_gridcoords()
{
    GridX[0] = ((BallX[0]-4)/17)+1;
    GridX[1] = ((BallX[1]-4)/17)+1;
    GridX[2] = ((BallX[2]-4)/17)+1;
    GridX[3] = ((BallX[3]-4)/17)+1;

    GridY[0] = ((BallY[0]-12)/7)+1;
    GridY[1] = ((BallY[1]-12)/7)+1;
    GridY[2] = ((BallY[2]-12)/7)+1;
    GridY[3] = ((BallY[3]-12)/7)+1;

}

void find_actuabricks()
{
    short i2, brick;

    for (i2=0; i2<4; i2++) {
        // Sjekk om denne brikken faktisk eksisterer
        brick = brickmap[level][13*(GridY[i2]-1)+(GridX[i2]-1)];
        if (brick < 11 || brick > 11) { // Hvis brikken eksisterer...
            BrickTable[i2] = 1;
        }
        else BrickTable[i2] = 0;
    }
}

void handle_doublebricks(short brick1, short brick2, short dim)
{
    // Sjekk om de to kandidat-brikkene er samme brikke eller to forskjellige
    if (BrickCoords[dim][brick1] == BrickCoords[dim][brick2]) {
	// Fjern en brikke
	destroy_brick(brick1);
    } // like brikker

    // Brikkene er to forskjellige
    else {
	// Fjern to brikker
	destroy_brick(brick1);
	destroy_brick(brick2);
    } // forskjellige brikker

    if (dim == X)
	Ball.yinc = Ball.yinc * -1;
    else
	Ball.xinc = Ball.xinc * -1;
}

/*
    Denne funksjonen fjerner en brikke

    Funksjonen hÜndterer n?impassable bricks, og impassable from below og
    above - bricks.
*/

void destroy_brick(short bricknum)
{
    short brick;
    // Finn ut hva slags brikke som er truffet
    brick = brickmap[level][(GridY[bricknum]-1)*13+(GridX[bricknum]-1)];

    if (brick < 5) {
        erase_brick(bricknum);
        brickscore = (50 * (strongball_active+1));
        if (bonus_mul > 1)
            brickscore *= bonus_mul;
        score += brickscore;
    }
    else if (brick < 10) {
        erase_brick(bricknum);
        brickscore = (100 * (strongball_active+1));
        if (bonus_mul > 1)
            brickscore *= bonus_mul;
        score += brickscore;
    }
    else if (brick == 12) {
        erase_brick(bricknum);
        brickscore = (250 * (strongball_active+1));
        if (bonus_mul > 1)
            brickscore *= bonus_mul;
        score += brickscore;
    }

    if (brick > 12 && strongball_active) {
        erase_brick(bricknum);
        brickscore = 500;
        if (bonus_mul > 1)
            brickscore *= bonus_mul;
        score += brickscore;
    }
    else if (brick > 12) {
        flash = new Flash;
        flash -> screenX = BrickCoords[X][bricknum];
        flash -> screenY = BrickCoords[Y][bricknum];
        flash -> delay = 5;
        flash -> flashtype = brick - 8;
        list_t -> next = flash;  // Sett gammel node til ?peke p?nye
        prevnode = list_t;      // Lagre gammel node i prevnode
        list_t = flash;          // Sett tail til nye node
        list_t -> prev = prevnode; // tail peker tilbake p?gammel
        list_t -> next = NULL;  // Sett nye node til ?ikke peke p?noe

        flash -> gridX = GridX[bricknum];
        flash -> gridY = GridY[bricknum];

        brickmap[level][(GridY[bricknum]-1)*13+(GridX[bricknum]-1)] = --brick;
    }

    if (BrickCount <= 0)
        next_level = TRUE;

    bonus = random(1, 140);
    if ((bonus < 24) && (!balltype) && (brick < 10) && (!bonus_mode)) {
        if (bonus <= 4) {
            if (!STRONGBALL) {
                STRONGBALL = TRUE;
                balltype = 1;
                new_message = TRUE;
                message = STRONG;
            }
            goto endbonus;
        }
        if (bonus <= 8) {
            if (!SHOOTBAT) {
                SHOOTBAT = TRUE;
                balltype = 2;
                new_message = TRUE;
                message = SHOOTING;
            }
            goto endbonus;
        }
        if (bonus <= 12) {
            if (!MAGNETBAT) {
                MAGNETBAT = TRUE;
                balltype = 3;
                new_message = TRUE;
                message = MAGNET;
            }
            goto endbonus;
        }
        if (bonus <= 14) {
            if (!LASERRESCUE) {
                LASERRESCUE = TRUE;
                balltype = 4;
                new_message = TRUE;
                message = LASER;
            }
            goto endbonus;
        }
        if (bonus == 15) {
            EXTRALIFE = TRUE;
            balltype = 5;
            new_message = TRUE;
            message = LIFE;
            goto endbonus;
        }
        if (bonus == 16) {
            if (!autobat) {
                MEGABOMB = TRUE;
                balltype = 6;
                new_message = TRUE;
                message = BOMB;
            }
            goto endbonus;
        }
        if (bonus <= 18) {
            if (!autobat) {
                ENHANCER = TRUE;
                balltype = 10;
                new_message = TRUE;
                message = ENHANCE;
            }
            goto endbonus;
        }
        if (bonus <= 23) {
            BONUSLETTER = TRUE;
            balltype = 7;
            new_message = TRUE;
            message = BONUS;
        }
        endbonus:
        // Oppdater power-up ikoner..
        put_block(ICON_COORDS[0], ICON_YCOORD, 18, 19, powerup_icons[STRONGBALL][0]);
        put_block(ICON_COORDS[1], ICON_YCOORD, 18, 19, powerup_icons[SHOOTBAT][1]);
        put_block(ICON_COORDS[2], ICON_YCOORD, 18, 19, powerup_icons[MAGNETBAT][2]);
        put_block(ICON_COORDS[3], ICON_YCOORD, 18, 19, powerup_icons[LASERRESCUE][3]);
    }
}

void shot_destroy_brick(short gridx, short gridy, short x, short y)
{
    short brick;
    // Finn ut hva slags brikke som er truffet
    brick = brickmap[level][(gridy-1)*13+(gridx-1)];

    if (brick < 10 || brick == 12) {
        block_copy(x, y, 17, 7, bgscreen, x, y);
        BrickCount--;
        brickmap[level][(gridy-1)*13+(gridx-1)] = 11; // Fjern brikken fra tabell
        score += 100;
    }

    if (brick > 12) {
        flash = new Flash;
        flash -> screenX = x;
        flash -> screenY = y;
        flash -> delay = 5;
        flash -> flashtype = brick - 8;
        list_t -> next = flash;  // Sett gammel node til ?peke p?nye
        prevnode = list_t;      // Lagre gammel node i prevnode
        list_t = flash;          // Sett tail til nye node
        list_t -> prev = prevnode; // tail peker tilbake p?gammel
        list_t -> next = NULL;  // Sett nye node til ?ikke peke p?noe

        flash -> gridX = gridx;
        flash -> gridY = gridy;

        brickmap[level][(gridy-1)*13+(gridx-1)] = --brick;
    }

    if (BrickCount <= 0)
        next_level = TRUE;
}

void erase_brick(short bricknum)
{
    block_copy(BrickCoords[X][bricknum], BrickCoords[Y][bricknum], 17, 7, bgscreen, BrickCoords[X][bricknum], BrickCoords[Y][bricknum]);
    BrickCount--;
    brickmap[level][(GridY[bricknum]-1)*13+(GridX[bricknum]-1)] = 11; // Fjern brikken fra tabell
}

void check_pad()
{
    if ((Ball.Y+BALLHEIGHT >= 190) &&
	(Ball.X+BALLWIDTH >= padX) &&
	(Ball.X <= padX+PADWIDTH)) {
        if (magnet_active && Ball.yinc > 0) {
            ball_launched = FALSE;
            Ball.xinc = 0;
            ball_offset = Ball.X - padX;
            if (ball_offset < 0)
                ball_offset = 0;
            if (ball_offset > 22)
                ball_offset = 22;
        }
        else {
            // Sjekk venstre indre del
            if (Ball.X+BALLWIDTH <= padX+12) {
                Ball.xinc = -speed;
            }

            // Sjekk venstre ytterkant
            if (Ball.X+BALLWIDTH <= padX+4) {
                Ball.xinc = -(speed+1);
            }

            // Sjekk hõyre indre del
            if (Ball.X >= padX+13) {
                Ball.xinc = speed;
            }

            // Sjekk hõyre ytre del
            if (Ball.X >= padX+21) {
                Ball.xinc = (speed+1);
            }

            // F?ballen til ?g?oppover igjen
            Ball.yinc = -speed;
            // Ballen vil vëre normal igjen etter en evt. bonus
            balltype = 0;
            // Sjekk etter 'hidden' power-ups
            if (BONUSLETTER)
                BONUSLETTER = AVAILABLE;
            if (MEGABOMB)
                MEGABOMB = AVAILABLE;
            if (EXTRALIFE)
                EXTRALIFE = AVAILABLE;
            if (ENHANCER)
                ENHANCER = AVAILABLE;
        }
    }
}

void check_walls()
{
    // sjekk venstre-vegg
    if (Ball.X+Ball.xinc <= 3) {
	// Sett ballen til ?g?i motsatt retning
	Ball.xinc = speed;
    }

    // sjekk hõyre-vegg
    if (Ball.X+BALLWIDTH+Ball.xinc >= 224) {
	Ball.xinc = -speed;
    }

    // sjekk tak
    if (Ball.Y+Ball.yinc <= 3) {
	    Ball.yinc = Ball.yinc * -1;
    }
}

void init_list()
{
    // Legg opp en liste med en node + head og tail

    flash = new Flash;
    flash -> screenX = 0;    // Viser at noden ikke er i bruk
    list_h = flash;          // Header peker p?fõrste node
    list_t = flash;          // Tail peker p?siste (og fõrste) node
    list_t -> next = NULL;  // Node har ingen node foran seg
    list_t -> prev = NULL;  // Node har ingen node etter seg
}

void load_pcx_graphics()       // Bare for versjoner som ikke gis ut
{
    pcxRead("tiles.pcx", 0, palette, 0, 0);
    pcxRead("bat.pcx", bats[0], 0, 26, 5);
    pcxRead("blubrick.pcx", bricks[0], 0, 16, 6);
    pcxRead("yelbrick.pcx", bricks[1], 0, 16, 6);
    pcxRead("redbrick.pcx", bricks[2], 0, 16, 6);
    pcxRead("grabrick.pcx", bricks[3], 0, 16, 6);
    pcxRead("whibrick.pcx", bricks[4], 0, 16, 6);
    pcxRead("newblu.pcx", bricks[5], 0, 16, 6);
    pcxRead("newyel.pcx", bricks[6], 0, 16, 6);
    pcxRead("newred.pcx", bricks[7], 0, 16, 6);
    pcxRead("newgray.pcx", bricks[8], 0, 16, 6);
    pcxRead("newwhi.pcx", bricks[9], 0, 16, 6);
    pcxRead("mulbrick.pcx", bricks[12], 0, 16, 6);
    pcxRead("double.pcx", bricks[13], 0, 16, 6);
    pcxRead("triple.pcx", bricks[14], 0, 16, 6);
    pcxRead("fldbl.pcx", flashbricks[5], 0, 16, 6);
    pcxRead("fltrp.pcx", flashbricks[6], 0, 16, 6);
    pcxRead("ball.pcx", balls[0], 0, 4, 4);
    pcxRead("green.pcx", balls[1], 0, 4, 4);
    pcxRead("lblue.pcx", balls[2], 0, 4, 4);
    pcxRead("dblue.pcx", balls[3], 0, 4, 4);
    pcxRead("red.pcx", balls[4], 0, 4, 4);
    pcxRead("hybrid.pcx", balls[5], 0, 4, 4);
    pcxRead("brown.pcx", balls[6], 0, 4, 4);
    pcxRead("dgreen.pcx", balls[7], 0, 4, 4);
    pcxRead("strong.001", balls[8], 0, 4, 4);
    pcxRead("strong.002", balls[9], 0, 4, 4);
    pcxRead("enhaball.pcx", balls[10], 0, 4, 4);
    pcxRead("testscrn.pcx", mainscreen, 0, 320, 200);
    pcxRead("mstrong.pcx", messages[0], 0, 78, 4);
    pcxRead("mshoot.pcx", messages[1], 0, 78, 4);
    pcxRead("mmagnet.pcx", messages[2], 0, 78, 4);
    pcxRead("mlaser.pcx", messages[3], 0, 78, 4);
    pcxRead("3xbricks.pcx", messages[4], 0, 78, 4);
    pcxRead("mbonus.pcx", messages[5], 0, 78, 4);
    pcxRead("mextra.pcx", messages[6], 0, 78, 4);
    pcxRead("menhance.pcx", messages[7], 0, 78, 4);
    pcxRead("mbomb.pcx", messages[8], 0, 78, 4);
    pcxRead("mpause.pcx", messages[9], 0, 78, 4);
    pcxRead("strong.pcx", powerup_icons[0][0], 0, 18, 19);
    pcxRead("strongl.pcx", powerup_icons[1][0], 0, 18, 19);
    pcxRead("shooting.pcx", powerup_icons[0][1], 0, 18, 19);
    pcxRead("shootl.pcx", powerup_icons[1][1], 0, 18, 19);
    pcxRead("magnet.pcx", powerup_icons[0][2], 0, 18, 19);
    pcxRead("magnetl.pcx", powerup_icons[1][2], 0, 18, 19);
    pcxRead("lasersym.pcx", powerup_icons[0][3], 0, 18, 19);
    pcxRead("laserl.pcx", powerup_icons[1][3], 0, 18, 19);
    pcxRead("rescueln.pcx", rescue_line, 0, 220, 2);
    pcxRead("shootbat.pcx", bats[1], 0, 26, 5);
    pcxRead("batshoot.pcx", batshoot, 0, 26, 5);
    pcxRead("bat.001", bats[2], 0, 26, 5);
    pcxRead("bat.002", bats[3], 0, 26, 5);
    pcxRead("bat.003", bats[4], 0, 26, 5);
    pcxRead("bat.004", bats[5], 0, 26, 5);
    pcxRead("bat.005", bats[6], 0, 26, 5);
    pcxRead("enhanbat.pcx", bats[8], 0, 26, 5);
    pcxRead("bullet.pcx", bullet, 0, 2, 5);
    pcxRead("magbat.pcx", bats[7], 0, 26, 5);
    pcxRead("litbrick.pcx", lightbrick, 0, 16, 6);
    pcxRead("b.pcx", bonus_letters[0][0], 0, 12, 11);
    pcxRead("bl.pcx", bonus_letters[1][0], 0, 12, 11);
    pcxRead("o.pcx", bonus_letters[0][1], 0, 12, 11);
    pcxRead("ol.pcx", bonus_letters[1][1], 0, 12, 11);
    pcxRead("n.pcx", bonus_letters[0][2], 0, 12, 11);
    pcxRead("nl.pcx", bonus_letters[1][2], 0, 12, 11);
    pcxRead("u.pcx", bonus_letters[0][3], 0, 12, 11);
    pcxRead("ul.pcx", bonus_letters[1][3], 0, 12, 11);
    pcxRead("s.pcx", bonus_letters[0][4], 0, 12, 11);
    pcxRead("sl.pcx", bonus_letters[1][4], 0, 12, 11);
    pcxRead("x2.pcx", panel_displays[0][0], 0, 46, 7);
    pcxRead("x2l.pcx", panel_displays[1][0], 0, 46, 7);
    pcxRead("x3.pcx", panel_displays[0][1], 0, 46, 7);
    pcxRead("x3l.pcx", panel_displays[1][1], 0, 46, 7);
    pcxRead("x4.pcx", panel_displays[0][2], 0, 46, 7);
    pcxRead("x4l.pcx", panel_displays[1][2], 0, 46, 7);
    pcxRead("jackpot.pcx", panel_displays[0][3], 0, 46, 7);
    pcxRead("jacpotl.pcx", panel_displays[1][3], 0, 46, 7);
    pcxRead("tile01.pcx", tiles[0], 0, 16, 14);
    pcxRead("tile02.pcx", tiles[1], 0, 16, 14);
    pcxRead("tile03.pcx", tiles[2], 0, 16, 14);
    pcxRead("tile04.pcx", tiles[3], 0, 16, 14);
    pcxRead("tile05.pcx", tiles[4], 0, 16, 14);
    pcxRead("tile06.pcx", tiles[5], 0, 16, 14);
    pcxRead("tile07.pcx", tiles[6], 0, 16, 14);
    pcxRead("tile08.pcx", tiles[7], 0, 16, 14);
    pcxRead("tile09.pcx", tiles[8], 0, 16, 14);
    pcxRead("tile10.pcx", tiles[9], 0, 16, 14);
    pcxRead("tile11.pcx", tiles[10], 0, 16, 14);
    pcxRead("tile12.pcx", tiles[11], 0, 16, 14);
    pcxRead("tile13.pcx", tiles[12], 0, 16, 14);
    pcxRead("tile14.pcx", tiles[13], 0, 16, 14);
    pcxRead("tile15.pcx", tiles[14], 0, 16, 14);
    pcxRead("tile16.pcx", tiles[15], 0, 16, 14);
    pcxRead("tile17.pcx", tiles[16], 0, 16, 14);
    pcxRead("tile18.pcx", tiles[17], 0, 16, 14);
    pcxRead("tile19.pcx", tiles[18], 0, 16, 14);
    pcxRead("tile20.pcx", tiles[19], 0, 16, 14);
    pcxRead("tile21.pcx", tiles[20], 0, 16, 14);
    pcxRead("select.pcx", select, 0, 8, 7);
}

void load_dat_graphics()
{
    pcxRead("tiles.pcx", 0, palette, 0, 0);
    LinkedPCXRead("gfx.dat", bats[0], 0);
    LinkedPCXRead("gfx.dat", bricks[0], 60);
    LinkedPCXRead("gfx.dat", bricks[1], 86);
    LinkedPCXRead("gfx.dat", bricks[2], 112);
    LinkedPCXRead("gfx.dat", bricks[3], 138);
    LinkedPCXRead("gfx.dat", bricks[4], 164);
    LinkedPCXRead("gfx.dat", bricks[5], 190);
    LinkedPCXRead("gfx.dat", bricks[6], 290);
    LinkedPCXRead("gfx.dat", bricks[7], 390);
    LinkedPCXRead("gfx.dat", bricks[8], 490);
    LinkedPCXRead("gfx.dat", bricks[9], 590);
    LinkedPCXRead("gfx.dat", bricks[12], 677);
    LinkedPCXRead("gfx.dat", bricks[13], 711);
    LinkedPCXRead("gfx.dat", bricks[14], 761);
    LinkedPCXRead("gfx.dat", flashbricks[5], 815);
    LinkedPCXRead("gfx.dat", flashbricks[6], 850);
    LinkedPCXRead("gfx.dat", balls[0], 890);
    LinkedPCXRead("gfx.dat", balls[1], 910);
    LinkedPCXRead("gfx.dat", balls[2], 930);
    LinkedPCXRead("gfx.dat", balls[3], 950);
    LinkedPCXRead("gfx.dat", balls[4], 981);
    LinkedPCXRead("gfx.dat", balls[5], 1001);
    LinkedPCXRead("gfx.dat", balls[6], 1021);
    LinkedPCXRead("gfx.dat", balls[7], 1052);
    LinkedPCXRead("gfx.dat", balls[8], 1083);
    LinkedPCXRead("gfx.dat", balls[9], 1103);
    LinkedPCXRead("gfx.dat", balls[10], 1123);
    LinkedPCXRead("gfx.dat", mainscreen, 1143);
    LinkedPCXRead("gfx.dat", messages[0], 10453);
    LinkedPCXRead("gfx.dat", messages[1], 10642);
    LinkedPCXRead("gfx.dat", messages[2], 10854);
    LinkedPCXRead("gfx.dat", messages[3], 11031);
    LinkedPCXRead("gfx.dat", messages[4], 11236);
    LinkedPCXRead("gfx.dat", messages[5], 11401);
    LinkedPCXRead("gfx.dat", messages[6], 11608);
    LinkedPCXRead("gfx.dat", messages[7], 11793);
    LinkedPCXRead("gfx.dat", messages[8], 11951);
    LinkedPCXRead("gfx.dat", messages[9], 12137);
    LinkedPCXRead("gfx.dat", powerup_icons[0][0], 12277);
    LinkedPCXRead("gfx.dat", powerup_icons[1][0], 12459);
    LinkedPCXRead("gfx.dat", powerup_icons[0][1], 12641);
    LinkedPCXRead("gfx.dat", powerup_icons[1][1], 12777);
    LinkedPCXRead("gfx.dat", powerup_icons[0][2], 12914);
    LinkedPCXRead("gfx.dat", powerup_icons[1][2], 13139);
    LinkedPCXRead("gfx.dat", powerup_icons[0][3], 13364);
    LinkedPCXRead("gfx.dat", powerup_icons[1][3], 13497);
    LinkedPCXRead("gfx.dat", rescue_line, 13630);
    LinkedPCXRead("gfx.dat", bats[1], 13652);
    LinkedPCXRead("gfx.dat", batshoot, 13713);
    LinkedPCXRead("gfx.dat", bats[2], 13798);
    LinkedPCXRead("gfx.dat", bats[3], 13869);
    LinkedPCXRead("gfx.dat", bats[4], 13935);
    LinkedPCXRead("gfx.dat", bats[5], 13993);
    LinkedPCXRead("gfx.dat", bats[6], 14051);
    LinkedPCXRead("gfx.dat", bats[8], 14109);
    LinkedPCXRead("gfx.dat", bullet, 14151);
    LinkedPCXRead("gfx.dat", bats[7], 14165);
    LinkedPCXRead("gfx.dat", lightbrick, 14235);
    LinkedPCXRead("gfx.dat", bonus_letters[0][0], 14256);
    LinkedPCXRead("gfx.dat", bonus_letters[1][0], 14351);
    LinkedPCXRead("gfx.dat", bonus_letters[0][1], 14460);
    LinkedPCXRead("gfx.dat", bonus_letters[1][1], 14557);
    LinkedPCXRead("gfx.dat", bonus_letters[0][2], 14670);
    LinkedPCXRead("gfx.dat", bonus_letters[1][2], 14774);
    LinkedPCXRead("gfx.dat", bonus_letters[0][3], 14890);
    LinkedPCXRead("gfx.dat", bonus_letters[1][3], 14989);
    LinkedPCXRead("gfx.dat", bonus_letters[0][4], 15102);
    LinkedPCXRead("gfx.dat", bonus_letters[1][4], 15191);
    LinkedPCXRead("gfx.dat", panel_displays[0][0], 15300);
    LinkedPCXRead("gfx.dat", panel_displays[1][0], 15366);
    LinkedPCXRead("gfx.dat", panel_displays[0][1], 15479);
    LinkedPCXRead("gfx.dat", panel_displays[1][1], 15545);
    LinkedPCXRead("gfx.dat", panel_displays[0][2], 15657);
    LinkedPCXRead("gfx.dat", panel_displays[1][2], 15728);
    LinkedPCXRead("gfx.dat", panel_displays[0][3], 15847);
    LinkedPCXRead("gfx.dat", panel_displays[1][3], 16005);
    LinkedPCXRead("gfx.dat", tiles[0], 16225);
    LinkedPCXRead("gfx.dat", tiles[1], 16611);
    LinkedPCXRead("gfx.dat", tiles[2], 16727);
    LinkedPCXRead("gfx.dat", tiles[3], 16886);
    LinkedPCXRead("gfx.dat", tiles[4], 16982);
    LinkedPCXRead("gfx.dat", tiles[5], 17178);
    LinkedPCXRead("gfx.dat", tiles[6], 17274);
    LinkedPCXRead("gfx.dat", tiles[7], 17470);
    LinkedPCXRead("gfx.dat", tiles[8], 17804);
    LinkedPCXRead("gfx.dat", tiles[9], 17948);
    LinkedPCXRead("gfx.dat", tiles[10], 18104);
    LinkedPCXRead("gfx.dat", tiles[11], 18217);
    LinkedPCXRead("gfx.dat", tiles[12], 18374);
    LinkedPCXRead("gfx.dat", tiles[13], 18480);
    LinkedPCXRead("gfx.dat", tiles[14], 18750);
    LinkedPCXRead("gfx.dat", tiles[15], 18902);
    LinkedPCXRead("gfx.dat", tiles[16], 19090);
    LinkedPCXRead("gfx.dat", tiles[17], 19311);
    LinkedPCXRead("gfx.dat", tiles[18], 19491);
    LinkedPCXRead("gfx.dat", tiles[19], 19707);
    LinkedPCXRead("gfx.dat", tiles[20], 19817);
    LinkedPCXRead("gfx.dat", select, 19997);

    ReadPalette("megbreak.pal", palette);
}

void clear_main_screen()
{
    block_copy(4, 4, 215, 190, bgscreen, 4, 4);
}

void draw_tiles()
{
    tile = random(0, 20);

    for (y=0; y<14; y++) {
        for (x=0; x<16; x++)
            bput_block((x*16), ((y*14)+3), 16, 14, tiles[tile], bgscreen);
    }
    block_copy(3, 3, 222, 196, bgscreen, 3, 3);
}

void appear_bat()
{
    int i;

    for (i=200; i>186; i--) {
        cput_block((padX+11), i, 4, 4, balls[0]);
        cput_block(padX, (i+4), 26, 5, bats[0]);
        wait_vbl();
        block_copy(padX, 186, 26, 14, bgscreen, padX, 186);
    }
}

void disappear_bat()
{
    int i;

    for (i=186; i<200; i++) {
        cput_block(padX, (i+4), 26, 5, bats[0]);
        wait_vbl();
        block_copy(padX, 186, 26, 14, bgscreen, padX, 186);
    }
}

void load_levels()
{
    if (user_levels) {
        short result, c=0;
        // Sjekk etter map-filer i MegaBreak katalogen

        result = _dos_findfirst("*.map", _A_NORMAL, &fileinfo);

        while (!result) {
            if (fileinfo.size == 260) {
                brick_load(fileinfo.name, brickmap[c]);
                c++;
            }
            result = _dos_findnext(&fileinfo);

            if (c > 39)
                result = 1;
        }
        last_level = (c-1);
    }
    else {
        int offset = 33447;

        LinkedFileRead("gfx.dat", brickmap[ 0], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 1], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 2], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 3], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 4], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 5], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 6], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 7], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 8], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[ 9], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[10], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[11], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[12], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[13], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[14], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[15], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[16], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[17], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[18], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[19], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[20], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[21], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[22], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[23], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[24], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[25], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[26], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[27], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[28], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[29], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[30], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[31], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[32], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[33], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[34], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[35], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[36], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[37], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[38], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[39], offset, 260);
        offset += 260;
        LinkedFileRead("gfx.dat", brickmap[41], offset, 260);
    }
}

void reset()
{
    lives = NUM_LIVES;
    score = 0;
    enhance_ani = FALSE;
    ENHANCER = FALSE;
    MEGABOMB = FALSE;
    BONUSLETTER = FALSE;
    EXTRALIFE = FALSE;
    STRONGBALL = FALSE;
    SHOOTBAT = FALSE;
    MAGNETBAT = FALSE;
    LASERRESCUE = FALSE;
    put_block(ICON_COORDS[0], ICON_YCOORD, 18, 19, powerup_icons[STRONGBALL][0]);
    put_block(ICON_COORDS[1], ICON_YCOORD, 18, 19, powerup_icons[SHOOTBAT][1]);
    put_block(ICON_COORDS[2], ICON_YCOORD, 18, 19, powerup_icons[MAGNETBAT][2]);
    put_block(ICON_COORDS[3], ICON_YCOORD, 18, 19, powerup_icons[LASERRESCUE][3]);
    bonusl_counter = 0;
    panelbonus_counter = 0;
    bonus_mul = 0;
    put_block(LETTER_COORDS[0], LETTER_YCOORD, 12, 11, bonus_letters[0][0]);
    put_block(LETTER_COORDS[1], LETTER_YCOORD, 12, 11, bonus_letters[0][1]);
    put_block(LETTER_COORDS[2], LETTER_YCOORD, 12, 11, bonus_letters[0][2]);
    put_block(LETTER_COORDS[3], LETTER_YCOORD, 12, 11, bonus_letters[0][3]);
    put_block(LETTER_COORDS[4], LETTER_YCOORD, 12, 11, bonus_letters[0][4]);
    put_block(PANEL_XCOORD, PANEL_COORDS[0], 46, 7, panel_displays[0][0]);
    put_block(PANEL_XCOORD, PANEL_COORDS[1], 46, 7, panel_displays[0][1]);
    put_block(PANEL_XCOORD, PANEL_COORDS[2], 46, 7, panel_displays[0][2]);
    put_block(PANEL_XCOORD, PANEL_COORDS[3], 46, 7, panel_displays[0][3]);
}

void init_shot_list()
{
    // Legg opp en tom liste med kun header og tail (ubrukt)

    Shot = new shot;
    slist_h = Shot;
    Shot = new shot;
    slist_t = Shot;
    slist_t -> prev = slist_h;    // Bind sammen noder begge veier
    slist_h -> next = slist_t;
    slist_t -> next = NULL;      // Ingen foran
    slist_h -> prev = NULL;      // Ingen bak
}

void delete_shot_list()
{
    // Skudd-listen m?slettes mellom hver level
    sreader = slist_h -> next;

    while (sreader -> next) {
        Shot = sreader;

        sprevnode = Shot -> prev;
        sprevnode -> next = Shot -> next;
        snextnode = Shot -> next;
        snextnode -> prev = sprevnode;
        delete Shot;

        sreader = sreader -> next;
    }
}

void init_hiscores()
{
    // Fõrste gang skal vi legge inn en default hiscore liste
    strcpy(hiscore_table[0].name, "MEGABREAK");
    hiscore_table[0].level = 40;
    hiscore_table[0].score = 500000;
    strcpy(hiscore_table[1].name, "COPYRIGHT");
    hiscore_table[1].level = 40;
    hiscore_table[1].score = 400000;
    strcpy(hiscore_table[2].name, "(C) 1998");
    hiscore_table[2].level = 40;
    hiscore_table[2].score = 300000;
    strcpy(hiscore_table[3].name, "KNUT");
    hiscore_table[3].level = 40;
    hiscore_table[3].score = 200000;
    strcpy(hiscore_table[4].name, "ANDREAS");
    hiscore_table[4].level = 40;
    hiscore_table[4].score = 150000;
    strcpy(hiscore_table[5].name, "RUUD");
    hiscore_table[5].level = 40;
    hiscore_table[5].score = 100000;
    strcpy(hiscore_table[6].name, "REMEMBER");
    hiscore_table[6].level = 40;
    hiscore_table[6].score = 75000;
    strcpy(hiscore_table[7].name, "UNION");
    hiscore_table[7].level = 40;
    hiscore_table[7].score = 50000;
    strcpy(hiscore_table[8].name, "RULEZ");
    hiscore_table[8].level = 40;
    hiscore_table[8].score = 40000;
    strcpy(hiscore_table[9].name, "4EVER");
    hiscore_table[9].level = 40;
    hiscore_table[9].score = 30000;

    lowest_score = hiscore_table[9].score;
}

void update_hiscores()
{
    short i, oldi, ii;

    // Finn ut hvor den nye score skal legges p?lista
    for (i=0; i<10; i++) {
        if (new_entry.score > hiscore_table[i].score) {
            oldi = i;
            i = 10;
        }
    }

    // Rykk ned et hakk
    for (i=9; i>oldi; i--) {
        hiscore_table[i].score = hiscore_table[i-1].score;
        hiscore_table[i].level = hiscore_table[i-1].level;
        for (ii=0; ii<10; ii++)
            hiscore_table[i].name[ii] = hiscore_table[i-1].name[ii];
    }

    hiscore_table[oldi].score = new_entry.score;
    hiscore_table[oldi].level = new_entry.level;
    for (ii=0; ii<10; ii++)
        hiscore_table[oldi].name[ii] = new_entry.name[ii];

    lowest_score = hiscore_table[9].score;
}

void new_hiscore()
{
    block_copy(30, 40, 160, 130, vrscreen, 30, 40);
    set_font_dimensions(8, 8);
    center_write("CONGRATULATIONS!", 3, 67, 222, smallfont);
    center_write("YOU HAVE A HI-SCORE", 3, 77, 222, smallfont);
    center_write("PLEASE ENTER YOUR NAME:", 3, 97, 222, smallfont);

    key=0;
    y=0;

    while (key != 13 && y < 9) {
        key = getch();
        if (key > 96 && key < 123)
            key = toupper(key);

        if (key > 64 && key < 91 || key == 32) {
            new_entry.name[y] = (char)key;
            y++;
            new_entry.name[y] = 0;
            block_copy(30,107,160,8,vrscreen,30,107);
            center_write(new_entry.name, 3, 107, 222, smallfont);
        }

        if (key == 8) {
            y--;
            new_entry.name[y] = 0;
            block_copy(30,107,160,8,vrscreen,30,107);
            center_write(new_entry.name, 3, 107, 222, smallfont);
        }
    }
    new_entry.score = score;
    new_entry.level = level+1;

    // Sjekk etter cheats
    if (new_entry.name[0] == 'F') {      // FISK - enabler level-skipping
        if (new_entry.name[1] == 'I') {
            if (new_entry.name[2] == 'S') {
                if (new_entry.name[3] == 'K') {
                    if (new_entry.name[4] == 0) {
                        level_jump = TRUE;
                    }
                }
            }
        }
    }

}

void startup()
{
    int result;

    // Write warning-message

    cout << endl << "WARNING:   If you run this game in a Win '95 environment, the game may";
    cout << endl << "           crash unexpectedly. Please run this game in DOS mode only.";
    cout << endl;
    cout << endl << "NOTE:      You can bypass Win '95 by pressing F8 when the";
    cout << endl << "           'Starting Windows 95...' message appears, and then selecting";
    cout << endl << "           'Command prompt only'.";
    cout << endl;
    cout << endl;
    cout << endl << "Press any key...";

    getch();
    
    system("cls");

    set_cursor(0,2);

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

    result = get_mouse_info();

    if (result)
        cout << "Mouse driver...........Present" << endl;
    else {
        cout << "A mouse driver was not found!" << endl;
        cout << "Please consult the trouble-shooting section of manual.txt" << endl;
        exit(EXIT_FAILURE);
    }
    if (detect_vga())
        cout << "VGA card...............Present" << endl;
    else {
        cout << "A VGA card was not found!" << endl;
        cout << "This program can only be run on machines with a VGA or 100% compatible" << endl;
        exit(EXIT_FAILURE);
    }

    if (alloc_memory())
        cout << "Allocating memory......Success" << endl;
    else {
        cout << "Error allocating memory!" << endl;
        exit(EXIT_FAILURE);
    }
}

void read_config()
{
    FILE *handle;

    handle = fopen("gfx.dat", "r+b");

    fseek(handle, 44107, SEEK_SET);

    NUM_LIVES = getc(handle);
    speed = getc(handle);
    sensitivity = getc(handle);

    // Gjõr om speed variabel
    if (speed == 2)
        speed = 1.5;
    else if (speed == 3)
        speed = 2.0;

    for (int t=0; t<10; t++) {
        for (int ii=0; ii<9; ii++)
            hiscore_table[t].name[ii] = getc(handle);
        hiscore_table[t].level = getc(handle);

        int num_sifre;
        int nummer;

        num_sifre = getc(handle);

        hiscore_table[t].score = 0;

        for (int ii=(num_sifre-1); ii>=0; ii--) {
            nummer = getc(handle);
            hiscore_table[t].score += exp(10, ii) * nummer;
        }
    }
    lowest_score = hiscore_table[9].score;

    fclose(handle);
}

void write_config()
{
    FILE *handle;
    int t, ii, NUM, i, numindex;
    int numlist[10];

    handle = fopen("gfx.dat", "r+b");

    fseek(handle, 44107, SEEK_SET);

    // Gjõr om speed variabel    ( 1 - 3 )
    if (speed == 1.5)
        speed = 2;
    else if (speed == 2.0)
        speed = 3;

    // Skriv settings fõrst
    putc((char)NUM_LIVES, handle);
    putc((char)speed, handle);
    putc((char)sensitivity, handle);


    for (t=0; t<10; t++) {       // 10 hi-scores
        for (ii=0; ii<9; ii++)
            putc(hiscore_table[t].name[ii], handle);
        putc(hiscore_table[t].level, handle);

        NUM = hiscore_table[t].score;

        for (i=0; i<10; i++)
        {
            numlist[i]=0;
        }

        numindex = 0;

        if (NUM < 10)
        {
            putc(1, handle);    // Antall siffer
            putc(NUM, handle);
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

            putc(numindex, handle);     // Antall siffer

            for (i=(numindex-1); i>=0; i--)
            {
                putc(numlist[i], handle);
            }
        } // else
    }

    fclose(handle);
}

int exp(int nummer, int exp)
{
    int result = 1;

    for (int i=0; i<exp; i++)
        result *= nummer;

    return result;
}

void show_end_screen()
{
    set_cursor(0,23);

    int i = 0, x = 0, y;
    byte bg_color, fg_color;

    for (y=0; y<23; y++) {
         x = 0;
         i = 0;
         while ( *(end_text[y] + i) != '\0') {
               if ( *(end_text[y] + i) < 20 || *(end_text[y] + i) == 143) {
                    fg_color = *(end_text[y] + i);
                    i++;
                    bg_color = *(end_text[y] + i);
                    i++;
               }
        print_character(x, y, *(end_text[y] + i), bg_color, fg_color);
        x++;
        i++;
        }
    }
}

void init_end_screen()
{
     startup_text = " MegaBreak startup v 1.1                                                        \0";
     end_text[ 0] = "⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø\0";
     end_text[ 1] = "?MegaBreak v 1.1                        copyright (c) 1998  Knut Andreas Ruud ?0";
     end_text[ 2] = "√ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¥\0";
     end_text[ 3] = "?Credits                                                                      ?0";
     end_text[ 4] = "?                                                                             ?0";
     end_text[ 5] = "?    Coding/Graphics & Level Design by Knut A. Ruud                           ?0";
     end_text[ 6] = "?    Betatesting/Playtesting by Tommy Berg Kristoffersen                      ?0";
     end_text[ 7] = "√ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¥\0";
     end_text[ 8] = "?I hope you enjoyed playing MegaBreak. Whatever you think of it, I would      ?0";
     end_text[ 9] = "?really appreciate it if you could e-mail me some comments, suggestions or    ?0";
     end_text[10] = "?whatever.                                                                    ?0";
     end_text[11] = "√ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¬ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¥\0";
     end_text[12] = "?Don't forget that you can make your ?MegaBreak on the net! You are welcome  ?0";
     end_text[13] = "?own levels for MegaBreak using the  ?to check out my MegaBreak page. As     ?0";
     end_text[14] = "?supplied Level Editor. Check out    ?well as some history and general info  ?0";
     end_text[15] = "?manual.txt for details.             ?you will be able to find extra levels  ?0";
     end_text[16] = "?                                    ?here in the future. (That is, if       ?0";
     end_text[17] = "?                                    ?anyone makes some cool levels)         ?0";
     end_text[18] = "√ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¡ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¥\0";
     end_text[19] = "??Wanna create an official extra-levels pack for MegaBreak?   Contact me!     ?0";
     end_text[20] = "√ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¥\0";
     end_text[21] = "?MegaBreak Homepage: http://www.sn.no/~oruud/       E-mail: oeyruud@online.no ?0";
     end_text[22] = "¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ\0";
}

int alloc_memory()
{
    int i, ii;

    for (i=0; i<15; i++) {
        if (!(bricks[i] = new byte[96]))
            return 0;
    }

    for (i=0; i<7; i++) {
        if (!(flashbricks[i] = new byte[96]))
            return 0;
    }

    for (i=0; i<11; i++) {
        if (!(balls[i] = new byte[16]))
            return 0;
    }

    for (i=0; i<10; i++) {
        if (!(messages[i] = new byte[312]))
            return 0;
    }

    for (i=0; i<9; i++) {
        if (!(bats[i] = new byte[130]))
            return 0;
    }

    for (i=0; i<21; i++) {
        if (!(tiles[i] = new byte[224]))
            return 0;
    }

    for (i=0; i<2; i++) {
        for (ii=0; ii<5; ii++) {
            if (!(bonus_letters[i][ii] = new byte[132]))
                return 0;
        }
    }

    for (i=0; i<2; i++) {
        for (ii=0; ii<4; ii++) {
            if (!(panel_displays[i][ii] = new byte[322]))
                return 0;
        }
    }

    for (i=0; i<2; i++) {
        for (ii=0; ii<4; ii++) {
            if (!(powerup_icons[i][ii] = new byte[342]))
                return 0;
        }
    }

    if (!(rescue_line = new byte[880]))
        return 0;

    if (!(lightbrick = new byte[96]))
        return 0;

    if (!(select = new byte[56]))
        return 0;

    if (!(vrscreen = new byte[64000]))
        return 0;

    if (!(bgscreen = new byte[64000]))
        return 0;

    if (!(mainscreen = new byte[64000]))
        return 0;

    if (!(batshoot = new byte[130]))
        return 0;

    if (!(bullet = new byte[10]))
        return 0;

    return 1;   // Success
}

void free_memory()
{
    delete flashbricks;
    delete balls;
    delete messages;
    delete bats;
    delete tiles;
    delete bonus_letters;
    delete panel_displays;
    delete powerup_icons;
    delete rescue_line;
    delete lightbrick;
    delete select;
    delete vrscreen;
    delete mainscreen;
    delete batshoot;
    delete bullet;
}

