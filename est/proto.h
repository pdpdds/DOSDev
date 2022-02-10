
/* EGI.C */
#ifndef _EGI
extern void vPointThick(int x, int y, BYTE mode);
extern void vPointNormal(int x, int y, BYTE mode);
extern BYTE bGetGraphMode(void);
extern void vSetHercGraphMode(BYTE flag);
extern void vSetPalette(BYTE s, BYTE *pal);
extern void vInitOffsetX(BYTE flag, WORD width);
extern int iSetVideoMode(BYTE flag);
extern int iInitGraphMode(BYTE g_mode, BYTE first_next);
extern void vDeinitGraphMode(void);
extern void vGraphClipOff(void);
extern void vGraphClipOn(int x, int y, int ex, int ey);
extern void vNLineXBit(int startx, int endx, int y, BYTE mode);
extern void vWLineXBit(int x, int endx, int y);
extern void vBLineXBit(int x, int endx, int y);
extern void vXORLineXBit(int x, int endx, int y);
extern void vWLineYBit(int x, int y, int endy);
extern void vBLineYBit(int x, int y, int endy);
extern void vXORLineYBit(int x, int y, int endy);
extern void vNLineYBit(int x, int y, int ey, BYTE mode);
extern void vLineBit(int startx,int starty,int endx,int endy, BYTE mode);
extern void vNBoxBit(int x, int y, int ex, int ey, BYTE mode);
extern void vWBoxBit(int startx,int starty,int endx,int endy);
extern void vBBoxBit(int startx,int starty,int endx,int endy);
extern void vXORBoxBit(int startx,int starty,int endx,int endy);
extern void vNBarBit(int x, int y, int ex, int ey, BYTE mode);
extern void vWBarBit(int startx,int starty,int endx, int endy);
extern void vBBarBit(int x,int y,int ex, int ey);
extern void vXORBarBit(int x, int y, int ex, int ey);
extern void vXORBarByte(int x, int y, int h, int len);
extern void vXORBarBytex16(int x, int y, int len);
extern void vXORBlockByte(int x, int y, BYTE height, BYTE width);
extern void vPutFreeImageByte(int x, int y, WORD hsize, WORD vsize, BYTE *image, BYTE mode);
extern void vPutWordImageBit(int x, int y, BYTE *buf);
extern WORD wSizeImageBuf(int x, int y, int endx, int endy);
extern void vDrawPanelByte(WORD x, WORD y, WORD endx, WORD endy,BYTE flag);
extern void vChangeColor(BYTE cur_color, BYTE new_color);
extern void vSetColors(void);
extern void vSetBlackWhite(void);
extern void vFillScreen(unsigned SNode, unsigned second);
extern void vBarByte(int x, int y, int endx, int endy, char attr);
extern void vScrollRightByte(unsigned x, unsigned endx, unsigned y,\
       									unsigned widthy, unsigned count, unsigned char attr);
extern void vScrollLeftByte(unsigned x, unsigned endx, unsigned y,\
       									unsigned widthy, unsigned count, unsigned char attr);
extern void vScrollUpByte(unsigned x, unsigned endx, unsigned y,\
       									unsigned widthy, unsigned count);
extern void vScrollDownByte(unsigned x, unsigned endx, unsigned y,\
       									unsigned widthy, unsigned count);
extern void vPutColorImageLByte(int x, int y, int h, int w, int c, BYTE *buf);
extern void vDrawVScrollBarBit(WORD x, WORD ex, WORD y);
extern void vDrawHScrollBarBit(WORD x, WORD y, WORD ey);
extern void vDelVThumbBit(WORD x, WORD ex, WORD y, WORD total, int old_sub);
extern void vDrawVThumbBit(WORD x, WORD ex, WORD y, WORD total, int sub, int old_sub);
extern void vDelHThumbBit(WORD x, WORD y, WORD ey, WORD total, int old_sub);
extern void vDrawHThumbBit(WORD x, WORD y, WORD ey, WORD total, int sub, int old_sub);
extern void vDarkenTextWindowTitle(int n);
extern void vResetTextWindowTitle(void);
extern void vDrawWindowByte(int x, int y, int lx, int ly, BYTE flag, BYTE *str);
extern void vDrawObjByte(int n);
#endif

/* MOUSE.C */
#ifndef _MOUSE
extern void vPutColorImageFarEByte(int x, int y, int ex, int ey, BYTE *buf);
extern void vMCON(void);
extern void vMCOFF(void);
extern void vSetMousePosition(int x, int y);
extern void vGetMousePosition(int *x,int *y);
extern void vChangeMC(int x, int y, BYTE *i);
extern void vSetRange(int x, int y, int endx, int endy);
extern void vRestoreRange(void);
extern void vSetMouseSpeed(WORD ratio);
extern void vInstallSerialInt(void);
extern void huge vMouseIntService(void);
extern void vInstallEventHandler(void huge *userfunc, int mask);
extern void vDeinitMouse(void);
extern int iInitMouse(BYTE kind, BYTE port);
#endif

/* INLINE.C */
#ifndef _INLINE
extern int wGetKey(char func);
extern void vPutKey(char scan_code, char ascii_code);
extern unsigned wCheckKeyHit(void);
extern void vResetKeyBuffer(void);
extern int iGetMacroNumber(void);
extern void vSetMacro(void);
extern void vUseMacro(int num);
extern void vUseMacroRepeat(void);
#endif

/* WINFUNC2.C */
#ifndef _WINFUNC2
extern void vDispKeyInputStatus(void);
extern void vDispExtraStatus(int i);
extern void vDispRowStatus(void);
extern void vDispColumeStatus(void);
extern void vDispInsertStatus(void);
extern void vResetTextWindowValue(BYTE i, BYTE flag);
extern void vInitWindowValue(void);
extern int iHScrollBarHandler(int x, int y, int ey, int total, int sub,\
       						 int old_sub, int vHomeKey, int vExit);
extern int iVscrollBarHandler(int x, int ex, int y, int total, int sub,\
       						 int old_sub, int vHomeKey, int vExit);
extern void vRestoreWindowSize(int n);
extern void vSaveWindowSize(int n);
extern void vDrawTextWindow(BYTE n);
extern void vPushWindow(BYTE wnum);
extern int iPopWindow(void);
extern void vRedrawAllWindow(void);
extern int iCloseTextWindow(int new);
extern int iCloseAllTextWindow(void);
extern int iCheckTextWindowArea(int mx, int my);
extern int iMoveWindow(int *px, int *py, int *plx, int *ply);
extern int iMoveTextWindow(BYTE n);
extern void vSetMinWindowSize(int x, int y);
extern int iResizeWindow(int *px, int *py, int *plx, int *ply);
extern int iResizeTextWindow(BYTE n);
extern void vMaximizeWindow(void);
extern void vChangeWindow(BYTE wnum);
extern void vNextWindow(void);
extern void vKeyMoveTextWindow(BYTE n);
extern void vXORTabLoc(int n, int choice);
extern void vSetTab(int flag);
extern int iMouseInTextWindow(void);
extern int iGetWindowHandle(void);
#endif

/* HANFUNC3.C */
#ifndef _HANFUNC3
extern int iReadBitmapFont(BYTE *font, char *fname, WORD size);
extern int iReadAsciiVectorFont(BYTE *efont, WORD *eftbl, char *e_name);
extern int iReadHanJohapVectorFont(BYTE *efont, WORD *eftbl, char *e_name );
extern void vClipOff(void);
extern void vClipOn(BYTE n);
extern void vPFFfar(int x, int y, BYTE *b);
extern void vPHFfar(int x, int y, BYTE *b);
extern int iPutGraphicFont(int x, int y, BYTE b, BYTE mode);
extern void vEraseRestArea(int x, int y, BYTE cur_size, BYTE width);
extern void vPT(int x, int y, BYTE flag);
extern void vBPT(int x, int y, int sl, int el, BYTE flag);
extern void vSPT(int x, int y, BYTE flag);
extern void vChangePutMode(BYTE flag);
extern void vWriteString(int x, int y, BYTE *str, BYTE mode);
extern int iPrintf(int x, int y, BYTE mode, BYTE *fmt, ...);
extern void vSetDispOpt(BYTE sp_flag, BYTE lf_flag, BYTE cr_flag);
extern void vPutBitmapHanjaTable(int x, int y, WORD *buf, BYTE count);
extern int iSetFontTable(BYTE *fname, BYTE *hname[], BYTE *ename[], BYTE *kind);
extern int iSetHangulFontTable(void);
extern int iIsFont(BYTE *name);
extern WORD wGetFontSize(char *name);
extern int iCheckFont(void);
extern int iInitDefaultFont(void);
extern int iInitHangul(BYTE gmode);
extern void vDeinitHangul(void);
extern int iFreeFontBuffer(int s);
extern void vFreeAllUMB(void);
extern void vSetImageStartLocation(int x);
extern void vSetUnit(int x, int y);
extern void vSetImageHeight(int len);

