    /*
        EXAMPLE3.C
        ADLIB IMS Play Test

        Compile option  : bcc -ml -K example3.c adlib.obj play.obj timer.obj
        usage           : example3 ???.ims standard.bnk

        (C) Kheun Sa Ram  1992 - 1994

        ·¡ ­¡¯a· ¶w· ¸á¸bŠ¥·e 'Çe¬aœq'µA‰A ·¶¯s“¡”a.

    */
    #include <conio.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <dos.h>
    #include <string.h>
    #include "adlib.h"
    #include "convert.h"
    #include "KorHdr.h"

int main(void)
{
    char imsname[80];
    char bnkname[80];
    struct ImsHeader mH;
        FILE *meloFile;

    if( _argc>1 )
        strcpy( imsname, _argv[1] );
    else
    {
	printf("Usage : Aplay.exe ims_file_name [ bnk_file_name ]\n");
        return 1;
    }
    if( _argc>2 )
        strcpy( bnkname, _argv[2] );
    else
        strcpy( bnkname, "STANDARD.BNK" );
    if( BoardCheck(0x388)==0 ) puts("Adlib Not found\n");
    graph_init(-1);
    gColor(0, 6);
    /* read the music file's header: */
    meloFile = fopen(imsname, "rb" );
    fread( &mH, sizeof( struct ImsHeader),1, meloFile );
    fclose(meloFile);
//    gprintf(1,4,1, "Version     : %2x %2x\n",mH.majorVersion,mH.minorVersion);
//    gprintf(1,5,1, "tuneId      : %ld\n",mH.tuneId);
    gprintf(1,3,1, "Song File : %s", imsname);
    gprintf(1,5,1, "‘¡œ ¹A¡¢ : %s", mH.tuneName);
    if( run_adlib( imsname , bnkname ,1)==0 )
    {
//      printf("Current Tick : Kasa Tick\n");
        while( PlayingState()==1 ) /* ‘¡œ µeºˆa ‰­¢–A‰¡ ·¶“e»¡ ˆñ¬a */
        {
        /*
            Ñe¹A µeº–A‰¡ ·¶“e TICK·i Îa¯¡Ðe”a.
            aº—µA ·¡¦¦…· ‹¡“w·i ÑÂ¸wÐa¡e ˆa¬aÂ¸â—w·
            ‹¡“w·i  e—i ® ·¶”a.
        */
//                printf(" %8ld      %5u\r",GetCurrentTick(),(unsigned int)GetCurrentTick()/8L);
        /*
            Ç¡Ÿi Ã¡¡e µeºŸi  ñÂ‰¡ ¨a»¥”a.
        */
        if( kbhit() ) break;
        }
//      printf("\n");
        clear_mus(1);
    }
    SoundColdInit(0x388);
    graph_close();
    return (0);
}
