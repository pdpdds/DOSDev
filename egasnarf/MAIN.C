#define	INMAIN	1

#include	<stdio.h>
#include	<fcntl.h>
#include	<math.h>
#include	"snarf.h"

void newbplotxy(unsigned short x, unsigned short y, unsigned short height, unsigned short width, char* picture, unsigned short rule);
void flabel (unsigned short x, unsigned short y, char* string, char fore_color, char back_color, char rule);
void horline(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short color, unsigned short rule);
void verline(unsigned short x2, unsigned short y1, unsigned short y2, unsigned short color, unsigned short rule);

	
extern	int	beepflg;

/************************************************************************
 * MAIN function							*
 ************************************************************************/

void	main(argc, argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	int	i, j;

	init();
new_game:
	init_screen();
	while( !iskey() ) ;
	next_screen();
start_fresh:

	while(1) {
		while( !iskey() ) {
			while( timepass < 1 ) ;
			timepass = 0;
			move_man();
			if( shots.dir != -1 ) move_shot();
			show_points();
			show_tag();
	for(i=0; i<MAXBUGS; i++) {
		if( bugs[i].dir == -1 || bugs[i].dir > 3 ) set_bug_loc(i, 0);
		else set_bug_loc(i, (19-(bugs[i].y>>4))*41+(bugs[i].x>>4)+1 );
	}
			man_dirs(field, (19-(mans.y>>4))*41+(mans.x>>4)+1);
			for(i=0; i<MAXBUGS; i++) move_bug(i);
			if( okbugmake ) {
				if( --bugmaketime ) {
					okbugmake = 0;
					bugmaketime = rand(256);
				}
				if( rand(10) > 8 ) make_bug(i);
			} else {
				if( --bugmaketime ) {
					okbugmake = 1;
					bugmaketime = rand(2048);
				}
			}
			if( dead==1 ) {
				clear_sscore();
				sscore += points;
				show_sscore();
				score += points;
				show_score();
				clear_points();
				if( score > highscore ) {
					highscore = score;
					show_high();
				}
				timepass = 0;
				while( timepass<18 ) ;
				while( iskey() ) key();
				pause();
				key();
				check_new_high();
				goto new_game;
			}
		}

		switch( key() ) {
		 case 0x3920:	/* space */
			pause();
			break;
		 case 0x3b00:	/* f1 */
			choose_palette();
			break;
		 case 0x3c00:	/* f2 */
			choose_colors();
			--curwall;
			next_screen();
			goto start_fresh;
		 case 0x3d00:	/* f3 */
			edit_walls();
			--curwall;
			next_screen();
			goto start_fresh;
		 case 0x4400:	/* f10 */
			beepflg = !beepflg;
			break;
		 case 0x011b:	/* esc */
			uninit();
			exit(0);
		 case 0x4d00:	/* rtcurs */
			newmandir = 0;
			break;
		 case 0x4800:	/* upcurs */
			newmandir = 1;
			break;
		 case 0x4b00:	/* lfcurs */
			newmandir = 2;
			break;
		 case 0x5000:	/* dncurs */
			newmandir = 3;
			break;
		 case 0x2064:	/* d */
		 case 0x2044:
			fire(0);
			break;
		 case 0x1177:	/* w */
		 case 0x1157:
			fire(1);
			break;
		 case 0x1e61:	/* a */
		 case 0x1e41:
			fire(2);
			break;
		 case 0x2d78:	/* x */
		 case 0x2d58:
			fire(3);
			break;
		 default:
			newmandir = mans.dir | 0x80;
			break;
		}
	}
}

