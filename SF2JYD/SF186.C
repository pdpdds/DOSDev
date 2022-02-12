/****
    Street Fighter ][
    Original Game By CapCom
    IBM PC Game By Jung Young Dug ( KETEL ID : WD40 )
    Program Skill level : 1990.1 (0.7)
    Source Code Bolarnd C++ 2.0
    BC++ 2.0 Compiler option is -ml -f -w- -B

    스파2 메인 프로그램
    원작 : CAPCOM
    IBM PC VERSION : 정영덕
    프로그램 기술 정도 : 1990.1 수준
    소스 코드 : 볼란드 C++ 2.0
****/

#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>
#include <dos.h>

unsigned extern _stklen = 20000U;

#include "sf2kbd0.c"  // SF ][ keyboard low level input routine
#include "animate0.h" // image & sprite Data structure definition
#include "grker0.h"   // graphic kernel MACRO definition
#include "sf20.h"     // street fighter ][ Data Structure definition

#include "SF186.H"

Image ImgL[MAXIMG], ImgR[MAXIMG];  // Left, Right Player Image  Data
Sprite SprL[MAXSPR], SprR[MAXSPR]; // Left, Right Player Sprite Data

unsigned char R_[256], G_[256], B_[256]; // VGA 256 Palette Buffer

char *pName[8] = // Player Name , Hangul Code is Special Data Format
    {"   FR@   ", " SID  DA@ ", "HS@ FAU PA@", " AA@  LUH ",
     "   PFD   ", " OND  FU@ ", "MAUAU@LF@RS@", " DAH  JUP "};

char bkFN[8][13];     // background image Name
int bkSize[8];        // background Column size
char pIdFN[8][13];    // character identification file name
char pSeqFN[8][13];   // chracter sequence file name
char pKeyFN[8][13];   // character key file name
char pMusicFN[8][13]; // background music file name

GameENV GENV;
ACTOR ARL[MAXACTOR], ARR[MAXACTOR]; // Left, Right Player Actor
ACTION ANL, ANR;                    // Left, Right Player Action
COMBRULE LCR[MAXCR], RCR[MAXCR];    // Left, Right Player Combination Rule
KEYRULE LKR[MAXKR], RKR[MAXKR];     // Left, Right Player Key Rule
PLAYER LP, RP;                      // Player Left,Right

void main(int argc, char **argv)
{
   int bk, ret;

   systemCheck();

   if (argc > 1)
      loadFileConfig(argv[1]);
   else
      loadFileConfig("sf2file.cfg");

   textLogo();
   systemWakeUp();

   setInitialValue(&GENV, &LP, &RP);
   viewWarn();
   for (;;)
   {
      demo(&GENV, &LP, &RP);
      offMusic();
      for (;;)
      {
         if (select(&GENV, &LP, &RP) == BREAK)
            break;
         if (game(&GENV, &LP, &RP) == BREAK)
            break;
      }
   }
   systemDown();
}

int systemCheck()
{
   if (isVga256K() == 0)
      cError("", "This Program Require VGA 256K adapter", 0);
   if (mem520KFree() == 0)
      cError("", "This Program Require 520K Free Memory.", 0);
   return (0);
};

int isVga256K()
{
   union REGS r;

   r.h.ah = 0xf;
   int86(0x10, &r, &r);
   if (r.h.al == 7)
      return (0);

   return (1);
}

/* 메모리가 충분한가 검사 */
mem520KFree()
{
   if (coreleft() < 430000L)
      return (0);
   return (1);
}

/* 텍스트 로고 출력 부분 */
int textLogo()
{
   FILE *fp;
   unsigned char far *p;
   p = (unsigned char far *)0x417;
   *p = 0;

   fp = fopen("tlogo.sf2", "rb");
   fread((int far *)0xb8000000, 80, 50, fp);
   fclose(fp);

   getch();
   return (0);
};

#define NO_VOICE 0
#define PC_SPEAKER 1
#define SOUND_BLASTER 2
#define COVOX 3

#define COLOR_LPT1_PORT 0x378
#define MONO_LPT1_PORT 0x3bc

int systemWakeUp()
{
   delay(1);
   randomize();
   setNewSystemInt();
   loadFont("sf2w.fnt");
   loadConfig("sf2.cfg");
   if (SOUND_DEV != NO_VOICE)
      allocVoiceMem();
   if (SOUND_DEV == SOUND_BLASTER)
   {
      reset_dsp();
      speaker_on();
   }
   vga320Mode();
   setRGBPalette();
   getNDACR(0, 256, R_, G_, B_);
   brightCtrl(0, 256, R_, G_, B_, 0);
   initImg(&ImgL, MAXIMG);
   initImg(&ImgR, MAXIMG);
   planeMode();
   return (0);
}

loadFileConfig(fn) char fn[];
{
   FILE *fp;
   int i;

   fp = fopen(fn, "rb");
   if (fp == NULL)
      cError(fn, " File not Found", 0);

   for (i = 0; i < 8; i++)
      fscanf(fp, "%s", bkFN[i]);
   for (i = 0; i < 8; i++)
      fscanf(fp, "%d", &bkSize[i]);
   for (i = 0; i < 8; i++)
      fscanf(fp, "%s", pIdFN[i]);
   for (i = 0; i < 8; i++)
      fscanf(fp, "%s", pSeqFN[i]);
   for (i = 0; i < 8; i++)
      fscanf(fp, "%s", pKeyFN[i]);
   for (i = 0; i < 8; i++)
      fscanf(fp, "%s", pMusicFN[i]);

   fclose(fp);
   return (0);
}

setInitialValue(ENV, LP, RP)
    GameENV *ENV;
PLAYER *LP, *RP;
{
   LP->K = &LK;
   LP->KB = &LKB;
   LP->KR = LKR;
   LP->CR = LCR;
   RP->K = &RK;
   RP->KB = &RKB;
   RP->KR = RKR;
   RP->CR = RCR;

   LP->AN = &ANL;
   LP->AR = ARL;
   LP->Spr = SprL;
   LP->Img = ImgL;
   RP->AN = &ANR;
   RP->AR = ARR;
   RP->Spr = SprR;
   RP->Img = ImgR;

   LP->key2asc = Lkey2asc;
   RP->key2asc = Rkey2asc;

   LP->power = RP->power = 50;
   LP->resis = RP->resis = 50;

   ENV->t = 99;
   ENV->n = 3;
   return (0);
}

viewWarn()
{
   FILE *fp;
   unsigned char far *p;

   fp = fopen("sf2.w", "rb");
   if (fp == NULL)
      return (0);
   p = malloc(64000);
   if (p == NULL)
      cError("", "Memory Error", 1);

   viewPage(0);
   actPage(0);
   pClrScr(0);
   fread(p, 320, 200, fp);
   fclose(fp);
   pSprCp(FP_SEG(p), FP_OFF(p), 320, 320, 200, 0xa000, 0, 80);
   free(p);
   fadeIn();
   for (_KBHIT = 0; !_KBHIT;)
      ;
   fadeOut();
   return (0);
}

demo(GENV, LP, RP)
    GameENV *GENV;
PLAYER *LP, *RP;
{
   int i, r;
   char info[80][80];

   freeImg(&ImgR, MAXIMG);
   freeImg(&ImgL, MAXIMG);
   loadDemoResource(info);
   playMusic("sf2ttl.mdi");
   _KBHIT = 0;
   for (;;)
   {
      r = CONTINUE;
      for (; r == CONTINUE;)
      {
         r = demo1();
         if (r == CONTINUE)
            for (i = 0; i < 8; i++)
            {
               r = meetChar(&SprL[i], info[8 * i]);
               if (r == BREAK)
                  break;
            }
      }
      if (gameSelect(GENV, LP, RP) == CONTINUE)
         return (CONTINUE);
   }
}

loadDemoResource(info) char info[][80];
{
   int i;
   FILE *fp;

   fp = fopen("charact.sf2", "r");
   for (i = 0;; i++)
      if (fscanf(fp, "%s", info[i]) != 1)
         break;
   fclose(fp);

   if (ImgL[20].p != NULL)
      free(ImgL[20].p);
   ImgL[20].p = malloc(29999);
   fp = fopen("title.229", "rb");
   if (ImgL[20].p == NULL)
      cError("", "Demo Memory Error", 1);
   fread(ImgL[20].p, 229, 131, fp);
   fclose(fp);

   planeWrite0;
   pClrScr(0);
   viewPage(0);
   actPage(3);
   pClrScr(0);

   loadImgSpr("facew.id", &ImgL, &SprL, 0);
   pCTrpSpr0(115, 101, &SprL[0], &ImgL);
   pCTrpSpr0(200, 156, &SprL[3], &ImgL);
   pCTrpSpr0(115, 156, &SprL[4], &ImgL);
   pCTrpSpr0(40, 156, &SprL[1], &ImgL);
   pCTrpSpr0(235, 196, &SprL[7], &ImgL);
   pCTrpSpr0(160, 196, &SprL[6], &ImgL);
   pCTrpSpr0(85, 196, &SprL[5], &ImgL);
   pCTrpSpr0(10, 196, &SprL[2], &ImgL);
   return (0);
}

demo1()
{
   int i, vp, y, vy;

   fadeOut();
   viewPage(3);
   fadeIn();
   page2PageCp(3, 0);

   vp = 0;
   y = 190;
   writeMode(0);

   for (i = 0; i < 80; i++)
   {
      viewPage(vp);
      vp = (vp + 1) % 2;
      actPage(vp);
      writeMode(1);
      page2PageCp(3, vp);
      writeMode(0);
      pCTrpRImg0(50, y, ImgL[20].p, 229, 131);
      vy = (i / 25) + 1;
      y -= vy;
      if (_KE[_ESC])
         systemDown();
      if (_KBHIT)
      {
         _KBHIT = 0;
         break;
      }
   }

   y = 14;

   for (i = 0; i < 30; i++)
   {
      viewPage(vp);
      vp = (vp + 1) % 2;
      actPage(vp);
      writeMode(1);
      page2PageCp(3, vp);
      writeMode(0);
      pCTrpRImg0(50, y, ImgL[20].p, 229, 131);
      if ((i / 5) % 2)
         fPText(100, 160, "JF@AH@LT@    AGAQN@AA@DSH", 0, 0xe0, 0xe0);
      viewPage(vp);
      if (_KE[_ESC])
         systemDown();
      if (_KBHIT)
      {
         _KBHIT = 0;
         return (BREAK);
      }
   }
   return (CONTINUE);
}

