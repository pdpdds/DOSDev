#include	<math.h>
#include	"snarf.h"

/****************************************************************
 * SHOW things							*
 ****************************************************************/

void	show_bug(int i)
{
	char	*p;

	if( bugs[i].dir>15 ) p = bang[bugs[i].dir-16];
	else p = bug1[bugs[i].dir & 7];
	newbplotxy(bugs[i].x+XORG, bugs[i].y+YORG, 16, 2, p, 3);
}

void	show_man(void) {

	char	*p;

	p = man[ (((mans.dir&0x7f)<2) ? 4 : 0) + ((mans.key) ? 2 : 0) + (((mans.x | mans.y) & 4) ? 1 : 0) ];
	newbplotxy(mans.x+XORG+3,mans.y+YORG, 16, 2, p, 3);
}


void	show_shot() {

	newbplotxy(shots.x+XORG+5,shots.y+YORG-5, 8, 1, shot[shots.dir], 3);
}

void	show_score() {

	int	i;
	char	temp[81];

	scorex = 296;
	sprintf(scorebuf, "%8.8ld", score);
	for(scoreptr=0; scorebuf[scoreptr]=='0'; scoreptr++) scorex += 14;
	if( !scorebuf[scoreptr] ) {
		--scoreptr;
		scorex -= 14;
	}
	while( scorebuf[scoreptr] ) {
		newbplotxy(scorex, scorey, 16, 2, digbits[scorebuf[scoreptr]-'0'], 0);
		++scoreptr;
		scorex += 14;
	}
}

void	show_sscore() {

	int	i;
	char	temp[81];

	scorex = 178;
	sprintf(scorebuf, "%6.6ld", sscore);
	for(scoreptr=0; scorebuf[scoreptr]=='0'; scoreptr++) scorex += 14;
	if( !scorebuf[scoreptr] ) {
		--scoreptr;
		scorex -= 14;
	}
	while( scorebuf[scoreptr] ) {
		newbplotxy(scorex, scorey, 16, 2, digbits[scorebuf[scoreptr]-'0'], 0);
		++scoreptr;
		scorex += 14;
	}
}

void	clear_sscore() {

	int	i;

	sscore = 0L;
	for(i=0; i<4; i++) newbplotxy(178+14*i, scorey, 16, 2, digbits[10], 0);
	newbplotxy(4*14+178, scorey, 16, 2, digbits[0], 0);
}


void	show_high() {

	int	i;
	char	temp[81];

	scorex = 492;
	sprintf(scorebuf, "%8.8ld", highscore);
	for(scoreptr=0; scorebuf[scoreptr]=='0'; scoreptr++) scorex += 14;
	if( !scorebuf[scoreptr] ) {
		--scoreptr;
		scorex -= 14;
	}
	while( scorebuf[scoreptr] ) {
		newbplotxy(scorex, scorey, 16, 2, digbits[scorebuf[scoreptr]-'0'], 0);
		++scoreptr;
		scorex += 14;
	}
}


void	clear_points() {

	int	i;

	points = 0L;
	for(i=0; i<3; i++) newbplotxy(98+14*i, scorey, 16, 2, digbits[10], 0);
	newbplotxy(3*14+98, scorey, 16, 2, digbits[0], 0);
}


void	show_points() {

	int	i;
	char	temp[81];

	if( pointbuf[pointptr] ) {
		newbplotxy(pointx, scorey, 16, 2, digbits[pointbuf[pointptr]-'0'], 0);
		++pointptr;
		pointx += 14;
	} else if( pointflg ) {
		pointflg = 0;
		pointx = 98;
		sprintf(pointbuf, "%4.4ld", points);
		for(pointptr=0; pointbuf[pointptr]=='0'; pointptr++) pointx += 14;
		if( !pointbuf[pointptr] ) {
			--pointptr;
			pointx -= 14;
		}
	}
}


