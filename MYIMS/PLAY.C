/*
    PLAY.C

    (C) Kheun Sa Ram  1992 - 1994

    ·¡ ­¡¯a· ¶w· ¸á¸bŠ¥·e 'Çe¬aœq'µA‰A ·¶¯s“¡”a.

*/

#include <dos.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <process.h>
#include <io.h>
#include <time.h>
#include <dir.h>
#include <alloc.h>
#include <mem.h>
#include <fcntl.h>
#include <time.h>

#include <conio.h>

#define O_RAW O_BINARY

#include "adlib.h"
#include "convert.h"
#include "KorHdr.h"

static unsigned char LongData(long position);
static void Init(int mode);
static void Terminate(int mode);
static void SkipToTiming(void);

#define UNSIGNED_CHAR unsigned char


struct      ImsHeader * headPtr;    /* pointer to header of .MUS file */
char        musRunning=0;             /* != 0 if music is playing */
char        status;                                 /* running status byte */

char        volume[ MAX_VOLUME ];     /* actual volume of all voices */
char        ind_volume[ MAX_VOLUME ];

char        Volume_View=0;

char        name_inst[151][9];

char        name_index[11];
char        pre_index[11];
unsigned    int pitch_b[11];

unsigned Master_Vol[ MAX_VOLUME ]={128,128,128,128,128,
			  128,128,128,128,128,128};
int  key_shift[MAX_VOLUME]={0,0,0,0,0,0,0,0,0,0,0};


int NChanges[MAX_VOLUME]={0,0,0,0,0,0,0,0,0,0,0};

extern   char slotRelMax[ 16 ];

unsigned Master_Tempo=200,Save_Tempo=0;

extern      char voiceNote[MAX_VOLUME];
extern      char voiceKeyOn[MAX_VOLUME];
extern      unsigned modeVoices;

extern      char percussion;



#define DEFAULT_BNK "STANDARD.BNK"

int         fail_count=0;
int         i_view=0;
int         change_name;
int         in_int8;
char        ImsTickBeat,TickBeat;
unsigned    BasicTempo;
//const static int IssSize = 64;
#define IssSize 	(64)

const static MaxIssSize  = sizeof(unsigned char)*IssSize;

typedef struct
{
    char  name[9];
}   Inst_Name;

int             int8_installed=0;
int             Inst_count=0,pInst_count=0;
int             Ins_error=0;
int             mus_load=0;
int             Bank_Memory=0;
Inst_Rec        *ins_data;
Inst_Buf_Rec    ins_buf2[6]=
{
    { // PIANO1
    0x00,0x00,
    0x01,0x01,0x03,0x0f,0x05,0x00,0x01,
    0x03,0x0f,0x00,0x00,0x00,0x01,0x00,0x01,
    0xf6,0x0d,0x07,0x00,0x02,0x04,0x00,0x00,
    0x00,0x01,0x01,0x00,0x00
    },
    { /*  BDRUM1    6   */
    0x01,0x06,
    0x00,0x00,0x00,0x0a,0x04,0x00,0x08,
    0x0c,0x0b,0x00,0x00,0x00,0x01,0x00,0x00,
    0x2f,0x0d,0x04,0x00,0x06,0x0f,0x00,0x00,
    0x00,0x00,0x01,0x00,0x00
    },
    { /* SNARE1     7   */
    0x01,0x07,
    0x00,0x0c,0x00,0x0f,0x0b,0x00,0x08,
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x2f,0x0d,0x04,0x00,0x06,0x0f,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
    },
    { /* TOM1       8   */
    0x01,0x08,
    0x00,0x04,0x00,0x0f,0x0b,0x00,0x07,
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x2f,0x0d,0x04,0x00,0x06,0x0f,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
    },
    { /*  CYMBAL1   9   */
    0x01,0x09,
    0x00,0x01,0x00,0x0f,0x0b,0x00,0x05,
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x2f,0x0d,0x04,0x00,0x06,0x0f,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
    },
    { /*  HIHAT1    10  */
    0x01,0x0a,
    0x00,0x01,0x00,0x0f,0x0b,0x00,0x07,
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x2e,
    0x04,0x11,0x00,0x00,0x00,0x00,0x00,0x2e,
    0x17,0x3c,0x00,0x00,0x00
    }
};
Inst_Buf_Rec    *ins_buf;
Bank_Header_Rec bank_header;

