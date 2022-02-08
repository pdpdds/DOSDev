#define SND_EXPLOSION   0
#define SND_LAUNCH      1
#define SND_MACHINEGUN  2
#define SND_FLAMER      3
#define SND_SHOTGUN     4
#define SND_POWERGUN    5
#define SND_SWITCH      6
#define SND_KILL        7
#define SND_KILL2       8
#define SND_KILL3       9
#define SND_KILL4      10
#define SND_HAHAHA     11
#define SND_BANG       12
#define SND_PICKUP     13
#define SND_DOOR       14
#define SND_DONE       15
#define SND_LASER      16
#define SND_MINIGUN    17
#define SND_COUNT      18

#define FX_MAXCHANNELS  8

#define SND_QUALITYMODE 1
#define SND_USE486      2

#define MODULE_OK       0
#define MODULE_NOLOAD   1


int  InitializeSound( void );
void ShutDownSound( void );
int  PlaySong( char *name );
void PlaySound( int sound, int panning, int volume );
void DoSounds( void );
void SoundTick( void );
void SetFXVolume( int volume );
int  FXVolume( void );
void SetMusicVolume( int volume );
int  MusicVolume( void );
void SetLeftEar( int x, int y );
void SetRightEar( int x, int y );
void PlaySoundAt( int x, int y, int sound );
void SetFXChannels( int channels );
int  FXChannels( void );
void SetMinMusicChannels( int channels );
int  MinMusicChannels( void );
void ToggleTrack( int track );
void InterruptOn( void );
void InterruptOff( void );
void SetDynamicInterrupts( int flag );
int  DynamicInterrupts( void );
int  ModuleStatus( void );
const char *ModuleMessage( void );
void SetModuleDirectory( const char *dir );
const char *ModuleDirectory( void );

