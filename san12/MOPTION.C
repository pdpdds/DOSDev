/*                                                      */
/*                                                      */
/*             Trance Word vesion 0.5 Beta              */
/*                                                      */
/*            (C) Copyright 1991 Magic Book             */
/*                                                      */
/*                  Environmemt & Menu                  */
/*                                                      */

#include "stddef.h"
#include "handrv.h"
#include "memory.h"
#include "const.h"
#include "edit.h"
#include "hwindow.h"
#include "crt.h"
#include "popup.h"
#include "filename.h"
#include "print.h"
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <io.h>

word extern BlockSpace;

        void  SelectKeyboard(void)
        {
              word i;
              word OKF;
              char *KeyboardList[]={
                   "��ɷ �a�w�a�� ���� �E�� �s���w �e�i �a�e�� ���鯢 �����a.",
                   "��q,�a���A,�{ �����w �i�A�a �����A�� ���e �e�i �a�e�� �A�鯢 �����a."
                   };

              ExplainList=KeyboardList;
              HInverse();
              HOpenWindow(17,6,28,9);
              HOpenMenu();
              HSetItem(18,7," D ���鯢",10,'D');
              HSetItem(18,8," S �A�鯢",10,'S');
              HNormal();
              i=(KeyboardType==2)?0:1;
              HManualSelect(i);
              OKF=HSelect();
              if (OKF==OK) HotKeyEndF=True;
              HCloseMenu();
              HCloseWindow();
              if (OKF==OK) KeyboardType=(Item==0)?2:3;
        }

        void   BlockOption(void)
        {
               word OKF;
               int  i;
               char Buf[10];
               char Left[10],Right[10],Indent[10],SpaceLine[10];
               char *BlockOptionList[]={
                    "�E�� �����i�i ���s���a.",
                    "���e�� �����i�i ���s���a.",
                    "���e �w�� ���i�� ���e�� ������ �i�a�a�� ���a���i ���s���a.",
                    "���e �w�� ���i�� ���� ���a���� ���� ���� �����a.",
                    "���e �w�� ���i�� �e��i ���s���a."
                    };

               ExplainList=BlockOptionList;
               Push(InverseF);
               HInverse();
               HOpenWindow(17,8,49,14);
               HOpenMenu();
               HSetItem(18, 9," L �i�� �E��   ��á :",20,'L');
               HSetItem(18,10," R �i�� ���e�� ��á :",20,'R');
               HSetItem(18,11," I �i�a �a��   �a�� :",20,'I');
               HSetItem(18,12," S ���� ���a�� ���� :",20,'S');
               HSetItem(18,13," T ���i �w��        :",20,'T');
               Pop(InverseF);
               do {
                  HInverse();
                  switch (BlockSortType) {
                    case 0 : DrawString(40,13,"���a�e��");
                             break;
                    case 1 : DrawString(40,13,"���e��");
                             break;
                    case 2 : DrawString(40,13,"�a�q���");
                  }
                  sprintf(Left,"%u",BlockLeft);
                  strcpy(Buf,Left); strcat(Buf,"   ");
                  DrawString(40, 9,Buf);
                  sprintf(Right,"%u",BlockRight);
                  strcpy(Buf,Right); strcat(Buf,"   ");
                  DrawString(40,10,Buf);
                  sprintf(Indent,"%d",(int)BlockIndent);
                  strcpy(Buf,Indent); strcat(Buf,"   ");
                  DrawString(40,11,Buf);
                  sprintf(SpaceLine,"%u",BlockSpace);
                  strcpy(Buf,SpaceLine); strcat(SpaceLine,"   ");
                  DrawString(40,12,Buf);
                  HNormal();
                  OKF=HSelect();
                  if (OKF!=OK || HotKeyF) break;
                  switch (Item) {
                    case 0 :if (HGetEng("�E�� ��á�i �ẁ�A�a.",Left,5)==OK) {
                                i=atoi(Left);
                                if (i<1) HSayMes(" 1 ���a�e �ᴡ �V�e�A�a.");
                                else if (i>BlockRight-5) HSayMes(" ���e�� �{�� ���aЁ �ᢁ �e�������a.");
                                else if ((int)(BlockRight-i)<(int)(BlockIndent)+5) HSayMes("�i�a�a�� ���A ��Ё�� �e�������a.");
                                else BlockLeft=i;
                            }
                            break;
                    case 1 :if (HGetEng("���e�� ��á�i �ẁ�A�a.",Right,5)==OK) {
                                i=atoi(Right);
                                if (i>254) HSayMes(" 254 ���a�e �b�a�� �s���a.");
                                else if (i<BlockLeft+5) HSayMes(" �E���{�� ���aЁ �ᢁ �e�������a.");
                                else if ((int)(i-BlockLeft)<(int)(BlockIndent)+5) HSayMes("�i�a�a�� ���A ��Ё�� �e�������a.");
                                else BlockRight=i;
                            }
                            break;
                    case 2 :if (HGetEng("�y�e���a �i�a �i�a�a?",Indent,5)==OK) {
                                i=atoi(Indent);
                                if ((int)i>(int)(BlockRight-BlockLeft-5)) HSayMes(" �����{�� ���aЁ �ᢁ �e�� �����a.");
                                else BlockIndent=i;
                            }
                            break;
                    case 3 :if (HGetEng("�y �����a �����a�a?",SpaceLine,5)==OK) {
                                i=atoi(SpaceLine);
                                if (i>10) HSayMes(" �� �a�a�� ���b�� �g�a�����a? 10���e �b�v�a�e ���V�e�A�a.");
                                else BlockSpace=i;
                            }
                            break;
                    case 4 :BlockSortType=(BlockSortType+1)%3;
                  }
               } while (!HotKeyF);
               if (Way!=ESC) HotKeyEndF=True;
               HCloseMenu();
               HCloseWindow();
        }

             void   PrinterSort(void)
             {
                    word OKF;
                    int  i;
                    char **BufList;
                    char *PrinterSortList[]={
                         "���s�w �e�i š�a�� �a�� �b�s���a.",
                         "�Ŭ��w �e�i š�a�� �a�� �b�s���a.",
                         "���s�w �e�i š�a�� �a�� �b�s���a. З�e ���� �a�� ������ �a�w�s���a.",
                         "�Ŭ��w �e�i š�a�� �a�� �b�s���a. З�e ���� �a�� ������ �a�w�s���a.",
                         "���e �a�w�a�� ���� �s���a.  8ϥ ���a �a����",
                         "���e �a�w�a�� ���� �s���a. 24ϥ ���a �a����",
                         "���e �a�w�a�� ���� �s���a. �A���a �U/�a�� �b�A",
                         "���e �a�w�a�� ���� �s���a. �A���a �U/�b�A",
                         "���e �a�w�a�� ���� �s���a. �A���a �U/��ɷ",
                         "���e �a�w�a�� ���� �s���a. �A���a �U/�a�A"
                         };

                    BufList=ExplainList;
                    ExplainList=PrinterSortList;
                    Push(InverseF);
                    HInverse();
		    HOpenWindow(31,8,50,19);
		    HOpenMenu();
		    HSetItem(32, 9," Normal     /���s",18,0);
		    HSetItem(32,10," Normal     /�Ŭ�",18,0);
		    HSetItem(32,11," EPSON      /���s",18,0);
		    HSetItem(32,12," EPSON      /�Ŭ�",18,0);
		    HSetItem(32,13," EPSON 8pin /�a��",18,0);
		    HSetItem(32,14," EPSON 24pin/�a��",18,0);
		    HSetItem(32,15," Jet 300DPI /�a��",18,0);
		    HSetItem(32,16," Jet 150DPI /�a��",18,0);
		    HSetItem(32,17," Jet 100DPI /�a��",18,0);
		    HSetItem(32,18," Jet  75DPI /�a��",18,0);
                    Pop(InverseF);

                    HNormal();
                    OKF=HSelect();
                    ExplainList=BufList;
                    if (OKF==OK && !HotKeyF) PrintingType=Item;
                    else if (Way==ESC) Way=Error;
                    HCloseMenu();
                    HCloseWindow();
             }

        void   PrinterOption(void)
        {
               word OKF;
               int  i;
               char Left[10];
               char Page[10];
               char Top [10];
               char Bot [10];
               char Port[10];
               char Space[10];
               char *PrinterOptionList[]={
                    "�b ���� ���� ���b �i�i ���s���a.",
                    "������ �� ���i �ẁ�A�a.",
                    "������ �y �弁 ������ ����i �i�a�a?",
                    "������ �y �弁 ���a�� ����i �i�a�a?",
                    "���� ���a�� �e�b�i ��a������ �i�a�a?",
                    "�a�w�a�� ���� �a����A �i�x�A �x¡ ���A�a.",
                    "�a�w�a�� ���� �a����A �i�x�A �x¡ ���A�a.",
                    "�a����� �e�i�A�� ���e �w�i͡�a�� ��ѡ�i �ẁ�A�a."
                    };

               ExplainList=PrinterOptionList;
               Push(InverseF);
               HInverse();
               HOpenWindow(17,9,49,17);
               HOpenMenu();
               HSetItem(18,10," L ���b �i   ",13,'L');
               HSetItem(18,11," G ���� �a�� ",13,'G');
               HSetItem(18,12," T ���b З   ",13,'T');
               HSetItem(18,13," B �añ З   ",13,'B');
               HSetItem(18,14," S �a�� ���� ",13,'S');
               HSetItem(18,15," R ���� �w�� ",13,'R');
               HSetItem(18,16," P �w�i ͡�a ",13,'P');
               Pop(InverseF);
               do {
                  HInverse();
                  switch (PrintingType) {
                    case 0 : DrawString(32,15,"Normal     /���s"); break;
                    case 1 : DrawString(32,15,"Normal     /�Ŭ�"); break;
                    case 2 : DrawString(32,15,"EPSON      /���s"); break;
                    case 3 : DrawString(32,15,"EPSON      /�Ŭ�"); break;
                    case 4 : DrawString(32,15,"EPSON 8pin /�a��"); break;
                    case 5 : DrawString(32,15,"EPSON 24pin/�a��"); break;
                    case 6 : DrawString(32,15,"Jet 300DPI /�a��"); break;
                    case 7 : DrawString(32,15,"Jet 150DPI /�a��"); break;
                    case 8 : DrawString(32,15,"Jet 100DPI /�a��"); break;
                    case 9 : DrawString(32,15,"Jet  75DPI /�a��"); break;
                  }
                  sprintf(Left,"%u",LeftMargin);
                  HClrBox(32,10,35,10);
                  DrawString(32,10,Left);

                  sprintf(Page,"%u",PageLength);
                  HClrBox(32,11,35,11);
                  DrawString(32,11,Page);

                  sprintf(Top,"%u",TopMargin);
                  HClrBox(32,12,35,12);
                  DrawString(32,12,Top);

                  sprintf(Bot,"%u",BotMargin);
                  HClrBox(32,13,35,13);
                  DrawString(32,13,Bot);

                  sprintf(Space,"%u",SpaceLength);
                  HClrBox(32,14,35,14);
                  DrawString(32,14,Space);

                  sprintf(Port,"%u",PrinterPort);
                  DrawString(32,16,Port);

                  HNormal();
                  OKF=HSelect();
                  if (OKF!=OK || HotKeyF) break;
                  switch (Item) {
                    case 0 :if (HGetEng("�E�� ���b ��á�i ��ẁ�A�a.",Left,5)==OK) {
                                i=atoi(Left);
                                if (i<1) HSayMes(" 0 ���a�e �ᴡ �V�e�A�a.");
                                else if (i>300) HSayMes(" �ᢁ �e ��á �a�����a? �a�� �ẁ�A�a.");
                                else LeftMargin=i;
                            }
                            break;
                    case 1 :if (HGetEng("���� �����a �y �����a�a?.",Page,5)==OK) {
                                i=atoi(Page);
                                if (i<1) HSayMes(" 0 ���a�e �ᴡ �V�e�A�a.");
                                else if (i>400) HSayMes(" �ᢁ �e ��á �a�����a? �a�� �ẁ�A�a.");
                                else PageLength=i;
                            }
                            break;
                    case 2 :if (HGetEng("����i ���b�i ���i ��ẁ�A�a.",Top,5)==OK) {
                                i=atoi(Top);
                                if (i<1) HSayMes(" 0 ���a�e �ᴡ �V�e�A�a.");
                                else if (i>PageLength) HSayMes(" ���� �������a �e ��á �����a. �a�� �ẁ�A�a.");
                                else TopMargin=i;
                            }
                            break;
                    case 3 :if (HGetEng("�y�弁 ���a�� ���� �i�a�a?",Bot,5)==OK) {
                                i=atoi(Bot);
                                if (i<1) HSayMes(" 0 ���a�e �ᴡ �V�e�A�a.");
                                else if (i>PageLength) HSayMes(" ���� �������a �e ��á �����a. �a�� �ẁ�A�a.");
                                else BotMargin=i;
                            }
                            break;
                    case 4 :if (HGetEng("���e �a���i ��a������ �i�a�a? ",Space,5)==OK) {
                                i=atoi(Space);
                                if (i<0) HSayMes(" 0 ���w ���a�� �s���a.");
                                else if (i>200) HSayMes(" �ᢁ �e ��á �{�s���a.");
                                else SpaceLength=i;
                            }
                            break;

                    case 5 :PrinterSort();

                            /*PrintingType++;
                            if ( PrintingType >9 ) PrintingType=0;*/

                            break;
                    case 6:if (PrinterPort==1) PrinterPort=2;
                           else PrinterPort=1;
                           break;
                  }
               } while (!HotKeyF);
               if (Way!=ESC) HotKeyEndF=True;
               HCloseMenu();
               HCloseWindow();
        }


        void SetEngFont(void);
        void SetHanFont(void);

        /*void InverseScreen(void)
        {
             HInverse();
             Way=AltO;
             SetHotKeyF();
             SetEditScn();
             PrintXYpos();
             SetMenuScn();
             HNormal();
             PrintXYpos();
             SetScnF=True;
        } */

        void SetFileExt(void)
        {
             byte Buf[80];

             strcpy(Buf,StdExt);
             if (HGetEng("����á�� �a�� ���� �¸w�a�i �ẁ�A�a.",Buf,79)==OK) {
                strcpy(StdExt,Buf);
                strupr(StdExt);
             }
        }