meetChar(Spr, info)
    Sprite *Spr;
char info[][80];
{
   int vp, i;

   fadeOut();
   actPage(2);
   pClrScr(0);
   writeMode(0);
   pCTrpSpr0(15, 111, Spr, &ImgL);
   fPText(180, 20, info[0], 0, 0xff, 0xff);
   fPText(120, 40, info[1], 0, 0xe0, 0xe0);
   fPText(120, 60, info[2], 0, 0xe0, 0xe0);
   fPText(120, 80, info[3], 0, 0xe0, 0xe0);
   fPText(120, 100, info[4], 0, 0xe0, 0xe0);
   fPText(120, 120, info[5], 0, 0xe0, 0xe0);
   fPText(120, 140, info[6], 0, 0xe0, 0xe0);
   fPText(120, 160, info[7], 0, 0xe0, 0xe0);
   page2PageCp(2, 0);
   viewPage(0);
   fadeIn();

   vp = 0;
   for (i = 0; i < 100; i++)
   {
      viewPage(vp);
      vp = (vp + 1) % 2;
      actPage(vp);
      writeMode(1);
      page2PageCp(2, vp);
      if (_KE[_ESC])
         systemDown();
      if (_KBHIT)
      {
         _KBHIT = 0;
         return (BREAK);
      }
   }
   return (CONTINUE);
}

gameSelect(ENV, LP, RP)
    GameENV *ENV;
PLAYER *LP, *RP;
{
   int p, s;
   CTRLKEY *K;

   K = RP->K;

   fadeOut();

   page2PageCp(3, 0);
   actPage(0);
   writeMode(0);
   pCTrpRImg0(50, 14, ImgL[20].p, 229, 131);
   page2PageCp(0, 2);
   writeMode(0);
   fPText(20, 161, "SIDMA@JE@ SA@AU@ DNHLU@JE@ SA@AU@ SJDAGU   JEHMEU", 0, 240, 240);
   fPText(20, 160, "SIDMA@JE@ SA@AU@ DNHLU@JE@ SA@AU@ SJDAGU   JEHMEU", 0, 245, 245);
   viewPage(0);
   fadeIn();

   LP->pl = NOTSELECT;
   LP->ctrl = HUMAN;
   RP->pl = NOTSELECT;
   RP->ctrl = HUMAN;

   p = 0;
   s = 1;

   flushKey(LP->K);
   flushKey(RP->K);

   for (;;)
   {
      viewPage(p);
      p = (p + 1) % 2;
      actPage(p);

      page2PageCp(2, p);
      writeMode(0);
      pBarL(s * 96 + 16, 158, 96, 21, 3);
      fPText(20, 161, "SIDMA@JE@ SA@AU@ DNHLU@JE@ SA@AU@ SJDAGU   JEHMEU", 0, 0, 0);
      fPText(20, 160, "SIDMA@JE@ SA@AU@ DNHLU@JE@ SA@AU@ SJDAGU   JEHMEU", 0, 245, 245);

      if (_KE[K->L])
      {
         _KE[K->L] = 0;
         s--;
      }
      if (_KE[K->R])
      {
         _KE[K->R] = 0;
         s++;
      }

      //     if (s==0) s++ ;
      if (s < 0)
         s = 2;
      if (s > 2)
         s = 0;

      if (_KE[K->x] || _KE[K->y] || _KE[K->z] || _KE[K->a] || _KE[K->b] || _KE[K->c])
         if (s == 2)
         {
            setOption(ENV, LP, RP);
            fadeOut();
            page2PageCp(3, p);
            actPage(p);
            writeMode(0);
            pCTrpRImg0(50, 14, ImgL[20].p, 229, 131);
            page2PageCp(p, 2);
            writeMode(0);
            pBarL(s * 96 + 16, 158, 96, 21, 3);
            fPText(20, 161, "SIDMA@JE@ SA@AU@ DNHLU@JE@ SA@AU@ SJDAGU   JEHMEU", 0, 0, 0);
            fPText(20, 160, "SIDMA@JE@ SA@AU@ DNHLU@JE@ SA@AU@ SJDAGU   JEHMEU", 0, 245, 245);
            viewPage(p);
            fadeIn();
            flushKey(K);
         }
         else
            break;

      if (_KE[_ESC])
      {
         _KE[_ESC] = 0;
         return (EXIT);
      }
   }

   ENV->tr = 0;
   LP->pl = NOTSELECT;
   LP->ctrl = COMPU;
   RP->pl = NOTSELECT;
   RP->ctrl = COMPU;
   LP->win = RP->win = LP->scr = RP->scr = ENV->mn = 0;
   for (p = 0; p < 12; p++)
      ENV->loser[p] = 0;
   if (s == 0)
      RP->ctrl = HUMAN;
   if (s == 1)
   {
      LP->ctrl = HUMAN;
      RP->ctrl = HUMAN;
   }
   return (CONTINUE);
}

setOption(ENV, LP, RP)
    GameENV *ENV;
PLAYER *LP, *RP;
{
   int p, v, bl;
   int n, t, lp, lr, rp, rr, x[8], y[8];
   char tmp[100];
   CTRLKEY *K;

   K = RP->K;
   n = ENV->n;
   t = ENV->t;
   lp = LP->power;
   lr = LP->resis;
   rp = RP->power;
   rr = RP->resis;

   x[0] = 20;
   y[0] = 64;
   x[1] = 20;
   y[1] = 84;
   x[2] = 292;
   y[2] = 64;
   x[3] = 292;
   y[3] = 84;
   x[4] = 66;
   y[4] = 124;
   x[5] = 66;
   y[5] = 144;
   x[6] = 102;
   y[6] = 184;
   x[7] = 166;
   y[7] = 184;

   drawOpt();
   p = 0;
   v = 0;
   bl = 0;
   flushKey(K);

   for (;;)
   {
      viewPage(p);
      p = (p + 1) % 2;
      actPage(p);
      page2PageCp(2, p);
      writeMode(0);

      if (_KE[K->U])
      {
         _KE[K->U] = 0;
         v--;
      }
      if (_KE[K->D])
      {
         _KE[K->D] = 0;
         v++;
      }
      if (v < 0)
         v = 7;
      if (v > 7)
         v = 0;
      //     if (bl) pBarL(x[v],y[v],9,5,255) ;
      if (bl)
         pCTrpSpr0(x[v], y[v], &SprL[10], &ImgL);
      bl = (bl + 1) % 2;

      switch (v)
      {
      case 0:
         macroEnv(K->R, K->L, &lp, 1, 100, 1);
         break;
      case 1:
         macroEnv(K->R, K->L, &lr, 1, 100, 1);
         break;
      case 2:
         macroEnv(K->L, K->R, &rp, 1, 100, 1);
         break;
      case 3:
         macroEnv(K->L, K->R, &rr, 1, 100, 1);
         break;
      case 4:
         macroEnv(K->L, K->R, &n, 1, 99, 2);
         break;
      case 5:
         macroEnv(K->L, K->R, &t, 20, 999, 1);
         break;
      }

      sprintf(tmp, "DB@MED SL@JN@ $%2d#MED$ %2d#JSUMF@\0", n, n / 2 + 1);
      fPText(76, 120, tmp, 0, _G7 | _R7, _B3);
      sprintf(tmp, "DB@MED JU@AAD $: %3d #OI@\0", t);
      fPText(76, 140, tmp, 0, _G7, _G7);
      pBarL(131 - lp, 63, lp, 10, _R6 | _G6);
      pBarL(131 - lr, 83, lr, 10, _R6 | _G6);
      pBarL(189, 63, rp, 10, _R6 | _G6);
      pBarL(189, 83, rr, 10, _R6 | _G6);

      if (_KE[_ESC])
      {
         _KE[_ESC] = 0;
         return (EXIT);
      }

      if (_KE[K->x] || _KE[K->y] || _KE[K->z] || _KE[K->a] || _KE[K->b] || _KE[K->c])
      {
         flushKey(K);
         if (v == 7)
            return (CONTINUE);
         if (v == 6)
            break;
      }
   }
   ENV->n = n;
   ENV->t = t;
   LP->power = lp;
   LP->resis = lr;
   RP->power = rp;
   RP->resis = rr;
   return (0);
}

macroEnv(L, R, v, l, u, i) unsigned char L, R;
int *v, l, u, i;
{
   if (_KS[L])
      (*v) -= i;
   if (_KS[R])
      (*v) += i;

   if ((*v) < l)
      (*v) = l;
   if ((*v) > u)
      (*v) = u;
   return (0);
}

