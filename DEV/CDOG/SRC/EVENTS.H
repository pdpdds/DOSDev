

struct MouseRect
{
  int left, top, right, bottom;
  int tag;
};


void InitMouse( void );
void Mouse( int *x, int *y, int *button );
int  GetKey( void );
void SetMouseRects( struct MouseRect *rects );
void SetSecondaryMouseRects( struct MouseRect *rects );
int  GetMouseRectTag( int x, int y, int *tag );

