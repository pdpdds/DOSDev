#include	<stdio.h>
#include	<fcntl.h>
#include	<math.h>
#include	"snarf.h"

/****************************************************************
 * BEYOND HERE, THE CODE IS DEVELOPMENT ROUTINES ONLY. THEY CAN	*
 * BE TRASHED WHEN THE CODE DEVELOPMENT CYCLE IS FINISHED.	*
 ****************************************************************/

void	choose_palette() {

	int	color, i;

	color = 1;
	while(1) {
		setpalette(color, 1);
		i = key();
		setpalette(color, 0);
		switch(i) {
			case 0x4d00:	/* rtcurs */
				color = (++color) & 15;
				break;
			case 0x4b00:	/* lfcurs */
				color = (--color) & 15;
				break;
			case 0x4800:	/* upcurs */
				palette[color] = (++palette[color]) & 63;
				break;
			case 0x5000:	/* dncurs */
				palette[color] = (--palette[color]) & 63;
				break;
			case 0x011b:	/* esc    */
				return;
			default:
				beepon(14000);
				for(i=0; i<500; i++) iskey();
				beepoff();
				break;
		}
	}
}


void	choose_colors() {

	int	i, color;
	char	temp[81];

	color = 0;
	while(1) {
		sprintf(temp, "color: %d = %d  ", color, colors[color]);
		flabel(400, YMAX+18, temp, 15, 0, 0);
		i = key();
		switch( i ) {
		 case 0x4d00:	/* rtcurs */
			if(color<MAXCOLOR-1) ++color;
			break;
		 case 0x4b00:	/* lfcurs */
			if(color) --color;
			break;
		 case 0x4800:	/* upcurs */
			colors[color] = (++colors[color]) & 15;
			break;
		 case 0x5000:	/* dncurs */
			colors[color] = (--colors[color]) & 15;
			break;
		 case 0x011b:	/* esc    */
			return;
		 default:
			beepon(14000);
			for(i=0; i<500; i++) iskey();
			beepoff();
			break;
		}
	}
}



void	up_walls() {

	int	i, j, k, t;
	unsigned char	*p, c;

	for(i=0; i<90; i++) walls[curwall][i] = 0;
	for(i=0; i<MAXGOODIES; i++) {
		treasure[curwall][i].x = 0;
		treasure[curwall][i].y = 0;
		treasure[curwall][i].item = 0;
	}
	t = 0;
	for(i=1; i<19; i++) {
		for(j=1; j<40; j++) {
			switch( (c=field[i*41+j]) & 0xf0 ) {
			 case 0x00:	/* WALL */
				if( c ) walls[curwall][(i-1)*5+((j-1)>>3)] |= (0x80>>((j-1)&7));
				break;
			 case 0x10:	/* TELEPORT */
				if( t<MAXOBJECTS && (c & 0xfe)==0x14) {
					treasure[curwall][t].x = j;
					treasure[curwall][t].y = i;
					treasure[curwall][t].item = c;
					++t;
				}					
				break;
			 case 0x20:	/* PIT */
				if( t<MAXOBJECTS && (c & 1) ) {
					treasure[curwall][t].x = j;
					treasure[curwall][t].y = i;
					treasure[curwall][t].item = c;
					++t;
				}					
				break;
			 case 0x30:	/* MAN start position */
			 case 0x40:	/* TREASURE */
			 case 0x60:	/* KEY */
			 case 0x70:	/* LOCK */
			 case 0x80:	/* TAGBOOST */
				if( t<MAXOBJECTS ) {
					treasure[curwall][t].x = j;
					treasure[curwall][t].y = i;
					treasure[curwall][t].item = c;
					++t;
				}					
				break;
			 default:
				break;
			}
		}
	}
}

void	edit_error() {

	int	i;

	beepon(10000);
	for(i=0; i<500; i++) iskey();
	beepoff();
}

int	empty(x,y)
int	x,y;
{
	unsigned char	c;

	c = field[y*41+x] & 0xf0;
	if( c==0x30 || c==0xf0 ) return 0;
	return 1;
}


int	ewx=20, ewy=9;