drawOpt()
{
   unsigned char *p, *p2;
   int i, r, d;

   fadeOut();
   p = malloc(29999);
   if (p == NULL)
      cError("", "Option Mx", 1);

   p2 = ImgL[20].p;
   for (i = 0; i < 29999; i++)
   {
      d = *(p2 + i);
      r = (d & 0xe0) >> 6;
      *(p + i) = (r << 5) | (r << 2) | (r >> 1);
   }

   actPage(0);
   writeMode(0);
   pClrScr(0);
   pCTrpRImg0(50, 34, p, 229, 131);
   writeMode(0);

   fPText(56, 4, "$STREET FIGHTER ][ #SJDAGU JEHMEU", 0, 255, 255);

   fPText(36, 40, "$LEFT WARRIOR       RIGHT WARRIOR", 0, 213, 213);
   fPText(136, 60, "AIUAGAFGA", 0, _R7, _R7);
   fPText(136, 80, "HAULE@FGA", 0, _R7, _R7);
   fPText(76, 120, "DB@MED SL@JN@   MED   JSUMF@", 0, _G7 | _R7, _B3);
   fPText(76, 140, "DB@MED JU@AAD $:     #OI@", 0, _G7, _G7);
   fPText(112, 180, "JEHMEU    OQ@JI@", 0, 200, _G7);

   pBarL(30, 62, 102, 12, _R5);
   pBarL(30, 82, 102, 12, _R5);
   pBarL(188, 62, 102, 12, _R5);
   pBarL(188, 82, 102, 12, _R5);
   pBoxL(30, 62, 102, 12, 255);
   pBoxL(30, 82, 102, 12, 255);
   pBoxL(188, 62, 102, 12, 255);
   pBoxL(188, 82, 102, 12, 255);

   page2PageCp(0, 2);
   viewPage(0);
   fadeIn();
   free(p);
   return (0);
}

select(GENV, LP, RP)
    GameENV *GENV;
PLAYER *LP, *RP;
{
   int r;

   freeImg(&ImgR, MAXIMG);
   freeImg(&ImgL, MAXIMG);

   loadSelectResource(GENV);
   playMusic("select.mdi");
   r = selectMode(GENV, LP, RP);
   offMusic();
   return (r);
}

unsigned char cx[8] = {95, 105, 153, 164, 167, 74, 31, 34};
unsigned char cy[8] = {44, 18, 60, 2, 34, 8, 10, 38};
unsigned char cx0[8] = {99, 127, 156, 184, 99, 127, 156, 184};
unsigned char cy0[8] = {116, 116, 116, 116, 148, 148, 148, 148};

loadSelectResource(ENV)
    GameENV *ENV;
{
   int i, x, y;
   unsigned char *p1, *p2;
   FILE *fp;

   actPage(3);

   p1 = malloc(195 * 93);
   p2 = malloc(122 * 72);
   if (p1 == NULL || p2 == NULL)
      cError("", "Select Memory Error", 1);

   fp = fopen("select.sf2", "rb");
   fread(p1, 195, 93, fp);
   fread(p2, 122, 72, fp);
   fclose(fp);

   planeWrite0;
   pClrScr(7);
   pCTrpRImg0(62, 25, p1, 195, 93);
   pCTrpRImg0(99, 120, p2, 122, 72);
   free(p1);
   free(p2);
   fPText(62, 4, "AGAQN@AA@FSH JEDQBA SA@JUQJU@LM@", 0, 255, 255);
   loadImgSpr("facew.id", &ImgL, &SprL, 0);
   loadImgSpr("sface.id", &ImgR, &SprR, 0);

   for (i = 0; i < 8; i++)
      if (ENV->loser[i])
      {
         pCTrpSpr0(cx[i] + 72, cy[i] + 30, &SprR[16], &ImgR);
         pCTrpSpr0(cx0[i] + 15, cy0[i] + 21, &SprR[8 + i], &ImgR);
      }
   return (0);
}

selectMode(ENV, LP, RP)
    GameENV *ENV;
PLAYER *LP, *RP;
{
   int xL, yL, xR, yR, vp, sL, sR, off;
   char *ptype[2] = {"$Human PLAYER", "$Computer PLAYER"};

   if (ENV->tr > 7)
      return (BREAK);

   fadeOut();
   writeMode(1);
   page2PageCp(3, 0);
   viewPage(0);
   fadeIn();

   xL = xR = 0;
   yL = 0;
   yR = 1;
   vp = 0;
   flushKey(LP->K);
   flushKey(RP->K);
   if (LP->pl != NOTSELECT)
   {
      xL = (LP->pl) % 4;
      yL = (LP->pl) / 4;
   }
   if (RP->pl != NOTSELECT)
   {
      xR = (RP->pl) % 4;
      yR = (RP->pl) / 4;
   }

   for (;;)
   {
      viewPage(vp);
      vp = (vp + 1) % 2;
      actPage(vp);
      writeMode(1);
      page2PageCp(3, vp);

      selectCtrl(ENV, LP, &xL, &yL);
      selectCtrl(ENV, RP, &xR, &yR);

      sL = xL + yL * 4;
      sR = xR + yR * 4;
      writeMode(0);
      pCTrpSpr0(5, 196, &SprL[sL], &ImgL);
      pCTrpSpr1(315, 196, &SprL[sR], &ImgL);

      fPText(20, 80, pName[sL], sL, 0xe0, 0xe0);
      fPText(0, 64, ptype[LP->ctrl], sL, 0xe0, 0xe0);

      fPText(230, 80, pName[sR], sR, 0xfc, 0xfc);
      fPText(200, 64, ptype[RP->ctrl], sR, 0xfc, 0xfc);

      off = 0;
      if ((sR % 4) > 1)
         off = 1;
      pCTrpSpr0(cx0[sR] + 15, cy0[sR] + 21, &SprR[sR], &ImgR);
      pCTrpSpr0(cx0[sR], cy0[sR], &SprL[13], &ImgL);
      pCTrpSpr0(cx[sR] + 60, cy[sR] + 20, &SprL[14], &ImgL);

      off = 0;
      if ((sL % 4) > 1)
         off = 1;
      pCTrpSpr0(cx0[sL] + 15, cy0[sL] + 21, &SprR[sL], &ImgR);
      pCTrpSpr0(cx0[sL], cy0[sL], &SprL[11], &ImgL);
      pCTrpSpr0(cx[sL] + 60, cy[sL] + 20, &SprL[12], &ImgL);

      if (_KE[_ESC])
         systemDown();
      if (_KE[_F10])
      {
         _KE[_F10] = 0;
         return (BREAK);
      }
      if (LP->pl != NOTSELECT && RP->pl != NOTSELECT)
         break;
   }
   page2PageCp(vp, 3);
   ENV->Bk = sL;
   moveAirplane(cx[sR] + 72, cy[sR] + 30, cx[sL] + 72, cy[sL] + 30, vp);
   vsScreen(LP, RP);
   return (CONTINUE);
}

selectCtrl(ENV, PL, x, y)
    GameENV *ENV;
PLAYER *PL;
int *x, *y;
{
   int tx, ty, i;
   CTRLKEY *K;

   K = PL->K;
   tx = *x;
   ty = *y;

   if (PL->pl == NOTSELECT && PL->ctrl == HUMAN)
   {
      if (_KE[K->U])
      {
         ty--;
         _KE[K->U] = 0;
      }
      if (_KE[K->D])
      {
         ty++;
         _KE[K->D] = 0;
      }
      if (_KE[K->L])
      {
         tx--;
         _KE[K->L] = 0;
      }
      if (_KE[K->R])
      {
         tx++;
         _KE[K->R] = 0;
      }
      if (tx < 0)
      {
         ty--;
         tx = 3;
      }
      if (tx > 3)
      {
         ty++;
         tx = 0;
      }
      if (ty < 0)
         ty = 1;
      if (ty > 1)
         ty = 0;
      if (_KE[K->x] || _KE[K->y] || _KE[K->z] || _KE[K->a] || _KE[K->b] || _KE[K->c])
         PL->pl = ty * 4 + tx;
   }

   if (PL->pl == NOTSELECT && PL->ctrl == COMPU)
   {
      for (i = 0;; i++)
      {
         tx = random(4);
         ty = random(2);
         if (ENV->loser[ty * 4 + tx] == 0)
            break;
      }
      if (random(20) == 0)
         PL->pl = ty * 4 + tx;
   }

   *x = tx;
   *y = ty;
   return (0);
}

moveAirplane(x1, y1, x2, y2, p) int x1, y1, x2, y2, p;
{
   int dx, dy, x, y, i, m, tn, ct;

   sayVoice("plane.voc");
   x = x1 << 5;
   y = y1 << 5;
   dx = (x2 - x1);
   dy = (y2 - y1);
   m = 0;
   if (dx != 0 && dy != 0)
   {
      tn = abs((dy * 16) / dx);
      if (tn < 6)
      {
         m = 3;
         if (dx > 0)
            m = 6;
      }
      if (tn > 38)
      {
         m = 1;
         if (dy > 0)
            m = 2;
      }
      if (tn > 5 && tn < 39)
      {
         if (dx < 0)
            m += 3;
         if (dx > 0)
            m += 6;
         if (dy < 0)
            m += 1;
         if (dy > 0)
            m += 2;
      }
   }

   for (i = 0; i < 32; i++)
   {
      viewPage(p);
      p = (p + 1) % 2;
      actPage(p);
      writeMode(1);
      page2PageCp(3, p);
      writeMode(0);
      pCTrpSpr0(x >> 5, y >> 5, &SprR[m + 17], &ImgR);
      x += dx;
      y += dy;
   }
   return (0);
}

vsScreen(LP, RP)
    PLAYER *LP,
    *RP;
{
   char tmp[100];

   fadeOut();

   actPage(0);
   pClrScr(7);
   writeMode(0);
   pCTrpSpr0(30, 156, &SprL[LP->pl], &ImgL);
   pCTrpSpr1(290, 156, &SprL[RP->pl], &ImgL);
   pCTrpSpr0(160, 130, &SprL[8], &ImgL);
   sprintf(tmp, "%s DB@ %sLT@ DB@AGHLUQCU@DA@\0", pName[LP->pl], pName[RP->pl]);
   fPText(24, 20, tmp, 0, 255, 255);
   viewPage(0);
   fadeIn();
   return (0);
}

game(GENV, LP, RP)
    GameENV *GENV;
