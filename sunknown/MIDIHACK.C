/*
** Copyright (c) 1992 Forte.  All rights reserved.
** Written late one night 6/27/93.
**
** Note from Five, And Then Some:
**   This file was originally included in the ultramid-package (or the
**   Software Development Kit for the Gravis Ultrasound).
**
** Additional programming by Ronny H.
**
*/


//#define DEBUG
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>

/* midi files */
#include "global.h"
#include "midi.h"
#include "totsr.h"
#include "midihack.h"


void midi_do_something(struct MIDILIB *ml, unsigned long now_time);
unsigned long midi_getvarinum(struct track *track);


struct header_type header;	/* format of midifile, # of tracks, and time */
int midi_libs=0;
int control_break = FALSE;
FILE *fpIn=NULL;	/* file pointer */
struct MIDILIB *ml;
int i, j, ret;
char *cp;
int not_done_playing=1;

/* callback routine to read a character from the midi file */
mygetc() { return(getc(fpIn)); }

/* callback routine for the midi library to print error messages */
void myerror(s)
char *s;
{
	midi_error("Error: %s\n", s);
}

/* callback routine for the midi library to record header information */
void myheader(format,ntrks,division)
int format,ntrks,division;
{
    header.format = format;
    header.ntrks = ntrks;
    header.division.quarter_note = division;
}

void mytrack(struct MIDILIB *ml, struct track *track)
{
	struct track *tp;

	if (ml->Mp_tracks) {
		for (tp=ml->Mp_tracks; tp->next_track; tp = tp->next_track) ;
		tp->next_track = track;
	} else {
		ml->Mp_tracks = track;
	}
}

/*
** This routine initalizes the midifile and midiplay libraries and
** sets up all of the callback functions
*/
void init_funcs(struct MIDILIB *ml)
{
    int i;
    int bm;

    ml->Mf_getc = mygetc;
    ml->Mf_header = myheader;
    ml->Mp_set_tempo = um_set_tempo;
    ml->Mp_reset_tick_counter = um_reset_tick_counter;
    ml->Mp_wait_for = um_wait_for;
    ml->Mp_init_timers = um_init_timers;
    ml->Mp_cleanup_timers = um_cleanup_timers;
    ml->Mp_reset_midi = reset_um;
    ml->Mp_note_on = um_note_on;
    ml->Mp_note_off = um_note_off;
    ml->Mp_pressure = um_pressure;
    ml->Mp_parameter = um_parameter;
    ml->Mp_pitch_bend = um_pitch_bend;
    ml->Mp_program = um_program;
    ml->Mp_chanpressure = um_chanpressure;
    ml->Mp_init_hardware = um_init_hardware;
    ml->Mp_cleanup = um_cleanup;
}

void midi_error(char *fmt, ...)
{
	char buffer[512];
	register int i;
	va_list argptr;
	int cnt;

	va_start(argptr, fmt);
	cnt = vsprintf(buffer, fmt, argptr);
	if (cnt != EOF) {
		puts(buffer);
	}
	va_end(argptr);
}

void midi_message(char *fmt, ...)
{
	char buffer[512];
	register int i;
	va_list argptr;
	int cnt;

	va_start(argptr, fmt);
	cnt = vsprintf(buffer, fmt, argptr);
	if (cnt != EOF) {
		puts(buffer);
	}
	va_end(argptr);
}

void smidistart(void)
{
	struct track *track;
	unsigned long next_event_time;
	int i, iret, c;

  /* try and open the midifile */
  fpIn = fopen(MIDIFILE, "rb");
  if (fpIn == NULL) {
#ifdef DEBUG
	  fprintf(stderr,"%s: unable to open file %s for reading.\n","smidistart()", MIDIFILE);
#endif
	  exit(1);
  }
  ml = mfinit(myerror);
  (void) init_funcs(ml); /* intialize midi libraries */
  mfread(ml);	     /* read midi file */
  fclose(fpIn);

	not_done_playing = 1;
	/* initialize track playback info */
	next_event_time = -1L;
	for (track=ml->Mp_tracks; track; track = track->next_track) {
	    track->playp = track->data;
	    track->position = 0L;
	    track->status = 0;
	    track->play_state = MPS_PLAYING;
			track->play_time = midi_getvarinum(track);
	    if (track->play_time < next_event_time) {
		next_event_time = track->play_time;
	    }
	}
	ml->Mp_next_event_time = next_event_time;
	if (ml->Mp_set_tempo && header.division.quarter_note < 0) {
	    (*ml->Mp_set_tempo)(0, header.division.quarter_note);
	}
	if (ml->Mp_wait_for == 0L) return;
	if (ml->Mp_reset_tick_counter) (*ml->Mp_reset_tick_counter)();
	if (ml->Mp_reset_midi) (*ml->Mp_reset_midi)();
	if (ml->Mp_init_hardware) {
	    iret = (*ml->Mp_init_hardware)(ml);
	    if (iret != 0) {
		if (ml->Mp_cleanup) (*ml->Mp_cleanup)();
		return;
	    }
	}
#ifdef DEBUG
        midi_message("Now playing");
#endif
        if (ml->Mp_init_timers) {
	    (*ml->Mp_init_timers)();
	}
	not_done_playing = 1;

}

void smidireset(void)
{
  if (ml->Mp_cleanup_timers)
    (*ml->Mp_cleanup_timers)();

	if (ml->Mp_cleanup)
    (*ml->Mp_cleanup)();

  mfcleanup(ml);	   // close the midifile library

  mfclose(&ml);		   // close the midi libraries
}

void smidigo(void)
{
  if (!((*ml->Mp_wait_for)(ml->Mp_next_event_time)))
    midi_do_something(ml, ml->Mp_next_event_time);
}

