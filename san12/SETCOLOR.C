
//
//        Set VGA Driver Color 1.0
//
//        by Magic Book
//

#include "stddef.h"
#include "handrv.h"
#include "crt.h"
#include "memory.h"
#include "colver.h"
#include <stdio.h>
#include <io.h>
#include <dos.h>

void SetBackgroundColor (word Data)
{
      *(byte*)Pointer(HanDrvSeg,0x102+20+60+16)=Data;
      _ES=HanDrvSeg;
      _DX=0x102+20+60+16;
      _AX=0x1002;
      Inter(0x10);
}

void SetFontColor(word Data)
{
     *(byte*)Pointer(HanDrvSeg,0x102+20+60+12+1+18)=Data;
     _ES=HanDrvSeg;
     _DX=0x102+20+60+16;
     _AX=0x1002;
     Inter(0x10);
}

void SetMessage(void)
{
     DrawString(7,3 ,"     �� �a���a���e '�e  1.XX' ��  �q�A  �A���A�e  VGA/EGA  ���e");
     DrawString(7,4 ,"�b�wá�i ���e ���a �១  ������  ���i  �i���a��  ��Ё  �e�i��");
     DrawString(7,5 ,"�v�s���a. ���e �a�� �w���E �i�A�i ���w�a�a�� ���a����  ���w���i");
     DrawString(7,6 ,"�i���a��ҁ �a�a���� �����A �����a ��w�A���a�e �a�q '�e'�� ����");
     DrawString(7,7 ,"���A�e ��Ȃ�E ���i �a���� �b�� �a�� �� ���s���a.");
     DrawString(7,9 ,"UP/DOWN ARROW : ���i �e�w�s���a.");
     DrawString(7,10,"RETURN        : �����i ��w �s���a.");
     DrawString(7,11,"ESC           : ��w ���� �b��i ���� �s���a.");
     DrawString(7,12,"SPACE         : �e�� �e�w���� ���i ���a��/���w�� �� ��Ȃ �s���a.");
     DrawString(7,14,"���a��        :");
     DrawString(7,15,"���w��        :");
     DrawString(7,16,"�e�w���� ��   :");
}

void SelectColor(void)
{
     word Item=0;
     word FontColor;
     word BackColor;
     char Buf[80];

     FontColor=*(byte*)Pointer(HanDrvSeg,0x102+20+60+12+1+18);
     BackColor=*(byte*)Pointer(HanDrvSeg,0x102+20+60+16);
     do {
        if (Item==0) DrawString(23,16,"���w��");
        else         DrawString(23,16,"���a��");
        sprintf(Buf,"%u   ",FontColor);  DrawString(23,14,Buf);
        sprintf(Buf,"%u   ",BackColor);  DrawString(23,15,Buf);

        GetKey();
        if (Item==0) {
           switch (Way) {
             case Down  : BackColor--;
                          BackColor%=64;
                          break;
             case Up    : BackColor++;
                          BackColor%=64;
                          break;
           }
           SetBackgroundColor(BackColor);
        } else {
           switch (Way) {
             case Down : FontColor--;
                         FontColor%=64;
                         break;
             case Up   : FontColor++;
                         FontColor%=64;
                         break;
           }
           SetFontColor(FontColor);
        }
        if (Way==' ') Item=!Item;
     } while (Way!=ESC && Way!=Return);
}

char WorkFile[81];

word SaveDriver(void)
{
     long i;
     int  handler;

     if ((handler=_open(WorkFile,0))==-1)       return Error;
     i=filelength(handler);
     _close(handler);
     if ((handler=_creat(WorkFile,FA_ARCH))==-1) return Error;
     if (_write(handler,Pointer(HanDrvSeg,0x100),i)==-1) return Error;
     return OK;
}
void main(void)
{

     word i,j;

     InitCrt();
     ClrScr();
     clrscr();
     puts(ColVersion);
     puts("1.VGA Driver");
     puts("2.EGA Driver");
     puts("3.Quit");
     puts("\nSelect Video Card :");
     do {
       i=getch();
     } while (i!='1' && i!='2' && i!='3');
     switch (i)  {
       case '1' : strcpy (WorkFile,"VGA640.DRV");
                  break;
       case '2' : strcpy (WorkFile,"EGA640.DRV");
                  break;
       case '3' : clrscr();
                  puts(ColVersion);
                  exit(0);
     }
     if (OpenHanDrv(WorkFile)!=OK) {
         clrscr();
         puts(ColVersion);
         puts("Error in reading VGA640/EGA640 Driver.");
         exit(1);
     }
     if (SetEng("SAN.ENG")!=OK) {
         clrscr();
         puts(ColVersion);
         puts("Error Occurred in Reading 'SAN.ENG'");
         exit(1);
     }
     if (SetHan("SAN.KOR")!=OK) {
         clrscr();
         puts(ColVersion);
         puts("Error Occurred in Reading 'SAN.KOR'");
         exit(1);
     }
     GrMode();
     HClrScr();
     InitCrt();
     SetMessage();
     SelectColor();
     if (Way==Return) {
       HClrScr();
       DrawString(7,10,"     ���q, ���� �����E ���� �����i VGA640.DRV �a EGA640.DRV  ��");
       DrawString(7,11,"��Ȃ�a�� �����A ��w�a�A �S���a.  �e,  SANVIDEO.DRV  �A�e  �a��");
       DrawString(7,12,"��w�a�e ������ ���a�a�� ���� �����E ���i  �a�w�a����  ��Ё��e");
       DrawString(7,13,"��Ȃ�a�� VGA640 ���a EGA640  ��  �a�a�i  SANVIDEO.DRV  ��  ����");
       DrawString(7,14,"���aЁ ���a�� �s���a.");
       getch();
       TextMode();
     clrscr();
       if (SaveDriver()!=OK) {
           puts(ColVersion);
           puts("Error Occurred. Driver Not Saved.");
           exit(1);
       }
       puts(ColVersion);
       puts("VGA640.DRV/EGA640.DRV Successfully Changed.");
       puts("Please Make New 'SANVIDEO.DRV'.");
     } else {
       TextMode();
       clrscr();
       puts(ColVersion);
       puts("Driver Color Not Saved.\n");
     }
}