/* vechan2.c */
extern void vClipPutImage(int x, int y, int height, int width,\
       										 BYTE flag, BYTE * buf);
extern void vPutImage(int x, int y, int ex, int ey, unsigned char *buf);
extern void vGetImage(int x, int y, int ex, int ey, unsigned char *buf);
extern void vEffectWordImage(WORD *image_16x16, WORD *timage, BYTE mode);
extern void vEffectByteImage(BYTE *image_16x8, BYTE *timage, BYTE mode);
extern void vFillPoly(void);
extern void vOpenPoly(void);
extern void vClosePoly(BYTE flag);
extern void vPutSmallFont(int x, int y, BYTE n, BYTE flag);
extern void vCutImage(int xlen, int bwid, int wid);
extern void vPutImageToPrintBuffer(int x, int y, int h, int v);
#endif

/* LAYOUT4.C */
#ifndef _LAYOUT4
extern WORD wSmallSCR2PRN(WORD n);
extern WORD wSmallPRN2SCR(WORD n);
extern WORD wMM2DOT(WORD i);
extern void vSetPaperSize(void);
extern void vSetPaper(void);
extern int iSetHeaderNodeFar(NODE *n);
extern void vFindHeader(void);
extern void vDrawPaper(void);
extern int iDispSmallNode(int sx, int sy, BYTE sline, BYTE flag, NODE *temp);
extern void vDispSmallText(int sx, int sy);
extern int iSkipText(WORD sbit);
extern void vDispPageStatus(void);
extern void vDispSmallPage(BYTE flag);
extern void vEndPreview(void);
extern void vDispPreviewMenu(void);
extern void vXORPreviewIcon(int n);
extern int iGetPreviewIconCheck(void);
extern int iPreviewMenu(void);
extern void vPreview(void);
extern int iCheckPrintScadule(void);
extern void vPutLot(int n, BYTE flag);
extern void vDelScadule(void);
extern int iSetScadule(void);
extern void vTWopt2PRNopt(int t);
extern void vPRNopt2TWopt(int t);
extern void vPrintOptDlg(int n);
extern int iGetPrinterDriver(BYTE *fname);
extern void vInitPrintSetup(void);
extern int iAddPaper(void);
extern int iDelPaper(int n);
extern void vPrinterSetupDlg(void);
extern BYTE bPrintStatus(int port);
extern int iCheckPrinter(int port);
extern void vSaveDisplayOpt(void);
extern void vRestoreDisplayOpt(void);
extern int iPrintTextWindow(int n);
extern void vPrintScaduler(void);
extern void vPrintStart(void);
#endif

/* OVLMAN.C */
#ifndef _OVLMAN
extern void vInitINT60(void);
extern void vDeinitINT60(void);
extern long lGetFileSize(BYTE *path);
extern void huge *pLoadModule(BYTE *path);
extern void far *pInitOverlay(BYTE *path);
extern void huge *pGetModuleAddr(void);
#endif

/* MENUDRV2.C */
#ifndef _MENUDRV2
extern void vFreeOurBuf(void);
extern void vExit(void);
extern void vMakeOurBuf(void);
extern int iMouseInMenu(int status,int pn);
extern void vDrawTopMenu(void);
extern void vSetMenuOpt(int n, WORD code);
extern void vRevTopMenu(int n);
extern void vSetMenu(int n,int x, int y, int count,BYTE **menu, BYTE *head);
extern void vInitMenu(void);
extern void vSetMenuParaAll(int n, int x, int y, int count, int ENode);
extern void vSetMenuPara(int n,int x, int y);
extern void vSaveMenuBk(int n);
extern void vRestoreMenuBk(int n);
extern void vRevMenuItem(int n, int choice);
extern void vPutMenu(int n);
extern int iIsIn(BYTE n, BYTE code);
extern int iGetUserResponse(int n);
extern int iPullDownMenu(int n);
extern int iGeneralMenuDrv(int pan_n,int add_val,int mul_val);
extern int iEditMenuDrv(void);
extern int iInputMenuDrv(void);
extern int iKeyMapMenuDrv(void);
extern int iWordMenuDrv(void);
extern int iPrintDrv(void);
extern int iMenuDrvier(void);
extern void vHideRod(void);
extern void vSetRod(void);
extern void vChangeActiveWindow(void);
extern void vLogo(void);
extern void vBeep(void);
extern int iAction(int n);
extern void vLetsGo(void);
#endif

/* POPUP4.C */
#ifndef _POPUP4
extern void vMakeDlgWindow(int n, int x, int y, BYTE use, BYTE **blk,\
       						 BYTE **dat, BYTE **hed, BYTE **cod, BYTE *winhead);
extern void vMakeExtDlgWindow(int n, BYTE *data, BYTE *pn, BYTE *process,\
       								BYTE *vb, BYTE *value, BYTE *set);
