
#include "bf.h"

unsigned char far *mcga0 = (unsigned char far *) 0xa0000000L ;
unsigned char far *mcga1 ;
unsigned char far *mcga2 ;
unsigned char far *mcga3 ;

int  XMS_hand  ;
int  XMS_air   ;
int  XMS_back  ;
int  XMS_pcm   ;
int  XMS_image ;

void XMS_Free_Memory_Check( void )
{
   int i ;

   i = init_xms() ;
   if( (i >= 0x0200) && (i != -1) )
   {
      XMS_hand = -1 ;
      XMS_hand = xmsmalloc( (int)((1024000L/1024L)+1L) ) ; // 16 Visual Page
      if( XMS_hand == -1 )
      {
         printf( "\n XMS Memory 1024k Not Found !\n\a" ) ;
         exit( 0 ) ;
      }
      printf( "\n XMS Memory 1024k Install - Ok !\n" ) ;
      xmsfree( XMS_hand ) ;                               // XMS Memory Free
   } else {
      printf( "\n Error ! XMS Low Version !\n\a" ) ;
      exit( 0 ) ;
   }
}

void main_data_init( void )
{
   XMS_hand  = xmsmalloc( (int)((128000L/1024L)+1L) ) ; // 2 Visual Page
   XMS_air   = xmsmalloc( (int)((192000L/1024L)+1L) ) ; // 3 Visual Page
   XMS_back  = xmsmalloc( (int)((256000L/1024L)+1L) ) ; // 4 Visual Page
   XMS_pcm   = xmsmalloc( (int)((256000L/1024L)+1L) ) ; // 4 Visual Page
   XMS_image = xmsmalloc( (int)((192000L/1024L)+1L) ) ; // 3 Visual Page

   mcga1 = (unsigned char far *)farmalloc( 320L*200L+4L ) ;
   mcga2 = (unsigned char far *)farmalloc( 320L*200L+4L ) ;
   mcga3 = (unsigned char far *)farmalloc( 320L*200L+4L ) ;

   Open_Image_File( "bf.img" ) ;
   Open_Music_File( "bf.mus" ) ;
   Open_Voice_File( "bf.pcm", 1 ) ;               // mode 0 == File, 1 == Xms
   Open_Anima_File( "bf.ani" ) ;
}

void main_data_close( void )
{
   xmsfree( XMS_hand  ) ;                         // XMS Memory Free
   xmsfree( XMS_air   ) ;                         // XMS Memory Free
   xmsfree( XMS_back  ) ;                         // XMS Memory Free
   xmsfree( XMS_pcm   ) ;                         // XMS Memory Free
   xmsfree( XMS_image ) ;                         // XMS Memory Free

   farfree( mcga1 ) ;                             // Main Memory Free
   farfree( mcga2 ) ;                             // Main Memory Free
   farfree( mcga3 ) ;                             // Main Memory Free

   Close_Image_File() ;                           // Close Image Data File
   Close_Music_File() ;                           // Close Music Data File
   Close_Voice_File() ;                           // Close Voice Data File
   Close_Anima_File() ;                           // Close Animation Data File
}

void VoiceSetup( int sbl_attr, int lpt )
{
   initVoice( sbl_attr ) ;
   if( sbl_attr == COVOX ) setCovoxLPT( lpt ) ;

   setSampleRate( 8000 ) ;
}