char            Bank_File_Name[80];
char            Playing_File_Name[80];

#define DIV_SIZE 4096
#define ADLIB_PORT 0x388

int  no_ins=0;
char NowPlay=0;
char *music[40];    /* MAXIMUM 4096*40==163840 byte */

int  divbuf=0;
int  usebuf=0;
int  insbuf=0;
int  pbuf=0;
struct ImsHeader mH;


int adlibNote[11];
unsigned int FM_PORT=ADLIB_PORT;
int         change_ims=0;
long        tickCount;              /* time counter, for information only */
unsigned    Delay;                  /* length of last delay */
long        gbyte;

struct {
     int  Time;
     unsigned char Line;
     unsigned char Number;
     unsigned char Sit;
} IssData[1000];
#define ON         1
#define TRUE       1
#define OFF	   0
#define FALSE      0

unsigned char Text[100][64];
long My_time = 0;
unsigned char ims_end_check = 0,iss_is_check = 0,iss_return_status = 0;
unsigned int Total_line,Total_event,now_line = 0,Iss_count = 0;
unsigned char now_iss[13];
int load_iss(unsigned char *ims_file);
int ready_iss(unsigned char *name);
void iss_word_out(void);
void iss_line_out(void);

static void kill_time(void)
{
    register int i;

    for(i=0;i<200;i++)
    {
        inp(FM_PORT);
    }
}

int SetMaxInst( int MaxCount , Inst_Buf_Rec **inst)
{
    insbuf=MaxCount;
    ins_buf=(Inst_Buf_Rec *)malloc(sizeof(Inst_Buf_Rec)*insbuf);
    if( ins_buf==NULL )
    {
        insbuf=0;
    }
    *inst=ins_buf;
    return insbuf;
}

void ClearInst( void )
{
    if( insbuf )
        free( ins_buf );
    insbuf=0;
    ins_buf=NULL;
}


int PlayingState(void)
{
    return NowPlay;
}

long GetCurrentTick(void)
{
    return ( tickCount+Delay-DelayLeft() );
}

