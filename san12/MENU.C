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
#include "sanver.h"
#include <dir.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <io.h>

void  SetEditScn(void);
void SetMenuScn(void);
void EditDel(void);
word extern BlockSpace;

void OptionMenu(void);
void QuitWork(void);
void FileMenu(void);

void RunMenu(void)
{
    word i;
    word OKF;
    word static RunN;
    word ExitF;
    byte Buf[21];
    byte Buf2[80];

    if (HotKeyF==True) {
        RunN=(Way==F5) ? 0 :
             (Way==AltF6) ? 1 :
             (Way==AltF7) ? 2 :
             (Way==AltF8) ? 3 :
             (Way==AltF9) ? 4 : 0;
        HotKeyF=False;
        HotKeyEndF=True;
        Run(RunN);
        return;
    }
    Push(InverseF);
    HInverse();
    HOpenWindow(8,1,36,7);
    Pop(InverseF);
    Explain("�a�w�a ���b �w�w. 'E' �i�A�i ���a���e ���w �e�w�� �a�w �s���a.");
    do {
        Push(InverseF);
        HInverse();
        for (i=0 ; i<5 ; i++) {
          DrawString(10,i+2,RunTitle[i]);
          if (i==0) sprintf(Buf,"   F%d",i+5);
          else               sprintf(Buf,"AltF%d",i+5);
          DrawString(30,i+2,Buf);
        }
        Pop(InverseF);
        HSetInverse(9,RunN+2,28,RunN+2);
        GetKey();
        SetHotKeyF();
        HSetInverse(9,RunN+2,28,RunN+2);
        if (HotKeyF) break;
        ExitF=True;
        switch(Way) {
          case Up     :
          case CtrlE  :RunN=(RunN==0) ? 4:RunN-1;
                       ExitF=False;
                       break;
          case ' '    :
          case CtrlX  :
          case Down   :RunN=(RunN==4) ?0:RunN+1;
                       ExitF=False;
                       break;
          case PgDn   :
          case CtrlC  :
          case End    :RunN=4;
                       ExitF=False;
                       break;
          case PgUp   :
          case CtrlR  :
          case Home   :RunN=0;
                       ExitF=False;
                       break;
          case 'e'    :
          case 'E'    :
                if (HGetMes("�a���e�A �a���I �w�w�� ���A�i �ẁ�A�a.",RunTitle[RunN],18)!=OK) break;
                AppendSpace(RunTitle[RunN],18);
                Explain("%T TEXT-MODE   %P ���Bɡ��   %N ����w    %E �¸w�a");
                HGetEng("���A ��З�a�� ���a �w�w��i ��ẁ�A�a.",RunItems[RunN],79);
                ExitF=False;
        }

    } while (!ExitF);
    HCloseWindow();
    if (Way==Return) Run(RunN),HotKeyEndF=True;
}


void TaskMenu(void)
{
    word OKF;
    word static TaskN;
    char *TaskList[]={
         "�b��a�w(����i ����i�a �b���a�e �b���E �w�b)�i �e�w �s���a.",
         "�a�q �b�� �a�w�a�� ���� �s���a.",
         "�b�� �a�w�� �q�e �a���w �wȁ�i ���a �����a.",
         "�a�w�i ���� ��a�� ���� �s���a.  ���e�� �a�A ���e�� ������ ���s���a."
    };

    ExplainList=TaskList;
    Push(InverseF);
    HInverse();
    HOpenWindow(24,1,38,7);
    HOpenMenu();
       HSetItem (25,2," W �b�� �a�w",13,'W');
       HSetItem (25,3," N �a�q �a�w",13,'N');
       HDrawBar (25,37,4);
       HSetItem (25,5," S �b�� �wȁ",13,'S');
       HSetItem (25,6," R �a�w ����",13,'R');
    Pop(InverseF);
    HManualSelect(TaskN);
    OKF=HSelect();
    TaskN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
      HotKeyEndF=True;
      switch (TaskN) {
        case 0 :HSayStatus(" ��a �a�w�a�� �����i�a�a? (1/2/3) ");
                GetKey();
                SetHotKeyF();
                HCloseWindow();
                switch (Way) {
                  case '1' :ChangeTasks(0); break;
                  case '2' :ChangeTasks(1); break;
                  case '3' :ChangeTasks(2); break;
                }
                break;
        case 1 :switch (NowTask) {
                  case 0 : ChangeTasks(1); break;
                  case 1 : ChangeTasks(2); break;
                  case 2 : ChangeTasks(0);
                }
                break;
        case 2 :SayBufStatus(); break;
        case 3 :ResizeBuf();    break;
        default:HotKeyEndF=False;
      }
    }
}

