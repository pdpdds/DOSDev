/*

    ADLIB.C

    Sound Driver Low level function

*/

#pragma inline

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "adlib.h"

typedef struct
{
    unsigned char mode;
    unsigned char percVoice;
    unsigned char ins[28];
}   Inst_Buf_Rec;

static void SndOutput( unsigned char addr, unsigned char data );
static void SndOutput2( unsigned char addr, unsigned char data );
static void InitSlotVolume( void );
static void InitSlotParams( void );
static long CalcPremFNum( int numDeltaDemiTon, int denDeltaDemiTon );
static void SetFNum( unsigned *fNumVec, int num, int den );
static void InitFNums( void );
static void ChangePitch( unsigned voice, int pitchBend );
static void SetASlotParam( unsigned slot, int param, unsigned val );
static void SetSlotParam( unsigned slot, char *param, unsigned waveSel );
static void SetCharSlotParam( unsigned slot, char *cParam, unsigned waveSel );
static void SndSetPrm( int slot, int prm );
static void SndSetAllPrm( int slot );
static void SndSAmVibRhythm( void );
static void SndSKslLevel ( int slot );
static void SndSNoteSel( void );
static void SndSFeedFm( int slot );
static void SndSAttDecay( int slot );
static void SndSSusRelease( int slot );
static void SndSAVEK( int slot );
static void SndWaveSelect( int slot );
static void SetFreq( unsigned voice, int pitch, unsigned keyOn);
static void SoundChut( unsigned voice );
static int BoardInstalled( void );

/*
    LOW LEVEL ROUTINE FOR 2-OPERATOR STEREO FM ( YMF262-M )
    ¥aª¢∑e 3-OPL °°óaìe ¨a∂w–aª° ¥gìeîa.
*/

#define TOM_PITCH   24              /* best frequency, in range of 0 to 95 */
#define TOM_TO_SD   7               /* 7 half-tones between voice 7 & 8 */
#define SD_PITCH    (TOM_PITCH + TOM_TO_SD)

#define NR_STEP_PITCH 25            /* 25 steps within a half-tone for pitch bend */

#define GetLocPrm( slot, prm)( (unsigned)paramSlot[ slot][ prm])
#define HighByte( word) ( ((char *)(&word))[ 1])    /* 80x86-8 only .. */
#define RhythmMode()    (percussion)

typedef char    SLOT_PARAM;

// êÅ¶Å ¥bã°üi ∏·∏w–e âµ
extern  Inst_Buf_Rec ins_buf2[11];

// Adlib FM port address
static unsigned genAddr,genAddr2;

// »a¥bã°∑Å bit mask
static char percBits;
static char percMasks[] = {
    0x10, 0x08, 0x04, 0x02, 0x01
    };

char voiceNote[ 11];         /* pitch of last note-on of each voice */
char voiceKeyOn[ 11];            /* state of keyOn bit of each voice */

static char noteDIV12[ 96];         /* table of (0..95) DIV 12 */
static char noteMOD12[ 96];         /* table of (0..95) MOD 12 */

char lVoiceVolume [11];             /* volume for each of 11 logical voices */
char slotRelMax [ 11]={127,127,127,127,127,127,127,127,127,127,127};

unsigned modeVoices=11;     /* 9 or 11, depending on 'percussion'*/

static SLOT_PARAM paramSlot[ 18][ nbLocParam];  /* all the parameters of slots...  */

char amDepth=0;            /* chip global parameters .. */
char vibDepth=0;           /* ... */
char noteSel=0;            /* ... */
char percussion;         /* percussion mode parameter */
char fmstereo=0;

unsigned char slotMVoice [9]  [2] = {
    { 0,  3},     /* 0 */
    { 1,  4},     /* 1 */
    { 2,  5},     /* 2 */
    { 6,  9},     /* 3 */
    { 7, 10},     /* 4 */
    { 8, 11},     /* 5 */
    {12, 15},     /* 6 */
    {13, 16},     /* 7 */
    {14, 17}      /* 8 */
};


