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


void QuitWork(void)
{
     word OKF;

     HotKeyF=False;
     if (Task[0].EditedF || Task[1].EditedF || Task[2].EditedF || EditedF)  {
         OKF=OK;
         if (HGetYesNo ("��w�A�� �g�e ����a ���� �s���a. ���� ��w�i�a�a?",&OKF)==OK)
             if (OKF) {
                if (SaveAll()==OK) Way=Error;
             } else Way=Error;
     } else Way=Error;
     if (Way==Error) HotKeyEndF=True;
}

void FileMenu(void)
{
    word OKF;
    word static FileMenuN;
    char* FileExplain[]={
          "����i ���� �i�����a.",
          "����A �b��Ж�� ���ẗ�A�� ���� �i�� ����i ���s���a.",
          "�e�� �b�� �a�w�� ����e ��w �s���a.",
          "���e �b�� �a�w�� ����i ��w �s���a.",
          "�e�� �b�� �a�w�i �����a�� ������ ���� �b��i ���b�s���a.",
          "�b�� ���� ����w�i �e�w�q�� �����A ��w�s���a.",
          "���a�e ���Bɡ�� ���� ���� �����i ���a �a�����a.",
          "�b�� ���Bɡ���i �e�w�s���a.",
          "����i �a����A�� ���� �s���a.",
          "�q�� ���a�� �a�s���a. ���a���� ���e 'EXIT' �w�w�i �a�w�a�A�a.",
          "���e �b��i �{�����a. ��w�a�� �g�a�� ����a �����a�e �·��a�A �S���a."
    };

    ExplainList=FileExplain;
    Push(InverseF);
    HInverse();
    HOpenWindow(0,1,19,14);
    HOpenMenu();
       HSetItem(1, 2," L ���� ����   F3",18,'L');
       HSetItem(1, 3," P ��   ���� A-F3",18,'P');
       HSetItem(1, 4," S ���� ��w   F2",18,'S');
       HSetItem(1, 5," A ���� ��w"     ,18,'A');
       HSetItem(1, 6," N ��   ����"     ,18,'N');
       HSetItem(1, 7," W ��   ���q A-W" ,18,'W');
       HDrawBar(1,18,8);
       HSetItem(1, 9," V ���� ����"     ,18,'V');
       HSetItem(1,10," C �w�� �e�w"     ,18,'C');
       HSetItem(1,11," I ���� ����"     ,18,'I');
       HSetItem(1,12," D ���a ���w"     ,18,'D');
       HSetItem(1,13," Q �b�� ���a A-X" ,18,'Q');
    Pop(InverseF);
    if (HotKeyF==True) {
        FileMenuN=(Way==F2   )? 2 :
                  (Way==F3   )? 0 :
                  (Way==AltF3)? 1 :
                  (Way==AltW )? 5 :
                  (Way==AltX )? 10: 0;
        HotKeyF=False;
        HotKeyEndF=True;
        Item=999;
    }  else Item=0;
    HManualSelect(FileMenuN);
    do {
      if (Item!=999) {
          OKF=HSelect();
          FileMenuN=NMP->NowPos;
          if (OKF==Error) break;
      } else Item=0;
      SetEditScn();
      PrintXYpos();
      switch (FileMenuN) {
        case 0 :OKF=LoadFile();
                if (OKF==OK) HotKeyEndF=True;
                break;
        case 1 :if (PutEditBuf()==OK) OKF=SelectPick();
                if (OKF==OK) HotKeyEndF=True;
                break;
        case 2 :if (PutEditBuf()==OK) SaveFile();
                HotKeyEndF=True;
                break;
        case 3 :if (PutEditBuf()==OK) SaveAll();
                HotKeyEndF=True;
                break;
        case 4 :NewFile();
                HotKeyEndF=True;
                SetScnF=True;
                break;
        case 5 :if (PutEditBuf()==OK) WriteTo();
                HotKeyEndF=True;
                break;
        case 6 :Dir();
                break;
        case 7 :ChangeDir();
                break;
        case 8 :PrintFile();
                break;
        case 9 :if (OsSaveF) {
                   OKF=OK;
                   if (HGetYesNo("�����i�i ���e ���� ��wЁ ���i�a�a?",&OKF)==OK) {
                      if (OKF==OK) if (PutEditBuf()!=OK || SaveAll()!=OK) break;
                   } else break;
                }
                if (PutEditBuf()==OK) {
                   int Status;

                   TextMode();
                   clrscr();
                   puts("Type 'Exit' to return to SAN.");
                   Status=system("");
                   GrMode();
                   if (Status==-1) HSayIOErr();
                   HClrScr();
                   SetMenuScn();
                   SetEditScn();
                   PrintXYpos();

                   SetScnF=True;
                   SetEditBuf();
                }
                break;
        case 10:PutEditBuf();
                QuitWork();
      }
      HotKeyEndF=True;
    } while (!HotKeyF && !HotKeyEndF);
    HCloseWindow();
    HCloseMenu();
}