void BlockMenuSelect(void)
{
    word OKF;
    word static BlockNS;
    char *BlockList[]={
         "���a �e�ᝡ �i��i ����s���a.",
         "�i �e�ᝡ �i��i ��� �s���a.",
         "З �e�ᝡ �i��i ��� �s���a.",
    };

       ExplainList=BlockList;
       HInverse();
       HOpenWindow(33,3,47,7);
       HOpenMenu();
          HSetItem (34, 4," A ���e �i��",13,'A');
          HSetItem (34, 5," K ��� �i��",13,'K');
          HSetItem (34, 6," L �a�� �i��",13,'L');
       HNormal();
       HManualSelect(BlockNS);
       OKF=HSelect();
       BlockNS=NMP->NowPos;
       HCloseWindow();
       HCloseMenu();
       HCloseWindow();
       HCloseMenu();
       if (OKF!=Error) {
              SetEditScn();
              PrintXYpos();
              switch(BlockNS) {
                case 0 :if (SelectPos("���b�i �����a�� �a�a�� [RETURN] á�A�a.")!=OK) break;
                        CancelSelectBlock();
                        EditAA();
                        if (SelectPos("�i�� ������ �{�a���e [Return] á�A�a.")==OK)
                            SelectingBlockF=False ;
                        else CancelSelectBlock();
                        HotKeyEndF=False;
                        break;
                case 1 :if (SelectPos("���b�i �����a�� �a�a�� [RETURN] á�A�a.")!=OK) break;
                        CancelSelectBlock();
                        EditAK();
                        if (SelectPos("�i�� ������ �{�a���e [Return] á�A�a.")==OK)
                            SelectingBlockF=False;
                        else CancelSelectBlock();
                        HotKeyEndF=False;
                        break;
                case 2 :if (SelectPos("���b�i �����a�� �a�a�� [RETURN] á�A�a.")!=OK) break;
                        CancelSelectBlock();
                        EditAL();
                        if (SelectPos("�i�� ������ �{�a���e [Return] á�A�a.")==OK)
                            SelectingBlockF=False;
                        else CancelSelectBlock();
                        HotKeyEndF=False;
                        break;
              }

       }
}

void  EditCKE(void);
void  EditCKZ(void);
void  EditCKX(void);
void  EditCKD(void);

void BlockMenuDelete(void)
{
    word OKF;
    word static BlockND;
    char *BlockList[]={
         "�i��i �i�a ���៳���a.",
         "�i�� �w�b�� ���e ���a�i �������a.",
         "�i�� �w�b�� �墅�a�i ���a�� �������a.",
         "�i�� �w�b�� �墅�a�i �A�A�e ���e ���a�i �������a."
    };

       ExplainList=BlockList;
       HInverse();
       HOpenWindow(33,8,47,13);
       HOpenMenu();
          HSetItem (34, 9," Y �i�a �៱",13,'Y');
          HSetItem (34,10," E ���� �៱",13,'E');
          HSetItem (34,11," Z ��e ����",13,'Z');
          HSetItem (34,12," X �i�a ����",13,'X');
       HNormal();
       HManualSelect(BlockND);
       OKF=HSelect();
       BlockND=NMP->NowPos;
       HCloseWindow();
       HCloseMenu();
       HCloseWindow();
       HCloseMenu();
       if (OKF!=Error) {
          switch(BlockND) {
            case 0 : EditCKY();  break;
            case 1 : EditCKE();  break;
            case 2 : EditCKZ();  break;
            case 3 : EditCKX();  break;
          }
       }
}