int run_adlib( char *mus, char *bnk, int  mode)
{
    FILE *meloFile;
    unsigned len;
    long     llen;
    int  check  ;
    int  suc;
    char Bank[100];
    int  no_alloc=0;
    int  divnum=0;
    int  i,j;
    void *buf;

    /* Perform some initialisations ... */
    if( NowPlay==0 )
    {
	ClearInst();

	if( divbuf )
	{
	    for(i=0;i<divbuf;i++)
		free(music[i]);
	    divbuf=0;
	    usebuf=0;
	}

	strcpy(Playing_File_Name,mus);
	strupr(Playing_File_Name);
	if( strstr(Playing_File_Name,".IMS") )
	{
	    /* initalize the low-level sound-driver: */
	    mus_load=0;
	    meloFile = fopen( mus, "rb" );
	    if( NULL == meloFile)
	    {
		Terminate( mode );
		return(1);
	    }

	/* read the music file's header: */
	    fread( &mH, sizeof( struct ImsHeader),1, meloFile );
#ifdef DEBUG
	    printf("Version     : %2x %2x\n",mH.majorVersion,mH.minorVersion);
	    printf("tuneId      : %ld\n",mH.tuneId);
	    printf("tuneName    : %s\n",mH.tuneName);
	    printf("tickBeat    : %d\n",mH.tickBeat);
	    printf("beatMeasure : %d\n",mH.beatMeasure);
	    printf("totalTick   : %ld\n",mH.totalTick);
	    printf("dataSize    : %ld\n",mH.dataSize);
	    printf("nrCommand   : %ld\n",mH.nrCommand);
	    printf("srctickBeat : %d\n",mH.srctickBeat);
	    printf("soundMode   : %d\n",mH.soundMode);
	    printf("pitchBRange : %d\n",mH.pitchBRange);
	    printf("basicTempo  : %u\n",mH.basicTempo);
#endif
	    llen = mH.dataSize;
	    if ( llen==0L )
	    {
		Terminate(mode);
		fclose( meloFile);
		return(1);
	    }
	    else
	    if( llen> 163840L )
	    {
		Terminate(mode);
		fclose( meloFile);
		return(1);
	    }

	    divnum=0;
	    while( llen )
	    {
		if( llen>(long)DIV_SIZE )
		{
		    len=DIV_SIZE;
		    llen-=(long)DIV_SIZE;
		}
		else
		{
		    len=(unsigned)llen;
		    llen=0L;
		}
		if( no_alloc==0 )
		{
		    music[divbuf] = (char *) malloc( DIV_SIZE+4U ); /* llen */
		    if( music[divbuf] == NULL)
		    {
			for(i=0;i<divbuf;i++)
			    free(music[i]);
			divbuf=0;
			usebuf=0;
			Terminate(mode);
			fclose( meloFile);
			return( 1);
		    }
		    divbuf++;
		}
		fread( music[divnum], len, 1, meloFile );
		divnum++;
	    }

	    usebuf=divnum;
	/* load all the data in memory: */
	    fread( &check, 2 ,1 ,meloFile );
	    Ins_error=1;

	    if( check==0x7777 )
	    {
		fread( &Inst_count, 2, 1, meloFile );
		if( no_alloc==0 )
		{
		    ins_buf=(Inst_Buf_Rec *)malloc(sizeof(Inst_Buf_Rec)*Inst_count);
		    if( ins_buf==NULL )
		    {
			insbuf=0;
		    }
		    else
			insbuf=Inst_count;
		}
		else
		    insbuf=Inst_count;

		Load_all_Timb( meloFile,Inst_count);
		fclose( meloFile);

		if( insbuf>=Inst_count )
		{
		    strcpy(Bank,bnk);
		    if( Bank[0]=='\0' )	sprintf(Bank,"%s", DEFAULT_BNK );
		    suc=Load_all_Inst( Bank, Inst_count);
		    if( suc==1 ) Ins_error=0;
		}
		else
		    fail_count=Inst_count;
	    }
	    else
	    {
		    if( no_alloc==0 )
		    {
			for(i=0;i<divbuf;i++) free( music[i] );
			divbuf=0;
			usebuf=0;
		    }
		    Terminate(mode);
		    fclose( meloFile);
		    return(1);
	    }
	    SoundColdInit(FM_PORT); /* return·¡ 0·¡¡e ret */
	    Init( mode );
	/* Start playing: */


	    NowPlay=0;
	    StartMelo( &mH, 0L );
	    NowPlay=1;
	    change_name=1;

	    iss_is_check = load_iss (mus);
	    //clrscr();
	    while (!kbhit())
	    {
		 if(iss_is_check==ON && IssData[Iss_count].Time<=My_time/8)
		 {
			 if(Total_line>now_line+1 && now_line!=IssData[Iss_count].Line)
			 {
			    now_line = IssData[Iss_count].Line;
			    iss_line_out();
			 }
			 if(Total_line>now_line)  iss_word_out();
			 Iss_count++;
		 }
	    }

	}
	else
	    return(1);
	return(0);
    }
//    else
	return(1);
}

static const long fast_div[41]=
    {
	0L,
	4096L,
	8192L,
	12288L,
	16384L,
	20480L,
	24576L,
	28672L,
	32768L,
	36864L,
	40960L,
	45056L,
	49152L,
	53248L,
	57344L,
	61440L,
	65536L,
	69632L,
	73728L,
	77824L,
	81920L,
	86016L,
	90112L,
	94208L,
	98304L,
	102400L,
	106496L,
	110592L,
	114688L,
	118784L,
	122880L,
	126976L,
	131072L,
	135168L,
	139264L,
	143360L,
	147456L,
	151552L,
	155648L,
	159744L,
	163840L,
    };

