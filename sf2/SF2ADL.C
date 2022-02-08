#include  <stdio.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include  <string.h>
#include  <ctype.h>
#include  <dos.h>

#define  TURBOC
#define  MAX_VOICES 20
#define  UCHAR unsigned char
#define  INT_METHOD
#define nbLocParam		14

#define prmKsl			0
#define prmMulti		1
#define prmFeedBack	2			/* use for opr. 0 only */
#define prmAttack		3
#define prmSustain		4
#define prmStaining	5			/* Sustaining ... */
#define prmDecay		6
#define prmRelease		7
#define prmLevel		8
#define prmAm			9
#define prmVib			10
#define prmKsr			11
#define prmFm			12			/* use for opr. 0 only */
#define prmWaveSel		13			/* wave select */

/* globals parameters: */
#define prmAmDepth		14
#define prmVibDepth	15
#define prmNoteSel		16
#define prmPercussion	17

/* melodic voice numbers: */
#define vMelo0			0
#define vMelo1			1
#define vMelo2			2
#define vMelo3			3
#define vMelo4			4
#define vMelo5			5
#define vMelo6			6
#define vMelo7			7
#define vMelo8			8

/* percussive voice numbers: */
#define BD				6
#define SD				7
#define TOM			8
#define CYMB			9
#define HIHAT			10


#define MAX_VOLUME		0x7f
#define LOG2_VOLUME	7			/* log2( MAX_VOLUME) */
#define MAX_PITCH		0x3fff
#define MID_PITCH		0x2000

#define MID_C			60			/* MIDI standard mid C */
#define CHIP_MID_C		48			/* sound chip mid C */
#define NR_NOTES		96			/* # of notes we can play on chip */



#define   FALSE     0
#define   TRUE      !FALSE

UCHAR *buf;                  /* Music File Pointer */
int init_success = 1;        /* Flag of Initilization Success */

/* Keeps track of last change of volume to avoid unnecessary calls to
   change of volume routine. */
extern int current_vol [MAX_VOICES];
extern int volume_flag;

int SoundColdInit( unsigned port );
void SoundWarmInit( void );
void SetMode( int mode );
void SetPitchRange( unsigned pR );
void SetGParam( int amD, int vibD, int nSel );
void SetVoiceTimbre( unsigned voice, unsigned * paramArray );
void SetVoiceVolume( unsigned voice, unsigned volume );
void SetVoicePitch( unsigned voice, unsigned pitchBend );
void NoteOn( unsigned voice, int pitch );
void NoteOff( unsigned voice );
void offMusic(void);
void Midi_End (void);
void StartTimeOut (int);
/*-------------------------------------------------------------------------
   Enable or disable the volume, but continue playing the song.
*/
Volume_OnOff (flag)
   int flag;
{
   int n;
   if (flag != volume_flag) {
	  if (!flag)
		 for (n=0; n < MAX_VOICES; n++) {
			SetVoiceVolume (n, 0);
			NoteOff (n);
		 }
	  else
		 for (n=0; n < MAX_VOICES; n++) SetVoiceVolume (n, current_vol [n]);
   }
   volume_flag = flag;
}


#define   END_OF_TRACK   0x2f
#define   TEMPO          0x51
#define   NR_CHANS       16
#define   SYSEX_F0       0xf0
#define   SYSEX_F7       0xf7
#define   META           0xff
#define   SEQ_SPECIFIC   0x7f

static int      tracks;                /* number of tracks */
static int      tickQnote;             /* ticks per quarter note */
static UCHAR    **musPtrPtr;           /* ptr into trkPtrs */
static UCHAR    *status;               /* ptr to running status of current track */
static unsigned clock_rate = 0;        /* original clock interrupt rate */

static long tickCount = 0;          /* current absolute time */
static int  ctrk = 0;               /* current track */

#define TRACKS 16
static UCHAR *trkPtrs [TRACKS];        /* ptrs to each data track */
static UCHAR trkStats [TRACKS];        /* running status for each track */
static long  abs_time [TRACKS];        /* time of next event for each track */

/* This is to make the program easier to read. */
#define  musPtr     (*musPtrPtr)

/* Keeps track of last change of volume to avoid unnecessary calls to
   change of volume routine. */
int current_vol [MAX_VOICES];
int volume_flag = 1;

/* Flags */
char    musRunning;                    /* != 0 if music is playing */
static char    end_of_data;            /* != 0 if end of data */
static char    clock_in = 0;           /* != 0 if installed */

/* Prototypes */
static SetUp_Data (UCHAR *);
static Start_Melo ();
static unsigned  Do_Event ();

extern UCHAR *buf;

#ifdef INT_METHOD
   unsigned TimeOut ();
#endif


/*-------------------------------------------------------------------------
	Install the clock interrupt routine.
*/
void Midi_init(void)
{
	if (clock_in) return;
	Clk_install();
	clock_in = 1;
}


/*-------------------------------------------------------------------------
   Main routine for playing a MIDI file.  It receives an array of function
   pointers which are used to call the routines which process the MIDI
   events.
   Returns 0 if interrupt routine not installed, else returns 1.
*/
int Midi_play (UCHAR *dataPtr)
{
   if (!clock_in) return (0);
   SetUp_Data (dataPtr);
   Start_Melo ();
   return (1);
}


/*-------------------------------------------------------------------------
	Uninstall the clock driver ...
*/
void Midi_End (void)
{
   if (clock_in) Clk_uninstall();
   clock_in = 0;
}


/*-------------------------------------------------------------------------
   Get word value from data.  Value is stored MSB first. */
static unsigned Get_Word (ptr)
   UCHAR *ptr;
{
   unsigned n;
   n = *ptr++;
   n = (n << 8) + *ptr++;
   return (n);
}


/* Get long value from data.  Value is stored MSB to LSB. */
static long Get_Long (ptr)
   UCHAR *ptr;
{
   long l = 0L;
   int n;
   for (n=0; n < 4; n++)
	  l = (l << 8) + *ptr++;
   return (l);
}


/*-------------------------------------------------------------------------
   Set up trkPtrs, which is an array of pointers, to point to the track
   chunks. Does not modify musPtr. */