void	show_tag() {

	int	i;
	char	*p;

	if( tagflg==2 ) {
		--tagflg;
		if( tagcount > 9 ) p = digbits[tagcount/10];
		else		    p = digbits[10];
		newbplotxy(46, scorey, 16, 2, p, 0);
	} else if( tagflg==1 ) {
		--tagflg;
		if( tagcount < 0 ) p = digbits[0];
		else		   p = digbits[tagcount%10];
		newbplotxy(60, scorey, 16, 2, p, 0);
	}
}


void	show_level() {

	int	i;
	char	*p;

	if( curscreen > 9 ) p = digbits[curscreen/10];
	else		    p = digbits[10];
	newbplotxy(430, scorey, 16, 2, p, 0);
	p = digbits[curscreen%10];
	newbplotxy(444, scorey, 16, 2, p, 0);
}


/****************************************************************
 * MOVE things							*
 ****************************************************************/

void	move_shot() {
	int	j;
	char	*p;

	show_shot();
	p = &field[41*(19-(shots.y>>4)) + (shots.x>>4) + 1];
	if( !((shots.x & 15) || (shots.y & 15)) ) {
		switch( shots.dir ) {
		 case 0:
			j = *(p+1);
			break;
		 case 1:
			j = *(p-41);
			break;
		 case 2:
			j = *(p-1);
			break;
		 case 3:
			j = *(p+41);
			break;
		 default:
			j = 1;
			break;
		}
		if( j==1 || j==-1 ) {
			shots.dir = -1;
			return;
		}
	}
	switch(shots.dir) {
	 case 0:	/* right */
		shots.x += SHOTJMP;
		break;
	 case 1:	/* up    */
		shots.y += SHOTJMP;
		break;
	 case 2:	/* left  */
		shots.x -= SHOTJMP;
		break;
	 case 3:	/* down  */
		shots.y -= SHOTJMP;
		break;
	}
	for(j=0; j<MAXBUGS; j++) {
		if( bugs[j].dir != -1 && bugs[j].dir < 4) {
			if( (abs(shots.x-bugs[j].x)+abs(shots.y-bugs[j].y)) < 5 ) {
				bang_bug(j);
				shots.dir = -1;
				return;
			}
		}
	}
	show_shot();
}

void	check_tag_man(i)
int	i;
{
	if( !dead && bugs[i].dir<4 && bugs[i].dir>=0 && (abs(bugs[i].x-mans.x)+abs(bugs[i].y-mans.y))<13 ) {
		setcolors(11, tagcount--);
		if( tagcount<0 ) {
			beep_seq(beep_dead);
			dead = 15;
		} else {
			beep_seq(beep_tag);
			tagflg=2;
		}
	}
}