static unsigned char LongData(long position)
{
    unsigned char *Ptr;
    long pos;

    pbuf=0;
    while( position >= fast_div[pbuf+1] )
    {
	pbuf++;
    }
    pos= position - fast_div[pbuf];
    Ptr= music[pbuf];
    Ptr+=pos;
    return( (*Ptr) );
}

void clear_mus(mode)
    int mode;
{
    int i;

    if( NowPlay==1 )
    {
	StopMelo();

	for(i=0;i<11;i++)
	{
	    name_index[i]=255;
	    pre_index[i]=254;
	}

	Terminate(mode);

	for(i=0;i<divbuf;i++)
	    free( music[i] );
	divbuf=0;
	usebuf=0;
	ClearInst();
	Inst_count=0;

	delay(600);
    }
}

void next_play(void)
{
    unsigned len;

    if( NowPlay==1 )
    {
	StopMelo();
	NowPlay=2;
    }
}

/*
    µeº•¡º— ·¡Ðq®Ÿi ¦Ÿa¡e Àá·q¦Èá ”a¯¡ µeºÐe”a.
*/

void re_play(void)
{
    unsigned len;

    if( NowPlay==1 )
    {
	StopMelo();
	SoundWarmInit();
	NowPlay=0;
	StartMelo( &mH, 0L );
	NowPlay=1;
	change_name=1;
//	iss_is_check      =0;
//	iss_return_status =0;
	now_line  =0;
	Iss_count =0;
	My_time   =0;
    }
}

/*
    Initialize the driver.
*/
static void Init(mode)
    int mode;
    {
    /*  install the clock driver: */
	musRunning=0;
	if( mode )
	{
	    if( int8_installed==0 )
	    {
		Clk_install();
		int8_installed=1;
	    }
	}
    }


/*
    Uninstall the clock driver ...
*/
static void Terminate(mode)
    int mode;
    {
	NowPlay=0;
	musRunning=0;
	if( mode )
	{
	    if( int8_installed )
	    {
		int8_installed=0;
		Clk_uninstall();
	    }
	}
    }


/*
    Start playing a melody. Set some global pointers for the interrupt
    routine. Set the tempo, sound mode & pitch bend range.
    Reset volume of each voice. Start the clock driver with the first
    Delay ( >= 1)
*/

void StartMelo( header, Roffset )
    struct ImsHeader * header;  /* pointer to header struc. of music file */
    long Roffset;            /* pointer to music data */

/*    unsigned len; */
/* size of data */
    {
    int i;

    musRunning=0;
    headPtr = header;
    for( i = 0; i < 11; i++)
    {
	NChanges[i]=0;
	volume[ i] = 0;
	ind_volume[ i] = 0;
	voiceKeyOn[ i]=0;
	name_index[ i]=255;
	pre_index[ i]=254;
	pitch_b[ i]=0x2000;
    }

    TickBeat=header->srctickBeat;
    ImsTickBeat=header->tickBeat;

    StartTimeOut( 0);
    BasicTempo= Save_Tempo = header->basicTempo;
    SetTempo( Save_Tempo, ImsTickBeat );
    SetMode( header->soundMode );
    SetPitchRange( header->pitchBRange);

    tickCount = 0L;
    /* NEVER START COUNT-DOWN WITH 0, since 0 means MAXIMUM delay!!! */
    gbyte = Roffset;
    Delay = LongData(gbyte++);

    StartTimeOut( max( Delay, 1));
    musRunning = 1;
    }   /* StartMelo() */

/*
    Stop playing the melody. Send note-off to all voices and reset
    the clock frequency to nominal ( 18.2 Hz).
*/
void StopMelo(void)
    {
    int i;

    musRunning = 0;
    SetTempo( 0, 1);
    Save_Tempo=0;
    for( i = 0; i < modeVoices; i++)
    {
	SetVoiceVolume (i, 0);
	NoteOff(i);
    }
}

void OnMelo(void)
    {
    musRunning = 1;
    }

void OffMelo(void)
    {
    musRunning = 2;
    }