static SetUp_Tracks (trcks, chunk)
   int trcks;
   UCHAR *chunk;
{
   int n;
   long length;
   UCHAR **tPtr = trkPtrs;

   for (n=0; n < trcks; n++) {
	  length = Get_Long (chunk + 4);
	  tPtr [n] = chunk + 8;
	  chunk += (length + 8L);
   }
}


/*-------------------------------------------------------------------------
   Reads a variable length value from the MIDI file data and advances the
   data pointer.  */

static long Get_Length ()
{
   long value;
   UCHAR c, *data;

   data = musPtr;
   if ((value = *data++) & 0x80) {
	  value &= 0x7f;
	  do {
		 value = (value << 7) + ((c = *data++) & 0x7f);
	  } while (c & 0x80);
   }
   musPtr = data;
   return (value);
}


/*-------------------------------------------------------------------------
  Set up all of the data structures used in playing a MIDI file. */
static SetUp_Data (dataPtr)
   UCHAR *dataPtr;
{
   long length;
   int i, j;

   for (i=0; i < MAX_VOICES; i++) {
	  current_vol [i] = 0;
	  SetVoiceVolume (i, 0);
	  NoteOff (i);
   }

   tickCount = 0L;                         /* current absolute time */
   ctrk = 0;                               /* current track */

   /* Read file header */
   length = Get_Long (dataPtr + 4);        /* header length */
   tracks = Get_Word (dataPtr + 10);       /* number of tracks */
   tickQnote = Get_Word (dataPtr + 12);    /* ticks per quarter note */

   /* Set musPtr to point to start of first chunk */
   dataPtr += (length + 8L);

   /* Set up the array trkPtrs */
   SetUp_Tracks (tracks, dataPtr);

   /* Initialize arrays */
   for (i=0; i < tracks; i++) {
	  musPtrPtr = &trkPtrs [i];         /* set global data ptr */
	  abs_time [i] = Get_Length ();     /* set start time for track */
	  trkStats [i] = *musPtr;           /* current running status for track */
   }
}

/*-------------------------------------------------------------------------
	Start playing a melody. Set some global pointers and the tempo.  Start
	the clock driver with the first delay (>= 1) */
static Start_Melo ()
{

	musPtrPtr = trkPtrs;
	status = trkStats;
	end_of_data = 0;
	Set_Tempo (0, 500000L);
	StartTimeOut (Do_Event ());
	musRunning = 1;
}


/*-------------------------------------------------------------------------
   Stop playing the melody. Reset the clock frequency to normal (18.2 Hz). */
Stop_Melo()
{
	musRunning = 0;
	Set_Original_Clock ();
}


/*-------------------------------------------------------------------------
   Set clock rate to its original interrupt rate. Note that the clock rate
   has been saved at 10 times its real value in order to preserve some
   accuracy. */
Set_Original_Clock ()
{
   SetClkRate (0);
}


/*-------------------------------------------------------------------------
	Change the tempo.

	Reload the timer-0 with the proper divider for generating
	the appropriate frequency.

	If tempo is zero, reprogram the counter for 18.2 Hz.
*/
Set_Tempo (tickQnote, usec)
	unsigned tickQnote;         /* ticks per quarter note */
	long     usec;              /* micro-seconds per quarter note */
{
	long count;

	if (!tickQnote)
		count = 0L;
	else {
		/* Calculate required interrupt rate (ticks per sec) */
		usec /= 1000L;
		count = (1194L * usec) / tickQnote;
	}

	/* and set the counter: */
	SetClkRate ((unsigned) count);
}


/*-------------------------------------------------------------------------
   Finds the next event to be processed by checking 'abs_times', which
   contains the absolute time (in ticks) of when the next event occurs
   for each track. Sets the global data ptr musPtr to point to the next
   event and sets the running status for that track as well.
   Returns number of ticks until next event. */

static unsigned  Get_Next_Delay ()
{
   long delta;
   int n, min;

   if (*status != END_OF_TRACK) {
	  delta = Get_Length ();              /* get delta time */
	  abs_time [ctrk] += delta;           /* set new time for track */
   }
   else abs_time [ctrk] = 0x7fffffffL;    /* impossibly large value */

   /* Find earliest time in abs_time array.  This determines which track
	  contains the next event.  */
   for (min=0, n=1; n < tracks; n++)
	  if (abs_time [n] < abs_time [min] && trkStats [n] != END_OF_TRACK)
		 min = n;

   if (trkStats [min] == END_OF_TRACK) {
	  /* end of data condition for all tracks */
	  end_of_data = 1;
	  Stop_Melo ();
          SetUp_Data(buf);
          Start_Melo();
          delta = 0L;
   }
   else {

      delta = abs_time [min] - tickCount; /* calculate time until next event */
      tickCount = abs_time [min];         /* set current time */
      musPtrPtr = &trkPtrs [min];         /* reset data ptr */
      status = &trkStats [min];           /* set running status */
      ctrk = min;
   }

   return ((unsigned) delta);
}

/*-------------------------------------------------------------------------*/
static myNoteOn (int voice, int note, int volume)
{
   if (!volume) {
	  /* A note-on with a volume of 0 is equivalent to a note-off. */
	  NoteOff (voice);
	  current_vol [voice] = volume;
   }
   else {
	  /* Regular note-on */
	  if (current_vol [voice] != volume) {
		 SetVoiceVolume (voice, volume);
		 current_vol [voice] = volume;
	  }
	  NoteOn (voice, note);
   }
}

/*-------------------------------------------------------------------------
   Process a regular MIDI event.  Which routine to call is determined by
   using the 3 LSB's of the high nibble. */

