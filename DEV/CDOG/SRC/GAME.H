//#define GAMETICKS_PER_SECOND 280
#define GAMETICKS_PER_SECOND 560
#define GAMETICKS_PER_FRAME  8

typedef void (*intHandler)(void);

void pascal interrupt synchronizer( void );
int gameloop( void );
void DisplayMessage( const char *s );
void ShakeScreen( void );

extern int missionTime;