/*
    Change the tempo.

    Reload the timer-0 with the proper divider for generating
    the appropriate frequency.

    If tempo is zero, reprogram the counter for 18.2 Hz.
*/

void SetTempo( tempo, tickBeat)
    unsigned tempo;             /* beats per minute */
    unsigned tickBeat;          /* ticks per beat */
    {
    long t1;
    unsigned freq;
    unsigned low, high, flags, count;

    t1 = tickBeat * (long)tempo * (long)Master_Tempo / 200L ;

    freq = t1 /60;              /* interrupt rate needed */

    if( !freq)
	count = 0;
    else {
	/* make sure that frequency is >= 19 Hz, since counter min. output
	    frequency is 18.2 Hz: */
	if( freq<19 )
	    count=0;
	else
	/* compute counter divider: */
	    count = ( 1193180L /(long)freq );
	}
	/* and set the counter: */
	SetClkRate( count);
    }   /* SetTempo() */

/*
    Interrupt routine. Called by low-level clock driver when
    the delay count has expired.

    When this routine is called, the active SS ( stack segment) is not
    the original of the application, so take care.
    This routine, and all others called by, must be compiled
    without stack-overflow checking, since the SS has changed!!!

    Return to caller the number of clock ticks to wait for before
    the next call.
*/
unsigned TimeOut()
    {
    unsigned pitch, tempo, haut, vol;
    unsigned pitch1,pitch2;
    UNSIGNED_CHAR newStatus;
    int timbre,note;
    int comm, id, integer, frac, voice = 1;
    int i;

    switch( musRunning )
    {
    case 0:
    case 2: in_int8=0;
	    return(1);
    case 3:
	    re_play();
	    in_int8=0;
	    return(0);
    }
    /* Music has not started or has been stopped, so wait the minimum delay ... */
    tickCount += Delay;
    if( tickCount>=headPtr->totalTick )
    {
	re_play();
	return 0;
    }
    Delay=0;
    do {
	newStatus = LongData(gbyte);
	if( newStatus == OVERFLOW_BYTE) {
	    /* timing overflow ... */
	    gbyte++;
	    Delay = OVERFLOW_BYTE;
	    break;
	}
	else if( newStatus == STOP_BYTE) {
	    gbyte++;
	    re_play();
	    return 0;       /* maximum delay ... */
	}
	else if( newStatus == SYSTEM_EXC_BYTE) {
	    /*
	    non-standard... this is a tempo multiplier:
	    data format: <F0> <7F> <00> <integer> <frac> <F7>

	    tempo = basicTempo * integerPart + basicTempo * fractionPart/128

	    */
	    gbyte++;
	      id = LongData(gbyte++);
	    comm = LongData(gbyte++);
	    if( id != ADLIB_CTRL_BYTE || comm != TEMPO_CTRL_BYTE) {
		/* unknown format ... skip all the XOR message */
		    gbyte -= 2;
		    while( LongData(gbyte++) != EOX_BYTE );
	    }
	    else {
		integer = LongData(gbyte++);
		   frac = LongData(gbyte++);
		  tempo = BasicTempo;

		/*  ÉQÍ¡‰¬e ¦¦…   */
		tempo = tempo * integer + (unsigned)(((long)tempo * frac) >> 7);

		if( tempo!=Save_Tempo )
		{
		    Save_Tempo=tempo;
		    SetTempo( tempo, ImsTickBeat );
		}
		gbyte++;      /* skip EOX_BYTE */
	    }
	    Delay = LongData(gbyte++);
	}
	else {
	    if( newStatus >= 0x80) {
		gbyte++;
		status = newStatus;
	    }
	    voice = (int) (status & 0x0f);

	    switch( status & 0xf0) {
		case NOTE_ON_BYTE:
		    note = LongData(gbyte++);
		    vol  = LongData(gbyte++);
		    note &=0x7f;
		    vol  &=0x7f;
		    NChanges[voice]++;
		    if( !vol ) {
			NoteOff( voice);
		    }
		    else
		    {
			note+=key_shift[voice];
			ind_volume[ voice] = vol;
			if( vol != volume[ voice] )
			{
			    SetVoiceVolume( voice, (Master_Vol[voice]*(unsigned)vol)/128U);
			    volume[ voice] = vol;
			}
			NoteOn( voice, note );
			adlibNote[voice]=note;
		    }
		    break;

		case NOTE_CHANGE_BYTE:
		    /* Skip haut vol    */
		    note = LongData(gbyte++);
		    vol  = LongData(gbyte++);
		    note &=0x7f;
		    vol  &=0x7f;
		    NoteOff( voice);
		    NChanges[voice]++;
		    if( vol )
		    {
			note+=key_shift[voice];
			if( vol != volume[ voice])
			{
			    SetVoiceVolume( voice, (Master_Vol[voice]*(unsigned)vol)/128U);
			    volume[ voice] = vol;
			}
			ind_volume[ voice] = vol;
			NoteOn( voice, note );
			adlibNote[voice]=note;
		    }
		    break;

		case AFTER_TOUCH_BYTE:
			vol = LongData(gbyte++);
			vol &= 0x7f;
			if( vol != volume[ voice])
			{
			    SetVoiceVolume( voice, (Master_Vol[voice]*(unsigned)vol)/128U);
			    volume[ voice] = vol;
			}
			if( voiceKeyOn[ voice ] )
			    ind_volume[ voice] = vol;
		    break;

		case PROG_CHANGE_BYTE:
		    timbre = LongData(gbyte++);
		    if( Ins_error || ins_buf[timbre].mode==0xff )
		    {
			if( percussion && voice>=6 )
			    SetVoiceTimbre( voice, ins_buf2[voice-5].ins );
			else
			    SetVoiceTimbre( voice, ins_buf2[0].ins ); /* Piano */
			name_index[voice]=timbre|0x80;
		    }
		    else
		    {
			SetVoiceTimbre( voice , ins_buf[timbre].ins );
			name_index[voice]=timbre;
		    }
		    break;

		case PITCH_BEND_BYTE:
		    pitch1 = LongData(gbyte++);
		    pitch1 &=0x7f;
		    pitch  = pitch1;
		    pitch2 = LongData(gbyte++);
		    pitch2 &= 0x7f;
		    pitch  += (unsigned)(pitch2) << 7;
		    pitch_b[ voice]=pitch;
		    SetVoicePitch( voice, pitch);
		    break;

		case CONTROL_CHANGE_BYTE:
		    /* not implemented ... */
		    gbyte+=2;
		    break;

		case CHANNEL_PRESSURE_BYTE:
		    /* not implemented ... */
		    gbyte++;
		    break;

		default:
		    SkipToTiming();
		    break;
		}

	    Delay = LongData(gbyte++);
	    }
	} while( Delay == 0);
	if( Delay >= OVERFLOW_BYTE )
	{
	    Delay = OVERFLOW;
	    if( LongData(gbyte) != OVERFLOW_BYTE )
	    {
		Delay += LongData(gbyte++);
	    }
	}
    My_time = My_time + Delay;
    return Delay;
}