static Midi_Event (event)
   unsigned event;
{
   /* Table of # of data bytes which follow a regular midi status byte */
   static int data_bytes [7] = { 2, 2, 2, 2, 1, 1, 2 };

   int stat, voice;

   stat = (event >> 4) & 7;
   voice = event & 0x0f;

   if (voice < MAX_VOICES) switch (stat) {
	  case 0:
		 NoteOff (voice);
		 break;
	  case 1:
		 myNoteOn (voice, *musPtr, *(musPtr+1));
		 break;
	  case 2:
		 if (current_vol [voice] != *(musPtr+1) && volume_flag)
			SetVoiceVolume (voice, *(musPtr+1));
		 current_vol [voice] = *(musPtr+1);
		 break;
	  case 5:
		 if (current_vol [voice] == *musPtr) break;
		 if (volume_flag) SetVoiceVolume (voice, *musPtr);
		 current_vol [voice] = *musPtr;
		 break;
	  case 6:
		 SetVoicePitch (voice, (*(musPtr+1) << 7) | *musPtr);
		 break;
   }

   musPtr += data_bytes [stat];
}


/*-------------------------------------------------------------------------
   Process an Ad Lib specific meta-event. */

static AdLib_Specific (code, data)
   int code;
   unsigned char *data;
{
   if (code == 1) {
	  /* Instrument change code.  First byte of data contains voice number.
		 Following bytes contain instrument parameters.  */
	  //extern SetVoiceTimbre (unsigned, unsigned *);
	  int n;
	  unsigned int params [28];
	  for (n=0; n < 28; n++) params [n] = data [n+1] & 0xff;
	  SetVoiceTimbre ((unsigned) data [0], params);
   }
   else if (code == 2) {
	  /* Melo/perc mode code.  0 is melodic, !0 is percussive. */
	  //extern SetMode (int);
	  SetMode ((int) data [0]);
   }
   else if (code == 3) {
	  /* Sets the interval over which pitch bend changes will be applied. */
	  SetPitchRange ((int) data [0]);
   }
}


/*-------------------------------------------------------------------------
   Process meta-event.  All events other than end-of-track and tempo events
   are ignored.  */
static Meta_Event ()
{
   /* musPtr points to the event type byte which follows the 0xff. */
   if (*musPtr == END_OF_TRACK) {
	  *status = END_OF_TRACK;
	  musPtr--;                  /* leave ptr on EOT event */
   }
   else if (*musPtr == TEMPO) {
	  long l;
	  musPtr += 2;               /* event type and length bytes */
	  l = *musPtr;
	  l = (l << 8) + *(musPtr+1);
	  l = (l << 8) + *(musPtr+2);
	  musPtr += 3;
	  Set_Tempo (tickQnote, l);
   }
   else if (*musPtr == SEQ_SPECIFIC) {
	  UCHAR far *data;
	  long l;
	  musPtr++;                  /* event type byte */
	  l = Get_Length ();
	  data = musPtr;

	  /* Ad Lib midi ID is 00 00 3f. */
	  if (data [0] == 0 && data [1] == 0 && data [2] == 0x3f) {
		 /* The first two bytes after the ID contain the Ad Lib event code.
			The following bytes contain the data pertaining to the event. */
		 AdLib_Specific ((data [3] << 8) | data [4], &data [5]);
	  }
	  musPtr += l;
   }
   else {
	  musPtr += 1;               /* event type byte */
	  musPtr += Get_Length ();   /* event data */
   }
}


/*-------------------------------------------------------------------------*/
static Sysex_Event (event)
   UCHAR event;
{
   long len = Get_Length ();

   /* skip over system exclusive event */
   musPtr += len;
}


#ifdef INT_METHOD
/*-------------------------------------------------------------------------
	Interrupt routine. Called by low-level clock driver when
	the delay count has expired.

	'musPtr' always points to the first byte AFTER the timing byte.

	When this routine is called, the active SS  (stack segment) is not
	the original of the application, so take care.
	This routine, and all others called by, must be compiled
	without stack-overflow checking, since the SS has changed!!!

	Return to caller the number of clock ticks to wait for before
	the next call.
*/
unsigned TimeOut()
{
	if (! musRunning)
		/* Music has not started or has been stopped, so wait the minimum delay ... */
		return 1;
	else return (Do_Event ());
}

static unsigned  Do_Event ()
{
	unsigned delay;
	do {
		/* If high bit set, set status, else this is running status. */
		if (*musPtr & 0x80) {
		   *status = *musPtr;
		   musPtr++;
		}

		/* Process event. */
		if (*status == SYSEX_F7 || *status == SYSEX_F0)
			  Sysex_Event (*status);
		   else if (*status == META) Meta_Event ();
			 else Midi_Event (*status);

		/* Read next delta time. */
		delay = Get_Next_Delay ();
	} while (delay == 0 && !end_of_data);

	if (delay == 0) return (1);
	  else return (delay);
}


#else
/*-------------------------------------------------------------------------
	Alternate method: instead of all the work being done by the interrupt
	routine, the interrupt routine sets a flag and exits.  The main
	program loops on this flag and processes an event whenever the flag
	is set.  The main program then resets the flag and tells the interrupt
	routine how long to wait until setting the flag again.
	The avantage of this is that it can be used for debugging.  Codeview
	cannot be used for debugging interrupt routines, for example.  As well,
	using "printf" inside an interrupt routine will cause the system to
	hang.
	The avantage of the first method is that you can go off and do other
	things while the music is playing.  With this method, you must watch
	the flag constantly.  Use this method to debug and then switch to the
	interrupt method for your final version.
	To use this method, go into the file "cflags.h", put the define for
	INT_METHOD in comments and recompile midimain.c and this file.
*/

static char timer_signal = 0;

/* Interrupt routine. Called by low-level clock driver when
   the delay count has expired.

   When this routine is called, the active SS  (stack segment) is not
   the original of the application, so take care.
   This routine, and all others called by, must be compiled
   without stack-overflow checking, since the SS has changed!!!
*/
unsigned TimeOut()
{
   timer_signal = 1;
   return (0x7fff);
}

