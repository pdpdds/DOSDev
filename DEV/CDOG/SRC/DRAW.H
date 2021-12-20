#include "map.h"

void SetBuffer( int x_origin, int y_origin, struct Buffer *buffer, int width );
void LineOfSight( int xc, int yc, struct Buffer *buffer, int shadowFlag );
void FixBuffer( struct Buffer *b, int isShadow );
void DrawBuffer( struct Buffer *b, int xOffset );