/*
    A bad status byte ( or unimplemented MIDI command) has been encontered.
    Skip bytes until next timing byte followed by status byte.
*/
static void SkipToTiming(void)
    {
	while( LongData(gbyte) < 0x80)
	{
	    gbyte++;
	}
	if( LongData(gbyte) != OVERFLOW_BYTE)
	{
	    gbyte--;
	}
    }   /* SkipToTiming() */


int GetInst(char *insname, int f_bnk)
{
    unsigned int L, R, M;
    int C;
    int I,i;
    unsigned char far *tmp;
    char buf[28];

    if( insname[0] == '\0' )
    {
	return(0);
    }
    else
	strupr(insname);
    L = 0;
    R = bank_header.Num + 1;
    do{
	 M = (R + L) / 2;
	 C= strcmp( insname, ins_data[M-1].name);
	 if(  C > 0 )
	    L = M;
	 else
	    R = M;
    }while( C!=0 && ((R - L) != 1 ) );
    if( C==0 )
    {
	if( ins_data[M-1].marker == 1 )
	{
	      no_ins++;
	      if( no_ins > 150 )
	      {
		printf("\nToo many Inst !! \n");
		return(-2);
	      }
	      ins_data[M-1].marker = no_ins+1;
	      lseek(f_bnk,( bank_header.D_offset + (ins_data[M-1].no)*30L ),SEEK_SET);
	      _read(f_bnk,&ins_buf[no_ins-1].mode,30);
	}
	if( ins_data[M-1].marker==0 )
	    return(0);
	return( ins_data[M-1].marker-1 );
    }
    else
    {
	return(0);
    }
}

