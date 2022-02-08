#include "graph.h"

int init_graph(int mode)
{
   int std_mode;

   org_video_mode = get_video_mode();

   gr_adapter = detect_adapter();
   if(gr_adapter == VGA) gr_adapter = detect_svga();

   if(mode == DETECT)
   {
      gr_mode = get_standard_mode(gr_adapter);
      if(gr_mode == -1) return(NOT_VALID_ADAPTER);
   }
   else if(mode < HGC_640x400x2_0 || mode > DETECT)
      return(NOT_VALID_MODE);
   else if(!is_avail_mode(gr_adapter, mode))
      return(NOT_VALID_MODE);
   else
      gr_mode = mode;

   set_gr_mode(gr_mode);
   return(OK_INIT);
}