extern void vRevControlObj(int n, int bn, int cn);
extern void vDrawScrollBarObj(int n, int bn, int cho);
extern void vDrawRecordObj(BYTE n, BYTE bn);
extern void vDrawDlgWindow(BYTE n);
extern void vOpenDlgWindow(BYTE n);
extern int iXYIsInObj(int n, BYTE *blk, BYTE *cho);
extern int iExamineMCInObj(int n, BYTE *blk, BYTE *cho);
extern void vXORPopup(int x, int y, int ex, int ey);
extern int iMoveDlgWindow(BYTE n);
extern int iKeyMoveWindow(int *wx, int *wy, int *wlx, int *wly, BYTE code);
extern int iKeyMoveDlgWindow(BYTE n);
extern void vCloseDlgWindow(BYTE n);
extern int iGetObjOffset(int n, BYTE cur_blk, BYTE choice);
extern int iCalcChiose(int n, BYTE blk, BYTE choice);
extern void vInt2Char(int i, BYTE *buf);
extern void vChar2Int(int *i, BYTE *buf);
extern void vNumberDlgWindow(BYTE n);
extern WORD wGetNumber(int def, int start, int vExit, BYTE *unit, BYTE *head);
extern void vDispChecked(void);
extern void vSetDlgObj(int n, BYTE cur_blk, BYTE cur_cho, BYTE *addr);
extern void vSetDlgWindow(int n, BYTE *addr);
extern int iGetNextObj(int n, BYTE blk, BYTE *addr);
extern int iCheckShortCut(int n, BYTE *cur_blk, BYTE *cur_cho, BYTE code);
extern int iXYIsInArea(int x, int y, int lx, int ly);
extern void vVBHead(int n, int bn, int cho);
extern int iVScrollBarObj(int n, int bn);
extern int iDialogDriver(int n, BYTE *buf);
extern int iUserChoiceDlg(char *str);
extern int huge ihUserChoiceDlg(char *str);
extern int iFileListSearch(BYTE b, int n);
extern int iFnSplit(BYTE *rev_buf);
extern int iGetFileSubject(BYTE *str);
extern int iGetFileInfo(struct ffblk *fb);
extern int iCompFileName(const void *a, const void *b);
extern void vSortFileList(void);
extern BYTE bCheckFile(BYTE *buf);
extern int iMakeFileList(BYTE check);
extern int iDispFileList(int st, int len, int x, int y);
extern void vSetDriverName(void);
extern int iCheckDriver(BYTE drv);
extern void vFileNameSplit2(BYTE *src);
extern void vFileNameMerge2(BYTE *dest);
extern void vSetExt(BYTE *name);
extern int iAnalFileName(BYTE *src);
extern WORD wGetDiskFreeSpace(BYTE *rev_buf);
extern void vAnalSlash(BYTE *dest, BYTE *src);
extern void vDeleteFile(int n);
extern void vGetDlgObjXY(int *x, int *y, int *ex, int *ey, BYTE blk);
extern int iIsDone(void);
extern void vDispFileListBox(int n);
extern void vEraseOldFileName(void);
extern int iSetFileList(int flag);
extern void vSetFileListHScrollBar(int i,int *old_i);
extern void vExitFileListDlg(void);
extern void vRevFileListObj(int choice);
extern void vGetBlock(int choice, BYTE *cb, BYTE *cc);
extern int iFileListDlg(BYTE *src, BYTE *fname, BYTE rev_code);
extern void huge vErrorMsgDlg(BYTE *str);
extern void huge vErrorMsgfDlg(BYTE type, BYTE num, BYTE *str);
extern void vPutMenuObj2(int n, int blk, int y, BYTE *menu, BYTE len);
extern void vPutMenuObj(int n, int blk, int y, BYTE *menu);
extern int iLoadOptDlg(BYTE *cod, BYTE *way);
extern int iSaveOptDlg(BYTE *cod, BYTE *way);
extern void vDispOptDlg(void);
extern void vWipeStringArray(BYTE total, BYTE n, BYTE **array);
extern void vTestDlg(void);
extern int iGetHelpData(int n);
extern WORD wGetHelpDataLoc(int lnum, BYTE *buf);
extern void vDispHelpDlg(int n);
extern int iMouseInHelpDlg(void);
extern void vHelpDlg(int n);
extern void vHeaderFooterSizeDlg(BYTE flag, WORD s, WORD cur, WORD e, BYTE *str);
extern void vHeaderSizeDlg(void);
extern void vScreenSaverDlg(void);
extern void vEtcDlg(void);
extern void vInsertKeyMap(int n);
extern void vDeleteKeyMap(int n);
extern void vCircleKeyboardDlg(void);
extern int iGetStringDlg(BYTE flag, BYTE *str, BYTE *head);
extern void vSaveClosedTextWindow(BYTE *name);
extern void vDeleteClosedTextWindow(int n);
extern void vMoveUpCTW(int s, int n, int tot, BYTE *dat[]);
extern void vMoveDownCTW(int s, int n, int tot, BYTE *dat[]);
extern int iDispChangeTextWindowDlg(int code);
extern void vChangeTextWindowDlg(void);
extern void vGLSFileIO(int code);
extern void vFreqTextDlg(void);
extern void vInitPrintStatusWindow(WORD rs, int re, int n);
extern void vShowPrintStatus(void);
extern void vHidePrintStatus(void);
extern void vEtcFileIODlg(BYTE flag, BYTE flag2);
extern void vMouseOptDlg(void);
extern int iSortOptDlg(void);
extern void vInitDlgWindow(void);
#endif

/* FINDHJ.C */
#ifndef _FINDHJ
extern int iCompWord(const unsigned *p1, const unsigned *p2);
extern int iGetHanjaInfo(WORD i, WORD *loc);
extern int iGetHanjaLoc(WORD i);
extern int iGetHanjaChoice(int num);
extern int iSelectHanja(BYTE *hc);
extern void vUpdateHanjaWordDlg(void);
extern int iSelectHanjaWord(BYTE *ch, BYTE *ttbl);
extern int iMakeNullHanjaDic(void);
#endif

/* FILE.C */
#ifndef _FILE
extern WORD wUseXMS(WORD *a, WORD *d);
extern WORD wMoveXMS(WORD s, WORD d);
extern void *pAllocUMB(WORD size);
extern void vFreeUMB(void *buf);
extern WORD wInitXMS(void);
extern void vInitEMS(void);
extern int iMake21VMS(void);
extern void vInitVMS(void);
extern void vDeinitVMS(void);
extern WORD wMemoryLeft(void);
extern int iInitMemorySystem(void);
extern void vDeinitMemorySystem(void);
extern void vFreeMem(Mem *h);
extern int iSwapMem(BYTE flag, WORD size, Mem *m, BYTE *buf);
extern int iCheckGWP(BYTE flag, FILE *fp, BYTE *data);
extern int iCheckHWP(FILE *fp);
extern void load_info(FILE *fp);
extern int iReadConfig(BYTE *mk, BYTE *mp, BYTE *fname);
extern void vWriteConfig(void);
extern void vReadKeyMacro(BYTE *fname);
extern void vWriteKeyMacro(BYTE *fname);
extern int iReadTextData(BYTE flag, BYTE wnum, FILE *fp);
extern void vWriteTextData(BYTE wnum, FILE *fp);
extern void vChangeExt(BYTE *str, BYTE *dest, BYTE *ext);
extern int iRenameFile(BYTE flag, BYTE *fname, BYTE *ext);
extern WORD wK2C (WORD in_code);
extern WORD wC2K (WORD in_code);
extern int iWriteTXT(BYTE *fname, BYTE code, BYTE way);
extern int iSaveScreen(BYTE *fname);
extern int iRestScreen(BYTE *fname);
extern void vSwapTextWindowArea(BYTE save_load, BYTE n);
extern int iSwapDocTofile(BYTE wnum);
extern int iSwapFileToDoc(BYTE wnum);
extern void vSetNewTextWindow(void);
extern void vNewTextWindow(void);
extern int iReadGWP(BYTE flag, BYTE *fname);
extern int iReadLineTXT(BYTE *vExit, int code, BYTE *line_buf, FILE *in);
extern BYTE bGetHanjaIndex(WORD i, WORD *t);
extern int iReadLineHWP(BYTE *vExit, BYTE *lc, BYTE *line, BYTE *effline, FILE *in);
extern int iReadLoop(BYTE func, BYTE flag, BYTE code, FILE *in);
extern int iReadTXT(BYTE flag, BYTE code, BYTE way, BYTE *fname);
extern int iReadHWP(BYTE flag, BYTE *fname);
extern int iGetFileType(BYTE *code, BYTE *way, BYTE *fname);
extern void vOpenTextDlg(BYTE flag);
extern void vOpenAgain(BYTE *name);
extern int iIsOverWrite(BYTE *fname);
extern int iSaveAsDlg(void);
extern int iSaveTextWindow(void);
extern void vAutoSave(void);
extern void vNewText(void);
extern void vKeyWait(void);
extern void vDosShell(void);
extern void vScreenSaver1(void);
extern void vScreenSaver2(void);
extern void vScreenSaver3(void);
extern void vScreenSaver(int num);

/* \est\21\harderr.c */
extern int iErrorHandler(int errval,int ax,int bp,int si);
extern int iCBreakHandler(void);
extern void vInitErrorHandler(void);
#endif

/* BLOCK.C */
#ifndef _BLOCK
extern int iMakeNullSwapFile(BYTE *buf);
extern int iWriteLineBlock(FILE *fp, WORD *total);
extern int iWriteBlockText(BYTE swp_or_save);
extern void vInsertBlock(BYTE flag, BYTE *fname);
extern void vReadBlock(BYTE paste_or_read);
extern void vClearBlock(void);
extern void vPasteBlock(void);
extern void vDelLineChar(BYTE flag);
extern void vSetFontKind(void);
extern void vSetFontEffect(void);
extern void vHanja2Hangul(void);
extern void vSetParagraph(int *temp, BYTE *fil);
extern void vParagraphDlg(void);
extern void vCtrlKKey(BYTE mode);
extern WORD wFindString(WORD loc, WORD eloc, BYTE *buf, BYTE *str);
extern WORD wFindStringBack(WORD loc, WORD eloc, BYTE *buf, BYTE *str);
extern void vSearchString(WORD sloc, NODE *temp);
extern int iSetSearchOpt(void);
extern void vSearchStringDlg(void);
extern void vChangeString(BYTE flag);
extern void vChangeStringDlg(void);
extern void vSearchAgain(void);
extern void vGotoPageDlg(int i);
extern void vSetFontDispOpt(void);
extern void vClipBoard(void);
extern void vSetKeyMapImage(void);
extern void vDrawKey(int x, int y, int count, int n);
extern void vDrawKeys(int x, int y);
extern void vDrawKeyMap(void);
extern void vShowKeyMap(void);
extern void vSortNodeDlg(void);
extern void vBlockSumDlg(void);
#endif