/*
   255 ìe –e¿ÅêÈµA –aêa∑Å Slot†e ¨a∂w–aìe àı∑i õu–eîa.
*/
unsigned char slotPVoice [11]  [2] = {
    {0, 3},     /* 0 */
    {1, 4},     /* 1 */
    {2, 5},     /* 2 */
    {6, 9},     /* 3 */
    {7, 10},    /* 4 */
    {8, 11},    /* 5 */
    {12, 15},   /* Bass Drum: slot 12 et 15 */
    {16, 255},  /* SD: slot 16 */           /*                          */
    {14, 255},  /* TOM: slot 14 */          /* They use only one slot.  */
    {17, 255},  /* TOP-CYM: slot 17 */      /*                          */
    {13, 255}   /* HH: slot 13 */           /*                          */
};

/*
    àbàb∑Å SlotµA îÅ–e ¨a∂Öóa √≥êÅ∑Å Offset table.
    °°ñÅ 18àÅ∑°îa.
*/
static const char offsetSlot[] = {
     0,  1,  2,  3,  4,  5,
     8,  9, 10, 11, 12, 13,
    16, 17, 18, 19, 20, 21
};


/* This table indicates if the slot is a modulator (operator 0) or a
   carrier (operator 1).
   opr = fn (slot)
*/
static const char carrierSlot[] = {
    0, 0, 0,        /* 1 2 3 */
    1, 1, 1,        /* 4 5 6 */
    0, 0, 0,        /* 7 8 9 */
    1, 1, 1,        /* 10 11 12 */
    0, 0, 0,        /* 13 14 15 */
    1, 1, 1,        /* 16 17 18 */
};