PLAYER *LP, *RP;
{
   int r, i;

   freeImg(&ImgR, MAXIMG);
   freeImg(&ImgL, MAXIMG);

   LP->win = RP->win = GENV->mn = 0;
   loadGameResource(GENV, LP, RP);
   for (i = 0; i < GENV->n; i++)
   {
      GENV->mn = i + 1;
      if ((r = playGame(GENV, LP, RP)) != CONTINUE)
      {
         offMusic();
         return (r);
      }
      if (LP->energe > RP->energe)
         LP->win++;
      if (LP->energe < RP->energe)
         RP->win++;
      if (LP->win * 2 > GENV->n || RP->win * 2 > GENV->n)
         break;
   }
   r = drawWinner(GENV, LP, RP);
   return (r);
}

drawWinner(ENV, LP, RP)
    GameENV *ENV;
PLAYER *LP, *RP;
{
   char tmp[100];
   int lp, lpc, r;

   freeImg(&ImgR, MAXIMG);
   freeImg(&ImgL, MAXIMG);

   loadImgSpr("facew.id", &ImgL, &SprL, 0);
   loadImgSpr("facex.id", &ImgR, &SprR, 0);

   fadeOut();

   actPage(0);
   pClrScr(7);
   writeMode(0);
   if (LP->win > RP->win)
   {
      pCTrpSpr0(30, 160, &SprL[LP->pl], &ImgL);
      pCTrpSpr1(290, 160, &SprR[RP->pl], &ImgR);
      lp = RP->pl;
      lpc = RP->ctrl;
      RP->pl = NOTSELECT;
   }
   else
   {
      pCTrpSpr0(30, 160, &SprR[LP->pl], &ImgR);
      pCTrpSpr1(290, 160, &SprL[RP->pl], &ImgL);
      lp = LP->pl;
      lpc = LP->ctrl;
      LP->pl = NOTSELECT;
   }

   fPText(44, 180, "$You Must Defeat ChengLong !", 0, 255, 255);
   viewPage(0);
   fadeIn();
   delay(2000);
   r = CONTINUE;
   if (lpc != COMPU)
      r = drawLoser(lp);
   else
   {
      ENV->tr++;
      ENV->loser[lp] = 1;
   }
   return (r);
}

drawLoser(int pl)
{
   int i;

   fadeOut();
   actPage(0);
   pClrScr(6);
   writeMode(0);
   pCTrpSpr0(40, 150, &SprR[pl], &ImgR);
   fPText(48, 20, "$Hit Any Key to Continue Game", 0, 255, 255);
   viewPage(0);
   fadeIn();
   _KBHIT = 0;
   for (i = '9'; i >= '0'; i--)
   {
      fPLargeECh(200, 60, 11, 10, i, &SprL[10], &SprL[9], &ImgL);
      delay(800);
      if (_KBHIT)
         return (CONTINUE);
   }
   return (BREAK);
}

int loadGameResource(GameENV *ENV, PLAYER *LP, PLAYER *RP)
{
   int xs;

   xs = bkSize[ENV->Bk];

   loadBkimg(bkFN[ENV->Bk], xs, 84);
   loadImgSpr(pIdFN[LP->pl], LP->Img, LP->Spr, 1);
   loadImgSpr(pIdFN[RP->pl], RP->Img, RP->Spr, 1);
   loadSeq(pSeqFN[LP->pl], LP->AN, LP->AR);
   loadSeq(pSeqFN[RP->pl], RP->AN, RP->AR);
   loadAttackRule(pKeyFN[LP->pl], &LP->nr, LP->KR, LP->CR, LP);
   loadAttackRule(pKeyFN[RP->pl], &RP->nr, RP->KR, RP->CR, RP);
   if (LP->pl == RP->pl)
      convertCharacterColor(RP->Img, MAXIMG);
   return (0);
}

int convertCharacterColor(Image Img[], int n)
{
   int i, r, g, b;
   unsigned char lookup[256];

   for (r = 0, i = 0; r < 8; r++)
      for (g = 0; g < 8; g++)
         for (b = 0; b < 4; b++, i++)
            lookup[i] = (g << 5) | (r << 2) | b;

   for (i = 0; i < n; i++)
      if (Img[i].p != NULL)
         ConvertImageColor((unsigned char *)Img[i].p, (int)Img[i].ls, (char*)lookup);
   return (0);
}

int ConvertImageColor(unsigned char *s, int l, unsigned char lookup[])
{
   int csize, i, j, dp;

   dp = 0;
   for (i = 0; i < l;)
   {
      i++;
      csize = (int)s[i] + (int)s[i + 1] * 256;
      i += 2;
      for (j = 0; j < csize; i++, j++, dp++)
         s[i] = lookup[s[i]];
   }
   return (dp);
}

int loadBkimg(char *fn, unsigned int xs, unsigned int ys2)
{
   FILE *fp;
   unsigned char far *p;
   unsigned int off;

   fp = fopen(fn, "rb");

   p = malloc(xs * ys2);
   if (p == NULL)
      cError("", "BK Mem Error", 1);

   if (fp == NULL)
      cError(fn, "Not Found!", 1);

   writeMode(0);
   actPage(2);
   pClrScr(0);
   actPage(3);
   pClrScr(0);
   off = xs * ys2;

   fread(p, xs, ys2, fp);
   pSprCp(FP_SEG(p), FP_OFF(p), xs, xs, ys2, 0xa800, 0, xs / 4);
   fread(p, xs, ys2, fp);
   pSprCp(FP_SEG(p), FP_OFF(p), xs, xs, ys2, 0xa800, off, xs / 4);
   fclose(fp);
   free(p);
   return (0);
}

#define WINNER_ACTION 41
#define LOSER_ACTION 43
#define TIME_LOSER_ACTION 44

int r1End, HIT, NewPlayer;

#define KO_END 1
#define TIME_END 2

playGame(ENV, LP, RP)
    GameENV *ENV;
PLAYER *LP, *RP;
{
   int p, off, moff, bks, t, vi, wait, scr, i;

   bks = bkSize[ENV->Bk];
   moff = bks / 4 - 80;
   off = moff / 2;
   t = ENV->t * 10;
   vi = wait = r1End = p = HIT = NewPlayer = 0;

   initPlayer(LP, 90, 180, 0);
   initPlayer(RP, 230, 180, 1);

   playMusic(pMusicFN[ENV->Bk]);

   startScene(LP, RP, ENV, off, moff, bks, t);
   flushKey(LP->K);
   flushKey(RP->K);

   for (;;)
   {
      viewPage(p);
      p = (p + 1) % 2;
      actPage(p);
      setPostition(LP, RP, &off, moff);
      scr = (min(LP->y, RP->y) - 180) / 12;
      drawBackground(scr, off, bks);
      drawActor(LP, RP, scr);
      if (COMTYPE)
         viewPage(p);
      drawScoreBoard(LP, RP, t);

      if (HIT)
      {
         HIT = 0;
         waitSec(4);
      }

      if (_KE[_F5])
      {
         _KE[_F5] = 0;
         for (i = 0; i < 100; i++)
            setStAdd(p * 0x4000 + (i % 2) * 800);
      }

      if ((t % 16) == 0)
      {
         if (LP->chicken > 0)
            LP->chicken--;
         if (RP->chicken > 0)
            RP->chicken--;
      }

      if (NewPlayer == 0)
      {
         if (setNextAction(LP, RP) == 0)
            setNextAction(RP, LP);
      }
      else
         NewPlayer++;

      if (NewPlayer > 20)
         return (NEWPLAYER);

      if (t > 0 && NewPlayer == 0)
         t--;
      if (t == 0)
         r1End = TIME_END;

      if (_KE[_ESC])
         systemDown();
      if (_KE[_F10])
      {
         _KE[_F10] = 0;
         return (BREAK);
      }
      if (_KE[_F1])
      {
         _KE[_F1] = 0;
         vi = (vi + 1) % 2;
      }
      if (_KE[_F9])
      {
         _KE[_F9] = 0;
         wait = (wait + 1) % 2;
      }
      if (_KE[_F8])
         cError("", "mem Test", 1);

      if (WAITCONST > 0)
         delay(WAITCONST);

      newPlayerComeIn(LP);
      newPlayerComeIn(RP);

      if (r1End)
         break;
      if (vi)
         viewInfo(LP, RP);
      if (wait)
      {
         viewPage(p);
         KBDWAIT;
      }
   }
   offMusic();
   endScene(LP, RP, off, moff, bks, t, p);
   return (CONTINUE);
}

newPlayerComeIn(PL)
    PLAYER *PL;
{
   CTRLKEY *K;

   if (PL->ctrl == HUMAN || NewPlayer)
      return (0);

   K = PL->K;
   if (_KS[K->x] || _KS[K->y] || _KS[K->z] || _KS[K->a] || _KS[K->b] || _KS[K->c])
   {
      PL->ctrl = HUMAN;
      PL->pl = NOTSELECT;
      NewPlayer = 1;
   }
}

startScene(LP, RP, ENV, off, moff, bks, t)
    PLAYER *LP,
    *RP;
GameENV *ENV;
int off, bks, t;
{
   int b, p;
   char tmp[40];
   fadeOut();
   p = 0;
   actPage(p);
   drawBackground(0, off, bks);
   drawActor(LP, RP, 0);
   drawScoreBoard(LP, RP, t);
   viewPage(p);
   sprintf(tmp, "$ROUND %2d #JU@MAA SAQCU@DA@$ !\0", ENV->mn);
   fadeIn();
   sayVoice("start.voc");
   for (b = 0; b < 30; b++)
   {
      viewPage(p);
      p = (p + 1) % 2;
      actPage(p);
      setPostition(LP, RP, &off, moff);
      drawBackground(0, off, bks);
      drawActor(LP, RP, 0);
      if (COMTYPE)
         viewPage(p);
      drawScoreBoard(LP, RP, t);
      fPText(76, 70, tmp, 0, 247 + ((b / 3) % 2) << 3, 0);
   }
   return (0);
}