/* READY.C */
#ifndef _READY
extern BYTE bCheck21(void);
extern void vInitINT66(void);
extern void vDeinitINT66(void);
extern void vExcuteSubEXE(BYTE n);
#endif

/* EDIT10.C */
#ifndef _EDIT10
extern BYTE bGetKeyState(BYTE c);
extern void vDispScriptStatus(void);
extern void vPutTabChar(int n, int i, BYTE flag);
extern void vPutTab(int n);
extern void vPutRod(int n, BYTE check);
extern BYTE *pGetLHB(int size);
extern void vFreeLHB(BYTE *t, int size);
extern BYTE *pRegetLHB(int size, NODE *n);
extern int iMakeMem(void);
extern void *pGetMem(WORD size);
extern void *pRegetMem(void *p, WORD size);
extern int iLockNode(NODE *n);
extern WORD wGetLTLoc(void);
extern int iLoc2Col(WORD loc, NODE *n);
extern WORD wLine2Loc(BYTE lnum, NODE *temp);
extern WORD wLoc2Line(WORD loc, BYTE *buf);
extern void vDividePage(int cod);
extern void vUserDividePage(void);
extern NODE *npGetPageStartNode(BYTE *slnum, NODE *temp);
extern NODE *npGetPageEndNode(BYTE *slnum, NODE *temp);
extern NODE *npGetPageStart(int page_num, BYTE *slnum);
extern int iSetCurrentLine(void);
extern void vGetCursorSize(void);
extern void vSetTopVar(void);
extern void vDispText(void);
extern void vShowFound(BYTE flag);
extern int iSyncDC(void);
extern void vSyncCR(BYTE flag);
extern void vEndKeyTask(void);
extern void vSetCurrentNode(void) ;
extern int iInsertSpaceChar(BYTE c, WORD loc, NODE *temp);
extern int iDelBytes(BYTE v, WORD loc, NODE *temp);
extern void vDeleteStr(int len);
extern void vInsertStr(BYTE *str);
extern void vUseGlossary(int num);
extern void vSetCurrent(void);
extern void vGotoLine(void);
extern void vEndCombine(void);
extern void vRestoreEdit(void);
extern void vResetDesktop(void);
extern void vSetMargin(int i);
extern void vAdjParagraph(int i);
extern void vInsertSpacialNode(BYTE kind);
extern void vInsertHeaderNode(int kd);
extern void vInsertSpecialCode(int i);
extern void vF9Key(void);
extern int iCheckHotKey(INKEY ch);
extern void vLineDraw(void);
extern void vSaveCR(int tot, int cur);
extern void vTextScrollBar(void);
extern void vLineUpDown(BYTE flag);
extern void vTest();
extern BYTE bInAscii(BYTE h_e, BYTE ch);
extern void vSetBlock(void);
extern int iRightMouseButton(void);
extern int iEditor(void);
extern void vInitSwap(void);
extern void vSetCtrlC(BYTE c);
extern void main(int argc, char *argv[]);

/* \est\21\align.c */
extern void vResetSpace(NODE *temp);
extern void vDelOverSpace(NODE *n);
extern void vAnalText(NODE *n);

/* \est\21\dblink.c */
extern NODE *npGetNode(void);
extern NODE *npGetFullNode(WORD len, BYTE hs);
extern void vFreeNode(NODE *temp);
extern void vInitNodeList(void);
extern int iAppendNode(BYTE *str, BYTE *lc, BYTE *hs,\
       								WORD len, BYTE kd);
extern int iInsertNode(NODE *over_node, BYTE *str, BYTE *lc, BYTE *hs,\
       								WORD len, BYTE kd);
extern void vDeleteNode(NODE *del_node);
extern void vClearNodeList(void);
extern void vResetNodeList(void);
extern void vInitLoad(void);
extern int iMergeNode(NODE *mnode);
extern int iSplitNode(void);

/* \est\21\tcursor.c */
extern void vTCON(void);
extern void vTCOFF(void);
extern void vTextCursorOperation(BYTE clip, BYTE h_e);
extern void vDeinitINT8(void);
extern void vWatch(int flag, int h, int m);
extern void vDispTime(int t);
extern void vInitINT8(void);
extern void vCheckSpeed(void);
extern void vDelay(WORD tick);

/* \est\21\combine.c */
extern BYTE bCombine2(char ch, WORD *f);

/* \est\21\390.c */
extern BYTE bCombine3(char ch, WORD *f);

/* \est\21\inputjap.c */
extern void vJapanSetStart(void);
extern int iJapanSoriAutomata(BYTE key, BYTE *value1, BYTE *value2);
extern void vJapanInputState(void);

/* \est\21\gets.c */
extern void vResetHangulStack(void);
extern void vPushHangul(WORD i, BYTE s);
extern WORD wPopHangul(void);
extern void vPutStringLength(int x, int y, BYTE *str, BYTE attr, BYTE sloc, BYTE len);
extern int iChechHangul(BYTE *str, int loc);
extern int iInsertByte(BYTE *buf, BYTE loc, BYTE len, BYTE flag);
extern void vSaveTC(void);
extern void vRestTC(void);
extern int iGetString(BYTE flag, int x, int y, BYTE len, BYTE scr_len, BYTE *buf);
#endif

/* PRNDRV2.C */
#ifndef _PRNDRV2
extern int SCR2PRN(int n);
extern int PRNm2d(int i, int dpi);
extern int iInitPrint(int *arg);
extern int iPrintFile(NODE *vHomeKey);
extern int iInitPrinterDriver(BYTE *path);
extern void vFreeDriver(void);
#endif

#ifdef _EGI
void vPointThick(int x, int y, BYTE mode);
void vPointNormal(int x, int y, BYTE mode);
BYTE bGetGraphMode(void);
void vSetHercGraphMode(BYTE flag);
void vSetPalette(BYTE s, BYTE *pal);
void vInitOffsetX(BYTE flag, WORD width);
int iSetVideoMode(BYTE flag);
int iInitGraphMode(BYTE g_mode, BYTE first_next);
void vDeinitGraphMode(void);
void vGraphClipOff(void);
void vGraphClipOn(int x, int y, int ex, int ey);
void vNLineXBit(int startx, int endx, int y, BYTE mode);
void vWLineXBit(int x, int endx, int y);
void vBLineXBit(int x, int endx, int y);
void vXORLineXBit(int x, int endx, int y);
void vWLineYBit(int x, int y, int endy);
void vBLineYBit(int x, int y, int endy);
void vXORLineYBit(int x, int y, int endy);
void vNLineYBit(int x, int y, int ey, BYTE mode);
void vLineBit(int startx,int starty,int endx,int endy, BYTE mode);
void vNBoxBit(int x, int y, int ex, int ey, BYTE mode);
void vWBoxBit(int startx,int starty,int endx,int endy);
void vBBoxBit(int startx,int starty,int endx,int endy);
void vXORBoxBit(int startx,int starty,int endx,int endy);
void vNBarBit(int x, int y, int ex, int ey, BYTE mode);
void vWBarBit(int startx,int starty,int endx, int endy);
void vBBarBit(int x,int y,int ex, int ey);
void vXORBarBit(int x, int y, int ex, int ey);
void vXORBarByte(int x, int y, int h, int len);
void vXORBarBytex16(int x, int y, int len);
void vXORBlockByte(int x, int y, BYTE height, BYTE width);
void vPutFreeImageByte(int x, int y, WORD hsize, WORD vsize, BYTE *image, BYTE mode);
void vPutWordImageBit(int x, int y, BYTE *buf);
WORD wSizeImageBuf(int x, int y, int endx, int endy);
void vDrawPanelByte(WORD x, WORD y, WORD endx, WORD endy,BYTE flag);
void vChangeColor(BYTE cur_color, BYTE new_color);
void vSetColors(void);
void vSetBlackWhite(void);
void vFillScreen(unsigned SNode, unsigned second);
void vBarByte(int x, int y, int endx, int endy, char attr);
void vScrollRightByte(unsigned x, unsigned endx, unsigned y,\
									unsigned widthy, unsigned count, unsigned char attr);