/*
   Processes the next event and returns the delay until the next event
   occurs.
   'musPtr' always points to the first byte AFTER the timing byte.
*/
static unsigned  Do_Event ()
{
	unsigned delay;
	timer_signal = 0;
	if (! musRunning)
		/* Music has not started or has been stopped, so wait the minimum delay ... */
		return 1;

	do {
		/* If high bit set, set status, else this is running status. */
		if (*musPtr & 0x80) {
		   *status = *musPtr;
		   musPtr++;
		}

		/* Process event. */
		if (*status == SYSEX_F7 || *status == SYSEX_F0)
			  Sysex_Event (*status);
		   else if (*status == META) Meta_Event ();
			 else Midi_Event (*status);

		/* Read next delta time. */
		delay = Get_Next_Delay ();

	} while (delay == 0 && !end_of_data);

	if (delay == 0) return (1);
	  else return (delay);
}

int  Test_Event ()
{
   //extern StartTimeOut (int);

   if (!timer_signal) return;

   StartTimeOut (Do_Event ());
}
#endif
#ifdef TURBOC
#endif

/* Declaring variables as near improves performance. */
#define N_V near


/*
	In percussive mode, the last 4 voices  (SD TOM HH CYMB) are created
	using melodic voices 7 & 8. A noise generator use channels 7 & 8
	frequency information for creating rhythm instruments. Best results
	are obtained by setting TOM two octaves below mid C and SD 7 half-tones
	above TOM.
	In this implementation, only the TOM pitch may vary, with the SD always
	set 7 half-tones above it.
*/


#define TOM_PITCH	24				/* best frequency, in range of 0 to 95 */
#define TOM_TO_SD	7				/* 7 half-tones between voice 7 & 8 */
#define SD_PITCH	(TOM_PITCH + TOM_TO_SD)

#define GetLocPrm(slot, prm) ((unsigned)paramSlot [slot] [prm])



/*
-----------------------------------------------------------------
*/

unsigned near genAddr;	/* addr. of sound chip, in DS, used by OUTCHIP.ASM */
int near 	pitchRange;			/* pitch variation, half-tone [+1,+12] */

static int	N_V 	modeWaveSel;		/* != 0 if used with the 'wave-select' parameters */

static char N_V percBits;					/* control bits of percussive voices */
static const char N_V percMasks[] = {
	0x10, 0x08, 0x04, 0x02, 0x01
	};

static char N_V voiceNote [9];          /* pitch of last note-on of each voice */
static char N_V voiceKeyOn [9];     /* state of keyOn bit of each voice */
static unsigned N_V vPitchBend [9]; /* current pitch bend of each voice */
static char N_V bxRegister [9];     /* current val. of 0xB0 - 0xB8 reg */
static char N_V lVoiceVolume [11];      /* volume for each of 11 logical voices */

static unsigned N_V modeVoices;		/* 9 or 11, depending on 'percussion'*/



/* definition of the ELECTRIC-PIANO voice (opr0 & opr1) */
static const char N_V pianoParamsOp0 [nbLocParam] = {
	1, 1, 3, 15, 5, 0, 1, 3, 15, 0, 0, 0, 1, 0 };
static const char N_V pianoParamsOp1 [nbLocParam] = {
	0, 1, 1, 15, 7, 0, 2, 4, 0, 0, 0, 1, 0, 0 };

/* definition of default percussive voices: */
static const char N_V bdOpr0[] =  {0,  0, 0, 10,  4, 0, 8, 12, 11, 0, 0, 0, 1, 0 };
static const char N_V bdOpr1[] =  {0,  0, 0, 13,  4, 0, 6, 15,  0, 0, 0, 0, 1, 0 };
static const char N_V sdOpr[] =   {0, 12, 0, 15, 11, 0, 8,  5,  0, 0, 0, 0, 0, 0 };
static const char N_V tomOpr[] =  {0,  4, 0, 15, 11, 0, 7,  5,  0, 0, 0, 0, 0, 0 };
static const char N_V cymbOpr[] = {0,  1, 0, 15, 11, 0, 5,  5,  0, 0, 0, 0, 0, 0 };
static const char N_V hhOpr[] =   {0,  1, 0, 15, 11, 0, 7,  5,  0, 0, 0, 0, 0, 0 };

static char N_V paramSlot [18] [nbLocParam];    /* all the parameters of slots...  */

static char	N_V amDepth;			/* chip global parameters .. */
static char	N_V vibDepth;			/* ... */
static char	N_V noteSel;			/* ... */
static char	N_V percussion;			/* percussion mode parameter */

/* Slot numbers as a function of the voice and the operator.
   (melodic mode only)
*/
unsigned char N_V slotMVoice [9]  [2] = {
	{0, 3},		/* voix 0 */
	{1, 4},		/* 1 */
	{2, 5},		/* 2 */
	{6, 9},		/* 3 */
	{7, 10},	/* 4 */
	{8, 11},	/* 5 */
	{12, 15},	/* 6 */
	{13, 16},	/* 7 */
	{14, 17}	/* 8 */
};


/* Slot numbers for the percussive voices.
   255 indicates that there is only one slot used by a voice.
*/
unsigned char N_V slotPVoice [11]  [2] = {
	{0, 3},		/* voice 0 */
	{1, 4},		/* 1 */
	{2, 5},		/* 2 */
	{6, 9},		/* 3 */
	{7, 10},	/* 4 */
	{8, 11},	/* 5 */
	{12, 15},	/* Bass Drum: slot 12 et 15 */
	{16, 255},	/* SD: slot 16 */
	{14, 255},	/* TOM: slot 14 */
	{17, 255},	/* TOP-CYM: slot 17 */
	{13, 255}	/* HH: slot 13 */
};

/* 
	This table gives the offset of each slot within the chip.
	offset = fn (slot)
*/
static const char N_V offsetSlot[] = {
	 0,  1,  2,  3,  4,  5,
	 8,  9, 10, 11, 12, 13,
	16, 17, 18, 19, 20, 21
};


/* This table indicates if the slot is a modulator (operator 0) or a
   carrier (operator 1).
   opr = fn (slot)
*/
static const char N_V carrierSlot[] = {
	0, 0, 0,		/* 1 2 3 */
	1, 1, 1,		/* 4 5 6 */
	0, 0, 0, 		/* 7 8 9 */
	1, 1, 1, 		/* 10 11 12 */
	0, 0, 0, 		/* 13 14 15 */
	1, 1, 1,		/* 16 17 18 */
};