void	check_new_high() {

	FILE	*fp;
	int	i, j, k;
	char	dots[13];

	strcpy(dots, ".......... ");
	dots[12] = 0;
	for(i=0; i<64; i++) {
		if( score > highlist[i].score )  {
			for(j=63; j && j>i; j--) {
				highlist[j].score = highlist[j-1].score;
				strcpy(highlist[j].name, highlist[j-1].name);
			}
			highlist[i].score = score;
			highlist[i].name[0] = 0;
			for(j=YORG; j<YMAX-2; j++) horline(XORG, j, XMAX-3, 0, 0);
			flabel(300, 175, "name: ", 2, 0, 0);
			k=0;
			flabel(336+6*k, 175, " ", 0,2,0);
			flabel(336+6*(k+1), 175, &dots[k], 2, 0, 0);
			while( (0x1c0d != (j=key())) && (j!=0x011b) ) {
				if( j==0x0e08 ) {
					if( k ) highlist[i].name[--k] = 0;
					else beep_seq(beep_ill_key);
				} else if( ((j&0xff)<1) || ((j&0xff)>127) || k==11) {
					beep_seq(beep_ill_key);
				} else {
					highlist[i].name[k++] = j;
					highlist[i].name[k] = 0;
				}
				flabel(336, 175, highlist[i].name, 6, 0, 0);
				flabel(336+6*k, 175, " ", 0,2,0);
				flabel(336+6*(k+1), 175, &dots[k], 2, 0, 0);
			}
			if( NULL!=(fp=fopen("SNARF.HI", "wb")) ) {
				fwrite((char *)(&highlist[0].score),16,64,fp);
				fclose(fp);
			}
			break;
		}
	}
}


void	pause() {

	flabel(320-6*8, 10, "...press a key...", 15, 9, 0);
	horline(320-6*8, 11, 320+6*9, 9, 0);
	while( !iskey() ) ;
	clear_bottom();
}


void	fire(dir)
int	dir;
{
	int	i;

	if(shots.dir == -1) {
		shots.dir = dir;
		shots.x   = mans.x & 0xfff0;
		shots.y   = mans.y & 0xfff0;
		show_shot();
		return;
	} else {
		beep_seq(beep_click);
	}
}


void	init() {

	int	i;
	FILE	*fp;

	setrand(0);
	setmode(0x10);	/* set ega mode 640x350 */
	take_timer();
	for(i=0; i<64; i++) {
		highlist[i].score = 0;
		highlist[i].name[0] = 0;
	}
	if( NULL!=(fp=fopen("SNARF.HI", "rb")) ) {
		fread((char *)(&highlist[0].score),16,64,fp);
		fclose(fp);
	}
}

void	setpalette(i,cursor)
int	i, cursor;
{

	int	j;
	char	temp[81];

	setcolors(i, palette[i]);
}


void	uninit() {

	setmode(0x03);	/* set 80x25 color alphanumerics */
	give_timer();
}


void	scorebox(x, digwidth, s)
int	x, digwidth;
char	*s;
{
	int	i;

	box(x, YMAX+3, x+14*digwidth+8, YMAX+22, 15, 0);
	for(i=YMAX+4; i<YMAX+22; i++) horline(x+1, i, x+14*digwidth+7, 0, 0);
	flabel(x+(14*digwidth-6*strlen(s))/2+5, 348, s, 15, 0, 0);
}

void	clear_bottom() {

	int	i;

	for(i=0; i<YORG-7; i++) horline(0,i,639,11,0);
}


/****************************************************************
 * INIT_SCREEN()						*
 ****************************************************************/

void	init_screen() {

	int	i, j, k;
	unsigned char	*p, *r, c;

	/* create bplot data for score digits */

	for(i=0; i<10; i++) {
		p = digits[i];
		r = hold1;
		for(j=0; j<28; j++) {
			c = *p++;
			for(k=0x80; k; k=k>>1) {
				if( c & k )	*r++ = score_color;
				else		*r++ = 0;
			}
		}
		for(j=0; j<32; j++) *r++ = '\0';
		convert(hold1, digbits[i]);
	}

	/* init screen */

	clrscreen(0);

	for(i=0; i<16; i++) setpalette(i, 0);
	box(XORG-8, YORG-6, XMAX, YMAX, colors[0], 0);
	box(XORG-6, YORG-4, XMAX-2, YMAX-2, colors[2], 0);
	for(i=XORG-7; i<XMAX; i++, i++) {
		plotxy(i, YORG-5, colors[1], 0);
		plotxy(i, YMAX-1, colors[1], 0);
	}
	for(i=YORG-5; i<YMAX; i++, i++) {
		plotxy(XORG-7, i, colors[1], 0);
		plotxy(XMAX-1, i, colors[1], 0);
	}

	clear_bottom();
	for(i=YMAX+2; i<350; i++) horline(0,i,639,11,0);
	scorebox( 40, 2, " TAGS ");
	scorebox( 92, 4, " POINTS ");
	scorebox(172, 6, " LEVEL SCORE ");
	scorebox(278, 9, " SCORE ");
	scorebox(424, 2, " LEVEL ");
	scorebox(474, 9, " HIGH SCORE ");

	tagcount = 50;
	tagflg = 2;
	dead = 0;
	curscreen = 0;

	/* build & display CURWALL field */

	curwall = -1;
	score = 0L;
	show_score();
	welcome();
}