endScene(LP, RP, off, moff, bks, t, p)
    PLAYER *LP,
    *RP;
int off, bks, t, p;
{
   int b, scr;
   char tmp[40], *vn = "#w.voc";

   if (LP->energe < 0)
      LP->energe = 0;
   if (RP->energe < 0)
      RP->energe = 0;

   sprintf(tmp, "%s LJ@ %s HU@AUP\0", pName[LP->pl], pName[RP->pl]);
   if (LP->energe > RP->energe)
   {
      sprintf(tmp, "%s LT@ JSUFU@\0", pName[LP->pl]);
      vn[0] = (LP->pl) + '1';
   }
   if (RP->energe > LP->energe)
   {
      sprintf(tmp, "%s LT@ JSUFU@\0", pName[RP->pl]);
      vn[0] = (RP->pl) + '1';
   }
   for (b = 0; b < 70; b++)
   {
      if (b == 40)
         sayVoice(vn);
      viewPage(p);
      p = (p + 1) % 2;
      actPage(p);
      setPostition(LP, RP, &off, moff);
      scr = (min(LP->y, RP->y) - 180) / 12;
      drawBackground(scr, off, bks);
      drawActor(LP, RP, scr);
      if (COMTYPE)
         viewPage(p);
      drawScoreBoard(LP, RP, t);
      if (b > 30)
         fPText(76, 70, tmp, 0, 255, 255);
      else
         delay(100);
   }
   return (0);
}

viewInfo(LP, RP)
    PLAYER *LP,
    *RP;
{
   char tmp[50];

   sprintf(tmp, "$LPOS %3d,%3d|%3d|%d  RPOS %3d,%3d|%3d|%d\0",
           LP->x, LP->y, LP->ar, LP->mirr, RP->x, RP->y, RP->ar, RP->mirr);
   fPText(0, 50, tmp, 0, 255, 255);
   sprintf(tmp, "$LTxy %3d,%3d  RTxy %3d,%3d\0", LP->x2, LP->y2, RP->x2, RP->y2);
   fPText(0, 70, tmp, 0, 255, 255);
   sprintf(tmp, "$LAnn %3d|%2d,%3d|%2d  RAnn %3d|%2d,%3d|%2d\0",
           LP->ann, LP->ani, LP->ann2, LP->ani2, RP->ann, RP->ani, RP->ann2, RP->ani2);
   fPText(0, 90, tmp, 0, 255, 255);
   sprintf(tmp, "$L %s|R %s\0", LP->KB->b, RP->KB->b);
   fPText(0, 110, tmp, 0, 255, 255);
   sprintf(tmp, "$%d L%d  R%d\0", r1End, LP->end, RP->end);
   fPText(0, 130, tmp, 0, 255, 255);
   return (0);
}

#define _AS(X) ((X)&0xc000)
#define _AT(X) ((X)&0x3800)
#define _AI(X) ((X)&0x0780)
#define _MK(X) ((X)&0x0040)
#define _AK(X) ((X)&0x0020)
#define _SK(X) ((X)&0x0010)
#define _AP(X) ((X)&0x000c)
#define _DP(X) ((X)&0x0003)

#define isMove(X) (((X)&0xc000) == 0x0000)
#define isAttack(X) (((X)&0xc000) == 0x4000)
#define isDefence(X) (((X)&0xc000) == 0x8000)
#define isDamage(X) (((X)&0xe000) == 0xc000)
#define isFork(X) (((X)&0xe000) == 0xe000)
#define isUpSideDown(X) ((X)&0x0080)
#define isFullDefence(X) (((X)&0x0003) == 0x0003)
#define isRAttack(X) ((X)&0x000c)
#define REAL_DEFENCE_ACTION 58
#define DAMAGE_ACTION 71
#define WAVE_FX_ACTION 66

#define LMARGIN 20
#define RMARGIN 310
#define END_OF_ACTION 253

initPlayer(P, x, y, mirr)
    PLAYER *P;
int x, y, mirr;
{
   P->ann = 0;
   P->ani = 0;
   P->energe = 120;
   P->ve = 120;
   P->x = x;
   P->y = y;
   P->mirr = mirr;
   P->x2 = 0;
   P->y2 = 0;
   P->end = 0;
   P->chicken = 2;
   P->ann2 = END_OF_ACTION;
   P->ani2 = 0;
   P->mirr2 = mirr;
   P->i = P->i2 = 0;
   P->hit = 0;
   return (0);
}

setPostition(LP, RP, off, moff)
    PLAYER *LP,
    *RP;
int *off, moff;
{
   int xl, xr, yl, yr, toff, aL, aR, ansizeL, ansizeR;
   int an, dxl, dxr, pull, appr;

   setPlayerActor(LP, LP->ann, LP->ani, &LP->ar, &LP->i);
   aL = LP->ar;
   setPlayerActor(RP, RP->ann, RP->ani, &RP->ar, &RP->i);
   aR = RP->ar;

   xl = LP->x;
   yl = LP->y + LP->AR[aL].dy;
   xr = RP->x;
   yr = RP->y + RP->AR[aR].dy;
   dxl = LP->AR[aL].dx;
   dxr = RP->AR[aR].dx;

   if (LP->mirr)
      dxl *= (-1);
   if (RP->mirr)
      dxr *= (-1);
   appr = 0;
   if (abs(xl - xr) > abs(xl + dxl - xr - dxr))
      appr = 1;

   if (LP->hit > 0)
   {
      LP->hit--;
      dxl = 0;
   }
   if (RP->hit > 0)
   {
      RP->hit--;
      dxr = 0;
   }

   toff = *off;
   xl += dxl;
   xr += dxr;
   pull = 0;
   /*
   if (abs(xl-xr)<30 && is2568Action(LP,RP))
   {
     xl += dxr ; xr += dxl ;
     pullControl(&xl,&xr,toff,moff) ;
   }

   if (isDamage(LP->i)||isDamage(RP->i)||LP->ann==67||LP->ann==68
       ||RP->ann==67||RP->ann==68) if (r1End==0) pullControl(&xl,&xr,toff,moff) ;
*/
   if (abs(xl - xr) < 30 && abs(yl - yr) < 30 && appr && LP->ann < 10 && RP->ann < 10)
   {
      xl += dxr;
      xr += dxl;
      pull = 1;
   }
   if (isDamage(LP->i) || isDamage(RP->i) || LP->ann == 67 || LP->ann == 68 || RP->ann == 67 || RP->ann == 68)
      if (r1End == 0)
         pull = 1;

   if (pull)
      pullControl(&xl, &xr, toff, moff);

   if (xl < LMARGIN && xr < RMARGIN && toff > 0)
   {
      toff -= ((LMARGIN - 4 - xl) / 8 + 1);
      xr += (LMARGIN - xl);
   }
   if (xl > RMARGIN && xr > LMARGIN && toff < moff)
   {
      toff += ((xl - RMARGIN - 4) / 8 + 1);
      xr -= (xl - RMARGIN);
   }
   if (xr < LMARGIN && xl < RMARGIN && toff > 0)
   {
      toff -= ((LMARGIN - xr - 4) / 8 + 1);
      xl += (LMARGIN - xr);
   }
   if (xr > RMARGIN && xl > LMARGIN && toff < moff)
   {
      toff += ((xr - RMARGIN - 4) / 8 + 1);
      xl -= (xr - RMARGIN);
   }

   if (xl < LMARGIN)
      xl = LMARGIN;
   if (xl > RMARGIN)
      xl = RMARGIN;
   if (xr < LMARGIN)
      xr = LMARGIN;
   if (xr > RMARGIN)
      xr = RMARGIN;

   if (toff < 0)
      toff = 0;
   if (toff > moff)
      toff = moff;

   *off = toff;
   LP->x = xl;
   LP->y = yl;
   RP->x = xr;
   RP->y = yr;

   LP->ani++;
   RP->ani++;
   ansizeL = LP->AN->n[LP->ann];
   ansizeR = RP->AN->n[RP->ann];

   if (isFork(LP->i))
      creatTempAction(LP);
   if (isFork(RP->i))
      creatTempAction(RP);

   if (LP->ani > ansizeL - 2)
   {
      an = LP->AN->Ap[LP->ann] + LP->ani;
      LP->ann = LP->AN->As[an];
      LP->ani = 0;
      if (r1End)
         setWinnerOrLoserAction(LP, RP);
   }
   if (RP->ani > ansizeR - 2)
   {
      an = RP->AN->Ap[RP->ann] + RP->ani;
      RP->ann = RP->AN->As[an];
      RP->ani = 0;
      if (r1End)
         setWinnerOrLoserAction(RP, LP);
   }

   setTempPosition(LP);
   setTempPosition(RP);
   return (0);
}

creatTempAction(RP)
    PLAYER *RP;
{
   int an;

   if (isRAttack(RP->i2))
   {
      RP->ani++;
      return (0);
   }
   an = RP->AN->Ap[RP->ann] + RP->ani;
   RP->ann2 = RP->AN->As[an];
   RP->ani2 = 0;
   RP->ani++;
   RP->mirr2 = RP->mirr;
   RP->x2 = RP->x;
   RP->y2 = RP->y;
   return (0);
}

pullControl(xl, xr, off, moff) int *xl, *xr, off, moff;
{
   if (abs((*xl) - (*xr)) > 109)
      return (0);
   if ((*xl) < LMARGIN && off <= 0)
      (*xr) += (LMARGIN - (*xl));
   if ((*xl) > RMARGIN && off >= moff)
      (*xr) -= ((*xl) - RMARGIN);
   if ((*xr) < LMARGIN && off <= 0)
      (*xl) += (LMARGIN - (*xr));
   if ((*xr) > RMARGIN && off >= moff)
      (*xl) -= ((*xr) - RMARGIN);
   return (0);
}