/* This table gives the voice number associated with each slot.
   (melodic mode only)
   voice = fn (slot)
*/
static const char N_V voiceMSlot[] = {
	0, 1, 2,
	0, 1, 2,
	3, 4, 5,
	3, 4, 5,
	6, 7, 8,
	6, 7, 8,
};


/* This table gives the voice number  (0-10) associated with each
   slot (percussive mode only),
   voice = fn (slot)
*/
static const char N_V voicePSlot[] = {
	0, 1, 2,
	0, 1, 2,
	3, 4, 5,
	3, 4, 5,
	BD, HIHAT, TOM,
	BD, SD, CYMB
};


/*----------------------------------------------------------
   Function prototypes.
*/

extern void SndOutput(unsigned, unsigned);			/* in file OUTCHIP.ASM */
extern SetFreq();				/* in file SETFREQ.ASM */

static InitSlotParams();
static SetSlotParam (unsigned, unsigned *, unsigned);
static SndSetPrm (int, int);
static SndSetAllPrm (int);
static SndSKslLevel (int);
static SndSNoteSel();
static SndSFeedFm (int);
static SndSAttDecay (int);
static SndSSusRelease (int);
static SndSAVEK (int);
static SndSAmVibRhythm();
static SndWaveSelect (int);
static UpdateFNums (int);
static int BoardInstalled();


/*
----------------------------------------------------------
*/

/*
	Must be called for start-up initialisation.

	Returns 0 if hardware not found.
*/
int SoundColdInit( unsigned port )
	/* io port address of sound board (0x388) */
	{
	int hardware;

	genAddr = port;
	hardware =  BoardInstalled();
	SoundWarmInit();
	return hardware;
	}   /* SoundColdInit() */


/*
-----------------------------------------------
	Initialize the chip in melodic mode (mode == 0),
	set all 9 voices to electric-piano timbres,
	set the 3 global parameters to zero,
	set the pitch bend range to 1 half-tone,
	set the pitch bend of each voice to 0x2000 (no detune),
	set the volume of each voice to maximum level,
	and enable the wave-select parameter.
-----------------------------------------------
*/
void SoundWarmInit( void )
	{
	int i;

	for (i = 1; i <= 0xF5; i++)
		SndOutput (i, 0);   /* clear all registers */
	SndOutput (4, 6);       /* mask T1 & T2 */

	for (i = 0; i < 9; i++)  {      /* pitch bend for each voice = no detune */
		vPitchBend [i] = MID_PITCH;
		voiceKeyOn [i] = 0;
		voiceNote [i] = 0;
		}

	for (i = 0; i < 11; i++)
		lVoiceVolume [i] = MAX_VOLUME;

	SetMode (0);                /* melodic mode */
	SetGParam (0, 0, 0);        /* init global parameters */
	SetPitchRange (1);          /* default pitch range is 1 half-tone */
	SetWaveSel (1);
	}   /* SoundWarmInit() */



/*
---------------------------------------------
	Put the chip in melodic mode (mode == 0),
	or in percussive mode  (mode != 0).

	If the melodic mode is chosen, all voices are
	set to electric-piano, else the first 5 are set
	to electric-piano, and the percussion voices
	to their default timbres.
---------------------------------------------
*/
void SetMode( int mode )
	{

	if (mode){
		/* set the frequency for the last 4 percussion voices: */
		voiceNote [TOM] = TOM_PITCH;
		vPitchBend [TOM] = MID_PITCH;
		UpdateFNums (TOM);

		voiceNote [SD] = SD_PITCH;
		vPitchBend [SD] = MID_PITCH;
		UpdateFNums (SD);
		}
	percussion = mode;
	modeVoices = mode ? 11 : 9;
	percBits = 0;

	InitSlotParams();
	SndSAmVibRhythm();
	}   /* SetMode() */




/*
	Enable (state != 0) / disable (state == 0)
	the wave-select parameters.

	If you do not want to use the wave-select parameters, call
	this function with a value of 0 AFTER calling SoundColdInit()
	or SoundWarmInit().
*/
SetWaveSel (state)
	{
	int i;

	modeWaveSel = state ? 0x20 : 0;
	for (i = 0; i < 18; i++)
		SndOutput (0xE0 + offsetSlot [i], 0);
	SndOutput (1, modeWaveSel);
	}   /* SetWaveSel() */



/*
	Routine to change the pitch bend range. The value can be from
	1 to 12 (in half-tones).

	For example, the value 12 means that the pitch bend will 
	range from -12 (pitchBend == 0, see function 'SetVoicePitch()')
	to +12 (pitchBend == 0x3fff) half-tones.

	The change will be effective as of the next call to
	'SetVoicePitch()'.
*/
void SetPitchRange( unsigned pR )
	{
	if (pR > 12)
		pR = 12;
	if (pR < 1)
		pR = 1;
	pitchRange = pR;
	}   /* SetPitchRange() */


/*
----------------------------------------------
	Set the 3 global parameters AmDepth,
	VibDepth & NoteSel

	The change takes place immediately.
----------------------------------------------
*/
void SetGParam( int amD, int vibD, int nSel )
	{
	amDepth = amD;
	vibDepth = vibD;
	noteSel = nSel;

	SndSAmVibRhythm();
	SndSNoteSel();
	}   /* SetGParam() */