void BlockMenu(void)
{
    word OKF;
    word static BlockN;
    char *BlockList[]={
         "�i��i �����s���a.",
         "�e�� ����E �i�� �a���i �a�� �g�A �s���a.",
         "�i��i �a�e ���A ���a�s���a.",
         "�i��i �a�e ���a�� ���� ��ǳ���a.",
         "����E �i��i ���� �៳���a.",
         "�i�� ���A�i ������ ������ǳ���a.",
         "�i��a�� ����E �����i �E��,���e��,�a���A�� ���x �a��a ���e ���i �s���a.",
         "�a�e �b�� �a�w�A ����E �i��i �e�� �b�� �a�w�a�� ���aЁ �����a.",
         "�e�� �b�� �a�w�A ����E �i��i �a�e �b�� �a�w�� ���� ��A �������a.",
         "����i ���� �i�a �e�� ���� �a���A �s���a��,�����A �i��a�� ����s���a.",
         "�i��a�� �����E �w�b�i ��w�s���a.",
         "�i��a�� �����E �w�b�i �����s���a."

    };


    do {
       ExplainList=BlockList;
       HInverse();
       HOpenWindow(32,1,46,18);
       HOpenMenu();
          HSetItem (33, 2," K �i�� ����",13,'K');
          HSetItem (33, 3," H �a�� �a��",13,'H');
          HDrawBar (33,45,4);
          HSetItem (33, 5," C ���a"     ,13,'C');
          HSetItem (33, 6," M ����"     ,13,'M');
          HSetItem (33, 7," D ����"     ,13,'D');
          HDrawBar (33,45,8);
          HSetItem (33, 9," I ���� �w��",13,'I');
          HSetItem (33,10," O ���i"     ,13,'O');
          HDrawBar (33,45,11);
          HSetItem (33,12," G �h��"     ,13,'G');
          HSetItem (33,13," S ������"   ,13,'S');
          HDrawBar (33,45,14);
          HSetItem (33,15," R ����"     ,13,'R');
          HSetItem (33,16," W ��w"     ,13,'W');
          HSetItem (33,17," P ����"     ,13,'P');
       HNormal();
       HManualSelect(BlockN);
       OKF=HSelect();
       BlockN=NMP->NowPos;
       if (OKF!=Error) {
           if (BlockN!=0 && BlockN!=4) {
              HCloseWindow();
              HCloseMenu();
           }
           if ( (BlockN==2 || BlockN==3  || BlockN==4  || BlockN==5 ||
                 BlockN==6 || BlockN==8 || BlockN==10 || BlockN==11 ) &
                 !ShowBlockF) {
                          HSayMes(" ��� �i��i ���Ё ���A�a. ");
                          if (BlockN==4) {
                            HCloseWindow();
                            HCloseMenu();
                          }
           } else {
              if ( BlockN==2 || BlockN==3  || BlockN==4  || BlockN==5 ||
                   BlockN==6 || BlockN==7 || BlockN==9 )
                   EditedF=True;
              SetEditScn();
              PrintXYpos();
              HotKeyEndF=True;
              switch(BlockN) {
                case 0 :BlockMenuSelect();
                        HotKeyEndF=False;
                        break;
                case 1 :EditCKH(); break;
                case 2 :
                        if (SelectPos("�ᗡ�� ���a �i�a�a?")==OK)
                            EditCKC();
                        break;
                case 3 :
                        if (SelectPos("�ᗡ�� �����a�a?")==OK)
                            EditCKV();
                        break;
                case 4 :
                        BlockMenuDelete();
                        break;
                case 5 :
                        Explain("���� ���i�a �i�A�i ���w�a�A�a.");
                        do {
                           if (SetScnF) SetScn();
                           if (HanF)
                                if ( HanResult==0 || HanResult==1 || EditType[HX+HSX-1]==1)
                                     HanCur2(HSX-1,SY-1);
                                else HanCur1(HSX-1,SY-1);
                           else
                                if ( EditType[HX+HSX-1]==1)
                                     EngCur2(HSX-1,SY-1);
                                else EngCur1(HSX-1,SY-1);
                           GetKey();
                           SetHotKeyF();
                           switch (Way) {
                             case Left   :
                             case CtrlS  : EditCKU();
                                           SetScnF=True;
                                           break;
                             case Right  :
                             case CtrlD  : EditCKI();
                                           SetScnF=True;
                                           break;
                           }
                        } while (Way!=ESC && Way!=Return && !HotKeyF);
                        break;
                case 6 :
                        EditCKL(); break;
                case 7 :HSayStatus("�y �弁 �b�� �a�w�A�� ���� ���a�a? (1/2/3)");
                        GetKey();
                        SetHotKeyF();
                        HCloseWindow();
                        switch(Way) {
                          case '1' :GetBlock(0); break;
                          case '2' :GetBlock(1); break;
                          case '3' :GetBlock(2); break;
                        }
                        Way=Return;
                        break;
                case 8 :
                        HSayStatus("�y �弁 �b�� �a�w�a�� �����a�a? (1/2/3)");
                        GetKey();
                        SetHotKeyF();
                        HCloseWindow();
                        switch(Way) {
                          case '1' :SendBlock(0); break;
                          case '2' :SendBlock(1); break;
                          case '3' :SendBlock(2); break;
                        }
                        Way=Return;
                        break;
                case 9 :if (SelectPos("�ᗡ�A�a ���� ���i �a�a?")==OK)
                            EditCKR();
                        break;
                case 10 :
                         EditCKW(); break;
                case 11 :
                         EditCKP(); break;
                default :HotKeyEndF=False;
              }
           }
       } else {
           HCloseWindow();
           HCloseMenu();
           break;
       }
    } while (HotKeyEndF==False && !HotKeyF);
}