setWinnerOrLoserAction(PL, PR)
    PLAYER *PL,
    *PR;
{
   if (PL->end)
      return (0);
   if (r1End == KO_END)
   {
      if (PL->energe > 0)
         setAction(PL, WINNER_ACTION + random(2));
      else
         setAction(PL, LOSER_ACTION);
   }

   if (r1End == TIME_END)
   {
      setAction(PL, TIME_LOSER_ACTION);
      if (PL->energe > PR->energe)
         setAction(PL, WINNER_ACTION + random(2));
      if (PL->energe < PR->energe)
         setAction(PL, TIME_LOSER_ACTION);
   }

   PL->end = 1;
   return (0);
}

setTempPosition(LP)
    PLAYER *LP;
{
   int xl, yl, aL, ansizeL;
   int an, dxl;

   if (LP->ann2 == END_OF_ACTION)
      return (0);

   setPlayerActor(LP, LP->ann2, LP->ani2, &LP->ar2, &LP->i2);
   aL = LP->ar2;
   dxl = LP->AR[aL].dx;
   if (LP->mirr2)
      dxl *= (-1);
   LP->x2 += dxl;
   LP->y2 += LP->AR[aL].dy;

   LP->ani2++;
   ansizeL = LP->AN->n[LP->ann2];

   if (LP->ani2 > ansizeL - 2)
   {
      an = LP->AN->Ap[LP->ann2] + LP->ani2;
      LP->ann2 = LP->AN->As[an];
      LP->ani2 = 0;
   }
   return (0);
}

is2568Action(LP, RP)
    PLAYER *LP,
    *RP;
{
   if (LP->ann == 6)
      switch (RP->ann)
      {
      case 0:
      case 2:
      case 3:
      case 5:
      case 8:
      case 6:
         return (1);
      }

   if (RP->ann == 6)
      switch (LP->ann)
      {
      case 0:
      case 2:
      case 3:
      case 5:
      case 8:
      case 6:
         return (1);
      }

   return (0);
}

drawBackground(scr, off, xs) int scr, off, xs;
{
   writeMode(0);
   imgHFill(_PSEG[0], 0, 32 * 80 - scr * 80, 0);
   writeMode(1);
   pImgCpLU(0xa800, off, xs / 4, 80, 168 + scr, _PSEG[32 - scr], 0, 80);
   writeMode(0);
   return (0);
}

drawActor(LP, RP, scr)
    PLAYER *LP,
    *RP;
int scr;
{
   int sL, sR, xl, yl, xr, yr, off;

   xl = LP->x;
   yl = LP->y - scr;
   xr = RP->x;
   yr = RP->y - scr;
   sL = LP->AR[LP->ar].sp;
   sR = RP->AR[RP->ar].sp;

   pPutRLESprMode(xl, 182 - scr, &LP->Spr[0], LP->Img, LP->mirr);
   pPutRLESprMode(xr, 182 - scr, &RP->Spr[0], RP->Img, RP->mirr);

   if (isAttack(LP->i) || isFork(LP->i))
   {
      off = 0;
      if (isUpSideDown(RP->i))
         off = 2;
      pPutRLESprMode(xr, yr, &RP->Spr[sR], RP->Img, RP->mirr + off);
      off = 0;
      if (isUpSideDown(LP->i))
         off = 2;
      pPutRLESprMode(xl, yl, &LP->Spr[sL], LP->Img, LP->mirr + off);
   }
   else
   {
      off = 0;
      if (isUpSideDown(LP->i))
         off = 2;
      pPutRLESprMode(xl, yl, &LP->Spr[sL], LP->Img, LP->mirr + off);
      off = 0;
      if (isUpSideDown(RP->i))
         off = 2;
      pPutRLESprMode(xr, yr, &RP->Spr[sR], RP->Img, RP->mirr + off);
   }

   drawTempActor(LP);
   drawTempActor(RP);
   return (0);
}

drawTempActor(PL)
    PLAYER *PL;
{

   int sL;

   if (PL->ann2 == END_OF_ACTION)
      return (0);

   sL = PL->AR[PL->ar2].sp;
   pPutRLESprMode(PL->x2, PL->y2, &PL->Spr[sL], PL->Img, PL->mirr2);
   if (PL->x2 <= -10 || PL->x2 >= 340)
   {
      PL->ann2 = END_OF_ACTION;
      PL->i2 = 0;
   }
   return (0);
}

drawScoreBoard(LP, RP, t)
    PLAYER *LP,
    *RP;
int t;
{
   char tmp[50];
   int c;

   writeMode(0);
   sprintf(tmp, "$1P %5d0      WD40 999999     2P %5d0\0", LP->scr, RP->scr);
   fPText(2, 0, tmp, 0, _R7 | _G5, _R7 | _G5);
   pBar2(31, 20, 289, 30, _R4);
   pBox2(31, 20, 289, 30, 255);
   if (LP->ve > LP->energe)
      LP->ve--;
   if (RP->ve > RP->energe)
      RP->ve--;

   if (RP->ve > 0)
      pBar2(168, 21, 168 + RP->ve, 29, _R5 | _G7);
   if (LP->ve > 0)
      pBar2(152 - LP->ve, 21, 152, 29, _R5 | _G7);

   pBar2(152, 20, 168, 30, 15);
   c = 255;
   if (LP->energe < 40 || RP->energe < 40)
      c = (t % 2) * 8 + 247;
   fPText(152, 18, "$KO", 0, c, 255);
   fPText(30, 31, pName[LP->pl], 0, 255, 255);
   fPText(228, 31, pName[RP->pl], 0, 255, 255);
   sprintf(tmp, "$%3d\0", t / 10);
   if (t > 150 || (t % 2) || r1End)
      fPText(148, 31, tmp, 0, 200, 200);
   sprintf(tmp, "$%2d\0", LP->win);
   fPText(8, 18, tmp, 0, 255, 255);
   sprintf(tmp, "$%-2d\0", RP->win);
   fPText(292, 18, tmp, 0, 255, 255);
   if (NewPlayer)
   {
      fPText(48, 80, "$>>>> #JB@FI@LND DI@MEDMA@ ONHSGD $<<<<", 0, 230 + NewPlayer / 4, 255);
      delay(100);
   }
   return (0);
}

setPlayerActor(PL, ann, ani, ar, i)
    PLAYER *PL;
int ann, ani, *ar;
unsigned int *i;
{
   int an;

   an = PL->AN->Ap[ann] + ani;
   *ar = PL->AN->As[an];
   *i = PL->AR[(*ar)].i;
   return (0);
}

setNextAction(MP, EP)
    PLAYER *MP,
    *EP;
{
   int r;

   if (r1End)
      return (0);
   if (!_AK(EP->i))
      flushKey(EP->K);
   if (!_AK(MP->i))
      flushKey(MP->K);

   if (isRAttack(EP->i) && !isFullDefence(MP->i))
      if (damageCheck(MP, EP))
      {
         return (0);
      }
   if (isRAttack(EP->i2) && !isFullDefence(MP->i))
      if (waveDamageCheck(MP, EP))
      {
         return (0);
      }

   if (_SK(MP->i))
      if (specialAttack(MP))
         return (0);
   if (_MK(MP->i))
      movePlayer(MP, EP);
   if (_AK(MP->i))
   {
      r = combAttack(MP, EP);
      if (r)
         return (r);
      if (normalAttack(MP, EP))
         return (0);
   }
   return (0);
}

#define attackPower(X) (((X)&0x3800) >> 11)
#define damageAction(X) (((X)&0x0700) >> 8)
#define KO_DAMAGE 79
#define CHICKEN_DAMAGE 80

damageCheck(MP, EP)
    PLAYER *MP,
    *EP;
{
   if (isOverLap(MP, MP->AR[MP->ar].sp, MP->x, MP->y, MP->mirr,
                 EP, EP->AR[EP->ar].sp, EP->x, EP->y, EP->mirr) == 0)
      return (0);
   if (isDefence(MP->i) && (((EP->i & 0x0c) >> 2) != (MP->i & 0x3)))
   {
      setAction(MP, REAL_DEFENCE_ACTION + MP->ann);
      if (EP->ann > 40)
         MP->energe -= 2;
      EP->hit = 3;
      sayVoice("defence.voc");
      return (1);
   }
   setDamageAction(MP, EP, DAMAGE_ACTION + damageAction(EP->i),
                   attackPower(EP->i), 0);
   return (1);
};

waveDamageCheck(MP, EP)
    PLAYER *MP,
    *EP;
{
   int a, b, xl, xr, dxl, dxr;

   if (isRAttack(MP->i2))
      if (isOverLap(MP, MP->AR[MP->ar2].sp, MP->x2, MP->y2, MP->mirr2,
                    EP, EP->AR[EP->ar2].sp, EP->x2, EP->y2, EP->mirr2))
      {
         EP->ann2 = WAVE_FX_ACTION;
         EP->ani2 = 0;
         MP->ann2 = WAVE_FX_ACTION;
         MP->ani2 = 0;
         sayVoice("waveFx.voc");
         return (1);
      }

   if (isOverLap(MP, MP->AR[MP->ar].sp, MP->x, MP->y, MP->mirr,
                 EP, EP->AR[EP->ar2].sp, EP->x2, EP->y2, EP->mirr2) == 0)
      return (0);

   if (isDefence(MP->i))
   {
      EP->ann2 = WAVE_FX_ACTION;
      EP->ani2 = 0;
      MP->energe -= 2;
      setAction(MP, REAL_DEFENCE_ACTION + MP->ann);
      sayVoice("defence.voc");
      return (1);
   }

   setDamageAction(MP, EP, DAMAGE_ACTION + damageAction(EP->i2),
                   attackPower(EP->i2), 0);
   EP->ann2 = WAVE_FX_ACTION;
   EP->ani2 = 0;
   return (1);
};

setDamageAction(MP, EP, da, power, flag)
    PLAYER *MP,
    *EP;