/*
-------------------------------------------------
	Set the parameters of the voice 'voice'.

	In melodic mode, 'voice' varies from 0 to 8.
	In percussive mode, voices 0 to 5 are melodic
	and 6 to 10 are percussive.

	A timbre (melodic or percussive) is defined as follows:
	the 13 first parameters of operator 0  (ksl, multi, feedBack,
	attack, sustain, eg-typem decay, release, level, am, vib, ksr, fm)
	followed by the 13 parameters of operator 1 (if a percussive voice, all
	the parameters are zero), followed by the wave-select parameter for
	the operators 0 and 1.

	'paramArray' is structured as follows:
		struct {
			int opr0Prm [13];       first 13 parameters
			int opr1Prm [13];       must be 0 if percussive timbre
			int	opr0WaveSel;		last parameter
			int opr1WaveSel;		must be 0 if percussive timbre
		} TimbreDef;

	The old timbre files (*.INS) do not contain the parameters
	'opr0WaveSel' and 'opr1WaveSel'.
	Set these two parameters to zero if you are using the old file
	format.
------------------------------------------------
*/
void SetVoiceTimbre( unsigned voice, unsigned*  paramArray )
	{
	unsigned wave0, wave1;
	unsigned * prm1, * wavePtr;
	unsigned char * slots;

	if (voice >= modeVoices)
		return;

	wavePtr = paramArray + 2 *  (nbLocParam -1);
	wave0 = * wavePtr++;
	wave1 = * wavePtr;
	prm1 = paramArray + nbLocParam -1;

	if (percussion)
		slots = slotPVoice [voice];
	else
		slots = slotMVoice [voice];
	SetSlotParam (slots [0], paramArray, wave0);
	if (slots [1] != 255)
		SetSlotParam (slots [1], prm1, wave1);
	}   /* SetVoiceTimbre() */




/*
--------------------------------------------------
	Set the volume of the voice 'voice' to 'volume'.

	The resulting output level is (timbreVolume * volume / 127).
	The change takes place immediately.

	0 <= volume <= 127
--------------------------------------------------
*/
void SetVoiceVolume (unsigned voice, unsigned volume)			/* 0 - 0x7f */
{
	unsigned char * slots;

	if (voice >= modeVoices)
		return;
	if (volume > MAX_VOLUME)
		volume = MAX_VOLUME;
	lVoiceVolume [voice] = volume;
	if (percussion)
		slots = slotPVoice [voice];
	else
		slots = slotMVoice [voice];
	SndSKslLevel (slots [0]);
	if (255 != slots [1])
		SndSKslLevel (slots [1]);
}   /* SetVoiceVolume() */


/*
-------------------------------------------------
	Change the pitch value of a voice.

	The variation in pitch is a function of the previous call
	to 'SetPitchRange()' and the value of 'pitchBend'.
	A value of 0 means -half-tone * pitchRange,
	0x2000 means no variation (exact pitch) and
	0x3fff means +half-tone * pitchRange.

	Does not affect the percussive voices, except for the bass drum.

	The change takes place immediately.

	0 <= pitchBend <= 0x3fff, 0x2000 == exact tuning
-------------------------------------------------
*/
void SetVoicePitch (unsigned voice, unsigned pitchBend)
{
	if ((!percussion && voice < 9) || voice <= BD) {
		/* melodic + bass-drum */
		if (pitchBend > MAX_PITCH)
			pitchBend = MAX_PITCH;
		vPitchBend [voice] = pitchBend;
		UpdateFNums (voice);
		}
}   /* SetVoicePitch() */



/*
-----------------------------------------------------------
	Routine to start a note playing.

	0 <= voice <= 8	in melodic mode,
	0 <= voice <= 10 in percussive mode;
	0 <= pitch <= 127, 60 == MID_C  (the card can play between 12 and 107 )
-----------------------------------------------------------
*/
void NoteOn (unsigned voice, int pitch)			/* 0 - 127 */
	{
	pitch -=  (MID_C - CHIP_MID_C);
	if (pitch < 0)
		pitch = 0;

	if ((!percussion && voice < 9) || voice < BD) {
		/* this is a melodic voice */
		voiceNote [voice] = pitch;
		voiceKeyOn [voice] = 0x20;
		UpdateFNums (voice);
		}
	else if (percussion && voice <= HIHAT) {
		/* this is a percussive voice */
		if (voice == BD) {
			voiceNote [BD] = pitch;
			UpdateFNums (voice);
			}
		else if (voice == TOM) {
			/* for the last 4 percussions, only the TOM may change in frequency,
				modifying the three others: */
			if (voiceNote [TOM] != pitch) {
				voiceNote [TOM] = pitch;
				voiceNote [SD] = pitch +TOM_TO_SD;
				UpdateFNums (TOM);
				UpdateFNums (SD);
				}
			}
		percBits |= percMasks [voice - BD];
		SndSAmVibRhythm();
		}
	}   /* NoteOn() */


/*
	Routine to stop playing the note which was started in 'NoteOn()'.

	0 <= voice <= 8	in melodic mode,
	0 <= voice <= 10 in percussive mode;
*/
void NoteOff (unsigned voice)
	{
	if ((!percussion && voice < 9) || voice < BD) {
		voiceKeyOn [voice] = 0;
		bxRegister [voice] &= ~0x20;
		SndOutput (0xB0 +voice, bxRegister [voice]);
		}
	else if (percussion && voice <= HIHAT) {
		percBits &= ~percMasks [voice - BD];
		SndSAmVibRhythm();
		}
	}   /* NoteOff() */





/*
------------------------------------------------------------------------
	static functions ...
------------------------------------------------------------------------
*/


/*
	In melodic mode, initialize all voices to electric-pianos.

	In percussive mode, initialize the first 6 voices to electric-pianos
	and the percussive voices to their default timbres.
*/
static InitSlotParams()
{
	int i;
	
	for (i = 0; i < 18; i++)
		if (carrierSlot [i])
			SetCharSlotParam (i, pianoParamsOp1, 0);
		else
			SetCharSlotParam (i, pianoParamsOp0, 0);
	if (percussion) {
		SetCharSlotParam (12, bdOpr0, 0);
		SetCharSlotParam (15, bdOpr1, 0);
		SetCharSlotParam (16, sdOpr, 0);
		SetCharSlotParam (14, tomOpr, 0);
		SetCharSlotParam (17, cymbOpr, 0);
		SetCharSlotParam (13, hhOpr, 0);
		}
}	/* InitSlotParams() */




/*
	Used to change the parameter 'param' of the slot 'slot'
	with the value 'val'. The chip registers are updated.
*/
SetASlotParam (slot, param, val)
	int slot, val;
	int param;		/* parameter number */
{
	paramSlot [slot] [param] = val;
	SndSetPrm (slot, param);
}	/* SetASlotParam() */

	