/* This table gives the voice number associated with each slot.
   (melodic mode only)
   voice = fn (slot)
*/
static const char voiceMSlot[] = {
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
static const char voicePSlot[] = {
    0, 1, 2,
    0, 1, 2,
    3, 4, 5,
    3, 4, 5,
    BD, HIHAT, TOM,
    BD, SD, CYMB
};


// ≠¢ï°üi ∂·–Å ∫ÅÃaÆÅüi £°ü° âÅ¨e–Å ê˝ìe âµ
static unsigned fNumNotes[ NR_STEP_PITCH][ 12];
static int  halfToneOffset[ 11];
static unsigned * fNumFreqPtr[ 11];

static int  pitchRange;         /* pitch variation, half-tone[+1,+12] */
static int  pitchRangeStep;     /* == pitchRange * NR_STEP_PITCH */
static int  modeWaveSel;        /* != 0 if used with the 'wave-select' parameters */


static void SndOutput( addr, data )
    unsigned char addr,data;
{
    asm    push dx
    asm    push ax
    asm    mov dx,genAddr
    asm    mov al,addr
    asm    out dx,al
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx

    asm    inc dx
    asm    mov al,data
    asm    out dx,al
    asm    dec dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx

    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx

    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx

    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx
    asm    in  al,dx

    asm    pop ax
    asm    pop dx
}

/*
    Must be called for start-up initialisation.

    Return 0 if harware not found.
*/
int SoundColdInit( port)
    unsigned port;          /* io port address of sound board (0x388) */
    {
    int hardware;

    genAddr = port;
    genAddr2 = port+2;
    hardware =  BoardInstalled();
    SoundWarmInit();
    return hardware;
    }   /* SoundColdInit() */


/*
    Set the volume of all slots.
*/
static void InitSlotVolume()
{
    int i;

    for( i = 0; i < 11; i++)
    {
        lVoiceVolume[ i] = slotRelMax[ i];
    }
}   /* InitSlotVolume() */

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

void SoundWarmInit()
    {
    int i;

    for (i = 1; i <= 0xF5; i++)
    {
        SndOutput (i, 0);   /* clear all registers */
    }
    SndOutput (4, 0x60);       /* mask T1 & T2 */
    for(i=0;i<11;i++)
    {
        voiceNote[i]=0;
        voiceKeyOn[i]=0;
    }
    InitSlotVolume();
    InitFNums();

    SetMode( 0 );                /* melodic mode */
    SetGParam( 0, 0, 0 );        /* init global parameters */
    SetPitchRange( 1 );          /* default pitch range is 1 half-tone */
    SetWaveformSel( 1 );                /* Set Wave select YMF262-MµA¨·ï° ≥a∑•îa.  */
    }   /* SoundWarmInit() */


/*
---------------------------------------------
    Put the chip in melodic mode (mode == 0),
    or in percussive mode ( mode != 0).

    If the melodic mode is chosen, all voices are
    set to electric-piano, else the first 5 are set
    to electric-piano, and the percussion voices
    to their default timbres.
---------------------------------------------
*/
void SetMode( mode)
    int mode;
    {
    int voice;

    modeVoices = mode ? 11 : 9;
    percBits = 0;
    percussion = mode;
    SndSAmVibRhythm();
    if( mode){

        /* set the frequency for the last 4 percussion voices: */
            for( voice=0;voice<modeVoices;voice++)
            {
                SetVoiceVolume( voice, 0 );
            }
            SetFreq( TOM, TOM_PITCH, 0);
            SetFreq( SD, SD_PITCH, 0);
        }
        InitSlotParams();
    }   /* SetMode() */




/*
    Enable (state != 0) / disable (state == 0)
    the wave-select parameters.

    If you do not want to use the wave-select parameters, call
    this function with a value of 0 AFTER calling SoundColdInit()
    or SoundWarmInit().
*/
void SetWaveformSel( state)
    int state;
    {
    int i;

    modeWaveSel = state ? 0x20 : 0;
/*  Enable Wave Mode */
    SndOutput( 1, modeWaveSel);
    SndOutput( 4, 0xE0 );
    for( i = 0; i < 18; i++)
    {
        SndOutput( 0xE0 + offsetSlot[ i], 0);
    }
    }   /* SetWaveformSel() */



/*
    Routine to change the pitch bend range. The value can be from
    1 to 12 (in half-tones).

    For example, the value 12 means that the pitch bend will
    range from -12 (pitchBend == 0, see function 'SetVoicePitch()')
    to +12 (pitchBend == 0x3fff) half-tones.

    The change will be effective as of the next call to
    'SetVoicePitch()'.
*/
void SetPitchRange( pR)
    unsigned pR;
    {
    if( pR > 12)
        pR = 12;
    if( pR < 1)
        pR = 1;
    pitchRange = pR;
    pitchRangeStep = pitchRange * NR_STEP_PITCH;
    }   /* SetPitchRange() */


/*
----------------------------------------------
    Set the 3 global parameters AmDepth,
    VibDepth & NoteSel

    The change takes place immediately.
----------------------------------------------
*/
void SetGParam( amD, vibD, nSel)
    int amD, vibD, nSel;
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
    the 13 first parameters of operator 0 ( ksl, multi, feedBack,
    attack, sustain, eg-typem decay, release, level, am, vib, ksr, fm)
    followed by the 13 parameters of operator 1 (if a percussive voice, all
    the parameters are zero), followed by the wave-select parameter for
    the operators 0 and 1.

    'paramArray' is structured as follows:
        struct {
            int opr0Prm[ 13];       first 13 parameters
            int opr1Prm[ 13];       last  13 parameter
            int opr0WaveSel;        must be 0 if percussive timbre
            int opr1WaveSel;        must be 0 if percussive timbre
        } TimbreDef;

    The old timbre files (*.INS) do not contain the parameters
    'opr0WaveSel' and 'opr1WaveSel'.
    Set these two parameters to zero if you are using the old file
    format.
-------------------------------------------------
*/
void SetVoiceTimbre (voice, paramArray)
    unsigned voice;
    char * paramArray;
    {
    unsigned wave0, wave1;
    char * prm1, * wavePtr;
    unsigned char * slots;

    if (voice >= modeVoices)
        return;

    wavePtr = paramArray + 2 *  (nbLocParam -1);
    wave0 = (unsigned ) * wavePtr++;
    wave1 = (unsigned ) * wavePtr;
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
void SetVoiceVolume (voice, volume)
    unsigned voice, volume;         /* 0 - 0x7f */
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
void SetVoicePitch( voice, pitchBend)
    unsigned voice;
    unsigned pitchBend;
{
    if( ! RhythmMode() || voice <= BD) {
        /* melodic + bass-drum */
        if( pitchBend > MAX_PITCH)
            pitchBend = MAX_PITCH;
        ChangePitch( voice, pitchBend);
        SetFreq( voice, voiceNote[ voice], voiceKeyOn[ voice]);
        }
}   /* SetVoicePitch() */



/*
-----------------------------------------------------------
    Routine to start a note playing.

    0 <= voice <= 8 in melodic mode,
    0 <= voice <= 10 in percussive mode;
    0 <= pitch <= 127, 60 == MID_C ( the card can play between 12 and 107 )
-----------------------------------------------------------
*/
void NoteOn( voice, pitch)
    unsigned voice;
    int pitch;          /* 0 - 127 */
    {
    pitch -= ( MID_C - CHIP_MID_C);
    if( pitch < 0)
        pitch = 0;
    voiceKeyOn[ voice] = 1;
    if( !RhythmMode() || voice < BD )
        /* this is a melodic voice */
        SetFreq( voice, pitch, 1);
    else {
        /* this is a percussive voice */
        if( voice == BD)
            SetFreq( BD, pitch, 0);
        else if( voice == TOM) {
            /* for the last 4 percussions, only the TOM may change in frequency,
                modifying the three others: */
            SetFreq( TOM, pitch, 0);
            SetFreq( SD, pitch + TOM_TO_SD, 0); /* f7 = 3 * f8 */
            }
            else
            if( voice != SD )
                voiceNote[ voice ] = pitch;
        percBits |= percMasks[ voice - BD];
        SndSAmVibRhythm();
        }
    }   /* NoteOn() */

/*
    Routine to stop playing the note which was started in 'NoteOn()'.

    0 <= voice <= 8 in melodic mode,
    0 <= voice <= 10 in percussive mode;
*/
void NoteOff( voice)
    unsigned voice;
    {
    voiceKeyOn[ voice] = 0;
    if( !RhythmMode() || voice < BD)
    {
        SetFreq( voice, voiceNote[ voice] , 0); /* shut off */
    }
    else {
        percBits &= ~percMasks[ voice - BD];
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
/*

static void InitSlotParams()
{
    int i;

    for( i = 0; i < 12; i++)
        if( carrierSlot[ i])
            SetSlotParam( i, pianoParamsOp1, 0);
        else
            SetSlotParam( i, pianoParamsOp0, 0);
    if( RhythmMode()) {
        SetSlotParam( 12, bdOpr0, 0);
        SetSlotParam( 15, bdOpr1, 0);
        SetSlotParam( 16, sdOpr, 0);
        SetSlotParam( 14, tomOpr, 0);
        SetSlotParam( 17, cymbOpr, 0);
        SetSlotParam( 13, hhOpr, 0);
        }
    else
    for( i = 12; i < 18; i++)
        if( carrierSlot[ i])
            SetSlotParam( i, pianoParamsOp1, 0);
        else
            SetSlotParam( i, pianoParamsOp0, 0);
}
*/

/*
    ∂A¶ÅµA¨· –Å∫Öîa.
*/
static void InitSlotParams()
{
/*

    int i;

    for( i = 0; i < 12; i++)
        if( carrierSlot[ i])
            SetSlotParam( i, &(ins_buf2[4].ins[14]), 0);
        else
            SetSlotParam( i, &(ins_buf2[4].ins[0]), 0);
    if( RhythmMode()) {
        SetSlotParam( 12, &(ins_buf2[6].ins[0]), 0);
        SetSlotParam( 15, &(ins_buf2[6].ins[14]), 0);
        SetSlotParam( 16, &(ins_buf2[7].ins[0]), 0);
        SetSlotParam( 14, &(ins_buf2[8].ins[0]), 0);
        SetSlotParam( 17, &(ins_buf2[9].ins[0]), 0);
        SetSlotParam( 13, &(ins_buf2[10].ins[0]), 0);
        }
    else
    {
    for( i = 12; i < 18; i++)
        if( carrierSlot[ i])
            SetSlotParam( i, &(ins_buf2[4].ins[14]), 0);
        else
            SetSlotParam( i, &(ins_buf2[4].ins[0]), 0);
    }
*/
}



/*
    Return binary value of the frequency 260.44 ( C)
    shifted by +/- numdeltaDemiTon/denDeltaDemiTon multiplied by 8.

    If the numerator (numDeltaDemiTon) is positive, the frequency is
    increased; if negative, it is decreased.

    Fo = Fb( 1 + 0.06 num /den)
    Fnum8 = Fo * 65536 * 72 / 3.58e6

    -100 <= numDeltaDemiTon <= +100
    1 <= denDeltaDemiTon <= 100
*/
static long CalcPremFNum( numDeltaDemiTon, denDeltaDemiTon)
{
    long    f8, fNum8, d100;

    d100 = denDeltaDemiTon * 100L;
    f8 = ( d100 + 6L * numDeltaDemiTon) * (26044L * 2L); /* 260.44 * 100 * 2 */
    f8 /= d100 * 25L;
    fNum8 = f8 * 16384L * 9L ;     /*( 16384L * 9L);   */

    fNum8 /= 179L * 625L;
    return fNum8;
}   /* CalcPremFNum() */


/*
    Initialize a line in the frequency table with shifted frequency values.
    The values are shifted a fraction (num/den) of a half-tone.
    See following routine.
*/
static void SetFNum( fNumVec, num, den)
    unsigned * fNumVec;
{
    int i;
    long val;

    *fNumVec++ = (unsigned)(4 +(val = CalcPremFNum( num, den))) >> 3;
    for ( i = 1; i < 12; i++) {
        /* 1.06 is 2^(1/12)   , 1.06^12 = 2 */
        val *= 106;
        *fNumVec++ = (unsigned)(4 +(val /= 100)) >> 3;
        }
}   /* SetFNum() */


/*
    Initialize all lines of the frequency table. Each line represents
    12 half-tones shifted by (n/NR_STEP_PITCH), where 'n' is the line number
    and ranges from 1 to NR_STEP_PITCH.
*/
static void InitFNums()
{
    unsigned i, j, k, num, numStep, pas;

    numStep = 100 / NR_STEP_PITCH;
    for( num = pas = 0; pas < NR_STEP_PITCH; pas++, num += numStep)
        SetFNum( fNumNotes[ pas], num, 100);
    for( i = 0; i < 11; i++) {
        fNumFreqPtr[ i] = (unsigned *) fNumNotes[ 0];
        halfToneOffset[ i] = 0;
        }

/*
  ‘≥  ‘≥1 ‘≥‘≥3 ‘≥  ‘≥  ‘≥6 ‘≥‘≥8 ‘≥‘≥10‘≥  ‘≥
  ‘≥  ‘¿‘¬‘Ÿ‘¿‘¬‘Ÿ  ‘≥  ‘¿‘¬‘Ÿ‘¿‘¬‘Ÿ‘¿‘¬‘Ÿ  ‘≥
  ‘≥ 0  ‘≥ 2  ‘≥ 4  ‘≥ 5  ‘≥ 7  ‘≥ 9  ‘≥ 11 ‘≥
  ‘¿‘ƒ‘ƒ‘¡‘ƒ‘ƒ‘¡‘ƒ‘ƒ‘¡‘ƒ‘ƒ‘¡‘ƒ‘ƒ‘¡‘ƒ‘ƒ‘¡‘ƒ‘ƒ‘Ÿ
     00 01 02 03 04    05 06 07 08 09 10 11
*/

    k = 0;
    for( i = 0; i < 8; i++)
        for( j = 0; j < 12; j++, k++) {
            noteDIV12[ k] = i;
            noteMOD12[ k] = j;
            }
}   /* InitFNums() */


/*
    Routine to set 'halfToneOffset[]' & 'fNumFreqPtr[]'.
    These two global variables are used to determine the frequency
    variation to use when a note is played.

    0 <= pitchBend <= 3fffH
*/
static void ChangePitch( voice, pitchBend)
    unsigned voice;
    int pitchBend;      /* 0 - 3fffH, 2000H == exact tuning */
    {
    int t1, t2, delta;
    long l;
    static long oldL = ~0;
    static int oldHt;
    static unsigned * oldPtr;

    l = (long)(pitchBend - MID_PITCH) * pitchRangeStep;
    if( oldL == l) {    /* optimisation ... */
        fNumFreqPtr[ voice] = oldPtr;
        halfToneOffset[ voice] = oldHt;
        }
    else {
        t1 = l / MID_PITCH;
        if( t1 < 0) {
            t2 = NR_STEP_PITCH -1 -t1;
            oldHt = halfToneOffset[ voice] = -(t2 / NR_STEP_PITCH);
            delta = (t2 - NR_STEP_PITCH +1) % NR_STEP_PITCH;
            if( delta)
                delta = NR_STEP_PITCH - delta;
            }
        else {
            oldHt = halfToneOffset[ voice] = t1 / NR_STEP_PITCH;
            delta = t1 % NR_STEP_PITCH;
            }
        oldPtr = fNumFreqPtr[ voice] = (unsigned *) fNumNotes[ delta];
        oldL = l;
        }
    }   /* ChangePitch() */



/*
    Used to change the parameter 'param' of the slot 'slot'
    with the value 'val'. The chip registers are updated.
*/
/*
static void SetASlotParam( slot, param, val)
    unsigned slot, val;
    int param;
{
    paramSlot[ slot][ param] = val;
    SndSetPrm( slot, param);
}   // SetASlotParam()
*/


/*
------------------------------------------------------
    Set the 14 parameters ( 13 in 'param', 1 in 'waveSel')
    of slot 'slot'. Update the parameter array and the chip.
------------------------------------------------------
*/
static void SetSlotParam( slot, param, waveSel)
    unsigned slot, waveSel;
    char * param;
{
    int i;
    SLOT_PARAM * ptr;

    for( i = 0, ptr = &paramSlot[ slot][ 0]; i < nbLocParam -1; i++)
        *ptr++ = *param++;
    *ptr = waveSel &= 0x3;
    SndSetAllPrm( slot);
}   /* SetSlotParam() */

/*
static void SetCharSlotParam( slot, cParam, waveSel)
    unsigned slot, waveSel;
    char * cParam;
    {
    unsigned param[ nbLocParam];
    int i;

    for( i = 0; i < nbLocParam -1; i++)
        param[ i] = *cParam++;
    SetSlotParam( slot, param, waveSel);
    }   // SetCharSlotParam()
*/

/*
-----------------------------------------------
    Update the parameter 'prm' for the slot 'slot'.
    Update the chip registers.
-----------------------------------------------
*/
static void SndSetPrm( slot, prm)
    int slot, prm;
{

    switch( prm) {
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
            SndSKslLevel( slot);
            break;

        case prmFm:
        case prmFeedBack:
            SndSFeedFm( slot);
            break;

        case prmAttack:
        case prmDecay:
            SndSAttDecay( slot);
            break;

        case prmRelease:
        case prmSustain:
            SndSSusRelease( slot);
            break;

        case prmMulti:
        case prmVib:
        case prmStaining:
        case prmKsr:
        case prmAm:
            SndSAVEK( slot);
            break;

        case prmWaveSel:
            SndWaveSelect( slot);
            break;
        }
}   /* SndSetPrm() */



/*-------------------------------------------------
    Transfer all the parameters from slot 'slot' to
    the chip.
*/
static void SndSetAllPrm( slot)
    int slot;
{
    SndWaveSelect( slot);
    SndSAttDecay( slot);
    SndSSusRelease( slot);
    SndSAmVibRhythm();
    SndSNoteSel();
    SndSKslLevel( slot);
    SndSFeedFm( slot);
    SndSAVEK( slot);
}   /* SndSetAllPrm() */

/*
    Set the values: AM Depth, VIB depth & Rhythm
    ?????????
*/
static void SndSAmVibRhythm()
{
    unsigned t1;

    t1 = amDepth ? 0x80 : 0;
    t1 |= vibDepth ? 0x40 : 0;
    t1 |= RhythmMode() ? 0x20 : 0;
    t1 |= percBits ; /* êÅàa ê˝∑e àı */
    SndOutput( 0xBD, t1);
}

/*
    Write to the register which controls output level and does
    key-on/key-offs for the percussive voice slots.

    KSL ( Key Scale Level )

*/
static void SndSKslLevel (slot)
    int slot;
{
    unsigned t1,tl,tr, tc, vc, singleSlot;

    if (percussion)
        vc = voicePSlot [slot];
    else
        vc = voiceMSlot [slot];

    t1 = 63 - (GetLocPrm (slot, prmLevel) & 63);    /* amplitude */
    singleSlot = percussion && vc > BD;

    if ( carrierSlot[slot] || singleSlot )
    {
        /* Change the 0 - 127 volume change value to 0 - 63 for the chip.
           (MAX_VOLUME+1)/2 is added to avoid round-off errors. */
        t1 = (t1 * lVoiceVolume[vc] + (MAX_VOLUME+1)/2 ) >> LOG2_VOLUME;
    }
    else
    if( !GetLocPrm (slot, prmFm) )
    {
        t1 = (t1 * slotRelMax[vc] +(MAX_VOLUME+1)/2 ) >> LOG2_VOLUME;
        /* ¿·ü°â¡∏˜ */
    }

    tc = GetLocPrm (slot, prmKsl) << 6;
    t1 = 63 - t1;
    t1 |= tc;
    SndOutput (0x40 + (int)offsetSlot [slot], t1);

}


/* --------------------------------------------
    Note sel
    ???????????????????????????????????????????
*/
static void SndSNoteSel(void)
{
    SndOutput( 0x08, noteSel ? 64 : 0);
}   /* SndSNoteSel() */


/* --------------------------------------------
    FEED-BACK and FM (connection).
    Applicable only to operator 0 in melodic mode.
    OK
*/
static void SndSFeedFm( slot)
    int slot;
{
    unsigned t1;
    unsigned vc;

    vc = voiceMSlot [slot];

    if( carrierSlot[ slot])
    {
        if( !RhythmMode() || slot<12 )
        {
//            SndOutput( 0xC0 + vc, t1);  /* (int)voiceSlot[ slot] */
        }
        return;
    }
    t1 = GetLocPrm( slot, prmFeedBack) << 1;
    t1 &= 0xcf;
    t1 |= GetLocPrm( slot, prmFm) ? 0 : 1;
    if( !RhythmMode() || slot<12 )
    {
        SndOutput( 0xC0 + vc , t1);  /* (int)voiceSlot[ slot] */
    }
    else
    {
        SndOutput( 0xC0 + vc, t1 );
    }
}


/*
    ATTACK, DECAY
    OK
*/
static void SndSAttDecay( slot)
int slot;
{
    unsigned t1;

    t1 = GetLocPrm( slot, prmAttack) << 4;
    t1 |= GetLocPrm( slot, prmDecay) & 0xf;
    SndOutput( 0x60 + (int)offsetSlot[ slot], t1);  /* offsetSlot */
}


/*
    SUSTAIN, RELEASE
    OK
*/
static void SndSSusRelease( slot)
int slot;
{
    unsigned t1;

    t1 = GetLocPrm( slot, prmSustain) << 4;
    t1 |= GetLocPrm( slot, prmRelease) & 0xf;
    SndOutput( 0x80 + (int)offsetSlot[ slot], t1);
}



/*
    AM, VIB, EG-TYP( Sustaining), KSR, MULTI
*/
static void SndSAVEK( slot)
int slot;
{
    unsigned t1;

    t1 = GetLocPrm( slot, prmAm) ? 0x80 : 0;
    t1 += GetLocPrm( slot, prmVib) ? 0x40 : 0;
    t1 += GetLocPrm( slot, prmStaining) ? 0x20 : 0;
    t1 += GetLocPrm( slot, prmKsr) ? 0x10 : 0;
    t1 += GetLocPrm( slot, prmMulti) & 0xf;
    SndOutput( 0x20 + (int)offsetSlot[ slot], t1);
}   /* SndSAVEK() */




/*
    Set the wave-select parameter.
*/
static void SndWaveSelect( slot)
int slot;
    {
    unsigned wave;

    if( modeWaveSel)
        wave = GetLocPrm( slot, prmWaveSel) & 0x03;
    else
        wave = 0;
    SndOutput( 0xE0 + offsetSlot[ slot], wave);
    }   /* SndWaveSelect() */


/*
    Change pitch of voices 0 to 8, for melodic or percussive mode.
*/
static void SetFreq( voice, pitch, keyOn)
    unsigned voice;             /* voice number */
    int     pitch;              /* 0 - 95 */
    unsigned    keyOn;          /* Set key-on/off */
    {
    unsigned int fNbr, t1;

    voiceNote[ voice] = pitch;
    pitch += halfToneOffset[ voice];
    if( pitch > 95)
        pitch = 95;
    if( pitch < 0)
        pitch = 0;
    fNbr = * ( fNumFreqPtr[ voice] + noteMOD12[ pitch]);
    t1 = keyOn ? 0x20 : 0;
    t1 |= ( (unsigned)noteDIV12[ pitch] << 2) + ( 0x3 & HighByte( fNbr) );
    SndOutput( 0xA0 +voice, fNbr );
    SndOutput( 0xB0 +voice, t1 );
    }



/*
    Set the frequency of voice 'voice' to 0 hertz.
*/

static void SoundChut( voice)
    unsigned voice;
{

    SndOutput( 0xA0 +voice, 0 );
    SndOutput( 0xB0 +voice, 0 );
    voiceKeyOn[ voice] = 0;
}   /* SoundChut() */


/*
    Return 0 if board is not installed
*/
static int BoardInstalled()
    {
    unsigned t1, t2, i;

    SndOutput( 4, 0x60 );    /* mask T1 & T2 */
    SndOutput( 4, 0x80 );    /* reset IRQ */
    for( i = 0; i < 220; i++)   /* 100 uSec delay for timer-1 overflow */
        inp( genAddr);
    t1 = inp( genAddr);     /* read status register */
    SndOutput( 2, 0xff );    /* set timer-1 latch */
    SndOutput( 4, 0x21 );    /* unmask & start T1 */
    for( i = 0; i < 220; i++)   /* 100 uSec delay for timer-1 overflow */
        inp( genAddr);
    t2 = inp( genAddr);     /* read status register */
    SndOutput( 4, 0x60 );
    SndOutput( 4, 0x80 );
    return (t1 & 0xE0) == 0 && (t2 & 0xE0) == 0xC0;
    }

int BoardCheck( port )
unsigned port;
{
    genAddr=port;
    genAddr2=port+2;
    return(BoardInstalled());
}

/*
--------------------------------------------------
    Set the register at address 'addr' to the value 'data'.

    Take care of data register to data register write delay.
--------------------------------------------------
*/