int da, power, flag;
{
   int sc, i;
   char *tmp = "d#.voc";

   HIT = 1;
   EP->scr += (power * 10);

   tmp[1] = power % 10 + '0';
   if (flag == 0)
      sayVoice(tmp);

   sc = (EP->power - MP->resis + 120) / 30;

   MP->energe -= (1 + power * sc);
   MP->chicken += power;
   if (flag)
      MP->chicken = 2;
   if (MP->chicken > 6 && MP->y == 180)
   {
      da = CHICKEN_DAMAGE;
      MP->chicken = 0;
   }
   if (MP->energe <= 0)
   {
      r1End = KO_END;
      sayVoice("ko.voc");
      if (flag == 0)
         da = KO_DAMAGE;
   }
   if (flag == 0)
   {
      if (MP->y < 150)
      {
         i = getIndex(MP);
         replaceAction(MP, da, 4, i, 0x3);
      }
      else
      {
         MP->y = 180;
         setAction(MP, da);
      }
   }
   else
      setAction(MP, da);
   return (0);
}

/*
isOverLap(MP,sL,xl0,yl0,lm,EP,sR,xr0,yr0,rm)
PLAYER *MP,*EP ;
int sL,xl0,yl0,lm,sR,xr0,yr0,rm ;
{
   int ln,rn,i,j ;
   int xl1,yl1,xl2,yl2,xr1,xr2,yr1,yr2 ;
   int X1,X2,Y1,Y2,ilp,irp ;
   char tmp[100] ;
   Sprite *SprM,*SprE ;
   Image  *ImgM,*ImgE  ;

   SprM = MP->Spr ; ImgM = MP->Img ;
   SprE = EP->Spr ; ImgE = EP->Img ;
   ln=SprM[sL].n  ; rn=SprE[sR].n ;

   for ( i=0 ; i<ln ; i++ )
    for ( j=0 ; j<rn ; j++ )
    {
      ilp=SprM[sL].i[i] ;
      yl1=yl0+SprM[sL].yo[i] ; yl2=yl1+((int)ImgM[ilp].ys*2)/3-1 ;
      xl1=xl0+SprM[sL].xo[i] ; if (lm) xl1=xl0-SprM[sL].xo[i]-SprM[sL].xs ;
      xl2=xl1+ImgM[ilp].xs-1 ;
      irp=SprE[sR].i[j] ;
      yr1=yr0+SprE[sR].yo[j] ; yr2=yr1+ImgE[irp].ys-1 ;
      xr1=xr0+SprE[sR].xo[j] ; if (rm) xr1=xr0-SprE[sR].xo[j]-SprE[sR].xs ;
      xr2=xr1+ImgE[irp].xs-1 ;
      X1=max(xl1,xr1) ; X2=min(xl2,xr2) ;
      Y1=max(yl1,yr1) ; Y2=min(yl2,yr2) ;
      if (X2>X1 && Y2>Y1) return(1) ;
    }
   return(0) ;
}
*/

isOverLap(MP, sL, xl0, yl0, lm, EP, sR, xr0, yr0, rm)
    PLAYER *MP,
    *EP;
int sL, xl0, yl0, lm, sR, xr0, yr0, rm;
{
   int ln, rn, i, j;
   int xl1, yl1, xl2, yl2, xr1, xr2, yr1, yr2;
   int X1, X2, Y1, Y2, ilp, irp, t1, t2;
   char tmp[100];
   Sprite *SprM, *SprE;
   Image *ImgM, *ImgE;

   SprM = MP->Spr;
   ImgM = MP->Img;
   SprE = EP->Spr;
   ImgE = EP->Img;

   ln = SprM[sL].n;
   rn = SprE[sR].n;

   //   printf("\n\n (%3d,%3d) sL=%3d  (%3d,%3d) sR=%3d\n",xl0,yl0,sL,xr0,yr0,sR) ;

   for (i = 0; i < ln; i++)
   {
      ilp = SprM[sL].i[i];
      yl1 = yl0 + SprM[sL].yo[i];
      yl2 = yl1 + ImgM[ilp].ys - 1;
      if (yl0 < 180)
         yl2 = yl1 + ((int)ImgM[ilp].ys * 2) / 3 - 1;
      xl1 = xl0 + SprM[sL].xo[i];
      if (lm)
         xl1 = xl0 - SprM[sL].xo[i] - SprM[sL].xs;
      xl2 = xl1 + ImgM[ilp].xs - 1;

      irp = SprE[sR].i[j];
      t1 = SprE[sR].xo[0];
      t2 = SprE[sR].yo[0];
      yr1 = yr0 + SprE[sR].yh1 + t2;
      yr2 = yr0 + SprE[sR].yh2 + t2;
      xr1 = xr0 + SprE[sR].xh1 + t1;
      if (rm)
         xr1 = xr0 - SprE[sR].xh1 - t1;
      xr2 = xr0 + SprE[sR].xh2 + t1;
      if (rm)
         xr2 = xr0 - SprE[sR].xh2 - t1;
      t1 = min(xr1, xr2);
      t2 = max(xr1, xr2);
      xr1 = t1;
      xr2 = t2;
      t1 = min(yr1, yr2);
      t2 = max(yr1, yr2);
      yr1 = t1;
      yr2 = t2;

      //      printf("%d : %3d %3d %3d %3d - %3d %3d %3d %3d\n",i,xl1,yl1,xl2,yl2,xr1,yr1,xr2,yr2) ;

      X1 = max(xl1, xr1);
      X2 = min(xl2, xr2);
      Y1 = max(yl1, yr1);
      Y2 = min(yl2, yr2);
      if (X2 >= X1 && Y2 >= Y1)
         return (1);
   }
   return (0);
}

specialAttack(PL)
    PLAYER *PL;
{
   int code;

   if ((code = getSpecialCode(PL->KR, PL->KB, PL->ctrl, PL->san)) == 0)
      return (0);
   PL->KB->b[0] = PL->KB->p = 0;
   flushKey(PL->K);
   PL->scr += 100;
   setAction(PL, code);
   return (1);
}

getSpecialCode(KR, KB, ctrl, san)
    KEYRULE *KR;
KEYBUFF *KB;
int ctrl;
char san;
{
   int i, p;
   unsigned char b[16];

   if (ctrl == COMPU)
   {
      if (random(10) == 0)
      {
         i = random(san);
         sayVoice(KR[i].vfn);
         return (KR[i].code);
      }
      return (0);
   }

   p = KB->p;
   for (i = 0; i <= 15; i++)
   {
      p--;
      if (p < 0)
         p = 15;
      b[i] = KB->b[p];
   }

   for (i = 0; KR[i].code > 0; i++)
      if (patternMatch(KR[i].s, b))
      {
         sayVoice(KR[i].vfn);
         KB->b[KB->p] = '?';
         return (KR[i].code);
      }
   return (0);
}

patternMatch(str1, str2) char *str1, *str2;
{
   int i;
   for (i = 0; str1[i] != '\0'; i++)
   {
      if (islower(str1[i]))
         str2[i] = tolower(str2[i]);
      if (str1[i] != str2[i])
         return (0);
   }
   return (1);
}

combAttack(MP, EP)
    PLAYER *MP,
    *EP;
{
   int lc, i, kc, ak, xmin, xmax, clc, dm;

   ak = getAttackCode(MP->K, MP->ctrl);
   kc = 10 * getMoveCode(MP->K, 0, MP->ctrl) + ak;

   if (ak == 0)
      return (0);
   lc = 0;
   if (MP->y == 180 && EP->y == 180 && abs(MP->x - EP->x) < MP->nr)
      lc = 1;
   if (MP->y < 120 && EP->y < 120 && abs(MP->x - EP->x) < MP->nr && abs(MP->y - EP->y) < 45)
      lc = 2;
   if (MP->y == 180 && EP->y == 180 && abs(MP->x - EP->x) > MP->nr && abs(MP->x - EP->x) < MP->nr + 15)
      lc = 3;
   if (lc == 0)
      return (0);

   for (i = 0; MP->CR[i].lc < 100; i++)
   {
      clc = MP->CR[i].lc % 10;
      dm = MP->CR[i].lc / 10;
      if (dm == 0)
         dm = 3;
      if (lc == clc && kc == MP->CR[i].kc)
      {
         if (clc == 2)
         {
            EP->y = 105;
            MP->y = 105;
         }
         if (MP->CR[i].dann == 0)
         {
            setAction(MP, MP->CR[i].ann);
            return (1);
         }
         xmin = min(MP->x, EP->x);
         xmax = max(MP->x, EP->x);
         if (!isMove(EP->i))
            break;
         switch (MP->CR[i].mc)
         {
         case 0:
            if (EP->mirr)
               EP->x = MP->x + 20;
            else
               EP->x = MP->x - 20;
            break;
         case 1:
            MP->x = xmin;
            EP->x = xmin + 20;
            MP->mirr = 0;
            EP->mirr = 1;
            break;
         case 2:
            MP->x = xmax;
            EP->x = xmax - 20;
            MP->mirr = 1;
            EP->mirr = 0;
            break;
         case 3:
            MP->x = xmin;
            EP->x = xmin + 20;
            MP->mirr = 1;
            EP->mirr = 1;
            break;
         case 4:
            MP->x = xmax;
            EP->x = xmax - 20;
            MP->mirr = 0;
            EP->mirr = 0;
            break;
         }
         setAction(MP, MP->CR[i].ann);
         sayVoice(MP->CR[i].vfn);
         setDamageAction(EP, MP, MP->CR[i].dann, dm, 1);
         flushKey(MP->K);
         return (1);
      }
   }
   return (0);
}