void InsertMenu(void)
{
    word OKF;
    word static InsertN;
    char *is;
    char *InsertList[]={
       "�� ���i �s���s���a.",
       "��� ��á���� �i�a ���a ������ З�i �e�i�� �����a.",
       "��Ȃ�E ���i �������a.",
       "��� ��á���� З�� �{�a�� �������a.",
       "���a �e�ᝡ �������a.",
       "�e�� �e�ᝡ �������a.",
       "�A�e ������ ������ ���i �i�����a."
    };

    ExplainList=InsertList;
    Push(InverseF);
    HInverse();
    HOpenWindow(40,1,54,11);
    HOpenMenu();
    HSetItem (41,2 ," I ���� �s��",13,'I');
    HSetItem (41,3 ," N З   ���i",13,'N');
    HDrawBar (41,53,4);
    HSetItem (41,5 ," Y З   ����",13,'Y');
    HSetItem (41,6 ," Q ���� ����",13,'Q');
    HSetItem (41,7 ," G �i�a ����",13,'G');
    HSetItem (41,8 ," T �e�� ����",13,'T');
    HDrawBar (41,53,9);
    HSetItem (41,10," U �a�� �i��",13,'U');
    Pop(InverseF);
    HManualSelect(InsertN);
    OKF=HSelect();
    InsertN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        EditedF=True;
        SetEditScn();
        PrintXYpos();
        HotKeyEndF=True;
        switch(InsertN) {
           case 0 :HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"�ᗡ�A �s���i�a�a?");
                   do {
                     if (SelectPos("�ᗡ�A �s���i�a�a?")==OK) {
                        InsertBlankLine(NowLine-1);
                        if (NowLine<BlockSY) {
                            BlockSY++;
                            if (BlockEY!=0) BlockEY++;
                        } else if (NowLine<=BlockEY) BlockEY++;
                        SetEditBuf();
                        SetScnF=True;
                     } else break;
                   } while (True);
                   break;
           case 1 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"�ᗡ�A�� ���i�i�a�a?");
                   do {
                     if (SelectPos("�ᗡ�A�� ���i�i�a�a?")==OK) {
                        EditCN();
                     } else break;
                   } while (True);
                   break;
           case 2 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"��a З�i �����a?");
                   do {
                     if (SelectPos("��a З�i �����a?")==OK) {
                        EditCY();
                     }
                     else break;
                   } while (True);
                   break;
           case 3 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"���� �����a�� �a�A�a.");
                   do {
                     if (SelectPos("���� �����a�� �a�A�a.")==OK) {
                       EditCQY();
                     } else break;
                   } while (True);
                   break;
           case 4 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"���� ���a�e�a?");
                   do {
                     if (SelectPos("���� ���a�e�a?")==OK)
                       EditDel();
                     else break;
                   } while (True);
                   break;
           case 5 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"���� �e��e�a?");
                   do {
                     if (SelectPos("���� �e��e�a?")==OK)
                       EditCT();
                     else break;
                   } while (True);
                   break;
           case 6 :
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"�i�� ��á�� �a�A�a.");
                   do {
                     if (SelectPos("�i�� ��á�� �a�A�a.")==OK)
                        EditCU();
                     else break;
                   } while (True);
                   break;
           default:HotKeyEndF=False;
        }
    }
}