void OptionMenu(void)
{
    word OKF;
    word static OptionN;
    char *OptionList[]={
         "���e�A �b�a�e �e�i �a�w�i �e�w�s���a.",
         "���e�A �b�a�e �i�a�U �a�w�i �e�w�s���a.",
         "���鯢�� �A�鯢�� �e�i �a�e�i �e�w�s���a.",
         "���a�a�a �a���ᝡ�� ���b�� �a�w�i �e�i š�a�i ��Ȃ �s���a.",
         "�i�� ���i ���A �a�w�A�e ���� �t�i �e�w�s���a.",
         "���� ����A ���e �a�w�i�i �e�w �s���a.",
         "�����t�a�� �a�w�A�e ���� ���a�E�e�i �e�w�s���a.",
         "�e�� ������ �a�w �A�e�A �b���a�� ����i �a���a�� ���� �i���� ���s���a.",
         "���� ��w�� �����E �����i *.BAK �a�� �����i���i �i���s���a.",
         "���a ���w���A ���� ��w�a���i ���i�� �i���s���a.",
         "���e �������A �i�A(ǡ) ���b�� ���a�e ���b�A �b�b �����i�� ���s���a.",
         "��Ȃ,��З,���b �a���e�� �ŝe�E ���w�i ��w�a�a �e�� �������A ���� �i�����a."
    };

    Push(InverseF);
    HInverse();
    HOpenWindow(16,1,36,17);
    HOpenMenu();
       HSetItem(17, 2," H �e�i �i��",13,'H');
       HSetItem(17, 3," F �w�� �i��",13,'F');
       HDrawBar(17,35,4);
       HSetItem(17, 5," K �e�i �a�e",13,'K');
       HSetItem(17, 6," C �e�i š�a",13,'C');
       HSetItem(17, 7," B �i�� ���i",13,'B');
       HSetItem(17, 8," I ���� ����",13,'I');
       HSetItem(17, 9," E ���� ����",13,'E');
       HDrawBar(17,35,10);
       HSetItem(17,11," P ���b ����",13,'P');
       HSetItem(17,12," M ���� ����",13,'M');
       HSetItem(17,13," A ���e ��w",13,'A');
       HSetItem(17,14," Q ���� ����",13,'Q');
       HDrawBar(17,35,15);
       HSetItem(17,16," S �ŉw ��w",13,'S');
    Pop(InverseF);
    HManualSelect(OptionN);
    do {
      Push(InverseF);
      HInverse();
      if (HanGulCodeType==1) DrawString(31,6,"�Ŭ�");
      else                   DrawString(31,6,"���s");
      if (PrepareF) DrawString(31,11,"�q");
      else          DrawString(31,11,"�q");
      if (BackUpF)  DrawString(31,12,"�q");
      else          DrawString(31,12,"�q");
      if (OsSaveF)  DrawString(31,13,"�q");
      else          DrawString(31,13,"�q");
      switch (SmartRefleshF) {
         case 1 :DrawString(31,14,"�q "); break;
         case 2 :DrawString(31,14,"�q'"); break;
         case 0 :DrawString(31,14,"�q ");
      }
      Pop(InverseF);
      ExplainList=OptionList;
      OKF=HSelect();
      OptionN=NMP->NowPos;
      if (OKF==Error) break;
      switch (OptionN) {
        case 0 :SetHanFont();          break;
        case 1 :SetEngFont();          break;
        case 2 :SelectKeyboard();      break;
        case 3 :HanGulCodeType=!HanGulCodeType; break;
        case 4 :BlockOption();         break;
        case 5 :PrinterOption();       break;
        case 6 :SetFileExt();          break;
        case 7 :PrepareF=!PrepareF;    break;
        case 8 :BackUpF =!BackUpF;     break;
        case 9 :OsSaveF =!OsSaveF;     break;
        case 10:SmartRefleshF=(SmartRefleshF+1)%3; break;
        case 11:if (SaveOptions()!=OK) HSayIOErr();
                else {
                      HSayMes(" �ŉw ��w ���� 'SAN.CNF' �a ���� �e�i�� �v�s���a.");
                      HotKeyEndF=True;
                }
                break;
      }
    } while (!HotKeyF && !HotKeyEndF);
    HCloseWindow();
    HCloseMenu();
}