void	move_bug(i)
int	i;
{
	int	j, k, shotflag;
	char	*p, *r, *d, *s;
/*char	temp[81];*/

	if( bugs[i].dir == -1 ) return;
	show_bug(i);
	switch(bugs[i].dir) {
	 case 0:	/* right */
		bugs[i].x += BUGJMP;
		break;
	 case 1:	/* up    */
		bugs[i].y += BUGJMP;
		break;
	 case 2:	/* left  */
		bugs[i].x -= BUGJMP;
		break;
	 case 3:	/* down  */
		bugs[i].y -= BUGJMP;
		break;
	 case 4:	/* coming alive */
		p = &field[41*(19-(bugs[i].y>>4)) + (bugs[i].x>>4) + 1];
		bugs[i].dir = (*p & 0x07)>>1;
		show_bug(i);
		return;
	 default:
		if( --bugs[i].dir == 15 ) {
			bugs[i].dir = -1;
			return;
		}
		break;
	}

	p = &field[41*(19-(bugs[i].y>>4)) + (bugs[i].x>>4) + 1];
	if( bugs[i].dir < 4 && (((bugs[i].x & 15) || (bugs[i].y & 15)) == 0) ) {
		if( (k=bug_dir(i))==-1 ) {
			if( ++bugs[i].lost > 50 ) {
				show_bug(i);
				bang_bug(i);
				return;
			}
		} else bugs[i].lost = 0;
		if( legal_bug_move(k, p) ) bugs[i].dir = k;
		else {
			while( !legal_bug_move(bugs[i].dir, p) ) {
				bugs[i].dir = rand(4);
			}
		}
		shotflag = 0;
		switch( bugs[i].dir ) {
		 case 0:
			if( shots.y == bugs[i].y && shots.dir == 2 ) ++shotflag;
			break;
		 case 1:
			if( shots.x == bugs[i].x && shots.dir == 3 ) ++shotflag;
			break;
		 case 2:
			if( shots.y == bugs[i].y && shots.dir == 0 ) ++shotflag;
			break;
		 case 3:
			if( shots.x == bugs[i].x && shots.dir == 1 ) ++shotflag;
			break;
		 default:
			break;
		}
		if( shotflag ) {
			j = rand(2) ? (bugs[i].dir+1)&3 : (bugs[i].dir-1)&3;
			if( !legal_bug_move( j, p ) ) {
				j ^= 2;
				if( !legal_bug_move( j, p ) ) {
					j = bugs[i].dir ^ 2;
					if( !legal_bug_move( j, p ) ) j = bugs[i].dir;
				}
			}
			bugs[i].dir = j;
		} else if( rand(10) > 7 ) {
			do
				bugs[i].dir = rand(4);
			while( !legal_bug_move( bugs[i].dir, p) ) ;
		}
		while( !legal_bug_move(bugs[i].dir, p) ) bugs[i].dir = rand(4);
		check_tag_man(i);
	}
	show_bug(i);
}


void	move_man() {

	int	i, j;
	char	*p, *r;

	if( dead ) {
		if( dead==1 ) return;
		mans.dir |= 0x80;
		show_man();
		--dead;
		show_man;
		return;
	}
	p = &field[41*(19-(mans.y>>4)) + (mans.x>>4) + 1];
	if( !( (mans.dir & 0x80) || (mans.x & 15) || (mans.y & 15)) ) {
		if( (i=look_man_move(mans.dir, p)) && !(i==0x70 && mans.key) ) mans.dir |= 0x80;
		if( i==-1 ) {
			i = tagcount;
			clear_sscore();
			do {
				timepass = 0;
				tagflg = 2;
				setcolors(11, tagcount);
				beepon( (50-tagcount)*300 );
				show_tag();
				show_tag();
				sscore += points;
				show_sscore();
				score += points;
				show_score();
				while( timepass < 1 ) ;
			} while( tagcount-- ) ;
			beepoff();
			clear_points();
			tagcount = i;
			tagflg = 2;
			show_tag();
			show_tag();
			show_man();
			next_screen();
			while( iskey() ) key();
			while( !iskey() );
			return;
		}
	}
	if( !(mans.dir & 0x80) ) {
		show_man();
		switch(mans.dir) {
		 case 0:	/* right */
			mans.x += MANJMP;
			break;
		 case 1:	/* up    */
			mans.y += MANJMP;
			break;
		 case 2:	/* left  */
			mans.x -= MANJMP;
			break;
		 case 3:	/* down  */
			mans.y -= MANJMP;
			break;
		}
		show_man();
	}
	if( ((mans.x & 15) || (mans.y & 15)) == 0 ) {
		p = &field[ 41*(19-(mans.y>>4)) + (mans.x>>4) + 1];
		if( (*p & 0xf0) == 0x40 ) {
			beep_seq(beep_goodies);
			disp_field( (mans.x>>4)+1, 19-(mans.y>>4) );
			points += values[*p & 0x0f];
			++pointflg;
			*p = 0;
		} else if( (*p & 0xf0) == 0x80 ) {
			while(tagcount<50) {
				++tagcount;
				setcolors(11, tagcount);
				beepon( (50-tagcount)*300 );
				tagflg = 2;
				show_tag();
				show_tag();
				timepass = 0;
				while(timepass < 1) ;
			}
			beepoff();
			disp_field( (mans.x>>4)+1, 19-(mans.y>>4) );
			*p = 0;
		} else if( !mans.key && *p == 0x60 ) {
			beep_seq(beep_goodies);
			disp_field( (mans.x>>4)+1, 19-(mans.y>>4) );
			show_man();
			++mans.key;
			show_man();
			*p = 0;
		} else if( mans.key && *p == 0x70 ) {
			beep_seq(beep_lock);
			disp_field( (mans.x>>4)+1, 19-(mans.y>>4) );
			show_man();
			mans.key = 0;
			show_man();
			*p = 0;
		} else if( (*p & 0xfe) == 0x14 ) {
			r = p-41;
			while( r > field ) {
				if( (*r & 0xfe) == 0x14 ) {
					show_man();
					mans.y = (19-(r-field)/41)*16;
					show_man();
					break;
				}
				r -= 41;
			}
			if( r <= field ) {
				r = p+41;
				while( r < (field+820) ) {
					if( (*r & 0xfe) == 0x14 ) {
						show_man();
						mans.y = (19-(r-field)/41)*16;
						show_man();
						break;
					}
					r += 41;
				}
			}
			beep_seq(beep_teleport);
		}
		if( newmandir != -1 ) {
			show_man();
			mans.dir = newmandir;
			newmandir = -1;
			show_man();
		}
		
	} else if( newmandir != -1 && newmandir==(mans.dir^2)) {
		show_man();
		mans.dir = newmandir;
		newmandir = -1;
		show_man();
	}
/*
	for(i=0; i<MAXBUGS; i++) {
		check_tag_man(i);
	}
*/
}

