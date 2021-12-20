#ifndef __SMS_HAN_HEADER__

#define __SMS_HAN_HEADER__

typedef unsigned char byte;

/* SMSHAN.c */
void graph_close ( void );
void graph_mode (const int mode);
void gr_goto ( const int _xx , const int _yy );
void font_init ( void );
int  graph_init ( const int grcolor );
void han_font ( const char *han );
void eng_font ( const char *eng );
void hputch ( const int x , const int y , byte *f );
void set_font ( const int new_font );
void han_chp ( byte cho , byte jung , byte jong , const int font );
void eng_chp ( const byte keycode , const int font );
void graph_cls ( const int color );
void gputs ( const int x , const int y , const int font , char *st );
void gprintf ( const int x , const int y , const int font , char *format , ...);
void gColor ( const int col , const int back );

void closehanja(void);
int inithanja(const char* name);

////  ??? BC Can Not Function Pointer ????  ////
void DoFontImage (byte *dest, const int bytes, const int ftype);

//extern void (*DoFontImage[7])(byte*, const int);
#endif