/*
------------------------------------------------------
	Set the 14 parameters  (13 in 'param', 1 in 'waveSel')
	of slot 'slot'. Update the parameter array and the chip.
------------------------------------------------------
*/
static SetSlotParam (slot, param, waveSel)
	unsigned slot, * param, waveSel;
{
	int i, k;
	char * ptr;

	for (i = 0, ptr = &paramSlot [slot] [0]; i < nbLocParam -1; i++)
		*ptr++ = *param++;
	*ptr = waveSel &= 0x3;
	SndSetAllPrm (slot);
}	/* SetSlotParam() */

SetCharSlotParam (slot, cParam, waveSel)
	unsigned slot, waveSel;
	unsigned char * cParam;
	{
	unsigned param [nbLocParam];
	int i;

	for (i = 0; i < nbLocParam -1; i++)
		param [i] = *cParam++;
	SetSlotParam (slot, param, waveSel);
	}	/* SetCharSlotParam() */


/*
-----------------------------------------------
	Update the parameter 'prm' for the slot 'slot'.
	Update the chip registers.
-----------------------------------------------
*/
static SndSetPrm (slot, prm)
	int slot, prm;
{

	switch (prm) {
		case prmPercussion:
		case prmAmDepth:
		case prmVibDepth:
			SndSAmVibRhythm();
			break;

		case prmNoteSel:
			SndSNoteSel();
			break;

		case prmKsl:
		case prmLevel:
			SndSKslLevel (slot);
			break;

		case prmFm:
		case prmFeedBack:
			SndSFeedFm (slot);
			break;

		case prmAttack:
		case prmDecay:
			SndSAttDecay (slot);
			break;

		case prmRelease:
		case prmSustain:
			SndSSusRelease (slot);
			break;

		case prmMulti:
		case prmVib:
		case prmStaining:
		case prmKsr:
		case prmAm:
			SndSAVEK (slot);
			break;

		case prmWaveSel:
			SndWaveSelect (slot);
			break;
		}
}	/* SndSetPrm() */



/*-------------------------------------------------
	Transfer all the parameters from slot 'slot' to
	the chip.
*/
static SndSetAllPrm (slot)
{
	SndSAmVibRhythm();
	SndSNoteSel();
	SndSKslLevel (slot);
	SndSFeedFm (slot);
	SndSAttDecay (slot);
	SndSSusRelease (slot);
	SndSAVEK (slot);
	SndWaveSelect (slot);
}	/* SndSetAllPrm() */
	

/*
	Write to the register which controls output level and does
	key-on/key-offs for the percussive voice slots.
*/
static SndSKslLevel (slot)
{
	unsigned t1, vc, singleSlot;

	if (percussion)
		vc = voicePSlot [slot];
	else
		vc = voiceMSlot [slot];

	t1 = 63 - (GetLocPrm (slot, prmLevel) & 63);    /* amplitude */
	singleSlot = percussion && vc > BD;

	if  ((carrierSlot [slot] || !GetLocPrm (slot, prmFm) || singleSlot))
		/* Change the 0 - 127 volume change value to 0 - 63 for the chip.
		   (MAX_VOLUME+1)/2 is added to avoid round-off errors. */
		t1 = (t1 * lVoiceVolume [vc] + (MAX_VOLUME+1)/2 ) >> LOG2_VOLUME;

	t1 = 63 - t1;
	t1 |= GetLocPrm (slot, prmKsl) << 6;
	SndOutput (0x40 + (int)offsetSlot [slot], t1);
}



/* --------------------------------------------
	Write to the register which controls the note select parameter.
*/
static SndSNoteSel()
{
	SndOutput (0x08, noteSel ? 64 : 0);
}	/* SndSNoteSel() */



/* --------------------------------------------
	FEED-BACK and FM (connection).
	Applicable only to operator 0 in melodic mode.
*/
static SndSFeedFm (slot)
{
	unsigned t1;

	if (carrierSlot [slot])
		return;
	t1 = GetLocPrm (slot, prmFeedBack) << 1;
	t1 |= GetLocPrm (slot, prmFm) ? 0 : 1;
	SndOutput (0xC0 + (int)voiceMSlot [slot], t1);
}


/*
	ATTACK, DECAY
*/
static SndSAttDecay (slot)
{
	unsigned t1;

	t1 = GetLocPrm (slot, prmAttack) << 4;
	t1 |= GetLocPrm (slot, prmDecay) & 0xf;
	SndOutput (0x60 + (int)offsetSlot [slot], t1);
}


/*
	SUSTAIN, RELEASE
*/
static SndSSusRelease (slot)
{
	unsigned t1;

	t1 = GetLocPrm (slot, prmSustain) << 4;
	t1 |= GetLocPrm (slot, prmRelease) & 0xf;
	SndOutput (0x80 + (int)offsetSlot [slot], t1);
}



/*
	AM, VIB, EG-TYP (Sustaining), KSR, MULTI
*/
static SndSAVEK (slot)
{
	unsigned t1;

	t1 = GetLocPrm (slot, prmAm) ? 0x80 : 0;
	t1 += GetLocPrm (slot, prmVib) ? 0x40 : 0;
	t1 += GetLocPrm (slot, prmStaining) ? 0x20 : 0;
	t1 += GetLocPrm (slot, prmKsr) ? 0x10 : 0;
	t1 += GetLocPrm (slot, prmMulti) & 0xf;
	SndOutput (0x20 + (int)offsetSlot [slot], t1);
}	/* SndSAVEK() */


/*
	Set the values: AM Depth, VIB depth & Rhythm (melo/perc mode)
*/
static SndSAmVibRhythm()
{
	unsigned t1;

	t1 = amDepth ? 0x80 : 0;
	t1 |= vibDepth ? 0x40 : 0;
	t1 |= percussion ? 0x20 : 0;
	t1 |= percBits;
	SndOutput (0xBD, t1);
}


/*
	Set the wave-select parameter.
*/
static SndWaveSelect (slot)
	{
	unsigned wave;

	if (modeWaveSel)
		wave = GetLocPrm (slot, prmWaveSel) & 0x03;
	else
		wave = 0;
	SndOutput (0xE0 + offsetSlot [slot], wave);
	}	/* SndWaveSelect() */