void	bang_bug(i)
int	i;
{
	show_bug(i);
	bugs[i].dir = 20;
	show_bug(i);
	beep_seq(beep_kill);
}


void	make_bug() {

	int	r, i, j, k, diff;

	if(dead) return;
	for(i=0; i<MAXBUGS; i++) {
		if( bugs[i].dir == -1 ) {
			diff = 1000;
			for(j=0; j<MAXPITS; j++) {
				if( pitlist[j].x == -1 ) break;

r = man_dist(field, (pitlist[j].x>>4)+1, 19-(pitlist[j].y>>4), (mans.x>>4)+1, 19-(mans.y>>4) );

				if( r < diff ) {
					k = j;
					diff = r;
				}
			}
			if( (j>1) && (rand(10)>7) ) {
				while( k==(r=rand(j)) ) ;
				k = r;
			}
			bugs[i].x = pitlist[k].x;
			bugs[i].y = pitlist[k].y;
			bugs[i].dir = 15;
			bugs[i].kind = 1;
			bugs[i].lost = 0;
			show_bug(i);
			return;
		}
	}
}


int	legal_bug_move( i, p )
int	i;
char	*p;
{
	char	c, d;

	switch( i ) {
	 case 0:
		c = *(p+1);
		break;
	 case 1:
		c = *(p-41);
		break;
	 case 2:
		c = *(p-1);
		break;
	 case 3:
		c = *(p+41);
		break;
	 default:
		return 0;		
	}
	d = c&0xf0;
	if( c==1 || d==0x10 || d==0x20 || d==0x70 ) return 0;
	return 1;
}


int	look_man_move( i, p )
int	i;
char	*p;
{
	char	c, d;

	switch( i ) {
	 case 0:
		c = *(p+1);
		break;
	 case 1:
		c = *(p-41);
		break;
	 case 2:
		c = *(p-1);
		break;
	 case 3:
		c = *(p+41);
		break;
	 default:
		return 0;
		break;
	}
	if( c == -1 ) {
		if( i == 2 ) return 1;
		else return -1;
	}
	d = c & 0xf0;
	if( d == 0xf0 || d== 0x20 || c == 1 || (d==0x10 && (c&0x0e)!=0x04) ) return (int)c;
	if( c == 0x70 && !mans.key ) return (int)c;
	return 0;
}