void	welcome() {

	int	i,j,k;
	char	buff[81];

	highscore = highlist[0].score;
	show_high();

	newbplotxy(346, 312, 16, 2, man[0], 0);
	newbplotxy(344, 292, 16, 2, goodies[3], 0);
	newbplotxy(344, 272, 16, 2, goodies[2], 0);
	newbplotxy(344, 252, 16, 2, goodies[1], 0);
	newbplotxy(344, 232, 16, 2, goodies[0], 0);
	newbplotxy(344, 212, 16, 2, tagboost, 0);

	newbplotxy(516, 312, 16, 2, keylock, 0);
	newbplotxy(516, 292, 16, 2, keys, 0);
	newbplotxy(516, 272, 16, 2, teleport, 0);
	newbplotxy(516, 252, 16, 2, pits, 0);
	newbplotxy(516, 232, 16, 2, bug1[0], 0);

	flabel(366, 308, "The Hero (that's you)", 4, 0, 0);
	sprintf(buff, "ring %d point", values[3]);
	flabel(366, 288, buff, 4, 0, 0);
	sprintf(buff, "ring %d points", values[2]);
	flabel(366, 268, buff, 4, 0, 0);
	sprintf(buff, "ring %d points", values[1]);
	flabel(366, 248, buff, 4, 0, 0);
	sprintf(buff, "crown %d points", values[0]);
	flabel(366, 228, buff, 4, 0, 0);
	flabel(366, 208, "First Aid station", 4, 0, 0);

	flabel(538, 308, "locked door", 4, 0, 0);
	flabel(538, 288, "key", 4, 0, 0);
	flabel(538, 268, "teleport", 4, 0, 0);
	flabel(538, 248, "Snarf nest", 4, 0, 0);
	flabel(538, 228, "the SNARF", 4, 0, 0);

	flabel(XORG+6, 310, "Pick up treasures, unlock doors, move from level to", 12, 0, 0);
	flabel(XORG+6, 300, "level.  At the end of each level, treasure POINTS  ", 12, 0, 0);
	flabel(XORG+6, 290, "are multiplied by the TAGS left to calculate the   ", 12, 0, 0);
	flabel(XORG+6, 280, "LEVEL SCORE.  The LEVEL SCORE is then added to the ", 12, 0, 0);
	flabel(XORG+6, 270, "game SCORE.  You lose a TAG count each time you are", 12, 0, 0);
	flabel(XORG+6, 260, "touched by a Snarf. If you are tagged when your TAG", 12, 0, 0);
	flabel(XORG+6, 250, "count is 0, the game ends.  You can restore your   ", 12, 0, 0);
	flabel(XORG+6, 240, "TAGS count to the maximum at a First Aid station.  ", 12, 0, 0);
	flabel(XORG+6, 230, "Snarfs are created in Snarf Nests.  You can travel ", 12, 0, 0);
	flabel(XORG+6, 220, "through a TELEPORT, but Snarfs can't. If you finish", 12, 0, 0);
	flabel(XORG+6, 210, "the last level (number xx), you win the game.      ", 12, 0, 0);
	flabel(XORG+6, 200, "---------------------------------------------------", 12, 0, 0);

	for(i=0; i<4; i++) {
		if( highlist[i*16].score == 0 ) break;
		for(j=0; j<16; j++) {
			if( highlist[i*16+j].score == 0 ) break;
			sprintf(buff, "%8.8ld", highlist[i*16+j].score);
			for(k=0; k<7 && buff[k]=='0'; k++) buff[k] = ' ';
			flabel(XORG+156*i, YORG+160-10*j, buff, 2, 0, 0);
			flabel(XORG+66+156*i, YORG+160-10*j, highlist[i*16+j].name, 6, 0, 0);
		}
	}
}


