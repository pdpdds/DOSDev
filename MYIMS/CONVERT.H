/*
    CONVERT.H

    28-jun-88, Marc Savary, AdLib Inc.

    Header for conversion procedures of *.ROL files to
    *.MUS ( MIDI like) files.
*/

#define NORMAL_STATE        0
#define LOAD_STATE          1
#define CLEAR_STATE         2

#define SYSTEM_EXC_BYTE     0xF0
#define EOX_BYTE            0xF7
#define OVERFLOW_BYTE       0xF8
#define STOP_BYTE           0xFC
#define META                0xFF
#define END_OF_TRACK        0x2f

#define NOTE_CHANGE_BYTE        0x80
#define NOTE_ON_BYTE            0x90
#define AFTER_TOUCH_BYTE        0xA0
#define CONTROL_CHANGE_BYTE     0xB0
#define PROG_CHANGE_BYTE        0xC0
#define CHANNEL_PRESSURE_BYTE   0xD0
#define PITCH_BEND_BYTE         0xE0

#define ADLIB_CTRL_BYTE         0x7F    /* for System exclusive */
#define SEQ_SPECIFIC            0x7f
#define TEMPO_CTRL_BYTE         0

#define NR_VOICES               11
#define N_ROLL_VOICES           NR_VOICES
#define TUNE_NAME_SIZE          30
#define FILLER_SIZE             7
#define FILLER2_SIZE            8
#define TIMBRE_DEF_LEN          28
#define TIMBRE_DEF_SIZE         ( TIMBRE_DEF_LEN * sizeof( int))
#define OVERFLOW                240
#define DEFAULT_TICK_BEAT       240

struct ImsHeader {
    char        majorVersion;
    char        minorVersion;
    long        tuneId;
    char        tuneName[ TUNE_NAME_SIZE];
    char        tickBeat;
    char        beatMeasure;
    long        totalTick;
    long        dataSize;
    long        nrCommand;
    char        srctickBeat;
    char        filler[ FILLER_SIZE];

    char        soundMode;          /* 0: melodic, 1: percussive */
    char        pitchBRange;            /* 1 - 12 */
    unsigned    basicTempo;
    char        filler2[ FILLER2_SIZE];

    /* char     data[]; */
    };

typedef struct
{
    unsigned int  One;
    char Str[6];
    unsigned int  Num,Max;
    long NL_offset,D_offset;
    char Reserved[8];
}   Bank_Header_Rec;

typedef struct
{
    unsigned int  no;
    unsigned char marker;
    char  name[9];
}   Inst_Rec;

typedef struct
{
    unsigned char mode;
    unsigned char percVoice;
    unsigned char ins[28];
}   Inst_Buf_Rec;


int SetMaxInst( int MaxCount , Inst_Buf_Rec **inst );
int LoadBank(char *BankFile);
int GetInst(char *insname, int f_bnk);
void CloseBank(void);
void ClearInst( void );

int  run_adlib( char *mus, char *bnk, int mode);
void clear_mus( int mode);
void next_play(void);
void re_play(void);

void StartMelo( struct ImsHeader * header, long Roffset );
void StopMelo(void);
void OnMelo(void);
void OffMelo(void);
void SetTempo( unsigned tempo, unsigned tickBeat);
unsigned TimeOut(void);
void Load_all_Timb( FILE *fileptr, int count );
int  Load_all_Inst( char *Bank, int count );
long GetCurrentTick(void);
int  PlayingState(void);

void Clk_install(void);
void Clk_uninstall(void);
void StartTimeOut( unsigned delay );
void SetClkRate( unsigned count );
unsigned DelayLeft(void);