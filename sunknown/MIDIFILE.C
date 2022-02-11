/*
** Copyright (c) 1992 Forte.  All rights reserved.
** Written late one night 6/27/93.
**
** Note from Five, And Then Some:
**   This file was originally included in the ultramid-package (or the
**   Software Development Kit for the Gravis Ultrasound).
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define NULLFUNC 0
#ifdef NULL
#undef NULL
#endif
#define NULL ((void *)0L)
#include "midi.h"

/* private stuff */
static long to32bit(int c1,int c2,int c3,int c4);
static to16bit(int c1,int c2);
static long read32bit(struct MIDILIB *ml);
static read16bit(struct MIDILIB *ml);
static void mftrack(struct MIDILIB *ml, struct track *track);

static void mferror(struct MIDILIB *ml, char *s)
{
	/* if a user routine for error handling is registered, call it */
	if ( ml->Mf_error )
		(*ml->Mf_error)(s);
	exit(1);
}

void mfcleanup(struct MIDILIB *ml)
{
	struct track *track;

	if (ml) {
		while (ml->Mp_tracks) {
			track = ml->Mp_tracks;
			ml->Mp_tracks = ml->Mp_tracks->next_track;
			if (track->data) farfree((void far *)track->data);
			free(track);
		}
	}
}

void mfclose(struct MIDILIB **ml)
{
	struct track *track;

	if (*ml) {
		mfcleanup(*ml);
		free(*ml);
		*ml = 0;
	}
}

struct MIDILIB *
mfinit(void (*errorfunc)(char *))
{
	/* The midi library communicates with the application's software by
	** calling functions.  The midilibrary allocates memory for a structure
	** that the application fills in with all of the functions it can
	** fufill.  This library is operating system and machine independant.
	*/
	struct MIDILIB *ml;

	ml = (struct MIDILIB *)malloc(sizeof(struct MIDILIB));
	if (ml == NULL) {
		if (errorfunc) {
			(void)(*errorfunc)("Couldn't initialize midi library - out of memory\n");
			exit(1);
		}
	}
	ml->Mf_error = errorfunc;
	ml->Mf_getc = NULLFUNC;		/* get a character (from a midi file) */
	ml->Mf_header = NULLFUNC;	/* tell application about header info */
	ml->Mf_toberead = 0L;		/* number of bytes not read yet */
	ml->Mp_tracks = NULL;
	return(ml);
}

/* read through the "MThd" or "MTrk" header string */
static readmt(struct MIDILIB *ml, char *s)
{
	int n = 0;
	char *p = s;
	int c;

	while ( n++<4 && (c=(*ml->Mf_getc)()) != EOF ) {
		if ( c != *p++ ) {
			char buff[32];
			if (c == 26) return(EOF);
			(void) strcpy(buff,"Expecting ");
			(void) strcat(buff,s);
			(void) strcat(buff,", corrupt midi file?");
			(void)mferror(ml, buff);
		}
	}
	return(c);
}

/* read a single character and abort on EOF */
static egetc(struct MIDILIB *ml)
{
	int c = (*ml->Mf_getc)();

	if ( c == EOF )
		(void)mferror(ml, "EOF reached, corrupt midi file");
	ml->Mf_toberead--;
	return(c);
}

/* read a header chunk */
static readheader(struct MIDILIB *ml)
{
	int format, ntrks, division;

	if ( readmt(ml, "MThd") == EOF )
		return(0);

	ml->Mf_toberead = read32bit(ml);
	format = read16bit(ml);
	ntrks = read16bit(ml);
	division = read16bit(ml);

	if ( ml->Mf_header ) (*ml->Mf_header)(format,ntrks,division);

	/* flush any extra stuff, in case the length of header is not 6 */
	while ( ml->Mf_toberead > 0 ) (void) egetc(ml);
	return(1);
}

/* read a track chunk */
static readtrack(struct MIDILIB *ml)
{
	struct track *track;
	char huge *cp;

	if ( readmt(ml, "MTrk") == EOF ) return(0);

	ml->Mf_toberead = read32bit(ml);
	track = malloc(sizeof(struct track));
	if (track == NULL) {
		(void)mferror(ml, "Out of memory - read_track()");
		return(0);
	}
	track->length = ml->Mf_toberead;
	track->data = (void huge *)farmalloc(ml->Mf_toberead);
	if (track->data == (void huge *)NULL) {
		(void)mferror(ml, "Out of memory - read_track()");
		return(0);
	}
	cp = track->data;
	while (ml->Mf_toberead > 0) {
		*cp++ = egetc(ml);
	}
	track->playp = track->data;
	track->play_time = 0L;
	track->play_state = MPS_BEGIN;
	track->next_track = NULL;
	mftrack(ml, track);
	return(1);
}

void mfread(struct MIDILIB *ml)	
{
	midi_message("Reading MIDI file");
	ml->tracknum=0;
	if ( ml->Mf_getc == NULLFUNC )
		(void)mferror(ml, "mfread() called without setting Mf_getc"); 

	readheader(ml);
	while ( readtrack(ml) ) ;
}

static long to32bit(int c1,int c2,int c3,int c4)
{
	long value = 0L;

	value = (c1 & 0xff);
	value = (value<<8) + (c2 & 0xff);
	value = (value<<8) + (c3 & 0xff);
	value = (value<<8) + (c4 & 0xff);
	return (value);
}

static to16bit(int c1,int c2)
{
	return ((c1 & 0xff ) << 8) + (c2 & 0xff);
}

static long read32bit(struct MIDILIB *ml)
{
	int c1, c2, c3, c4;

	c1 = egetc(ml);
	c2 = egetc(ml);
	c3 = egetc(ml);
	c4 = egetc(ml);
	return to32bit(c1,c2,c3,c4);
}

static read16bit(struct MIDILIB *ml)
{
	int c1, c2;
	c1 = egetc(ml);
	c2 = egetc(ml);
	return to16bit(c1,c2);
}

static void mftrack(struct MIDILIB *ml, struct track *track)
{
	struct track *tp;

	if (ml->Mp_tracks) {
	    for (tp=ml->Mp_tracks; tp->next_track; tp = tp->next_track) ;
	    tp->next_track = track;
	} else {
	    ml->Mp_tracks = track;
	}
}