void vScrollLeftByte(unsigned x, unsigned endx, unsigned y,\
									unsigned widthy, unsigned count, unsigned char attr);
void vScrollUpByte(unsigned x, unsigned endx, unsigned y,\
									unsigned widthy, unsigned count);
void vScrollDownByte(unsigned x, unsigned endx, unsigned y,\
									unsigned widthy, unsigned count);
void vPutColorImageLByte(int x, int y, int h, int w, int c, BYTE *buf);
void vDrawVScrollBarBit(WORD x, WORD ex, WORD y);
void vDrawHScrollBarBit(WORD x, WORD y, WORD ey);
void vDelVThumbBit(WORD x, WORD ex, WORD y, WORD total, int old_sub);
void vDrawVThumbBit(WORD x, WORD ex, WORD y, WORD total, int sub, int old_sub);
void vDelHThumbBit(WORD x, WORD y, WORD ey, WORD total, int old_sub);
void vDrawHThumbBit(WORD x, WORD y, WORD ey, WORD total, int sub, int old_sub);
void vDarkenTextWindowTitle(int n);
void vResetTextWindowTitle(void);
void vDrawWindowByte(int x, int y, int lx, int ly, BYTE flag, BYTE *str);
void vDrawObjByte(int n);
#endif

#ifdef _MOUSE
static void near vClipPutImageBit(WORD x, WORD y, BYTE *buf);
static void near vGetColorImageEByte(WORD x, WORD y, WORD endx, WORD endy, BYTE *buf);
static void near vPutColorImageEByte(WORD x, WORD y, WORD endx, WORD endy, BYTE *buf);
void vPutColorImageFarEByte(int x, int y, int ex, int ey, BYTE *buf);
static void near vMouseCursorOperation(int x, int y, BYTE flag);
void vMCON(void);
void vMCOFF(void);
void vSetMousePosition(int x, int y);
void vGetMousePosition(int *x,int *y);
void vChangeMC(int x, int y, BYTE *i);
void vSetRange(int x, int y, int endx, int endy);
void vRestoreRange(void);
void vSetMouseSpeed(WORD ratio);
void interrupt vPC_MouseInt(void);
void interrupt vMS_MouseInt(void);
void vInstallSerialInt(void);
void huge vMouseIntService(void);
void vInstallEventHandler(void huge *userfunc, int mask);
void vDeinitMouse(void);
int iInitMouse(BYTE kind, BYTE port);
#endif

#ifdef _INLINE
int wGetKey(char func);
void vPutKey(char scan_code, char ascii_code);
unsigned wCheckKeyHit(void);
void vResetKeyBuffer(void);
int iGetMacroNumber(void);
void vSetMacro(void);
void vUseMacro(int num);
void vUseMacroRepeat(void);
#endif

#ifdef _WINFUNC2
void vDispKeyInputStatus(void);
void vDispExtraStatus(int i);
void vDispRowStatus(void);
void vDispColumeStatus(void);
void vDispInsertStatus(void);
void vResetTextWindowValue(BYTE i, BYTE flag);
void vInitWindowValue(void);
int iHScrollBarHandler(int x, int y, int ey, int total, int sub,\
						 int old_sub, int vHomeKey, int vExit);
int iVscrollBarHandler(int x, int ex, int y, int total, int sub,\
						 int old_sub, int vHomeKey, int vExit);
void vRestoreWindowSize(int n);
void vSaveWindowSize(int n);
void vDrawTextWindow(BYTE n);
void vPushWindow(BYTE wnum);
int iPopWindow(void);
void vRedrawAllWindow(void);
int iCloseTextWindow(int new);
int iCloseAllTextWindow(void);
int iCheckTextWindowArea(int mx, int my);
int iMoveWindow(int *px, int *py, int *plx, int *ply);
int iMoveTextWindow(BYTE n);
void vSetMinWindowSize(int x, int y);
int iResizeWindow(int *px, int *py, int *plx, int *ply);
int iResizeTextWindow(BYTE n);
void vMaximizeWindow(void);
void vChangeWindow(BYTE wnum);
void vNextWindow(void);
void vKeyMoveTextWindow(BYTE n);
void vXORTabLoc(int n, int choice);
void vSetTab(int flag);
int iMouseInTextWindow(void);
int iGetWindowHandle(void);
#endif

#ifdef _HANFUNC3
int iReadBitmapFont(BYTE *font, char *fname, WORD size);
int iReadAsciiVectorFont(BYTE *efont, WORD *eftbl, char *e_name);
int iReadHanJohapVectorFont(BYTE *efont, WORD *eftbl, char *e_name );
static int near iTG2KSIndex(WORD x);
static void near vPFF(int x, int y, BYTE *b5);
static void near vPHF(int x, int y, BYTE *b4);
void vClipOff(void);
void vClipOn(BYTE n);
void vPFFfar(int x, int y, BYTE *b);
void vPHFfar(int x, int y, BYTE *b);
int iPutGraphicFont(int x, int y, BYTE b, BYTE mode);
void vEraseRestArea(int x, int y, BYTE cur_size, BYTE width);
void vPT(int x, int y, BYTE flag);
void vBPT(int x, int y, int sl, int el, BYTE flag);
void vSPT(int x, int y, BYTE flag);
void vChangePutMode(BYTE flag);
void vWriteString(int x, int y, BYTE *str, BYTE mode);
int iPrintf(int x, int y, BYTE mode, BYTE *fmt, ...);
void vSetDispOpt(BYTE sp_flag, BYTE lf_flag, BYTE cr_flag);
void vPutBitmapHanjaTable(int x, int y, WORD *buf, BYTE count);
int iSetFontTable(BYTE *fname, BYTE *hname[], BYTE *ename[], BYTE *kind);
int iSetHangulFontTable(void);
int iIsFont(BYTE *name);
WORD wGetFontSize(char *name);
int iCheckFont(void);
int iInitDefaultFont(void);
int iInitHangul(BYTE gmode);
void vDeinitHangul(void);
int iFreeFontBuffer(int s);
void vFreeAllUMB(void);
void vSetImageStartLocation(int x);
void vSetUnit(int x, int y);
void vSetImageHeight(int len);
void vClipPutImage(int x, int y, int height, int width,\
										 BYTE flag, BYTE * buf);