extern char speed ;                        // Game Speed
int BfConfig( int load_save, int mesg_onoff )
{
   char file_name[] = "bf.cnf" ;
   FILE *configfile ;
   int  key ;
   char i ;

   if( !load_save )
   {
      configfile = fopen( file_name, "rb" ) ;
      if( mesg_onoff ) printf( "\n\n +**** Blue Falcon ****+" ) ;
      fread( &i, 1, 1, configfile ) ;
      adlib_card = InitAdLib() ;
      if( adlib_card && (i==1) )
      {
	 if( mesg_onoff ) printf( "\n ³ * AdLib Install !!  ³" ) ;
	 if( mesg_onoff ) printf( "\n ³   Music On          ³" ) ;
	 adlib_card = 1 ;
      }
      else
      {
	 if( mesg_onoff ) printf( "\n ³ * AdLib Not Install ³" ) ;
	 if( mesg_onoff ) printf( "\n ³   Music Off         ³" ) ;
	 adlib_card = 0 ;
      }

      fread( &i, 1, 1, configfile ) ;
      if( !Sb_Init() )
      {
	 SBLAutoDetect() ;
	 if( mesg_onoff ) printf( "\n ³ * SB Install !!     ³" ) ;
      }
      else
      {
	 if( mesg_onoff ) printf( "\n ³ * SB Not Install !! ³" ) ;
	 if( i == 3 ) i = 2 ;
      }
      if( i == 1 ) { if( mesg_onoff ) printf( "\n ³   Voice Off         ³" ) ; VoiceSetup( NO_VOICE, 0 ) ; }
      if( i == 2 ) { if( mesg_onoff ) printf( "\n ³   PC Speaker        ³" ) ; VoiceSetup( PC_SPEAKER, 0 ) ; }
      if( i == 3 ) { if( mesg_onoff ) printf( "\n ³   Sound Blaster     ³" ) ; VoiceSetup( SOUNDBLASTER, 0 ) ; }
      if( i == 4 ) { if( mesg_onoff ) printf( "\n ³   Covox LPT 1       ³" ) ; VoiceSetup( COVOX, 1 ) ; }
      if( i == 5 ) { if( mesg_onoff ) printf( "\n ³   Covox LPT 2       ³" ) ; VoiceSetup( COVOX, 2 ) ; }
      if( i == 6 ) { if( mesg_onoff ) printf( "\n ³   Covox LPT 3       ³" ) ; VoiceSetup( COVOX, 3 ) ; }

      fread( &i, 1, 1, configfile ) ;
      if( i == 1 ) { if( mesg_onoff ) printf( "\n ³   Low  Graphic Mode ³" ) ; speed = 0 ; } // Slow Game Play
      if( i == 2 ) { if( mesg_onoff ) printf( "\n ³   High Graphic Mode ³" ) ; speed = 1 ; } // Fast Game Play
      fread( &i, 1, 1, configfile ) ;
      if( i == 1 ) { if( mesg_onoff ) printf( "\n ³   Direct Game       ³" ) ; }
      if( i == 2 ) { if( mesg_onoff ) printf( "\n ³   Demo & Game       ³" ) ; }
      if( mesg_onoff ) printf(                "\n ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ" ) ;
      fclose( configfile ) ;
   }
   else
   {
      printf( "\n\n\n +******* Blue Falcon Setup *******+" ) ;
      printf(     "\n ³                                 ³" ) ;

      configfile = fopen( file_name, "wb" ) ;

      adlib_card = InitAdLib() ;
      if( adlib_card ) printf( "\n ³ AdLib Music Card Install !      ³" ) ;
      else             printf( "\n ³ AdLib Music Card Not Install !  ³" ) ;
      printf( "\n ³                                 ³" ) ;
      printf( "\n ³    1..        Music Off         ³" ) ;
      printf( "\n ³    Default..  Music On          ³" ) ;
      printf( "\n ³                                 ³" ) ;
      key = Getkey( 1 ) ;
      if( key == '1' ) { adlib_card = 0 ; i = 0 ; }
      else             { adlib_card = 1 ; i = 1 ; }
      fwrite( &i, 1, 1, configfile ) ;

      if( !Sb_Init() )
      {
	 SBLAutoDetect() ;
	 printf( "\n ³ Sound Blaster Card Install      ³" ) ;
      }
      else
	 printf( "\n ³ Sound Blaster Card Not Install  ³" ) ;
      printf( "\n ³                                 ³" ) ;
      printf( "\n ³    1..        No Voice          ³" ) ;
      printf( "\n ³    2..        Pc Speaker        ³" ) ;
      printf( "\n ³    3..        Sound Blaster     ³" ) ;
      printf( "\n ³    4..        Covox LPT 1       ³" ) ;
      printf( "\n ³    5..        Covox LPT 2       ³" ) ;
      printf( "\n ³    6..        Covox LPT 3       ³" ) ;
      printf( "\n ³                                 ³" ) ;
      key = Getkey( 1 ) ;
      if( key == '1' ) { VoiceSetup( NO_VOICE, 0 ) ;     i = 1 ; }
      if( key == '2' ) { VoiceSetup( PC_SPEAKER, 0 ) ;   i = 2 ; }
      if( key == '3' ) { VoiceSetup( SOUNDBLASTER, 0 ) ; i = 3 ; }
      if( key == '4' ) { VoiceSetup( COVOX, 1 ) ;        i = 4 ; }
      if( key == '5' ) { VoiceSetup( COVOX, 2 ) ;        i = 5 ; }
      if( key == '6' ) { VoiceSetup( COVOX, 3 ) ;        i = 6 ; }
      fwrite( &i, 1, 1, configfile ) ;

      printf( "\n ³    1..        Low  Mode         ³" ) ;
      printf( "\n ³    Default..  High Mode         ³" ) ;
      printf( "\n ³                                 ³" ) ;
      key = Getkey( 1 ) ;
      if( key == '1' ) { speed = 0 ; i = 1 ; }	 // Slow Game Play
      else             { speed = 1 ; i = 2 ; }   // Fast Game Play
      fwrite( &i, 1, 1, configfile ) ;

      printf( "\n ³    1..        Direct Game       ³" ) ;
      printf( "\n ³    Default..  Demo & Game       ³" ) ;
      printf( "\n ³                                 ³" ) ;
      printf( "\n ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ" ) ;
      key = Getkey( 1 ) ;
      if( key == '1' ) i = 1 ;
      else             i = 2 ;
      fwrite( &i, 1, 1, configfile ) ;

      fclose( configfile ) ;
   }

   key = i  ;
   return i ;
}

