#include "defs.h"


int cmd2dir[ 16] =
{
  0,                   // Nothing
  DIRECTION_LEFT,      // CMD_LEFT
  DIRECTION_RIGHT,     // CMD_RIGHT
  0,                   // CMD_LEFT + CMD_RIGHT
  DIRECTION_UP,        // CMD_UP
  DIRECTION_UPLEFT,    // CMD_UP + CMD_LEFT
  DIRECTION_UPRIGHT,   // CMD_UP + CMD_RIGHT
  0,                   // CMD_UP + CMD_LEFT + CMD_RIGHT
  DIRECTION_DOWN,      // CMD_DOWN
  DIRECTION_DOWNLEFT,  // CMD_DOWN + CMD_LEFT
  DIRECTION_DOWNRIGHT, // CMD_DOWN + CMD_RIGHT
  0,                   // CMD_DOWN + CMD_LEFT + CMD_RIGHT
  0,                   // CMD_UP + CMD_DOWN
  0,                   // CMD_UP + CMD_DOWN + CMD_LEFT
  0,                   // CMD_UP + CMD_DOWN + CMD_RIGHT
  0                    // CMD_UP + CMD_DOWN + CMD_LEFT + CMD_RIGHT
};

int dir2cmd[ 8] =
{
  CMD_UP,
  CMD_UP + CMD_RIGHT,
  CMD_RIGHT,
  CMD_DOWN + CMD_RIGHT,
  CMD_DOWN,
  CMD_DOWN + CMD_LEFT,
  CMD_LEFT,
  CMD_UP + CMD_LEFT
};

int dir2angle[ 8] =
{
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160
};

static int cSinus[ 65] =
{
    0,   6,  12,  18,  25,  31,  37,  43,
   49,  56,  62,  68,  74,  80,  86,  92,
   97, 103, 109, 115, 120, 126, 131, 136,
  142, 147, 152, 157, 162, 167, 171, 176,
  181, 185, 189, 193, 197, 201, 205, 209,
  212, 216, 219, 222, 225, 228, 231, 234,
  236, 238, 241, 243, 244, 246, 248, 249,
  251, 252, 253, 254, 254, 255, 255, 255,
  256
};


void GetVectorsForAngle( int angle, int *dx, int *dy )
{
  if (angle <= 64)
  {
    *dx = cSinus[ 64-angle];
    *dy = cSinus[ angle];
  }
  else if (angle <= 128)
  {
    *dx = -cSinus[ angle-64];
    *dy = cSinus[ 128-angle];
  }
  else if (angle <= 192)
  {
    *dx = -cSinus[ 192-angle];
    *dy = -cSinus[ angle-128];
  }
  else
  {
    *dx = cSinus[ angle-192];
    *dy = -cSinus[ 256-angle];
  }
  *dy *= 3;
  *dy /= 4;
}

