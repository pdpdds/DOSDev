/*
    ADLIB.H

    Defintions of sound chip parameters.

*/

/* Parameters of each voice: */

#define nbLocParam      14

#define prmKsl          0
#define prmMulti        1
#define prmFeedBack     2           /* use for opr. 0 only */
#define prmAttack       3
#define prmSustain      4
#define prmStaining     5           /* Sustaining ... */
#define prmDecay        6
#define prmRelease      7
#define prmLevel        8
#define prmAm           9
#define prmVib          10
#define prmKsr          11
#define prmFm           12          /* use for opr. 0 only */
#define prmWaveSel      13          /* wave select */

/* globals parameters: */
#define prmAmDepth      14
#define prmVibDepth 15
#define prmNoteSel      16
#define prmPercussion   17

/* melodic voice numbers: */
#define vMelo0          0
#define vMelo1          1
#define vMelo2          2
#define vMelo3          3
#define vMelo4          4
#define vMelo5          5
#define vMelo6          6
#define vMelo7          7
#define vMelo8          8

/* percussive voice numbers: */
#define BD              6
#define SD              7
#define TOM             8
#define CYMB            9
#define HIHAT           10


#define MAX_VOLUME      0x7f
#define LOG2_VOLUME     7           /* log2( MAX_VOLUME) */
#define MAX_PITCH       0x3fff
#define MID_PITCH       0x2000

#define MID_C           60          /* MIDI standard mid C */
#define CHIP_MID_C      48          /* sound chip mid C */
#define NR_NOTES        96          /* # of notes we can play on chip */

int BoardCheck( unsigned port );
int SoundColdInit( unsigned port );
void SetStereo(int mode);
void SetVoicePan ( unsigned voice, int pan );
void SoundWarmInit( void );
void SetMode( int mode );
void SetWaveformSel( int state );
void SetPitchRange( unsigned pR );
void SetGParam( int amD, int vibD, int nSel );
void SetVoiceTimbre( unsigned voice, char * paramArray );
void SetVoiceVolume( unsigned voice, unsigned volume );
void SetVoicePitch( unsigned voice, unsigned pitchBend );
void NoteOn( unsigned voice, int pitch );
void NoteOff( unsigned voice );

