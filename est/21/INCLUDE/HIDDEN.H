#define HJB 0
#define JPB 1
#define FSB 2
#define HSB 3

#define BIT2VEC 4
#define HJV 4
#define JPV 5
#define FSV 6
#define HSV 7

#define HGC 8

int iInitFontCache(void);
BYTE (*pCheckFontCacheImage(void));
BYTE *pGetFontCacheImage(BYTE fc1, BYTE fc2, BYTE fc3, BYTE fc4);
BYTE (*pGetHanFontCacheImage(void));
int iReadHanWansungVectorfont(BYTE *fname);

extern BYTE fcp1, fcp2, fcp3, fcp4;
extern BYTE hfcnum;
extern WORD hfcode, hfcsize;