int  f_bnk_inst;

int LoadBank(char *BankFile)
{
      int i, j, k;
      char Bank[80];

      no_ins = 0;

	Bank_Memory=0;
	if( BankFile[0]=='\0') sprintf(BankFile,"%s",DEFAULT_BNK);
	strcpy(Bank_File_Name,BankFile);
	f_bnk_inst=_open(Bank_File_Name,0x8001);
	if( f_bnk_inst==-1 )
	{
	    return(-1);
	}
	_read(f_bnk_inst,(char *)(&bank_header),sizeof(Bank_Header_Rec));
	if( bank_header.Num > 5400 )
	{
//            printf("\n\t\t[*] ERROR (LoadBank) : Too Many INS in Bank File\r");
//            printf("\n\t\t    Total %5d Instruments.\r\n",bank_header.Num);
	    bank_header.Num = 5400;
	}
	lseek(f_bnk_inst,bank_header.NL_offset,SEEK_SET);
	ins_data=(Inst_Rec *)malloc((unsigned int)sizeof(Inst_Rec)*(unsigned int)bank_header.Num);
	if( ins_data==NULL )
	{
	    _close(f_bnk_inst);
	    return(-1);
	}
	else
	    Bank_Memory=1;
	_read(f_bnk_inst,&ins_data[0],12*bank_header.Num);
	for(i=0;i<bank_header.Num;i++)
	{
	    ins_data[i].name[8]='\0';
	    strupr(ins_data[i].name);
	}

    return( f_bnk_inst );
}

void Load_all_Timb( FILE *fileptr, int count )
{
    int name_error=0;
    int i;

    if( count>=150 )
	name_error=1;
    if( name_error==0 )
	fread( &name_inst[0][0] , 9, count, fileptr);
    for(i=0;i<count;i++)
    {
	strupr(name_inst[i]);
    }

}

int Load_all_Inst( char *Bank, int count )
{
    int i,j,index;
    int f_bnk;


    fail_count=count;
    if( LoadBank( Bank )==-1 )
    {
	f_bnk_inst=-1;
	return(0);
    }

    for(i=0;i<count;i++)
    {
	if ( f_bnk_inst == -1 || Bank_Memory==0 )
	    index = 0;
	else
	{
	    index = GetInst( name_inst[i], f_bnk_inst );
	    strlwr( name_inst[i] );
	}

	if( index==0 )
	{
	    /* aº—µA Àx´aº‹¡ */
	    no_ins++;
	    ins_buf[no_ins-1].mode=0xff; /* FAIL */
	}
	else
	if( index< 0 )
	{
	    _close( f_bnk_inst );
	    if( Bank_Memory==1 )
	    {
		free( ins_data );
		Bank_Memory=0;
	    }
	    return(0);
	}
	else
	    fail_count--;

    }
    CloseBank();
    return(1);
}


void CloseBank(void)
{
    _close( f_bnk_inst );
    if( Bank_Memory==1 )
    {
	free( ins_data );
	Bank_Memory=0;
    }
}