void InputMenu(void)
{
    word OKF;
    word static InputN;
    char *InputList[]={
         "�e�i,�i�a�U �a���A ���b �wȁ�i ���� �s���a.",
         "�s�� ���w�i �a�� �s���a.",
         "�a�� ���x ���w�i �a�� �s���a.",
         "�墅�a ��ѡ ���w�i �a�� �s���a.",
         "���� �A�e ���b ���w�i �a�� �s���a."
         };

    ExplainList=InputList;
    Push(InverseF);
    HInverse();
    HOpenWindow(48,1,66,7);
    HOpenMenu();
    HSetItem (49,2," E �e/�w    ����",17,'E');
    HSetItem (49,3," I �s�����a �q�q",17,'I');
    HSetItem (49,4," N �i�a�a�� �q�q",17,'N');
    HSetItem (49,5," L �奡ѡ   �q�q",17,'L');
    HSetItem (49,6," K �����A�e �q�q",17,'K');
    Pop(InverseF);
    HManualSelect(InputN);
    OKF=HSelect();
    InputN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        HotKeyEndF=True;
        switch(InputN) {
           case 0 :ChangeLanguage(); break;
           case 1 :EditIns();        break;
           case 2 :EditCQI();        break;
           case 3 :EditCOL();        break;
           case 4 :EditCOK();        break;
           default:HotKeyEndF=False;
        }
    }
}

void FindMenu(void)
{
    word OKF;
    word static FindN;
    char *FindList[]={
         "�����e ���a�i�i �x�s���a.",
         "���a�i �񬂉� �q�A á���s���a.",
         "�A�e ��, á�� �b��i �e���s���a."
         };

    ExplainList=FindList;
    Push(InverseF);
    HInverse();
    HOpenWindow(56,1,72,5);
    HOpenMenu();
    HSetItem (57,2," F ���a�i �x��",15,'F');
    HSetItem (57,3," A ���a�i á��",15,'A');
    HSetItem (57,4," L �e��   �b��",15,'L');
    Pop(InverseF);
    HManualSelect(FindN);
    OKF=HSelect();
    FindN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        EditedF=True;
        HotKeyEndF=True;
        switch(FindN) {
           case 0 :EditCQF(); break;
           case 1 :EditCQA(); break;
           case 2 :EditCL(); break;
           default:HotKeyEndF=False;
        }
    }
}