void	edit_walls() {

	int	i, j, k;
	unsigned char	*p, temp[81];
	FILE	*fp;

	curscreen = curwall-1;
ewstart:
	curscreen = curwall+1;
	next_screen();
	field[(19-(mans.y>>4))*41+(mans.x>>4)+1] = 0x30;

	flabel(0, 8, "F1-save  F2-tagboo  F3-field  F4-teleport  F5-lock  F6-key  F7-gold  F8-pit  F9-wall  F10-man  ESC-end_edit",15,0,0);
	while(1) {
		while( !iskey() ) {
			horline(XORG-4+((ewx-1)<<4), YORG-8+((19-ewy)<<4), XORG+4+(ewx<<4), 15, 3);
			verline(XORG+8+((ewx-1)<<4), YORG+4+((19-ewy)<<4), YORG-4+((18-ewy)<<4), 15, 3);
			for(i=0; i<500 && !iskey(); i++) ;
			horline(XORG-4+((ewx-1)<<4), YORG-8+((19-ewy)<<4), XORG+4+(ewx<<4), 15, 3);
			verline(XORG+8+((ewx-1)<<4), YORG+4+((19-ewy)<<4), YORG-4+((18-ewy)<<4), 15, 3);
			for(i=0; i<200 && !iskey(); i++) ;
		}
	 	p = &field[ewy*41+ewx];
		switch( key() ) {
		 case 0x3b00:	/* f1 */
			up_walls();
			sprintf(temp,"WALL%d", curwall);
			fp = fopen(temp, "w");
			fprintf(fp, "walls\n");
			for(i=0; i<18; i++) {
				fprintf(fp, "\t\t");
				for(j=0; j<5; j++) {
					fprintf(fp, "0x%2.2x, ", walls[curwall][i*5+j]);
				}
				fprintf(fp, "\n");
			}
			fprintf(fp, "treasure\n");
			for(i=0; i<MAXOBJECTS; i++) {
				if(treasure[curwall][i].x==0) continue;
				fprintf(fp, "\t\t0x%2.2x, 0x%2.2x, 0x%2.2x,\n",treasure[curwall][i].x,treasure[curwall][i].y,treasure[curwall][i].item);
			}
			fclose(fp);
			break;
		 case 0x3c00:	/* f2 */
			disp_field( ewx, ewy );
			*p = 0x80;
			disp_field( ewx, ewy );
			break;
		 case 0x3d00:	/* f3 */
			up_walls();
			goto ewstart;
		 case 0x5600:	/* shift f3 */
			up_walls();
			if( (curwall -= 2) < 0 ) curwall += MAXWALLS;
			goto ewstart;
		 case 0x3e00:	/* f4 */
			disp_field( ewx, ewy );
			disp_field( ewx-1, ewy);
			disp_field( ewx+1, ewy);
			switch( key() ) {
			 case 0x4800:	/* up cursor */
				*p = 0x15;
				*(p+1) = 0x13;
				*(p-1) = 0x12;
				break;
			 case 0x5000:	/* dn cursor */
				*p = 0x14;
				*(p+1) = 0x11;
				*(p-1) = 0x10;
				break;
			 default:
				*p = 0;
				break;
			}
			disp_field( ewx, ewy );
			disp_field( ewx-1, ewy);
			disp_field( ewx+1, ewy);
			break;
		 case 0x3f00:	/* f5 */
			disp_field( ewx, ewy );
			*p = 0x70;
			disp_field( ewx, ewy );
			break;
		 case 0x4000:	/* f6 */
			disp_field( ewx, ewy );
			*p = 0x60;
			disp_field( ewx, ewy );
			break;
		 case 0x4100:	/* f7 */
			disp_field(ewx, ewy);
			switch( *p ) {
			 case 0:
				*p = 0x3f;
			 case 0x40:
			 case 0x41:
			 case 0x42:
				*p = ++(*p);
				disp_field(ewx, ewy);
				break;
			 default:
				*p = 0;
				break;
			}
			break;
		 case 0x011b:	/* esc */
			return;
		 case 0x4d00:	/* rtcurs */
			if(ewx<39) ++ewx;
			break;
		 case 0x4800:	/* upcurs */
			if(ewy>1) --ewy;
			break;
		 case 0x4b00:	/* lfcurs */
			if(ewx>1) --ewx;
			break;
		 case 0x5000:	/* dncurs */
			if(ewy<18) ++ewy;
			break;
		 case 0x4200:	/* f8 */
			disp_field(ewx, ewy);
			switch( key() ) {
			 case 0x4d00:	/* rtcurs */
				disp_field( ewx, ewy+1);
				disp_field( ewx, ewy-1);
				*p = 0x21;
				*(p-41) = 0x2a;
				*(p+41) = 0x20;
				disp_field(ewx, ewy);
				disp_field(ewx, ewy+1);
				disp_field(ewx, ewy-1);
				break;
			 case 0x4800:	/* upcurs */
				disp_field( ewx+1, ewy);
				disp_field( ewx-1, ewy);
				*p = 0x23;
				*(p-1) = 0x2e;
				*(p+1) = 0x24;
				disp_field(ewx, ewy);
				disp_field(ewx+1, ewy);
				disp_field(ewx-1, ewy);
				break;
			 case 0x4b00:	/* lfcurs */
				disp_field( ewx, ewy+1);
				disp_field( ewx, ewy-1);
				*p = 0x25;
				*(p-41) = 0x26;
				*(p+41) = 0x22;
				disp_field(ewx, ewy);
				disp_field(ewx, ewy+1);
				disp_field(ewx, ewy-1);
				break;
			 case 0x5000:	/* dncurs */
				disp_field( ewx+1, ewy);
				disp_field( ewx-1, ewy);
				*p = 0x27;
				*(p-1) = 0x2c;
				*(p+1) = 0x28;
				disp_field(ewx, ewy);
				disp_field(ewx+1, ewy);
				disp_field(ewx-1, ewy);
				break;
			 default:
				*p = 0;
				break;
			}
			break;
		 case 0x4300:	/* f9 */
			if( *p ) {
				disp_field(ewx, ewy);
				*p = 0;
			} else {
			 	*p = 1;
				disp_field(ewx, ewy);
			}
			break;
		 case 0x4400:	/* f10 */
			show_man();
			*p = 0x30;
		 	field[(19-(mans.y>>4))*41+(mans.x>>4)+1] = 0;
			mans.x = (ewx-1)<<4;
			mans.y = (19-ewy)<<4;
			show_man();
			break;
		 default:
			break;
		}
	}
}