void vPutImage(int x, int y, int ex, int ey, unsigned char *buf);
void vGetImage(int x, int y, int ex, int ey, unsigned char *buf);
static near WORD wShiftRightWord(WORD i);
static near WORD wShiftLeftWord(WORD i);
void vEffectWordImage(WORD *image_16x16, WORD *timage, BYTE mode);
void vEffectByteImage(BYTE *image_16x8, BYTE *timage, BYTE mode);
static void near vPutImage16x16(int x, int y, WORD *image_16x16, BYTE mode);
static void near vPutImage16x8(int x, int y, BYTE *image_16x8, BYTE mode);
static void near vHLineInImageBuf(int x, int y_start, int y_end);
static void near vQSort(unsigned loff, unsigned roff);
void vFillPoly(void);
void vOpenPoly(void);
void vClosePoly(BYTE flag);
static void near vLineInPolyArray(int x2, int y2);
static void near vLineInImageBuf(void);
static void near vDrawLineInPolyArray(int x2, int y2);
static void near vRasterize(BYTE *vf, WORD count);
static void near vDrawCurveInPolyArray(int deep, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
static void near vNewRasterizeLarge(BYTE *ef, int count);
static void near vNewRasterize(BYTE *ef, int count);
static void near vShiftRightBuf(WORD soff, WORD doff, BYTE count, WORD len);
static void near vCopyBuffer(WORD soff, WORD doff, WORD count);
static void near vGrayBuffer(int bufsize);
static void near vPutRastorImage(int x, int y, BYTE eff);
static BYTE near b2to1(BYTE src1, BYTE src2);
static void near v2to3(BYTE *src, BYTE *dest);
static void near v2to4(BYTE *src, BYTE *dest);
static void near v2to5(BYTE *src, BYTE *dest);
static void near vPutSmallImage16x16(int x, int y, WORD *image_16x16, BYTE mode);
static void near vPutSmallImage16x8(int x, int y, BYTE *image, BYTE mode);
static void near vPutSmallRastorImage(int x, int y, BYTE eff);
void vPutSmallFont(int x, int y, BYTE n, BYTE flag);
void vCutImage(int xlen, int bwid, int wid);
void vPutImageToPrintBuffer(int x, int y, int h, int v);
#endif

#ifdef _LAYOUT4
WORD wSmallSCR2PRN(WORD n);
WORD wSmallPRN2SCR(WORD n);
WORD wMM2DOT(WORD i);
void vSetPaperSize(void);
void vSetPaper(void);
static int near iSetHeaderNode(NODE *temp);
int iSetHeaderNodeFar(NODE *n);
void vFindHeader(void);
void vDrawPaper(void);
int iDispSmallNode(int sx, int sy, BYTE sline, BYTE flag, NODE *temp);
void vDispSmallText(int sx, int sy);
int iSkipText(WORD sbit);
void vDispPageStatus(void);
void vDispSmallPage(BYTE flag);
void vEndPreview(void);
void vDispPreviewMenu(void);
void vXORPreviewIcon(int n);
int iGetPreviewIconCheck(void);
int iPreviewMenu(void);
void vPreview(void);
int iCheckPrintScadule(void);
void vPutLot(int n, BYTE flag);
void vDelScadule(void);
int iSetScadule(void);
void vTWopt2PRNopt(int t);
void vPRNopt2TWopt(int t);
void vPrintOptDlg(int n);
int iGetPrinterDriver(BYTE *fname);
void vInitPrintSetup(void);
int iAddPaper(void);
int iDelPaper(int n);
void vPrinterSetupDlg(void);
BYTE bPrintStatus(int port);
int iCheckPrinter(int port);
void vSaveDisplayOpt(void);
void vRestoreDisplayOpt(void);
int iPrintTextWindow(int n);
void vPrintScaduler(void);
void vPrintStart(void);
#endif

#ifdef _OVLMAN
void vInitINT60(void);
void vDeinitINT60(void);
long lGetFileSize(BYTE *path);
void huge *pLoadModule(BYTE *path);
void far *pInitOverlay(BYTE *path);
void huge *pGetModuleAddr(void);
#endif

#ifdef _MENUDRV2
void vFreeOurBuf(void);
void vExit(void);
void vMakeOurBuf(void);
int iMouseInMenu(int status,int pn);
void vDrawTopMenu(void);
void vSetMenuOpt(int n, WORD code);
void vRevTopMenu(int n);
void vSetMenu(int n,int x, int y, int count,BYTE **menu, BYTE *head);
void vInitMenu(void);
void vSetMenuParaAll(int n, int x, int y, int count, int ENode);
void vSetMenuPara(int n,int x, int y);
void vSaveMenuBk(int n);
void vRestoreMenuBk(int n);
void vRevMenuItem(int n, int choice);
void vPutMenu(int n);
int iIsIn(BYTE n, BYTE code);
int iGetUserResponse(int n);
int iPullDownMenu(int n);
int iGeneralMenuDrv(int pan_n,int add_val,int mul_val);
int iEditMenuDrv(void);
int iInputMenuDrv(void);
int iKeyMapMenuDrv(void);
int iWordMenuDrv(void);
int iPrintDrv(void);
int iMenuDrvier(void);
void vHideRod(void);
void vSetRod(void);
void vChangeActiveWindow(void);
void vLogo(void);
void vBeep(void);
int iAction(int n);
void vLetsGo(void);
#endif

#ifdef _POPUP4
void vMakeDlgWindow(int n, int x, int y, BYTE use, BYTE **blk,\
						 BYTE **dat, BYTE **hed, BYTE **cod, BYTE *winhead);
void vMakeExtDlgWindow(int n, BYTE *data, BYTE *pn, BYTE *process,\
								BYTE *vb, BYTE *value, BYTE *set);
void vRevControlObj(int n, int bn, int cn);
void vDrawScrollBarObj(int n, int bn, int cho);
void vDrawRecordObj(BYTE n, BYTE bn);
void vDrawDlgWindow(BYTE n);
void vOpenDlgWindow(BYTE n);
int iXYIsInObj(int n, BYTE *blk, BYTE *cho);
int iExamineMCInObj(int n, BYTE *blk, BYTE *cho);
void vXORPopup(int x, int y, int ex, int ey);
int iMoveDlgWindow(BYTE n);
int iKeyMoveWindow(int *wx, int *wy, int *wlx, int *wly, BYTE code);
int iKeyMoveDlgWindow(BYTE n);
void vCloseDlgWindow(BYTE n);
int iGetObjOffset(int n, BYTE cur_blk, BYTE choice);
int iCalcChiose(int n, BYTE blk, BYTE choice);
void vInt2Char(int i, BYTE *buf);
void vChar2Int(int *i, BYTE *buf);
void vNumberDlgWindow(BYTE n);
WORD wGetNumber(int def, int start, int vExit, BYTE *unit, BYTE *head);
void vDispChecked(void);
void vSetDlgObj(int n, BYTE cur_blk, BYTE cur_cho, BYTE *addr);
void vSetDlgWindow(int n, BYTE *addr);
int iGetNextObj(int n, BYTE blk, BYTE *addr);
int iCheckShortCut(int n, BYTE *cur_blk, BYTE *cur_cho, BYTE code);
int iXYIsInArea(int x, int y, int lx, int ly);
void vVBHead(int n, int bn, int cho);
int iVScrollBarObj(int n, int bn);
int iDialogDriver(int n, BYTE *buf);
int iUserChoiceDlg(char *str);
int huge ihUserChoiceDlg(char *str);
int iFileListSearch(BYTE b, int n);
int iFnSplit(BYTE *rev_buf);
int iGetFileSubject(BYTE *str);
int iGetFileInfo(struct ffblk *fb);
int iCompFileName(const void *a, const void *b);
void vSortFileList(void);
BYTE bCheckFile(BYTE *buf);
int iMakeFileList(BYTE check);
int iDispFileList(int st, int len, int x, int y);
void vSetDriverName(void);
int iCheckDriver(BYTE drv);
void vFileNameSplit2(BYTE *src);
void vFileNameMerge2(BYTE *dest);
void vSetExt(BYTE *name);
int iAnalFileName(BYTE *src);
WORD wGetDiskFreeSpace(BYTE *rev_buf);
void vAnalSlash(BYTE *dest, BYTE *src);
void vDeleteFile(int n);
void vGetDlgObjXY(int *x, int *y, int *ex, int *ey, BYTE blk);
int iIsDone(void);
void vDispFileListBox(int n);
void vEraseOldFileName(void);
int iSetFileList(int flag);
void vSetFileListHScrollBar(int i,int *old_i);
void vExitFileListDlg(void);
void vRevFileListObj(int choice);
void vGetBlock(int choice, BYTE *cb, BYTE *cc);
int iFileListDlg(BYTE *src, BYTE *fname, BYTE rev_code);
void huge vErrorMsgDlg(BYTE *str);
void huge vErrorMsgfDlg(BYTE type, BYTE num, BYTE *str);
void vPutMenuObj2(int n, int blk, int y, BYTE *menu, BYTE len);
void vPutMenuObj(int n, int blk, int y, BYTE *menu);
int iLoadOptDlg(BYTE *cod, BYTE *way);
int iSaveOptDlg(BYTE *cod, BYTE *way);
void vDispOptDlg(void);
void vWipeStringArray(BYTE total, BYTE n, BYTE **array);
void vTestDlg(void);
int iGetHelpData(int n);
WORD wGetHelpDataLoc(int lnum, BYTE *buf);
void vDispHelpDlg(int n);
int iMouseInHelpDlg(void);
void vHelpDlg(int n);
void vHeaderFooterSizeDlg(BYTE flag, WORD s, WORD cur, WORD e, BYTE *str);
void vHeaderSizeDlg(void);
void vScreenSaverDlg(void);
void vEtcDlg(void);
void vInsertKeyMap(int n);
void vDeleteKeyMap(int n);
void vCircleKeyboardDlg(void);
int iGetStringDlg(BYTE flag, BYTE *str, BYTE *head);
void vSaveClosedTextWindow(BYTE *name);
void vDeleteClosedTextWindow(int n);
void vMoveUpCTW(int s, int n, int tot, BYTE *dat[]);
void vMoveDownCTW(int s, int n, int tot, BYTE *dat[]);
int iDispChangeTextWindowDlg(int code);
void vChangeTextWindowDlg(void);
void vGLSFileIO(int code);
void vFreqTextDlg(void);
void vInitPrintStatusWindow(WORD rs, int re, int n);
void vShowPrintStatus(void);
void vHidePrintStatus(void);
void vEtcFileIODlg(BYTE flag, BYTE flag2);
void vMouseOptDlg(void);
int iSortOptDlg(void);
void vInitDlgWindow(void);
#endif

#ifdef _FINDHJ
int iCompWord(const unsigned *p1, const unsigned *p2);
int iGetHanjaInfo(WORD i, WORD *loc);
int iGetHanjaLoc(WORD i);
int iGetHanjaChoice(int num);
int iSelectHanja(BYTE *hc);
static int near iSearchHanjaWord(BYTE flag, BYTE *str, BYTE *tbl, FILE *fp);
static void near vPutHanjaWordToFile(BYTE *str, BYTE *tbl, FILE *fp);
void vUpdateHanjaWordDlg(void);
int iSelectHanjaWord(BYTE *ch, BYTE *ttbl);
int iMakeNullHanjaDic(void);
#endif

#ifdef _FILE
WORD wUseXMS(WORD *a, WORD *d);
WORD wMoveXMS(WORD s, WORD d);
void *pAllocUMB(WORD size);
void vFreeUMB(void *buf);
WORD wInitXMS(void);
void vInitEMS(void);
int iMake21VMS(void);
void vInitVMS(void);
void vDeinitVMS(void);
WORD wMemoryLeft(void);
int iInitMemorySystem(void);
void vDeinitMemorySystem(void);
static WORD wAllocMem(WORD size);
void vFreeMem(Mem *h);
static BYTE bSwapVMS(BYTE r_w, WORD handle, BYTE *buf);
static BYTE bSwapXMS(BYTE r_w, WORD handle, BYTE *buf);
int iSwapMem(BYTE flag, WORD size, Mem *m, BYTE *buf);
int iCheckGWP(BYTE flag, FILE *fp, BYTE *data);
int iCheckHWP(FILE *fp);
void load_info(FILE *fp);
int iReadConfig(BYTE *mk, BYTE *mp, BYTE *fname);
void vWriteConfig(void);
void vReadKeyMacro(BYTE *fname);
void vWriteKeyMacro(BYTE *fname);
int iReadTextData(BYTE flag, BYTE wnum, FILE *fp);
void vWriteTextData(BYTE wnum, FILE *fp);
void vChangeExt(BYTE *str, BYTE *dest, BYTE *ext);
int iRenameFile(BYTE flag, BYTE *fname, BYTE *ext);
static int iWriteText(BYTE *fname);
static int iSave(BYTE *fname);
WORD wK2C (WORD in_code);
WORD wC2K (WORD in_code);
static int iCodeCmp(const unsigned *p, const unsigned *q);
static unsigned wTG2KS(WORD code, BYTE code2);
int iWriteTXT(BYTE *fname, BYTE code, BYTE way);
int iSaveScreen(BYTE *fname);
int iRestScreen(BYTE *fname);
void vSwapTextWindowArea(BYTE save_load, BYTE n);
int iSwapDocTofile(BYTE wnum);
int iSwapFileToDoc(BYTE wnum);
void vSetNewTextWindow(void);
void vNewTextWindow(void);
int iReadGWP(BYTE flag, BYTE *fname);
int iReadLineTXT(BYTE *vExit, int code, BYTE *line_buf, FILE *in);
BYTE bGetHanjaIndex(WORD i, WORD *t);
int iReadLineHWP(BYTE *vExit, BYTE *lc, BYTE *line, BYTE *effline, FILE *in);
int iReadLoop(BYTE func, BYTE flag, BYTE code, FILE *in);
int iReadTXT(BYTE flag, BYTE code, BYTE way, BYTE *fname);
int iReadHWP(BYTE flag, BYTE *fname);
int iGetFileType(BYTE *code, BYTE *way, BYTE *fname);
void vOpenTextDlg(BYTE flag);
void vOpenAgain(BYTE *name);
int iIsOverWrite(BYTE *fname);
int iSaveAsDlg(void);
int iSaveTextWindow(void);
void vAutoSave(void);
void vNewText(void);
void vKeyWait(void);
void vDosShell(void);
void vScreenSaver1(void);
static char near cRandomPoint(char vnum);
void vScreenSaver2(void);
void vScreenSaver3(void);
void vScreenSaver(int num);
int iErrorHandler(int errval,int ax,int bp,int si);
int iCBreakHandler(void);
void vInitErrorHandler(void);
#endif

#ifdef _BLOCK
int iMakeNullSwapFile(BYTE *buf);
static void near vWriteNode(NODE *temp, FILE *fp);
int iWriteLineBlock(FILE *fp, WORD *total);
static int near iCol2Loc(int col, NODE *n);
static int near iWriteColumeBlock(FILE *fp, WORD *total);
int iWriteBlockText(BYTE swp_or_save);
static int near iReadInsertNode(FILE *fp, NODE *over_node);
void vInsertBlock(BYTE flag, BYTE *fname);
void vReadBlock(BYTE paste_or_read);
void vClearBlock(void);
void vPasteBlock(void);
void vDelLineChar(BYTE flag);
void vSetFontKind(void);
void vSetFontEffect(void);
void vHanja2Hangul(void);
void vSetParagraph(int *temp, BYTE *fil);
void vParagraphDlg(void);
void vCtrlKKey(BYTE mode);
static int near iCompString(WORD tloc, BYTE *buf, BYTE *str);
WORD wFindString(WORD loc, WORD eloc, BYTE *buf, BYTE *str);
WORD wFindStringBack(WORD loc, WORD eloc, BYTE *buf, BYTE *str);
void vSearchString(WORD sloc, NODE *temp);
int iSetSearchOpt(void);
void vSearchStringDlg(void);
void vChangeString(BYTE flag);
void vChangeStringDlg(void);
void vSearchAgain(void);
void vGotoPageDlg(int i);
void vSetFontDispOpt(void);
void vClipBoard(void);
void vSetKeyMapImage(void);
void vDrawKey(int x, int y, int count, int n);
void vDrawKeys(int x, int y);
void vDrawKeyMap(void);
void vShowKeyMap(void);
static int near iCompNode(BYTE *astr, BYTE *bstr);
static BYTE *pGetNumberInNode(BYTE *str, long *rtn, BYTE *eloc);
static void near vSwapNode(NODE *a, NODE *b);
void vSortNodeDlg(void);
void vBlockSumDlg(void);
#endif

#ifdef _READY
BYTE bCheck21(void);
void vInitINT66(void);
void vDeinitINT66(void);
void vExcuteSubEXE(BYTE n);
#endif

#ifdef _EDIT10
static WORD near wScr2Prn(WORD n);
static WORD near wPrn2Scr(WORD n);
static void near _WINSCRUP(int sbit);
static void near _WINSCRDN(int sbit);
static void near _WINSCRR(int sbyte);
static void near _WINSCRL(int sbyte);
static void near vResetHanin(void);
BYTE bGetKeyState(BYTE c);
void vDispScriptStatus(void);
void vPutTabChar(int n, int i, BYTE flag);
void vPutTab(int n);
void vPutRod(int n, BYTE check);
BYTE *pGetLHB(int size);
void vFreeLHB(BYTE *t, int size);
BYTE *pRegetLHB(int size, NODE *n);
int iMakeMem(void);
void *pGetMem(WORD size);
void *pRegetMem(void *p, WORD size);
int iLockNode(NODE *n);
WORD wGetLTLoc(void);
int iLoc2Col(WORD loc, NODE *n);
WORD wLine2Loc(BYTE lnum, NODE *temp);
WORD wLoc2Line(WORD loc, BYTE *buf);
static WORD near wGetTotalLineNum(void);
void vDividePage(int cod);
void vUserDividePage(void);
NODE *npGetPageStartNode(BYTE *slnum, NODE *temp);
NODE *npGetPageEndNode(BYTE *slnum, NODE *temp);
NODE *npGetPageStart(int page_num, BYTE *slnum);
int iSetCurrentLine(void);
void vGetCursorSize(void);
static void near vSetTopRow(void);
void vSetTopVar(void);
static void near vMoveUpTop(int sbit);
static void near vMoveDnTop(int sbit);
static int near iSetEndLine(int *tx, BYTE *skip, WORD *cr, WORD *cp);
static void near vSkipLine(void);
static WORD near wSkipNode(WORD x, BYTE sline, WORD *cr, WORD *cp, NODE *temp);
static void near vClearCurLine(int x, BYTE line_gap);
static void near vPutHeaderMark(int x, int kd);
static void near vDispBlock(int x, int y, BYTE height, WORD cr, BYTE lnum);
static int near iSumLeft(BYTE lnum, NODE *temp);
static int near iPTC(WORD x, BYTE lnum, NODE *temp);
static void near vDispPrevRow(void);
static void near vDispNextRow(void);
static WORD near wDispNode(int x, BYTE sline, NODE *temp, WORD *cr);
static void near vDispPageDivide(void);
void vDispText(void);
void vShowFound(BYTE flag);
static int near iSyncScroll(BYTE how);
int iSyncDC(void);
void vSyncCR(BYTE flag);
static void near vBatchSync(void);
static void near vHome(void);
void vEndKeyTask(void);
static void near vHomeKey(void);
static int near iEndOfLine(void);
static void near vEndLine(void);
static int near iMoveNextLine(WORD flag);
static int near iMovePrevLine(WORD flag);
static void near vRightKey(void);
static void near vLeftKey(void);
static int near iUpKey(BYTE ctrl);
static int near iDownKey(BYTE ctrl);
void vSetCurrentNode(void) ;
static int near iSetCurrentNodeXY(int x, int y, BYTE flag);
static void near vAltHomeEndKey(BYTE flag);
static void near vPgUpKey(void);
static void near vPgDnKey(void);
static void near vCtrlRightKey(void);
static void near vCtrlLeftKey(void);
static void near vCtrlHomeKey(void);
static void near vCtrlEndKey(void);
static void near vCtrlDownKey(void);
static void near vCtrlUpKey(void);
static void near vCtrlPgUpKey(void);
static void near vCtrlPgDnKey(void);
static void near vCtrlYKey(void);
int iInsertSpaceChar(BYTE c, WORD loc, NODE *temp);
int iDelBytes(BYTE v, WORD loc, NODE *temp);
static int near iDelBytesBack(BYTE v, WORD loc, NODE *temp);
static void near vCopyBytes(BYTE flag, BYTE *dat);
static int near iSumBytes(WORD sloc, WORD eloc, BYTE *str);
static WORD near wFindLineChar(BYTE *check);
static int near iDelKey(void);
static void near vCtrlTKey(void);
static void near vCtrlBSKey(void);
static void near vAltYKey(void);
static int near iIntoBuf(BYTE wors, BYTE skip, BYTE *dat);
void vDeleteStr(int len);
void vInsertStr(BYTE *str);
void vUseGlossary(int num);
static int near iGetTabLoc(int cur_loc);
static int near iGetPrevTapLoc(int cur_loc);
static void near vTabKey(BYTE flag);
static void near vEnterKey(void);
void vSetCurrent(void);
void vGotoLine(void);
void vEndCombine(void);
void vRestoreEdit(void);
void vResetDesktop(void);
void vSetMargin(int i);
void vAdjParagraph(int i);
void vInsertSpacialNode(BYTE kind);
void vInsertHeaderNode(int kd);
void vInsertSpecialCode(int i);
static int near iCtrlNKey(void);
static int near iCtrlQKey(void);
void vF9Key(void);
int iCheckHotKey(INKEY ch);
static void near vStartDrawLine(void);
static BYTE near bCheckLeftChar(WORD loc);
static BYTE near bCheckRightChar(WORD loc);
static BYTE near bCheckColume(int cc, int sum, WORD loc, NODE *temp);
static BYTE near bCheckUpChar(int cc);
static BYTE near bCheckDownChar(int cc);
static void near vPutLineChar(BYTE v_h);
static int near iDrawLineRight(BYTE flag);
static int near iDrawLineLeft(BYTE flag);
static void near vDrawLineUp(void);
static void near vDrawLineDown(void);
static void near vDrawLineAutoRight(void);
static void near vDrawLineAutoLeft(void);
static void near vDrawLineToStart(void);
static void near vDrawLineToEnd(void);
void vLineDraw(void);
void vSaveCR(int tot, int cur);
void vTextScrollBar(void);
void vLineUpDown(BYTE flag);
void vTest();
BYTE bInAscii(BYTE h_e, BYTE ch);
static void near vKeyTask(void);
void vSetBlock(void);
int iRightMouseButton(void);
int iEditor(void);
void vInitSwap(void);
void vSetCtrlC(BYTE c);
void main(int argc, char *argv[]);
void vResetSpace(NODE *temp);
static int iResizeLHB(BYTE tlnum, NODE *n);
static void vMarkEOL(int loc, BYTE *str);
static void vAnalWord(NODE *n);
static int near iCheckLineEnd(int wors, WORD loc, BYTE *str);
static void vAnalChar(NODE *n);
static void vAnalMix(NODE *n);
static void vGetLineWidth(BYTE *str);
static int iInsertNullSpace1(int flag, NODE *n);
static int iInsertNullSpace2(NODE *n);
void vDelOverSpace(NODE *n);
void vAnalText(NODE *n);
NODE *npGetNode(void);
static void vFreeNodeBuf(NODE *iDelKey);
NODE *npGetFullNode(WORD len, BYTE hs);
void vFreeNode(NODE *temp);
void vInitNodeList(void);
int iAppendNode(BYTE *str, BYTE *lc, BYTE *hs,\
								WORD len, BYTE kd);
int iInsertNode(NODE *over_node, BYTE *str, BYTE *lc, BYTE *hs,\
								WORD len, BYTE kd);
void vDeleteNode(NODE *del_node);
void vClearNodeList(void);
void vResetNodeList(void);
void vInitLoad(void);
int iMergeNode(NODE *mnode);
int iSplitNode(void);
static void near vXORCursor(int x, int y, BYTE cur_h, BYTE h, BYTE v, BYTE s, BYTE h_e);
void vTCON(void);
void vTCOFF(void);
void vTextCursorOperation(BYTE clip, BYTE h_e);
void vDeinitINT8(void);
void vWatch(int flag, int h, int m);
void vDispTime(int t);
void interrupt vNewINT8(void);
void vInitINT8(void);
void vCheckSpeed(void);
void vDelay(WORD tick);
BYTE bCombine2(char ch, WORD *f);
BYTE bCombine3(char ch, WORD *f);
static int near iSearchSori(BYTE *key, int count);
void vJapanSetStart(void);
int iJapanSoriAutomata(BYTE key, BYTE *value1, BYTE *value2);
void vJapanInputState(void);
void vResetHangulStack(void);
void vPushHangul(WORD i, BYTE s);
WORD wPopHangul(void);
void vPutStringLength(int x, int y, BYTE *str, BYTE attr, BYTE sloc, BYTE len);
int iChechHangul(BYTE *str, int loc);
int iInsertByte(BYTE *buf, BYTE loc, BYTE len, BYTE flag);
void vSaveTC(void);
void vRestTC(void);
int iGetString(BYTE flag, int x, int y, BYTE len, BYTE scr_len, BYTE *buf);
#endif

#ifdef _PRNDRV2
int SCR2PRN(int n);
int PRNm2d(int i, int dpi);
static void PRN_setp(int dpi);
int iInitPrint(int *arg);
static BYTE bIsInRange(int pn);
static int iMakeLineGap(BYTE *str, int flag);
static void vPutLineGap(BYTE *str, int lc0, int vLeftKey, int width);
static int near iPTP(int flag, BYTE *Pstr);
static int iPrintLine(int n, int Hsize, BYTE *str2, BYTE *lc, BYTE attr);
static void vPrintHeader(int pn);
static void vPrintFooter(int pn);
static int iNewPaper(int pn);
int iPrintFile(NODE *vHomeKey);
int iInitPrinterDriver(BYTE *path);
void vFreeDriver(void);
#endif