void black_palette( int mode )
{
   unsigned char iPal[256][3] ;
   int i, j ;

   if( !mode )
   {
      for( i=0; i<256; i++ )
      {
	iPal[i][0] = 0 ;
	iPal[i][1] = 0 ;
	iPal[i][2] = 0 ;
      }
      time_int_push = 1 ;
      SetRgbPaletteSnow( (unsigned char *)iPal ) ;
      time_int_push = 0 ;
   }
   else
   {
      for( i=16; i>0; i-- )
      {
	 for( j=0; j<256; j++ )
	 {
	   iPal[j][0]=(Pal[j][0]*i)>>4;
	   iPal[j][1]=(Pal[j][1]*i)>>4;
	   iPal[j][2]=(Pal[j][2]*i)>>4;
	 }
	 time_int_push = 1 ;
	 SetRgbPaletteSnow( (unsigned char *)iPal ) ;
	 time_int_push = 0 ;
      }
   }
}

void white_palette( void )
{
   unsigned char iPal[256][3] ;
   int i, j ;

   for( i=0; i<16; i++ )
   {
      for( j=0; j<256; j++ )
      {
	 iPal[j][0]=(Pal[j][0]*i)>>4;
	 iPal[j][1]=(Pal[j][1]*i)>>4;
	 iPal[j][2]=(Pal[j][2]*i)>>4;
      }
      time_int_push = 1 ;
      SetRgbPaletteSnow( (unsigned char *)iPal ) ;
      time_int_push = 0 ;
   }
}

void key_clear( void )
{
   while( kbhit() )
   {
      if( getch()==0 ) getch() ;
   }
}
