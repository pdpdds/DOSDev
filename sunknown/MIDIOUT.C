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
#include <dos.h>
#include <malloc.h>
#include "midi.h"

extern int not_done_playing;

/* externals */
extern struct header_type header;

extern int control_break;
unsigned long midi_getvarinum(struct track *track);

#pragma warn -par
static void chanmessage( struct MIDILIB *ml, int c, int c1, int c2, unsigned long time)
{
	int chan;

	chan = c & 0x0f;
	switch (c & 0xf0) {
	    case note_on:
		(*ml->Mp_note_on)(chan, c1, c2);
		break;
	    case note_off:
		(*ml->Mp_note_off)(chan, c1, c2);
		break;
	    case poly_aftertouch:
		(*ml->Mp_pressure)(chan, c1, c2);
		break;
	    case control_change:
		(*ml->Mp_parameter)(chan, c1, c2);
		break;
	    case pitch_wheel:
		(*ml->Mp_pitch_bend)(chan, c1, c2);
		break;
	    case program_chng:
		(*ml->Mp_program)(chan, c1);
		break;
	    case channel_aftertouch:
		(*ml->Mp_chanpressure)(chan, c1, c2);
		break;
	}
}
#pragma warn .par

void midi_do_something(struct MIDILIB *ml, unsigned long now_time)
{
	/* This array is indexed by the high half of a status byte.  It's */
	/* value is either the number of bytes needed (1 or 2) for a channel */
	/* message, or 0 (meaning it's not  a channel message). */
	static int chantype[] =
  {
	    0, 0, 0, 0, 0, 0, 0, 0,		/* 0x00 through 0x70 */
	    2, 2, 2, 2, 1, 1, 2, 0		/* 0x80 through 0xf0 */
	};
	int not_done = 0,
      len,
      needed,
      type,
      i;
	unsigned long us;
	register struct track *track;
	unsigned char c,
                c1,
                c2,
                check_next_time;

	if (now_time < ml->Mp_next_event_time)
    return;

	ml->Mp_next_event_time = -1L;

	for (track=ml->Mp_tracks; track; track = track->next_track)
  {
	  check_next_time = 1;
	  if (track->play_state == MPS_PLAYING)
    {
	    while (track->play_time <= now_time)
      {
		    check_next_time = 0;
        if (*track->playp & 0x80)
			    c = track->status = midi_getc(track);
        else
          c = track->status;
		    needed = chantype[ (c>>4) & 0xf ];
		    if (needed) /* is it a channel message? */
        {
          c1 = midi_getc(track);
          c2 = (needed>1) ? midi_getc(track) : 0;
          chanmessage( ml, c, c1, c2, now_time );
		    }
        else
        {
          switch (c)
          {
            case MT_meta_event:
              type = midi_getc(track);
              len = midi_getvarinum(track);
              switch (type)
              {
                case MT_set_tempo:
                  len -= 3;
                  us = ((long)midi_getc(track)<<16) + ((long)midi_getc(track)<<8) + (long)midi_getc(track);
                  for ( ; len > 0; len--)
                    midi_getc(track);
                  (*ml->Mp_set_tempo)(us, header.division.quarter_note);
                  break;
                default:
                  for (i=0; i < len; i++)
                    midi_getc(track);
              }
			        break;
			      case system_exclusive:
			      case sysex_continue:
			        len = midi_getvarinum(track);
			        for ( i=0; i < len; i++)
				        midi_getc(track);
			        break;
			      default: /* whoops */
			        track->play_state = MPS_DONE;
			        break;
			    }
		    }
		    midi_get_next_time(ml, track);
		    if (track->play_state != MPS_PLAYING)
          break;
		  }
	  }
	  if (track->play_state == MPS_PLAYING)
    {
		  if (check_next_time)
		    if (track->play_time < ml->Mp_next_event_time)
          ml->Mp_next_event_time = track->play_time;
		  not_done = 1;
	  }
	}
	not_done_playing = not_done;
}

int midi_getc(struct track *track)
{
	if (track->position < track->length) {
		track->position++;
		return(*track->playp++);
	} else {
		track->play_state = MPS_DONE;
		return(0);
	}
}

unsigned long midi_getvarinum(struct track *track)
{
	long value=0L;
	int c;

	do {
		c = midi_getc(track);
		value = (value << 7) + (c & 0x7f);
	} while (c & 0x80);
	return (value);
}

void midi_get_next_time(struct MIDILIB *ml, struct track *track)
{
	if (track->play_state == MPS_PLAYING) {
		track->play_time += midi_getvarinum(track);
		if (track->play_time < ml->Mp_next_event_time) {
			ml->Mp_next_event_time = track->play_time;
		}
	}
}