void SpecialMenu(void)
{
    word OKF;
    word static SpecialN;
    char *SpecialList[]={
         "������ ���� �a���A �a�a ��� ��á�i �����s���a.",
         "���a �a�e З�a�� �����a�� �g�e �w���A �b�� ��a�� З�i �����s���a.",
         "�a�� ���a�i ���b �s���a.",
         "���� �����w�i ��Ё ��a���� ���w�� ���� �S���a.",
         "��� �i��a�� �����E �w�b ����A �A�����i �u�s���a.",
         "З�� ���b�i 1�i�A �x���a.",
         "З�� ���w�i 1�i�� 80�i�� �a���A ��á ��ǳ���a.",
         "З�� �{�i 80�i�A �x���a.",
         "���ᐁ�A ͡�q�E TAB ���a�i�i �i�x�e ���� �������a�� á�� �s���a.",
         "�e�i�i �{�e �q�i �a�� �e�a�� �e�� �s���a."

    };

    ExplainList=SpecialList;
    Push(InverseF);
    HInverse();
    HOpenWindow(64,1,78,15);
    HOpenMenu();
    HSetItem (65,2 ," I ���� �a�q",13,'I');
    HSetItem (65,3 ," Q ���� З  ",13,'Q');
    HDrawBar (65,77,4);
    HSetItem (65,5 ," G �a�� ���a",13,'G');
    HSetItem (65,6 ," D ��   �a��",13,'D');
    HSetItem (65,7 ," O ���� �a��",13,'O');
    HDrawBar (65,77,8);
    HSetItem (65,9 ," L ���� ���i",13,'L');
    HSetItem (65,10 ," C ���w ���i",13,'C');
    HSetItem (65,11," R ���� ���i",13,'R');
    HDrawBar (65,77,12);
    HSetItem (65,13," T TAB  �e��",13,'T');
    HSetItem (65,14," H �e�a �e��",13,'H');
    Pop(InverseF);
    HManualSelect(SpecialN);
    OKF=HSelect();
    SpecialN=NMP->NowPos;
    HCloseWindow();
    HCloseMenu();
    if (OKF!=Error) {
        EditedF=True;
        HotKeyEndF=True;
        switch(SpecialN) {
           case 0 :EditTab(); break;
           case 1 :EditCQL(); break;
           case 2 :EditF4();  break;
           case 3 :SetEditScn();
                   PrintXYpos();
                   EditDraw();  break;
           case 4 :if (!ShowBlockF) {
                      HSayMes(" ��� ��� �i��i ���Ё ���A�a. ");
                      break;
                   }
                   EditedF=True;
                   EditCKD(); break;
           case 5 :EditAQ();  break;
           case 6 :EditAW();  break;
           case 7 :EditAE();  break;
           case 8 :EditAT();  break;
           case 9 :SetEditScn();
                   PrintXYpos();
                   HClrBox(0,0,VideoRight,0);
                   HWriteMid(0,"�a�� �e�i�e�a?");
                   do {
                     if (SelectPos("�a�� �e�i�e�a?")==OK)
                       EditF9(),EditedF=True;
                     else break;
                   } while (True);
                   break;
           default:HotKeyEndF=False;
        }
    }
}

void DisplayTitle(void);

