#ifndef YES
#define YES            -1
#define NO             0
#endif


#define JOYSTICK_LEFT       1
#define JOYSTICK_RIGHT      2
#define JOYSTICK_UP         4
#define JOYSTICK_DOWN       8
#define JOYSTICK_BUTTON1   16
#define JOYSTICK_BUTTON2   32
#define JOYSTICK_BUTTON3   64
#define JOYSTICK_BUTTON4  128
#define JOYSTICK_PRESENT  256


struct JoyRec
{
  int present;
  int inUse;
  int xMid, yMid;
  int x, y;
  int buttons;
};


extern struct JoyRec gSticks[2];


void PollSticks( int maxWait );
void InitSticks( void );
void AutoCalibrate( void );
void PollDigiSticks( int *joy1, int *joy2 );
void EnableSticks( int joy1, int joy2 );
