/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          header file - miscellaneous functions(16)           */
/*   FILENAME           misc.h                                              */
/*   DATE & TIME        06/29/92 (MON) 13:45                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/* play() · Ðe‰Ã¡ */
#define MAX_OCTAVE        7
#define MAX_LENGTH       64
#define MAX_TEMPO       255
#define MAX_NOTE         84
#define MAX_VARIANCE  10000

/* play() · Ÿ¡Èåˆt */
enum
{
   INVALID_COMMAND = 1,
   INVALID_OCTAVE,
   INVALID_LENGTH,
   INVALID_TEMPO,
   INVALID_NUM_NOTE,
   INVALID_NOTE,
   INVALID_VARIANCE,
   INVALID_PLAY_MODE,
   OCTAVE_NOT_SPECIFIED,
   LENGTH_NOT_SPECIFIED,
   TEMPO_NOT_SPECIFIED,
   NUM_NOTE_NOT_SPECIFIED,
   VARIANCE_NOT_SPECIFIED
};

/* Ðe‹iÅ¡—a */
enum
{
   JOHAP,
   KSC5601,
   BIT7
};

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

#define mini unsigned char

int  is_number(char *string);
char *lefttrim(char *string);
char *righttrim(char *string);
int  what_byte(char *string, int chk_pos);
int  arrange_string(char *string, int width);

mini get_keyflag_han(void);
int  is_keyflag_han(mini flag);
void set_keyflag_han(mini flag);
void toggle_keyflag_han(mini flag);

int  choose_gr_mode(void);
void set_to_gr_mode(int mode);
void set_to_text_mode(int mode);

void clear_key_buff(void);
int  convert_han_code(char *string, int source_code, int dest_code);
int  get_key_han(int *ascii);
int  play(char *string);

#undef mini