void	next_screen() {

	int	i, j, k;
	unsigned char	*p, *r, c;

	++curscreen;
	show_level();
	tagflg = 2;
	setcolors(11, tagcount);

	if(++curwall >= MAXWALLS) curwall=0;

	for(i=YORG; i<YMAX-2; i++) horline(XORG, i, XMAX-3, 0, 0);

	build_wall();

	for(i=0; i<820; i++) field[i] = -1;
	p = walls[curwall];
	for(i=1; i<19; i++) {
		for(j=0; j<5; j++) {
			c = *p++;
			for(k=0; k<8; k++) {
				field[i*41+j*8+k+1] = (c & 0x80) ? 1 : 0;
				c = c<<1;
			}
		}
	}
	for(i=0; i<MAXPITS; i++) pitlist[i].x = -1;
	for(i=0; i<MAXOBJECTS; i++) {
		p = &field[ 41*treasure[curwall][i].y + treasure[curwall][i].x ];
		switch( treasure[curwall][i].item & 0xf0 ) {
		 case 0x10:	/* teleport */
			*p = treasure[curwall][i].item;
/*
			*(p-1) = ((*p & 0x01)<<1) | 0x10;
			*(p+1) = (((*p & 0x01)<<1)+1) | 0x10;
*/
			break;
		 case 0x30:	/* man start position */
			mans.x = (treasure[curwall][i].x-1)*16;
			mans.y = (19-treasure[curwall][i].y)*16;
			break;
		 case 0x20:	/* pit */
			for(j=0; j<MAXPITS; j++) {
				if( pitlist[j].x == -1 ) {
					pitlist[j].x = (treasure[curwall][i].x-1)*16;
					pitlist[j].y = (19-treasure[curwall][i].y)*16;
					break;
				}
			}
			*p = treasure[curwall][i].item;
/*
			switch( *p & 0x0f ) {
			 case 1:
				*(p-41) = 0x2a;
				*(p+41) = 0x20;
				break;
			 case 3:
				*(p-1) = 0x2e;
				*(p+1) = 0x24;
				break;
			 case 5:
				*(p-41) = 0x26;
				*(p+41) = 0x22;
				break;
			 case 7:
				*(p-1) = 0x2c;
				*(p+1) = 0x28;
				break;
			 default:
				break;
			}
*/
			break;
		 case 0x40:	/* treasure */
		 case 0x60:	/* key */
		 case 0x70:	/* lock */
		 case 0x80:
			*p = treasure[curwall][i].item;
			break;
		 default:
			break;
		}
	}

	for(i=1; i<40; i++) {
		for(j=1; j<19; j++) {
			disp_field( i, j );
		}
	}

	shots.dir = -1;

	mans.dir = 0x80;
	mans.key = 0;
	show_man();

	for(i=0; i<MAXBUGS; i++) {
		bugs[i].dir = -1;
	}
}