/*
	Change pitch, pitchBend & keyOn of voices 0 to 8, for melodic
	or percussive mode.
*/
static UpdateFNums (voice)
	{
	bxRegister [voice] = SetFreq (voice, voiceNote [voice],
								  vPitchBend [voice], voiceKeyOn [voice]);
	}



/*
	Return 0 if board is not installed.  The chip's timers are used to
	determined if an Ad Lib card is present.  When being used, the timers
	place specific values in the status register.  If we do not read the
	correct values from the status register, then we can assume that no
	board is present.
*/
static int BoardInstalled()
	{
	unsigned t1, t2, i;

	SndOutput (4, 0x60);    /* mask T1 & T2 */
	SndOutput (4, 0x80);    /* reset IRQ */
	t1 = inportb(genAddr);     /* read status register */
	SndOutput (2, 0xff);    /* set timer-1 latch */
	SndOutput (4, 0x21);    /* unmask & start T1 */

	for (i = 0; i < 200; i++)   /* 100 uSec delay for timer-1 overflow */
		inportb (genAddr);

	t2 = inportb(genAddr);     /* read status register */
	SndOutput (4, 0x60);
	SndOutput (4, 0x80);

	return (t1 & 0xE0) == 0 && (t2 & 0xE0) == 0xC0;
	}

/*----------------------------------------------------------------------*/
#if 0

/* The following, OutFreq(), is provided as an alternative to SetFreq().
   You may find it easier to understand as SetFreq is written in assembler.
   However, SetFreq is faster so you should definitely use SetFreq. The
   main purpose of including OutFreq() is to show you how to set the
   f-number registers.
*/

static unsigned  freqNums [12] = {
/*  C    C#   D    D#   E    F */
   343, 363, 385, 408, 432, 458,
/*  F#   G    G#   A    A#   B */
   485, 514, 544, 577, 611, 647
};

unsigned  OutFreq (voice, pitch, bend, keyOn)
   int voice, pitch, bend, keyOn;
{
   unsigned effNbr, octave, t1;

   /* Integer division by 12 gives the octave (referred to as BLOCK
	  information in the manual). */
   octave = (pitch / 12) - 1;

   /* The remainder of dividing by 12 gives the half-tone within the octave.
	  The freqNums table gives the value to place in the register given the
	  half-tone. */
   effNbr = freqNums [pitch % 12];

   if (bend != 0x2000) {
	  /* Do a pitch bend.
		 pitchRange is the maximum interval for a pitch bend.
		 'bend' is the amount of pitch bend. */
	  long n;

	  /* Below, the maximum interval is calculated in terms of fnums, and
		 then a fraction of this is added to the base value (effNbr). */
	  if (bend > 0x2000) {
		 bend -= 0x2000;
		 n = freqNums [(pitch + pitchRange) % 12];
		 if (n < effNbr) n <<= 1;
		 n = n - effNbr;                        /* interval as f-num */
		 effNbr = effNbr + ((n * bend) >> 13);  /* >> 13 is div by 0x2000 */

		 /* If effNbr exceeds its maximum possible value, bring it into range
			by dividing by two, which lowers the pitch by an octave, and add
			1 to the octave to keep the pitch in the correct octave. */
		 while (effNbr > 1023) {
			effNbr >>= 1;
			octave++;
		 }
	  }
	  else {
		 bend = 0x2000 - bend;
		 n = freqNums [(pitch - pitchRange) % 12];
		 if (n > effNbr) n >>= 1;
		 n = effNbr - n;
		 effNbr = effNbr - ((n * bend) >> 13);

		 /* If effNbr exceeds its minimum desirable value, bring it into range
			by multiplying by two, which raises the pitch by an octave, and
			subtract 1 from the octave to keep the pitch in the correct octave. */
		 while (effNbr < freqNums [0]) {
			effNbr <<= 1;
			octave--;
		 }
	  }
   }

   /* Write the lower 8 bits of the f-number. */
   SndOutput (0xA0 + voice, effNbr);

   /* Arrange the key-on bit, the octave and the upper two bits of the
	  f-number in the correct order and write to the register. */
   t1 = keyOn | (octave << 2) | (effNbr >> 8);

   SndOutput (0xB0 + voice, t1);

   return (t1);
}

#endif

#define BACK_MUSIC_IDLE 0
#define BACK_MUSIC_BUSY 1
#define ADLIB_NOT_FOUND 2

unsigned char _playMusicFlag=BACK_MUSIC_IDLE ;

playMusic(fn)
char *fn ;
{
   int file ;
   long length,result ;
   int n;

   if (_playMusicFlag==ADLIB_NOT_FOUND) return(0)  ;
   if (_playMusicFlag==BACK_MUSIC_BUSY) offMusic() ;

   if (!SoundColdInit(0x388)) { _playMusicFlag=ADLIB_NOT_FOUND ; return(0) ; }

   file = open (fn, O_RDONLY + O_BINARY);
   if (file < 0) return (0);

   length = filelength (file) ;
   if (length > 65534L) { close(file) ; return(0) ; }

   buf = (UCHAR *) malloc (length) ;
   if (buf==NULL) { close(file) ; return(0) ; }

   result = read (file, buf, length) ;
   if (length != result) { free(buf) ; close(file) ; return(0) ; }
   close (file) ;

   Midi_init() ;
   Midi_play(buf) ;
   _playMusicFlag=BACK_MUSIC_BUSY ;
}

void offMusic(void)
{
   int i,n ;

   if (_playMusicFlag==ADLIB_NOT_FOUND) return(0) ;
   if (_playMusicFlag==BACK_MUSIC_IDLE) return(0) ;
   for ( n=0 ; n <MAX_VOICES ; n++ )
   {
      SetVoiceVolume (n, 0) ;
      NoteOff (n) ;
   }
   Stop_Melo() ; Midi_End () ;
   free (buf) ;
   _playMusicFlag=BACK_MUSIC_IDLE ;
}