normalAttack(MP, EP)
    PLAYER *MP,
    *EP;
{
   int mc, ac, nr, mr, an;
   char *tmp = "a##.voc";

   ac = getAttackCode(MP->K, MP->ctrl);

   if (ac == 0)
      return (0);
   nr = 0;
   if (abs(MP->x - EP->x) < MP->nr && EP->y == 180)
      nr = 1;
   switch (MP->ann)
   {
   case 2:
   case 5:
   case 8:
      mc = 4;
      nr = 0;
      break;
   case 1:
      mc = 2;
      nr = 0;
      break;
   case 4:
   case 7:
      mc = 3;
      nr = 0;
      break;
   default:
      mc = 0;
      break;
   }
   an = (nr + mc) * 6 + ac + 10;
   tmp[1] = an / 10 + '0';
   tmp[2] = an % 10 + '0';
   sayVoice(tmp);
   if (mc != 2 & mc != 3)
      setAction(MP, (nr + mc) * 6 + ac + 10);
   else
      replaceAction(MP, mc * 6 + ac + 10, MP->ann, MP->ani, 0);
   flushKey(MP->K);
   return (1);
}

#define DUMY_ACTION 109

replaceAction(PL, ac, ann, ani, mask)
    PLAYER *PL;
int ac, ann, ani, mask;
{
   int sanp, sans, tanp, sarp, tarp;
   int ranp, rans, rarp, i, j, tar0, k;

   sanp = PL->AN->Ap[ann];
   sans = PL->AN->n[ann];
   tanp = PL->AN->Ap[DUMY_ACTION];
   tar0 = PL->AN->As[tanp];
   ranp = PL->AN->Ap[ac];
   rans = PL->AN->n[ac];
   rans--;

   for (j = 0, i = ani; i < sans - 1; i++, j++)
   {
      sarp = PL->AN->As[i + sanp];
      tarp = tar0 + j;
      PL->AR[tarp].sp = PL->AR[sarp].sp;
      PL->AR[tarp].dx = PL->AR[sarp].dx;
      PL->AR[tarp].dy = PL->AR[sarp].dy;
      PL->AR[tarp].i = 0x0000 | mask;
      PL->AN->As[tanp + j] = tarp;
   }

   for (i = 0, k = 0; i < rans; i++)
   {
      rarp = PL->AN->As[i + ranp];
      tarp = tar0 + k;
      if (isFork(PL->AR[rarp].i))
      {
         i++;
         continue;
      }
      PL->AR[tarp].sp = PL->AR[rarp].sp;
      PL->AR[tarp].i = PL->AR[rarp].i | mask;
      k++;
   }

   PL->AN->n[DUMY_ACTION] = j + 1;
   PL->AN->As[tanp + j] = 0;
   PL->ann = DUMY_ACTION;
   PL->ani = 0;
   return (0);
}

getIndex(PL)
    PLAYER *PL;
{
   int i, y, ar, anp, ans;

   y = 180;

   anp = PL->AN->Ap[4];
   ans = PL->AN->n[4];
   for (i = 0; i < ans; i++)
   {
      ar = PL->AN->As[anp + i];
      y += PL->AR[ar].dy;
      if (abs(y - PL->y) < 20)
         break;
   }
   PL->y = y - PL->AR[ar].dy;
   return (i);
}

getAttackCode(K, ctrl)
    CTRLKEY *K;
int ctrl;
{
   if (ctrl == COMPU)
   {
      if (random(20) == 0)
         return (random(6) + 1);
      return (0);
   }
   if (_KE[K->x])
      return (1);
   if (_KE[K->y])
      return (2);
   if (_KE[K->z])
      return (3);
   if (_KE[K->a])
      return (4);
   if (_KE[K->b])
      return (5);
   if (_KE[K->c])
      return (6);
   return (0);
}

#define HEAD_DEFENCE_ACTION 9
#define FOOT_DEFENCE_ACTION 10

/* 레이어를 움직인다 */
movePlayer(PL, EP)
    PLAYER *PL,
    *EP;
{
   int key, mc, defence;

   if (PL->x < EP->x)
   {
      PL->mirr = 0;
      PL->key2asc[1] = 'b';
      PL->key2asc[2] = 'f';
   }
   if (PL->x > EP->x)
   {
      PL->mirr = 1;
      PL->key2asc[1] = 'f';
      PL->key2asc[2] = 'b';
   }

   mc = getMoveCode(PL->K, PL->mirr, PL->ctrl);
   defence = 0;
   if (isAttack(EP->i) && abs(PL->x - EP->x) < 130)
      defence = 1;
   if (isAttack(EP->i2) && abs(PL->x - EP->x2) < 130)
      defence = 1;

   if (defence)
   {
      if (mc == 3)
      {
         setAction(PL, HEAD_DEFENCE_ACTION);
         return (0);
      }
      if (mc == 5)
      {
         setAction(PL, FOOT_DEFENCE_ACTION);
         return (0);
      }
   }
   if (PL->ann != mc)
      setAction(PL, mc);
   return (0);
}

/* 동작을 설정한다 */
setAction(PL, act)
    PLAYER *PL;
int act;
{
   PL->ann = act;
   PL->ani = 0;
   return (0);
}

/* 움직임 키이 상하를 얻는다 */
getMoveCode(K, mirr, ctrl)
    CTRLKEY *K;
int mirr, ctrl;
{
   int h, v, l, r;

   h = v = 0;

   if (ctrl == COMPU)
   {
      if (random(20) == 0)
         return (random(9));
      return (0);
   }

   l = 3;
   r = 6;
   if (mirr)
   {
      l = 6;
      r = 3;
   }

   if (_KS[K->U])
      v = 1;
   if (_KS[K->D])
      v = 2;
   if (_KS[K->L])
      h = l;
   if (_KS[K->R])
      h = r;

   return (h + v);
}

/* key 화일을 읽어서 버퍼에 저장 */
int loadAttackRule(char fn[], int *nr, KEYRULE *PKR, COMBRULE *PCR, PLAYER *PL)
{
   unsigned char s[10];
   FILE *fp;
   int n, i, code, lc, kc, ann, dann, mc;

   fp = fopen(fn, "r");
   if (fp == NULL)
      cError(fn, " not found!", 1);

   fscanf(fp, "%d", nr);
   for (n = 0;; n++)
   {
      fscanf(fp, "%s %d %s", &PKR[n].s[0], &code, &PKR[n].vfn[0]);
      PKR[n].code = code;
      if (code < 0)
         break;
   }

   PL->san = n;
   for (n = 0;; n++)
   {
      fscanf(fp, "%d %d %d %d %d %s", &lc, &kc,
             &ann, &dann, &mc, &PCR[n].vfn[0]);
      PCR[n].lc = lc;
      PCR[n].kc = kc;
      PCR[n].ann = ann;
      PCR[n].dann = dann;
      PCR[n].mc = mc;
      if (PCR[n].lc == 255)
         break;
   }

   PL->can = n;
   fclose(fp);
   return (0);
}

/* seq화일을 읽어서 seq버퍼에 저장한다 */
loadSeq(fn, AN, AR) char fn[];
ACTION *AN;
ACTOR *AR;
{
   FILE *fp;
   int code, n, k, s, dx, dy;
   char info[10];
   unsigned int f0, f1, f2, f3, f4, f5, f6;

   fp = fopen(fn, "r");
   if (fp == NULL)
      cError(fn, " Not Found", 1);

   for (n = 0;;)
   {
      fscanf(fp, "%d", &code);
      if (code == (-1))
         break;
      AN->Ap[code] = n;
      for (k = 0;; n++, k++)
      {
         fscanf(fp, "%d", &s);
         if (s == (-1))
            break;
         AN->As[n] = s;
      }
      AN->n[code] = k;
   }

   for (n = 0;; n++)
   {
      if (fscanf(fp, "%d %d %d %d %s", &code, &s, &dx, &dy, info) != 5)
         break;
      AR[code].sp = s;
      AR[code].dx = dx;
      AR[code].dy = dy;
      field(info[0], "MADF", &f6);
      field(info[1], "01234567", &f5);
      field(info[2], "012345678", &f4);
      field(info[3], "NU", &f3);
      field(info[4], "01234567", &f2);
      field(info[5], "xLHA", &f1);
      field(info[6], "xLHA", &f0);
      AR[code].i = (f6 << 14) | (f5 << 11) | (f4 << 8) | (f3 << 7) | (f2 << 4) | (f1 << 2) | f0;
   }
   fclose(fp);
   return (n);
}

int field(char ch, char str[], unsigned int *f)
{
   unsigned int i;

   *f = 0;

   for (i = 0; str[i] != '\0'; i++)
      if (str[i] == ch)
         *f = i;
   return (0);
}

/* 점점 화면을 밝게 한다 */
void fadeIn()
{
   int i;
   for (i = 0; i <= 100; i += 10)
   {
      brightCtrl(0, 256, R_, G_, B_, i);
      if (_KBHIT)
         break;
   }
   brightCtrl(0, 256, R_, G_, B_, 100);
}

/* 점점 화면을 어둡게 한다 */
void fadeOut()
{
   int i;

   _KBHIT = 0;
   for (i = 100; i >= 0; i -= 10)
   {
      brightCtrl(0, 256, R_, G_, B_, i);
      if (_KBHIT)
         break;
   }
   brightCtrl(0, 256, R_, G_, B_, 0);   
}

/* 에러 출력하고 도스로 빠져나옴 */
cError(ms1, ms2, m) char ms1[], ms2[];
int m;
{
   if (m)
   {
      setOldSystemInt();
      vgaTextMode();
   }
   offMusic();
   silentVoice();
   printf("\n\n%s%s\n", ms1, ms2);
   printf("Free Memory = %ld + 120000\n", coreleft());
   exit(-1);
   return (0);
}

/* 게임이 종료하면 도스로 원상 복귀 */
void systemDown(void)
{
   offMusic();
   silentVoice();
   vgaTextMode();
   setOldSystemInt();
   printf("Good Bye !\n");
   printf("Thanks for Hitel Members\n");
   printf("Presented By Jung Y.D. & Game Association of Hitel\n");
   exit(0);
};