void	disp_field(x,y)
int	x,y;
{
	char	*p;

	p = &field[y*41+x];
	switch( *p & 0xf0 ) {
	 case 0x00:	/* wall */
		if( (*p & 0x0f) != 1 ) return;
		p = newwall;
		break;
	 case 0x60:		/* key */
		p = keys;
		break;
	 case 0x70:
		p = keylock;	/* lock */
		break;
	 case 0x80:		/* tagboost */
		p = tagboost;
		break;
	 case 0x10:		/* teleport */
		if( *p==0x14 ) {
			flip(teleport, hold1);
			p = hold1;
		} else  p = teleport;
/*
		switch( *p ) {
		 case 0x14:
		 case 0x15:
			return;
		 default:
			p = teleport[*p & 0x0f];
			break;
		}
*/
		break;
	 case 0x40:	/* treasure */
		switch( *p ) {
		 case 0x40:
			p = goodies[0];
			break;
		 case 0x41:
			p = goodies[1];
			break;
		 case 0x42:
			p = goodies[2];
			break;
		 case 0x43:
			p = goodies[3];
			break;
		 default:
			return;
			break;
		}
		break;
	 case 0x20:	/* pit */
		switch(  *p ) {
		 case 0x21:
			rotate(pits, hold1);
			reverse(hold1, hold2);
			p = hold2;
			break;
		 case 0x23:
			p = pits;
			break;
		 case 0x25:
			rotate(pits, hold1);
			p = hold1;
			break;
		 case 0x27:
			flip(pits, hold1);
			p = hold1;
			break;
/*		 case 0x20:
			p = pits[0];
			break;
		 case 0x21:
			p = pits[1];
			break;
		 case 0x22:
			reverse(pits[0], hold1);
			p = hold1;
			break;
		 case 0x23:
			rotate(pits[1], hold1);
			p = hold1;
			break;
		 case 0x24:
			rotate(pits[0], hold1);
			p = hold1;
			break;
		 case 0x25:
			reverse(pits[1], hold1);
			p = hold1;
			break;
		 case 0x26:
			flip(pits[0], hold1);
			reverse(hold1, hold2);
			p = hold2;
			break;
		 case 0x27:
			rotate(pits[1], hold1);
			flip(hold1, hold2);
			p = hold2;
			break;
		 case 0x28:
			rotate(pits[0], hold1);
			flip(hold1, hold2);
			p = hold2;
			break;
		 case 0x2a:
			flip(pits[0], hold1);
			p = hold1;
			break;
		 case 0x2c:
			rotate(pits[0], hold1);
			reverse(hold1, hold2);
			flip(hold2, hold1);
			p = hold1;
			break;
		 case 0x2e:
			rotate(pits[0], hold1);
			reverse(hold1, hold2);
			p = hold2;
			break;
*/
		 default:
			return;
		}
		break;
	 default:
		return;
	}
	newbplotxy(((x-1)<<4)+XORG, ((19-y)<<4)+YORG, 16, 2, p, 3);
}


void	box(x1, y1, x2, y2, color, rule)
int	x1, y1, x2, y2, color, rule;
{

	horline(x1, y1, x2, color, rule);
	verline(x2, y1, y2, color, rule);
	horline(x2, y2, x1, color, rule);
	verline(x1, y2, y1, color, rule);
}

void	build_wall() {

	int	i, j, a, b;

	b = pattcolors[curwall][0];
	a = pattcolors[curwall][1];
	for(i=0; i<128; i++) newwall[i] = 0;
	for(i=0; i<32; i++) {
		for(j=0x80; j; j>>=1) {
			if( !(wallpatt[i] & j) ) {
				if( wallpatt[i+32] & j ) {
					if( a & 1 ) newwall[i]    |= j;
					if( a & 2 ) newwall[i+32] |= j;
					if( a & 4 ) newwall[i+64] |= j;
					if( a & 8 ) newwall[i+96] |= j;
				} else {
					if( b & 1 ) newwall[i]    |= j;
					if( b & 2 ) newwall[i+32] |= j;
					if( b & 4 ) newwall[i+64] |= j;
					if( b & 8 ) newwall[i+96] |= j;
				}
			}
		}
	}
/*
	int	i, j;
	unsigned char	*p, *q, z;
	unsigned char	a, b, c, d;

	j = pattcolors[curwall][0];
	for(i=0; i<18; i++) wallpatt[i] = j;
	wallpatt[30] = j;
	wallpatt[31] = j;
	wallpatt[32] = j;
	wallpatt[47] = j;
	wallpatt[48] = j;
	wallpatt[63] = j;
	wallpatt[64] = j;
	wallpatt[79] = j;
	wallpatt[80] = j;
	wallpatt[95] = j;
	wallpatt[96] = j;
	wallpatt[111] = j;
	wallpatt[112] = j;
	wallpatt[113] = j;
	for(i=126; i<144; i++) wallpatt[i] = j;
	wallpatt[150] = j;
	wallpatt[151] = j;
	wallpatt[152] = j;
	wallpatt[167] = j;
	wallpatt[183] = j;
	wallpatt[199] = j;
	wallpatt[215] = j;
	wallpatt[231] = j;
	wallpatt[246] = j;
	wallpatt[247] = j;
	wallpatt[248] = j;

	j = pattcolors[curwall][1];
	for(i=34; i<46; i++) wallpatt[i] = j;
	wallpatt[50] = j;
	wallpatt[52] = j;
	wallpatt[54] = j;
	wallpatt[56] = j;
	wallpatt[58] = j;
	wallpatt[60] = j;
	wallpatt[61] = j;
	wallpatt[66] = j;
	wallpatt[67] = j;
	wallpatt[69] = j;
	wallpatt[71] = j;
	wallpatt[73] = j;
	wallpatt[75] = j;
	wallpatt[77] = j;
	wallpatt[82] = j;
	wallpatt[84] = j;
	wallpatt[86] = j;
	wallpatt[88] = j;
	wallpatt[90] = j;
	wallpatt[92] = j;
	wallpatt[93] = j;
	for(i=98; i<110; i++) wallpatt[i] = j;
	for(i=160; i<166; i++) 	wallpatt[i] = j;
	for(i=169; i<176; i++)	wallpatt[i] = j;
	wallpatt[177] = j;
	wallpatt[179] = j;
	wallpatt[181] = j;
	wallpatt[185] = j;
	wallpatt[187] = j;
	wallpatt[189] = j;
	wallpatt[191] = j;
	wallpatt[192] = j;
	wallpatt[194] = j;
	wallpatt[196] = j;
	wallpatt[197] = j;
	wallpatt[201] = j;
	wallpatt[202] = j;
	wallpatt[204] = j;
	wallpatt[206] = j;
	wallpatt[209] = j;
	wallpatt[211] = j;
	wallpatt[213] = j;
	wallpatt[217] = j;
	wallpatt[219] = j;
	wallpatt[221] = j;
	for(i=223; i<230; i++)	wallpatt[i] = j;
	for(i=233; i<240; i++)	wallpatt[i] = j;
	convert(wallpatt, newwall);
*/
}


