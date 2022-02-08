#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include "malloc.h"
#include "sounds.h"


static int soundInitialized = 0;
static int fxVolume = 64;
static int musicVolume = 64;
static int noOfFXChannels = 4;
static int maxModChannels = 0;


static int channelPriority[ FX_MAXCHANNELS];
static int channelPosition[ FX_MAXCHANNELS];
static int channelTime[ FX_MAXCHANNELS];
static int fxChannel;

static int interruptEnabled = 0;
static int dynamicInterrupts = 0;


static int moduleStatus = 0;
static char moduleMessage[128];
static char moduleDirectory[128] = "";



struct SndData
{
  char name[81];
  int playPrio;
  int cmpPrio;
  int exists;
  int freq;
  int playFlag;
  int time;
  int panning;
  int volume;
};

struct SndData snd[ SND_COUNT] =
{
  { "BOOOM.RAW",    20, 20, 0, 11025, 0,  82, 0, 0 },
  { "LAUNCH.RAW",    8,  8, 0, 11025, 0,  27, 0, 0 },
  { "MG.RAW",       10, 11, 0, 11025, 0,  36, 0, 0 },
  { "FLAMER.RAW",   10, 10, 0, 11025, 0,  44, 0, 0 },
  { "SHOTGUN.RAW",  12, 13, 0, 11025, 0, 118, 0, 0 },
  { "FUSION.RAW",   12, 13, 0, 11025, 0,  90, 0, 0 },
  { "SWITCH.RAW",   18, 19, 0, 11025, 0,  36, 0, 0 },
  { "SCREAM.RAW",   15, 16, 0, 11025, 0,  70, 0, 0 },
  { "AARGH1.RAW",   15, 16, 0,  8060, 0,  79, 0, 0 },
  { "AARGH2.RAW",   15, 16, 0,  8060, 0,  66, 0, 0 },
  { "AARGH3.RAW",   15, 16, 0, 11110, 0,  67, 0, 0 },
  { "HAHAHA.RAW",   22, 22, 0,  8060, 0,  58, 0, 0 },
  { "BANG.RAW",     14, 15, 0, 11025, 0,  60, 0, 0 },
  { "PICKUP.RAW",   14, 15, 0, 11025, 0,  27, 0, 0 },
  { "CLICK.RAW",     4,  5, 0, 11025, 0,  11, 0, 0 },
  { "WHISTLE.RAW",  20, 20, 0, 11110, 0,  90, 0, 0 },
  { "POWERGUN.RAW", 13, 14, 0, 11110, 0,  90, 0, 0 },
  { "MG.RAW",       10, 11, 0, 11025, 0,  36, 0, 0 }
};

static void loadSampleConfiguration( void )
{
  int i;
  FILE *f;


  f = fopen( "SOUND_FX.CFG", "r");
  if (!f)
    return;

  printf( "Reading SOUND_FX.CFG\n");
  for (i = 0; i < SND_COUNT; i++)
  {
    memset( snd[i].name, 0, sizeof( snd[i].name));
    fscanf( f, "%80s %d\n", snd[i].name, &snd[i].freq);
    printf( "%2d. File:'%s' at %dHz\n", i, snd[i].name, snd[i].freq);
  }
  fclose( f);
}

void shutDown( void )
{
  // Shutdown goes here...
}

int InitSoundDevice( void )
{
  int i;

  // Initialization goes here...

  // C-Dogs internals:
  loadSampleConfiguration();
  for (i = 0; i < SND_COUNT; i++)
  {
    if (!snd[i].name[0] || snd[i].freq <= 0)
      snd[i].exists = 0;
    else
    {
//    snd[i].exists = <load the sample data reference by snd[i]> ;
      if (!snd[i].exists)
        printf( "Error loading sample '%s'\n", snd[i].name);
    }
  }

  memset( channelPriority, 0, sizeof( channelPriority));
  memset( channelPosition, 0, sizeof( channelPosition));
  memset( channelTime, 0, sizeof( channelTime));
  soundInitialized = 1;

  return 1;
}

static void EnableSoundInterrupt( void )
{
  if (!soundInitialized || interruptEnabled)
    return;

  // yanked

  interruptEnabled = 1;
}

static void DisableSoundInterrupt( void )
{
  if (!interruptEnabled)
    return;

  // yanked

  interruptEnabled = 0;
}

int InitializeSound( void )
{
  atexit( shutDown);

  // load and init sound device
  if (!InitSoundDevice())
  {
    printf( "Unable to initalize sound device\n");
    return 0;
  }

//  if ( <device needs polling> )
//    pollSndDev = 1;

  EnableSoundInterrupt();
  return 1;
}

int PlaySong( char *name )
{
  if (!soundInitialized)
    return 0;

//if (module)
//{
//	release it
//  module = NULL;
//}

  fxChannel = 0;
  if (name)
  {
	// load module
//  if (module)
//    fxChannel = <channel count of module>;
  }

  if (fxChannel > maxModChannels)
    maxModChannels = fxChannel;

  SetMusicVolume( musicVolume);
//if (module)
//	play module

//return module != NULL;

  return 0;
}

