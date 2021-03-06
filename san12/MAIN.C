
/*                                                        */
/*                                                        */
/*              Trance Word version 0.1 Beta               */
/*                                                        */
/*            (C) Copyright 1991 Magic Book               */
/*                                                        */
/*                   Start Up Routine                     */
/*                                                        */

#include "stddef.h"
#include "crt.h"
#include "handrv.h"
#include "hwindow.h"
#include "memory.h"
#include "filename.h"
#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include "const.h"
#include "edit.h"
#include "sanver.h"
#include "popup.h"
#include <io.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "hanja.h"


extern unsigned _stklen=8096;

void interrupt (*CriticalBuf)(),
     interrupt (*CtrlBrkBuf)();

void interrupt CtrlBrkHandler(void)
{

}

void interrupt CriticalHandler(word bp,word di,word si,
                              word ds,word es,
                              word dx,word cx,word bx,word ax,
                              word ip,word cs,word flags )
{
    char *i;

    switch (di&0x00FF) {
       case   0 : i="�a�� �w�� �S�� ���� ���s���a."; break;
       case   1 : i="�i�� ���e �e���A�a �a�����a.";        break;
       case   2 : i="�a�a���a���� �i�a ���s���a.";     break;
       case   4 : i="�A���� �A�a �A��a �i��Ж�s���a.";           break;
       case   9 : i="�a���� �����a ���s���a.";break;
       case 0xA : i="�a���a ���a�w�s���a.";         break;
       case 0xB : i="�����a ���a�w�s���a.";          break;
       default  : i="���a �១�A�� �A��a �i��Ж�s���a.";      break;
    }
    Push(InverseF);
    HInverse();
    HOpenWindow(3,15,58,18);
    DrawString(4,16,i);
    DrawString(4,17,"�a�� ���� �i�a�a? �a�� �a�e ���a�a? [ Retry / Fail ]");
    while (True) {
      GetKey();
      if (Way=='R' || Way=='r') {
          ax=(ax&0xFF00)+1;
          break;
      }
      if (Way=='F' || Way=='f') {
          ax=(ax&0xFF00)+3;
          break;
      }
    }
    HCloseWindow();
    Pop(InverseF);
}

word SetHelpData(void);

void main(int argc,char *argv[])
{
     char _Dir[MAXDIR],_Name[MAXFILE];
     byte buf[81];
     extern int HanjaFileHandler;

     fnsplit(argv[0],(char*)SysPath,_Dir,_Name,_Name);
     strcat(SysPath,_Dir);
     OutPortB(0x3BF,2);
     FExpand(SysPath);
     if (RetrieveOptions()!=OK) {
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in CONFIGURATION.");
         puts("Please, run 'SANSETUP' for this version first.");
         exit(1);
     }
     if (SetHelpData()!=OK) {
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading HELP FILE.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,"sanvideo.drv");
     if (OpenHanDrv(WorkFileName)!=OK) {
         clrscr();
         puts(SanVersion);
         puts("\nError in reading CRT Driver.");
         puts("Please, run 'SANSETUP' for this version first.");
         exit(1);
     }
     GrMode();
     ScnRight=VideoRight;
     ScnLeft =1;
     ScnBot  =VideoBot-2;
     ScnTop  =3 ;
     ScnHigh =ScnBot-ScnTop+1;
     ScnWidth=ScnRight-ScnLeft+1;
     HClrScr();
     if (InitWindow()!=OK ||
         InitPopUp()!=OK  ||
         InitHanja()!=OK  ||
         (UnKillBuf    =(byte far * far *)farmalloc(MaxUnKillBuf*4))==NULL ||
         (Task[0].Paper=(byte far * far *)farmalloc((Task[0].MaxPaper+1)*4))==NULL ||
         (Task[1].Paper=(byte far * far *)farmalloc((Task[1].MaxPaper+1)*4))==NULL ||
         (Task[2].Paper=(byte far * far *)farmalloc((Task[2].MaxPaper+1)*4))==NULL ||
         (DiskBuf=(byte far *)farmalloc(DiskBufSize))==NULL ||
         (PickList=(struct PickListEle (*)[])farmalloc(sizeof(*PickList)))==NULL ||
         farcoreleft()<5*1024) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nNot enough work space.");
         exit(1);
     }
     OpenPickList();
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,"san.lin");
     if (SetExtEng(WorkFileName)==Error) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("LINE FONT." );
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,HanFontName);
     if (SetHan(WorkFileName)!=OK) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("HAN-GUL FONT.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,EngFontName);
     if (SetEng(WorkFileName)==Error) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("ENGLISH FONT.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,GrfFontName);
     if (SetGrf(WorkFileName)==Error) {
         TextMode();
         clrscr();
         puts(SanVersion);
         puts("\nOccurred error in reading ");
         puts("GRAPHIC CHARACTER.");
         exit(1);
     }
     strcpy(WorkFileName,SysPath);
     strcat(WorkFileName,"SAN.HAN");
     HanjaFileHandler=_open(WorkFileName,0);
     if (HanjaFileHandler==-1) HanjaOff();
     ChangeFontF=False;
     InitCrt();

     CtrlBrkBuf =getvect(0x1B);
     CriticalBuf=getvect(0x24);
     setvect(0x1B,CtrlBrkHandler);
     setvect(0x24,CriticalHandler);
     if ( argc>1 && strlen(argv[1])<sizeof(WorkFileName)) {
         strcpy (WorkFileName,argv[1]);
         FileNameSetF=True;
     } else {
     WorkFileName[0]=0;
         FileNameSetF=False;
     }
     HNormal();
     MaxBufSize=farcoreleft();

/*     {
        unsigned long count,i,j=0;
        word k,l;

       i=*(unsigned long *)Pointer(0x40,0x6C);
        for (count=0; j<2 && j>=0 ; count++) {
            j=*(unsigned long*)Pointer(0x40,0x6C)-i;
        }


       DrawString(4,2,"�e�e ������� �a���a�������a.");
       DrawString(4,3,"�������e ���e �a�w�a�a ���� �a�w�i �a���a���i ���a�� ������");
       DrawString(4,4,"Ӂ�w�� ���i�� ������ �a�b�i �����a�e �������a.");
       DrawString(4,5,"���e �� �����e�i �a�wЁ ������ ���� Ӂ�w�� ���a��  ���e  �A�I");
       DrawString(4,6,"�a����  'magicbk'  �a�� �A�� �����a.");
       DrawString(4,7,"���q�a�� �� ���a���e �����a�a�� ���s���a.");
       DrawString(4,8,"�a�A�e �a�w�i �i�a�a���V�s���a.");

       DrawString(4,10,"............................................................");


       for (k=0 ; k<60 ; k++) {
         for (l=0 ; l<9 ; l++)
            for (i=0 ; i<count ; i++)
               j=*(unsigned long*)Pointer(0x40,0x6C)-i;
         DrawEng(4+k,10,' ');
         if (KeyPressed())
       }
       NextChar=FirstAvailable;
     }
  */


     Editor();

     /* Watch Out 'BufErr' */

     if (HanjaFileHandler!=-1) _close(HanjaFileHandler);
     ClosePickList();
     TextMode();
     clrscr();
     puts(SanVersion);
     setvect(0x1B,CtrlBrkBuf);
}