void	convert(s, des)
unsigned char	*s, *des;
{
	int	i, j, a, b, c, d;
	unsigned char z;

	for(i=0; i<32; i++) {
		a = b = c = d = 0;
		for(j=0; j<8; j++) {
			z = *s++;
			a = a<<1;
			b = b<<1;
			c = c<<1;
			d = d<<1;
			if( z & 1 ) a |= 1;
			if( z & 2 ) b |= 1;
			if( z & 4 ) c |= 1;
			if( z & 8 ) d |= 1;
		}
		*des      = a;
		*(des+32) = b;
		*(des+64) = c;
		*(des+96) = d;
		++des;
	}
}


void	flip(s, d)
unsigned int	*s, *d;
{
	unsigned int	i, j;

	d += 15;
	for(i=0; i<4; i++) {
		for(j=0; j<16; j++) {
			*d-- = *s++;
		}
		d += 32;
	}
}

void	reverse(s, d)
unsigned int	*s, *d;
{
	unsigned int	a, b, j, k, c;

	for(c=0; c<4; c++) {
		for(j=0; j<16; j++) {
			a = *s++;
			b = 0;
			for(k=0; k<16; k++) {
				b = (b<<1) | (a & 1);
				a >>= 1;
			}
			*d++ = b;
		}
	}
}

void	rotate(s, d)
unsigned char	*s, *d;
{
	unsigned int	k, c;
	unsigned char	a, j;

	for(c=0; c<4; c++) {
		for(j=0x01; j; j <<= 1) {
			a = 0;
			for(k=1; k<16; k+=2) {
				a <<= 1;
				if(s[k] & j) a |= 1;
			}
			*d++ = a;
			a = 0;
			for(k=17; k<32; k+=2) {
				a <<= 1;
				if(s[k] & j) a |= 1;
			}
			*d++ = a;
		}
		for(j=0x01; j; j <<= 1) {
			a = 0;
			for(k=0; k<16; k+=2) {
				a <<= 1;
				if(s[k] & j) a |= 1;
			}
			*d++ = a;
			a = 0;
			for(k=16; k<32; k+=2) {
				a <<= 1;
				if(s[k] & j) a |= 1;
			}
			*d++ = a;
		}
		s += 32;
	}
}
