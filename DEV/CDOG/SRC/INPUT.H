#define KEYBOARD            0
#define JOYSTICK_ONE        1
#define JOYSTICK_TWO        2

void GetPlayerCmd( int *cmd1, int *cmd2 );
void GetMenuCmd( int *cmd );
void WaitForRelease( void );
void WaitForPress( void );
void Wait( void );