int load_iss(unsigned char *ims_file)
{
     struct
     {
	  unsigned char IssId[150];
	  unsigned int  IssDef;
	  unsigned int  IssLine;
     } IssHead;
     struct
     {
	  unsigned char IssId[128];
	  unsigned int  IssDef;
	  unsigned int  IssLine;
     } IssHeadV1;

     FILE *fp;
     int i;
     unsigned char iss[13], *iss_ext = ".ISS";
     const char iss_id[] ="IMPlay Song V2.0";
     strcpy(iss,ims_file );
     strcpy(strrchr(iss,'.'),iss_ext);

     if((fp = fopen(iss,"rb"))==NULL) return 0;
     fread(&IssHead, sizeof(IssHead), 1, fp);

     if(strnicmp(IssHead.IssId, iss_id, strlen(iss_id)))
     { //Iss Ver. 1
#if 0
       fseek(fp, (long)0, SEEK_SET);
       fread(&IssHeadV1, sizeof(IssHeadV1), 1, fp);
       for(i=0;i<IssHeadV1.IssDef;i++)
	fread(&IssData[i],sizeof(IssData[i]),1, fp);
       for(i=0;i<IssHeadV1.IssLine;i++)
	fread(Text[i],64,1,fp);
       Total_line  = IssHeadV1.IssLine;
       Total_event = IssHeadV1.IssDef;
#endif
     } else
     { // Iss Ver. 2
       for(i=0;i<IssHead.IssDef;i++)
	fread(&IssData[i],sizeof(IssData[i]),1, fp);
       for(i=0;i<IssHead.IssLine;i++)
	fread(Text[i],64,1,fp);
       Total_line  = IssHead.IssLine;
       Total_event = IssHead.IssDef;
     }
     fclose(fp);
     return 1;      /* IssRunning = 1; */
}

int ready_iss(unsigned char *name)
{
     iss_is_check      = 0;
     iss_return_status = 0;
     now_line  = 0;
     Iss_count = 0;

     return(load_iss(name));
}

void iss_line_out(void)
{
    unsigned char cur_text[IssSize], next_text[IssSize];
    int idx1 =0, idx2 =0;

    if(strlen(Text[IssData[Iss_count].Line])   > MaxIssSize) return;
    if(strlen(Text[IssData[Iss_count].Line+1]) > MaxIssSize) return;

    (void)strncpy(cur_text,Text[IssData[Iss_count].Line], MaxIssSize);
    if(now_line<Total_line+1)
	(void)strncpy(next_text,Text[IssData[Iss_count].Line+1], MaxIssSize);

    idx1 = strlen(cur_text);
    if(cur_text [idx1+1] != NULL) return;
    idx2 = strlen(next_text);
    if(next_text[idx2+1] != NULL) return;

#ifdef TEXT
	textcolor ( 15);
	gotoxy ( 1, 7);
	cprintf ("                                                                              ");
	cprintf ("                                                                              ");
	gotoxy ( 1, 7); cprintf ( "%s", cur_text);
	if(now_line<Total_line+1)
	{
		gotoxy ( 1, 8); cprintf ( "%s", next_text);
	}
#else
	gColor  (0, 15);
	gprintf (1, 7, 0, "                                                                              ");
	gprintf (1, 8, 0, "                                                                              ");
//	if(cur_text [idx1+1] != NULL) goto NEXT;
	gprintf (1, 7, 0, "%s", cur_text);

NEXT:
	gColor  (0, 7);
//	delay(15000);
	if(now_line<Total_line+1)
	{
//		if(next_text [idx2+1] != NULL) return;
		  gprintf (1, 8, 0, "%s", next_text);
	}
#endif
}

void iss_word_out(void)
{
    unsigned char part[IssSize];
    int i;
    int start = (int)IssData[Iss_count].Number;
    int end   = (int)IssData[Iss_count].Sit;

//    if(strlen(Text[IssData[Iss_count].Line]) > MaxIssSize) return;

    for(i=start;i<start+end;i++)
    {
	part[i-start] = Text[IssData[Iss_count].Line][i];
    }
	part[end] = NULL;
    if(strlen(part) > MaxIssSize) return;

#ifdef TEXT_OUT
	textcolor ( 10);
	gotoxy  ( start+1, 7);
	cprintf ( "%s", part);
#else
	gColor  (0, 10);
	gprintf (start+1, 7, 0, "%s", part);
#endif
}
