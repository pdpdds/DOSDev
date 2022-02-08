#include "graph.h"

void close_graph(void)
{
   set_video_mode(org_video_mode);
   gr_mode = -1;
}