void MainMenu(void)
{
    word FulledF=False;
    word static EditMenuN;
    char *is;

    if (HotKeyF==True) {
       switch (Way) {
         case F10   : break;
         case F2    :
         case F3    :
         case AltF3 :
         case AltW  :
         case AltX  :
         case AltF  : EditMenuN=0;  break;
         case AltC  :
         case F5    :
         case AltF6 :
         case AltF7 :
         case AltF8 :
         case AltF9 : EditMenuN=1;  break;
         case AltO  : EditMenuN=2;  break;
         case AltY  : EditMenuN=3;  break;
         case AltB  : EditMenuN=4;  break;
         case AltI  : EditMenuN=5;  break;
         case AltE  : EditMenuN=6;  break;
         case AltH  : EditMenuN=7;  break;
         case AltP  : EditMenuN=8;  break;
       }
       if   (  Way==AltF ||
               Way==AltO ||
               Way==AltC ||
               Way==AltY ||
               Way==AltB ||
               Way==AltI ||
               Way==AltE ||
               Way==AltH ||
               Way==AltP ||
               Way==F10
               )  HotKeyF=False;
       if (Way==F10) {
          Item=0;
       } else {
          Item=999;
          FulledF=True;
       }
    } else Item=0;
    do {
      if (Item!=999 && !FulledF)
          do {
             switch (EditMenuN) {
               case 0 : is="����(FILE)�i �ş��a��a ���a�A �ŝe�E �b�� ������ �����a.";
                        break;
               case 1 : is="���a�� �w�wЗ�A�� �a�w �a�w�e ���e �w�w�� �a�w�a �����A ��Ё ������ �����a.";
                        break;
               case 2 : is="�a�w�a�� ���u�A �x���� �y�a�� ���w�i ����Ё ������ ���s���a.";
                        break;
               case 3 : is="�����A �b�󺗷� ����i�e�A ������ �wȁ �a���i �s���a";
                        break;
               case 4 : is="�i�ⷁ ����� ����E �i��i�A �ŝe�E �b�� ������ �����a.";
                        break;
               case 5 : is="���w�i �����a��a ������a ������ ���w�i �i���e�w�� ���w�� ���a ���s���a.";
                        break;
               case 6 : is="�a�w�a�� ���b�A �ŝe�E �a�� ���w�i �ş� �s���a.";
                        break;
               case 7 : is="���a�e ���w�i �x��a �x�e ���a�i�i á���a�e ���w�� ���s���a.";
                        break;
               case 8 : is="�a�����a �a��, ��a����, ���i�w ���a ���w�� ���s���a.";
                        break;
               case 9 : is=" �e " SanVersionNum;
             }
             Explain(is);
             if (EditMenuN==9) HSetInverse(EditMenuN*8+1,0,EditMenuN*8+6,0);
             else              HSetInverse(EditMenuN*8+1,0,EditMenuN*8+8,0);
             GetKey();
             if (EditMenuN==9) HSetInverse(EditMenuN*8+1,0,EditMenuN*8+6,0);
             else              HSetInverse(EditMenuN*8+1,0,EditMenuN*8+8,0);
             SetHotKeyF();
             if (Way==ESC || HotKeyF) break;
             switch (Way) {
                case ' '   :
                case CtrlD :
                case Right : EditMenuN++;
                             if (EditMenuN==10) EditMenuN=0;
                             break;
                case CtrlS :
                case Left  : EditMenuN--;
                             if (EditMenuN>8) EditMenuN=9;
                             break;
                case CtrlR :
                case Home  : EditMenuN=0;
                             break;
                case CtrlC :
                case End   : EditMenuN=9;
                             break;
                case CtrlX :
                case Down  :
                case Return:FulledF=True;
                            break;
                case 'f'   :
                case 'F'   : EditMenuN=0;
                             FulledF=True;
                             break;
                case 'c'   :
                case 'C'   : EditMenuN=1;
                             FulledF=True;
                             break;
                case 'o'   :
                case 'O'   : EditMenuN=2;
                             FulledF=True;
                             break;
                case 'y'   :
                case 'Y'   : EditMenuN=3;
                             FulledF=True;
                             break;
                case 'b'   :
                case 'B'   : EditMenuN=4;
                             FulledF=True;
                             break;
                case 'i'   :
                case 'I'   : EditMenuN=5;
                             FulledF=True;
                             break;
                case 'e'   :
                case 'E'   : EditMenuN=6;
                             FulledF=True;
                             break;
                case 'h'   :
                case 'H'   : EditMenuN=7;
                             FulledF=True;
                             break;
                case 'p'   :
                case 'P'   : EditMenuN=8;
                             FulledF=True;
                             break;
                default    : if (Way>='1' && Way<='0') {
                               EditMenuN=Way-'0'-1;
                               FulledF=True;
                             }
                             break;
             }
          } while (!FulledF);
      else Item=0;
      if (Way!=ESC) {
          if (EditMenuN==9) HSetInverse(EditMenuN*8+1,0,EditMenuN*8+6,0);
          else              HSetInverse(EditMenuN*8+1,0,EditMenuN*8+8,0);
          switch (EditMenuN) {
            case 0 :FileMenu();     break;
            case 1 :RunMenu();      break;
            case 2 :OptionMenu();   break;
            case 3 :TaskMenu();     break;
            case 4 :BlockMenu();    break;
            case 5 :InsertMenu();   break;
            case 6 :InputMenu();    break;
            case 7 :FindMenu();     break;
            case 8 :SpecialMenu();  break;
            case 9 :if (Way==Return || Way==Down || Way==CtrlX ) {
                       DisplayTitle();
                       HotKeyEndF=True;
                    }
                    else {
                       Explain(" �e " SanVersionNum);
                       GetKey();
                       SetHotKeyF();
                    }
                    break;

          }
          SetMenuScn();
      }
      switch (Way) {
         case CtrlD:
         case Right: EditMenuN++;
                     if (EditMenuN==10) EditMenuN=0;
                     break;
         case CtrlS:
         case Left : EditMenuN--;
                     if (EditMenuN>8) EditMenuN=9;
                     break;
         case ESC  : HotKeyEndF=True;
                     break;
         default   : if (Way>='1' && Way<='0')
                       EditMenuN=Way-'0'-1;
                     break;
      }
    } while (!HotKeyF && !HotKeyEndF && Way!=ESC);
    HotKeyEndF=False;
    if (ChangeFontF) {
        ChangeFontF=False;
        FoolSetCur(HX+SX-1,NowLine);
        SetMenuScn();
        SetEditScn();
        PrintXYpos();
    } else {
        SetEditScn();
        PrintXYpos();
    }
    SetScnF=True;
}