void PlaySound( int sound, int panning, int volume )
{
  snd[ sound].playFlag = 1;
  snd[ sound].panning = panning;
  snd[ sound].volume = (volume * fxVolume)/256;
}

static void DoSound( int sound )
{
  int i;
  int low, min = 100;

  for (i = 0; i < noOfFXChannels; i++)
  {
    if (channelPriority[i] < min)
    {
      min = channelPriority[i];
      low = i;
    }
  }
  if (min < snd[ sound].cmpPrio)
  {
/*
    if (min > 0)
	  stop sound on that channel (fxChannel + low)	

	now play new sound:
     channel fxChannel + low
	 data in sndSamples[ sound]
*/
  }
}

void DoSounds( void )
{
  int i;

  if (!soundInitialized)
    return;

  for (i = 0; i < noOfFXChannels; i++)
  {
    if (channelPriority[i] > 0)
    {
      channelTime[i]--;
      if (channelTime[i] <= 0)
        channelPriority[i] = 0;
    }
  }

  for (i = 0; i < SND_COUNT; i++)
  {
    if (snd[i].playFlag && snd[i].exists)
    {
      DoSound( i);
      snd[i].playFlag = 0;
    }
  }
}

void SoundTick( void )
{
  if (!soundInitialized || interruptEnabled)
    return;

  // poll device if needed
}

void SetFXVolume( int volume )
{
  int i;

  fxVolume = volume;
  if (!soundInitialized)
    return;

  for (i = 0; i < noOfFXChannels; i++)
  {
	// Set volume
  }	
}

int FXVolume( void )
{
  return fxVolume;
}

void SetMusicVolume( int volume )
{
  musicVolume = volume;
  if (!soundInitialized)
    return;

  // Set music volume
}

int MusicVolume( void )
{
  return musicVolume;
}

#define RANGE_FULLVOLUME    70
#define RANGE_FACTOR       128


static int xLeft,  yLeft;
static int xRight, yRight;


void SetLeftEar( int x, int y )
{
  xLeft = x;
  yLeft = y;
}

void SetRightEar( int x, int y )
{
  xRight = x;
  yRight = y;
}

void PlaySoundAt( int x, int y, int sound )
{
  int d, dLeft, dRight;
  int leftVolume, rightVolume;
  int volume, panning;

  if (xLeft != xRight || yLeft != yRight)
  {
    dLeft = max( abs( x - xLeft), abs( y - yLeft));
    dRight = max( abs( x - xRight), abs( y - yRight));

    d = (dLeft > RANGE_FULLVOLUME ? dLeft - RANGE_FULLVOLUME : 0);
    leftVolume = 255 - (RANGE_FACTOR*d)/256;
    if (leftVolume < 0)
      leftVolume = 0;

    d = (dRight > RANGE_FULLVOLUME ? dRight - RANGE_FULLVOLUME : 0);
    rightVolume = 255 - (RANGE_FACTOR*d)/256;
    if (rightVolume < 0)
      rightVolume = 0;

    volume = leftVolume + rightVolume;
    if (volume > 256)
      volume = 256;

    panning = rightVolume - leftVolume;
    panning /= 4;
  }
  else
  {
    d = max( abs( x - xLeft), abs( y - yLeft));
    d -= d/4;
    d = (d > RANGE_FULLVOLUME ? d - RANGE_FULLVOLUME : 0);
    volume = 255 - (RANGE_FACTOR*d)/256;
    if (volume < 0)
      volume = 0;

    panning = (x - xLeft)/4;
  }

  if (volume > 0)
    PlaySound( sound, panning, volume);
}

void SetFXChannels( int channels )
{
  if (channels >= 2 && channels <= FX_MAXCHANNELS)
    noOfFXChannels = channels;
}

int  FXChannels( void )
{
  return noOfFXChannels;
}

void SetMinMusicChannels( int channels )
{
  if (channels >= 0 && channels <= 32)
    maxModChannels = channels;
}

int MinMusicChannels( void )
{
  return maxModChannels;
}

void ToggleTrack( int track )
{
  // toggle music track on/off
}

void InterruptOn( void )
{
  EnableSoundInterrupt();
}

void InterruptOff( void )
{
  if (!dynamicInterrupts)
    return;
  DisableSoundInterrupt();
}

void SetDynamicInterrupts( int flag )
{
  dynamicInterrupts = flag;
}

int DynamicInterrupts( void )
{
  return dynamicInterrupts;
}

int ModuleStatus( void )
{
  return moduleStatus;
}

const char *ModuleMessage( void )
{
  return moduleMessage;
}

void SetModuleDirectory( const char *dir )
{
  strcpy( moduleDirectory, dir);
}

const char *ModuleDirectory( void )
{
  return moduleDirectory;
}

